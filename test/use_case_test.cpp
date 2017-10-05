//
//  use_case_test.cpp
//  ql_fetcher_test
//
//  Created by Yaric Shkurat on 27/09/2017.
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
#include QL_RELEASE_TEST

#include "lib/json.hpp"
#include "lib/catch.hpp"
#include <iostream>

TEST_CASE("Use-case test")
{
    using json = nlohmann::json;
    namespace ql = ql_fetcher;
    
    QUANDL_PAUSE
    QUANDL_PAUSE
    SECTION("Basic usage")
    {
        auto resp = ql::fetch("EDF2017", "CME").limit(100)
            .column(3).order(ql::asc).make().get();
        
        if (!resp.empty())
        {
            const auto parsed_resp = json::parse(resp.str());

            REQUIRE(parsed_resp["dataset"]["start_date"].dump() ==
                    "\"2016-07-15\"");
            REQUIRE(parsed_resp["dataset"]["data"][4].dump() ==
                    "[\"2016-09-09\",99.025]");
        }
    }
    QUANDL_PAUSE
    
    SECTION("Usage with callback")
    {
        std::string resp_str;
        auto parsed_resp = ql::fetch("EDF2017", "CME")
            .limit(100).column(3).order(ql::asc)
            .with_or([&] (ql::response& r)
            {
                resp_str = r.str();
                return json::parse(r.str());
            }, [] (ql::response& r)
            {
                REQUIRE(r.empty());
                return json::parse("{}");
            }).get();
        
        if (!resp_str.empty())
        {
            std::cout << " \n Some data point: "
                << parsed_resp["dataset"]["data"][4].dump() << std::endl;
            REQUIRE(parsed_resp["dataset"]["start_date"].dump() ==
                    "\"2016-07-15\"");
            REQUIRE(parsed_resp["dataset"]["data"][4].dump() ==
                    "[\"2016-09-09\",99.025]");
        }
    }
    QUANDL_PAUSE
}

