#include <curl/curl.h>
#include "stdio.h"
#include "stdlib.h"
#include "util.h"
#include <sstream>
#include <functional>
#include <iostream>
#include <string>
#ifndef REST_H
#define REST_H

namespace Asago {
 class REST {
public:
    std::string url;
    struct MemoryStruct chunk;
    std::vector<std::pair<std::string, std::string>> additional_headers;
    std::function<void(char*, size_t)> onFinish;

    char * get_buffer()
    {
        return chunk.memory;
    }

    void run()
    {
        CURL *curl_handle;
        CURLcode res;

        chunk.memory = (char*)malloc(1);  
        chunk.size = 0;
        chunk.onFinish = this->onFinish;

        curl_handle = curl_easy_init();
        if(curl_handle) {
            curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
            curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
            curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

            if(!additional_headers.empty())
            {
                struct curl_slist* headers = NULL;
                for(auto & element : additional_headers)
                {
                    std::string f = element.first +": "+ element.second;
                    headers = curl_slist_append(headers, f.c_str());
                }
                curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers);
            }
            res = curl_easy_perform(curl_handle);

            if(res != CURLE_OK) {
                fprintf(stderr, "error: %s\n", curl_easy_strerror(res));
            }
            else {
                chunk.onFinish(chunk.memory, chunk.size);
            }
        }
        curl_easy_cleanup(curl_handle);
        free(chunk.memory);
    }
};   
}

#endif