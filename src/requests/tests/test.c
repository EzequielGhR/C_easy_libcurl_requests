#include <stdio.h>
#include "../requests.h"

int main() {
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


