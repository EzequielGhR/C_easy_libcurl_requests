#ifndef REQUESTS_H
  #define REQUESTS_H

  #include <stdlib.h>
  
  typedef struct {
    char* content;
    long status_code;
  } Response;
    
  Response* get_request(const char* url);
  Response* post_request(const char* url, char* data, size_t data_size, char** headers_array, int header_count);
  void destroy_response(Response** response_ptr);
  
#endif
