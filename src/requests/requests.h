#ifndef REQUESTS_H
  #define REQUESTS_H
  
  typedef struct {
    char* content;
    long status_code;
  } Response;
    
  Response* get_request(const char* url);
  void destroy_response(Response** response_ptr);
  
#endif
