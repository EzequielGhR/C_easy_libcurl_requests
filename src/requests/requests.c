#include "../logging/logging.h"
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


Response* get_request(const char* url) {
  CURL* curl;
  CURLcode response;

  struct MemoryStruct chunk;
  chunk.memory = malloc(1);
  chunk.size = 0;

  curl = curl_easy_init();
  if (curl == NULL) {
    log_error("Failed to fetch from url: %s", url);
    free(chunk.memory);
    return NULL;
  }

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

  long response_code;
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

  Response* output = malloc(sizeof(Response));
  output->content = chunk.memory;
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
  struct curl_slist* headers = NULL;
  CURL* curl;
  CURLcode response;

  if (data == NULL || data_size <= 0) {
    log_warning("No data to post");
    return NULL;
  }
  
  if (headers_array != NULL) {
    for (int i = 0; i < header_count; i++) {
      headers = curl_slist_append(headers, headers_array[i]);
    }
  }

  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _write_memory_callback);
  cul_easy_setopt(curl, CURLOPT_WRITEDATA, )

  if (headers != NULL) {
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  }
  
  
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
