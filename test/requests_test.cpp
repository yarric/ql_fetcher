//
//  requests_test.cpp
//  ql_fetcher_test
//
//  Created by Yaric Shkurat on 26/09/2017.
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

#include "test_config.h"
#include QL_TEST

#include "lib/catch.hpp"
#include <iostream>


TEST_CASE("Basic request and response fetching test")
{
    using namespace ql_fetcher;
    
    SECTION("Retrieving data")
    {
        request req_1{ "EDF2017", "CME" };
        
        req_1.api_key(QUANDL_TESTKEY).as(json).limit(200);
        
        std::string raw_data_ref{ fetch_raw(req_1.url()) };
        QUANDL_PAUSE
        std::string raw_data_test{ req_1.make().get().str() };
        
        REQUIRE_FALSE(raw_data_test.empty());
        REQUIRE(raw_data_test == raw_data_ref);
    }
    QUANDL_PAUSE
    
    SECTION("Retrieving data with callback")
    {
        request req_1{ "EDF2016", "CME" };
        std::string raw_data_test;
        req_1.api_key(QUANDL_TESTKEY).as(json).limit(500)
            .with([&](response& resp)
            {
                raw_data_test = resp.str()+"cb";
            }).get();
        QUANDL_PAUSE
        std::string raw_data_ref{ fetch_raw(req_1.url())+"cb" };
        
        REQUIRE_FALSE(raw_data_test.empty());
        REQUIRE(raw_data_test == raw_data_ref);
    }
    QUANDL_PAUSE
    
    SECTION("Repeated data retrieval when retrieval required")
    {
        request req_1{ "EDF2016", "CME" };
        
        req_1.api_key(QUANDL_TESTKEY).as(json).limit(25);
        
        std::string raw_data_ref{ fetch_raw(req_1.url()) };
        QUANDL_PAUSE
        std::string raw_data_test{ req_1.make().get().str() };
        
        REQUIRE_FALSE(raw_data_test.empty());
        REQUIRE(raw_data_test == raw_data_ref);
        
        req_1.as(csv).limit(150);
        
        raw_data_ref = fetch_raw(req_1.url());
        QUANDL_PAUSE
        raw_data_test = req_1.make().get().str();
        
        REQUIRE_FALSE(raw_data_test.empty());
        REQUIRE(raw_data_test == raw_data_ref);
    }
    QUANDL_PAUSE
    
    SECTION("Repeated data retrieval when retrieval not required")
    {
        request req_1{ "EDF2016", "CME" };
        
        req_1.api_key(QUANDL_TESTKEY).as(csv).limit(125);
        
        std::string raw_data_ref{ fetch_raw(req_1.url()) };
        QUANDL_PAUSE
        std::string raw_data_test{ req_1.make().get().str() };
        
        REQUIRE_FALSE(raw_data_test.empty());
        REQUIRE(raw_data_test == raw_data_ref);
        
        req_1.limit(125);
        
        raw_data_test = req_1.make().get().str();
        
        REQUIRE_FALSE(raw_data_test.empty());
        REQUIRE(raw_data_test == raw_data_ref);
    }
    QUANDL_PAUSE
}




