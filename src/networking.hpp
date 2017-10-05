//
//  networking.hpp
//  ql_fetcher
//
//  Created by Yarrik Shkurat on 22/08/2017.
//  Copyright © 2017 Yarrik Sh. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is furnished
//  to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
//  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#ifndef ql_networking_hpp
#define ql_networking_hpp

#include <string>
#include <curl/curl.h>
#include <sstream>
#include <iostream>
#include <chrono>
#include <thread>

namespace ql_fetcher
{
    std::string fetch_raw(const std::string&);
}



#ifdef QL_FETCHER_IMPL
namespace ql_fetcher
{
    size_t store_callback(const char* ptr, const size_t size,
                                 const size_t nmemb, std::stringstream* stream)
    {
        auto buff = std::string(ptr, ptr + nmemb/size);
        (*stream) << buff;
        return size * nmemb;
    }
    
    std::string fetch_raw(const std::string& data_url)
    {
        CURL* curl;
        CURLcode res;
        
        curl = curl_easy_init();
        
        curl_easy_setopt(curl, CURLOPT_URL, data_url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, store_callback);
        
        std::stringstream stream;
        
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&stream);
        //        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
        
        std::cout << "\nREQUESTING: " << data_url;
        
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(500ms);
        
        res = curl_easy_perform(curl);
        std::cout << "\n -- OK\n";
        
        curl_easy_cleanup(curl);
        
        if(res == CURLE_OK) return stream.str();
        else return "";
    }
};
#endif // QL_FETCHER_IMPL

#endif /* ql_networking_hpp */
