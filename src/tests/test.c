#include <stdio.h>
#include "requests.h"
#include "logging.h"
#include "openai.h"


int test_get_request() {
  Response* response = get_request("https://jsonplaceholder.typicode.com/todos/1", NULL, 0);
  if (response == NULL) return 1;
  
  log_info("Status code: %ld", response->status_code);
  printf("-------------------------\n");
  log_info("Response Content:\n\n%s\n", response->content);
  printf("-------------------------\n");

  destroy_response(&response);
  log_info("%s", response == NULL ? "Nullified response" : "Failed to nullify response");
  return 0;
}

int test_post_request() {
  const char hardcoded_data[21] = "{\"title\": \"My test\"}";
  char* hardcoded_headers[1];
  hardcoded_headers[0] = "content-type: application/json";
  Response* response = post_request(
    "https://jsonplaceholder.typicode.com/posts",
    (char*)hardcoded_data,
    21,
    hardcoded_headers,
    1
  );

  if (response == NULL) return 1;
  log_info("Status code: %ld", response->status_code);
  printf("-------------------------\n");
  log_info("Response Content:\n\n%s\n", response->content);
  printf("-------------------------\n");

  destroy_response(&response);
  log_info("%s", response == NULL ? "Nullified response" : "Failed to nullify response");
  return 0;
}

int test_openai_login() {
  Response* response = openai_models(); 
  if (response == NULL) return 1;
  log_info("Status code: %ld", response->status_code);
  printf("-------------------------\n");
  log_info("Response Content:\n\n%s\n", response->content);
  printf("-------------------------\n");

  destroy_response(&response);
  log_info("%s", response == NULL ? "Nullified response" : "Failed to nullify response");
  return 0;
}

int main() {
  log_info("Testing get request:\n");
  printf("~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
  test_get_request();
  printf("\n~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
  log_info("Testing post request:\n");
  printf("~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
  test_post_request();
  printf("\n~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
  log_info("Testing openai login:\n");
  printf("~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
  test_openai_login();
  printf("\n~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
  
  
}
