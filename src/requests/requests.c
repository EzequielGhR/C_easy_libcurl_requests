#include "logging.h"
#include "requests.h"
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>


struct MemoryStruct {
  char* memory;
  size_t size;
};


size_t _write_memory_callback(void* contents, size_t size, size_t nmemb, void* userp) {
  size_t realsize = size * nmemb;
  
  struct MemoryStruct* mem = (struct MemoryStruct*) userp;
  char* new_memory = realloc(mem->memory, mem->size + realsize + 1);
  
  if (new_memory == NULL) {
    log_error("Not enough memory");
    return 0;
  }

  mem->memory = new_memory;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}


void _free_headers(struct curl_slist** headers_list_ptr) {
  if (headers_list_ptr == NULL || *headers_list_ptr == NULL) {
    log_warning("Headers list is null");
    return;
  }

  free(*headers_list_ptr);
  *headers_list_ptr = NULL;
}


int _set_headers(struct curl_slist** headers_list_ptr, char** headers_array, int header_count) {
  if (headers_array == NULL || header_count <= 0) {
    log_warning("No headers to set");
    return 1;
  }

  if (headers_list_ptr == NULL) {
    log_error("Headers list pointer is null, cannot dereference");
    return 0;
  }

  struct curl_slist* dref_headers = *headers_list_ptr;
  for (int i = 0; i < header_count; i++) {
    dref_headers = curl_slist_append(dref_headers, headers_array[i]);
  }

  *headers_list_ptr = dref_headers;
  return 1; 
}


Response* get_request(const char* url) {
  CURL*                 curl;
  CURLcode              response;
  struct MemoryStruct   chunk;
  long                  response_code;
  Response*             output;
  
  curl = curl_easy_init();
  if (curl == NULL) {
    log_error("Failed to fetch from url: %s", url);
    return NULL;
  }

  chunk.memory = malloc(1);
  chunk.size   = 0;
  
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _write_memory_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);
  
  response = curl_easy_perform(curl);
  if (response != CURLE_OK) {
    log_warning("Failed request");
    free(chunk.memory);
    curl_easy_cleanup(curl);
    return NULL;
  }

  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

  output              = malloc(sizeof(Response));
  output->content     = chunk.memory;
  output->status_code = response_code;

  curl_easy_cleanup(curl);

  return output;
}


Response* post_request(
  const char* url,
  char* data,
  size_t data_size,
  char** headers_array,
  int header_count
) {
  CURL*                curl;
  CURLcode             response;
  struct curl_slist*   headers = NULL;
  struct MemoryStruct  chunk;
  long                 status_code;
  Response*            output;

  if (data == NULL || data_size <= 0) {
    log_warning("No data to post");
    return NULL;
  }

  if (!_set_headers(&headers, headers_array, header_count)) return NULL;
  
  if (headers_array != NULL) {
    for (int i = 0; i < header_count; i++) {
      headers = curl_slist_append(headers, headers_array[i]);
    }
  }

  curl = curl_easy_init();
  if (curl == NULL) {
    log_error("Could not initialize curl");
    return NULL;
  }

  chunk.memory = malloc(1);
  chunk.size   = 0;

  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _write_memory_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);

  if (headers != NULL) {
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  }

  response = curl_easy_perform(curl);
  if (response != CURLE_OK) {
    log_error("Failed to perform post request");
    free(chunk.memory);
    curl_easy_cleanup(curl);
    _free_headers(&headers);
    return NULL;
  }

  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status_code);

  output              = malloc(sizeof(Response));
  output->content     = chunk.memory;
  output->status_code = status_code;
  
  curl_easy_cleanup(curl);
  _free_headers(&headers);
  return output;
}


void destroy_response(Response** response_ptr) {
  if (response_ptr == NULL || *response_ptr == NULL) {
    log_warning("Cannot free void pointer");
    return;
  }

  if ((*response_ptr)->content != NULL) {
    free((*response_ptr)->content);
    (*response_ptr)->content = NULL;
  }

  free(*response_ptr);
  *response_ptr = NULL;
}
