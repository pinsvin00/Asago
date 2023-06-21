#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <functional>

struct MemoryStruct {
  char *memory;
  size_t size;
  std::function<void(char*, size_t)> onFinish;
};

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
int32_t bytes_to_int(char* buffer);
double bytes_to_double(char* buffer);

#endif