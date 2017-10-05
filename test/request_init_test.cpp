//
//  request_test.cpp
//  ql_fetcherTest
//
//  Created by Yarrik Shkurat on 22/09/2017.
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


TEST_CASE("request init test")
{   
    using namespace ql_fetcher;
    
    SECTION("Setting basic and default params")
    {
        const std::string correct_url
        {
            "https://www.quandl.com/api/v3/datasets/CME/EDF2017.json?"
            "order=desc&collapse=none&transform=none"
            "&api_key=T9TxyfZHeANyQsmLqg7U"
        };
        
        request basic_req_2{ "EDF2017", "CME" };
        basic_req_2.api_key( QUANDL_TESTKEY );
        
        REQUIRE(basic_req_2.url() == correct_url);
    }
    
    SECTION("Setting all params")
    {
        const std::string correct_url
        {
            "https://www.quandl.com/api/v3/datasets/CME/EDF2017.json?"
            "limit=200&column_index=2&start_date=2016-09-04&end_date=2017-01-01"
            "&order=asc&collapse=weekly&transform=diff&api_key=T9TxyfZHeANyQsmLqg7U"
        };
        
        request allparams_req{ "EDF2017", "CME" };
        
        allparams_req.api_key(QUANDL_TESTKEY).as(json)
            .limit(200).column(2).order(asc).timeframe(weekly)
            .transform(diff).begin("2016-09-04").end("2017-01-01");
   
        REQUIRE(allparams_req.url() == correct_url);
    }
    
    SECTION("Setting some params")
    {
        const std::string correct_url
        {
            "https://www.quandl.com/api/v3/datasets/CME/EDF2016.csv?"
            "limit=20&start_date=2015-08-09&order=asc&collapse=weekly&"
            "transform=none&api_key=T9TxyfZHeANyQsmLqg7U"
        };
        
        request someparams_req{ "EDZ2015", "CME" };
        
        request& someparams_req_ref = someparams_req.dataset("EDF2016")
                .api_key(QUANDL_TESTKEY).database("CME").as(csv)
                .limit(20).order(asc);
        
        someparams_req.timeframe(weekly).begin("2015-08-09");
        
//                std::cout << " ---> " << someparams_req.url() << std::endl;
        REQUIRE(someparams_req.url() == correct_url);
        REQUIRE(someparams_req_ref.url() == correct_url);
    }
    
    SECTION("No API key")
    {
        const std::string correct_url_without_key
        {
            "https://www.quandl.com/api/v3/datasets/CME/EDF2016.csv?"
            "limit=20&start_date=2015-08-09&order=asc&collapse=weekly&"
            "transform=none"
        };
        
        request req_without_key{ "EDF2016", "CME" };
        
        req_without_key.dataset("EDF2016").database("CME").as(csv)
            .limit(20).order(asc).timeframe(weekly).begin("2015-08-09");
        
        REQUIRE(req_without_key.url() == correct_url_without_key);
    }
    
    SECTION("Setting global API key")
    {
        const std::string global_key{ "yt6Te6HDayt47AhY" };
        
        const std::string correct_url_with_key
        {
            "https://www.quandl.com/api/v3/datasets/CME/EDF2016.csv?"
            "limit=20&start_date=2015-08-09&order=asc&collapse=weekly&"
            "transform=none&api_key=T9TxyfZHeANyQsmLqg7U"
        };
        
        const std::string correct_url_with_global_key
        {
            "https://www.quandl.com/api/v3/datasets/CME/EDF2016.csv?"
            "limit=20&start_date=2015-08-09&order=asc&collapse=weekly&"
            "transform=none&api_key=yt6Te6HDayt47AhY"
        };
        
        set_api_key(global_key);
        
        request req_with_global_key{ "EDF2016", "CME" };
        
        req_with_global_key.dataset("EDF2016")
            .database("CME").as(csv).limit(20).order(asc)
            .timeframe(weekly).begin("2015-08-09");
        
        request req_with_key{ "EDF2016", "CME" };
        
        req_with_key.api_key(QUANDL_TESTKEY).dataset("EDF2016")
            .database("CME").as(csv).limit(20).order(asc)
            .timeframe(weekly).begin("2015-08-09");
        
        REQUIRE(get_api_key() == global_key);
        REQUIRE(req_with_global_key.url() == correct_url_with_global_key);
        REQUIRE(req_with_key.url() == correct_url_with_key);
    }
}



