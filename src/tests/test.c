#include <stdio.h>
#include "requests.h"

int test_get_request() {
  Response* response = get_request("https://jsonplaceholder.typicode.com/todos/1");
  if (response == NULL) return 1;
  
  printf("Status code: %ld\n", response->status_code);
  printf("-------------------------\n");
  printf("Response Content:\n\n%s\n\n", response->content);
  printf("-------------------------\n");

  destroy_response(&response);
  printf("%s\n", response == NULL ? "Nullified response" : "Failed to nullify response");
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
  printf("Status code: %ld\n", response->status_code);
  printf("-------------------------\n");
  printf("Response Content:\n\n%s\n\n", response->content);
  printf("-------------------------\n");

  destroy_response(&response);
  printf("%s\n", response == NULL ? "Nullified response" : "Failed to nullify response");
  return 0;
}

int main() {
  test_get_request();
  test_post_request();
}
