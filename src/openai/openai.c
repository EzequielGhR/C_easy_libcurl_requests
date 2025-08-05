#include <stdio.h>
#include <stdlib.h>
#include "openai.h"
#include "requests.h"
#include "logging.h"

#define LOGIN_URL "https://api.openai.com/v1/models"


Response* openai_models() {
  const char* OPENAI_API_KEY;
  char        auth_header[200];
  char*       headers[1];

  OPENAI_API_KEY = getenv("OPENAI_API_KEY");
  if (OPENAI_API_KEY == NULL) {
    log_error("API KEY not defined");
    return NULL;
  }

  snprintf(auth_header, 200, "Authorization: Bearer %s", OPENAI_API_KEY);
  headers[0] = auth_header;

  return get_request(LOGIN_URL, headers, 1);
}
