#include "util.h"

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  char *ptr = (char*) realloc(mem->memory, mem->size + realsize + 1);
  if(ptr == NULL) {
    printf("error: not enough memory\n");
    return 0;
  }

  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
  //mem->onFinish(mem->memory, mem->size);

  return realsize;
}

int32_t bytes_to_int(char *buffer)
{
    int32_t some = 0;
    some |= (unsigned char)buffer[0];
    some |= (unsigned char)buffer[1] << 8;
    some |= (unsigned char)buffer[2] << 16;
    some |= (unsigned char)buffer[3] << 24;

    return some;
}

double bytes_to_double(char *buffer)
{
    double some;
    memcpy(&some, buffer, sizeof(double));
    return some;
}