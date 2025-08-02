#include <stdio.h>
#include <stdarg.h>

#define MAX_LOG_LENGTH 1024


typedef enum {
  INFO,
  WARNING,
  ERROR
} LogLevel;

void _vlog(LogLevel lvl, const char* format, va_list args) {
  FILE* stream;
  const char* str_lvl;

  switch(lvl) {
    case INFO:
      stream = stdout;
      str_lvl = "INFO";
      break;
    case WARNING:
      stream = stderr;
      str_lvl = "WARNING";
      break;
    case ERROR:
      stream = stderr;
      str_lvl = "ERROR";
      break;
    default:
      fprintf(stderr, "Invalid log level: %d\n", lvl);
      return;
  }

  fprintf(stream, "%s: ", str_lvl);
  vfprintf(stream, format, args);
  fprintf(stream, "\n");
}


void log_info(const char* format, ...) {
  va_list args;
  
  va_start(args, format);
  _vlog(INFO, format, args);
  va_end(args);
}

void log_warning(const char* format, ...) {
  va_list args;
  
  va_start(args, format);
  _vlog(WARNING, format, args);
  va_end(args);
}

void log_error(const char* format, ...) {
  va_list args;
  
  va_start(args, format);
  _vlog(ERROR, format, args);
  va_end(args);
}
