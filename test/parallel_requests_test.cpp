//
//  parallel_requests_test.cpp
//  ql_fetcher_test
//
//  Created by Yaric Shkurat on 24/09/2017.
//  Copyright © 2017 Yarrik Sh. All rights reserved.
//

#include "test_config.h"
#include QL_TEST

#include "lib/catch.hpp"
#include <iostream>
#include <chrono>
#include <thread>

std::string make_data_request(std::string data_id, std::string source)
{
    using namespace ql_fetcher;
    
    request req{ data_id, source };
    
    //req.api_key(QUANDL_TESTKEY).limit(n_lim).column(4).order(asc);
    req.api_key(QUANDL_TESTKEY).as(json).limit(1000).column(2).order(asc)
        .timeframe(weekly).transform(diff);
    
//    std::cout << "\n    - S - - - > " << req.url() << std::endl;

    return req.make().get().str();
}

std::future<void> make_future_data_request(std::string data_id, std::string source,
                              std::string& resp_data)
{
    using namespace ql_fetcher;
    
    return std::async(std::launch::async, [&]
        {
            ql_fetcher::request parallel_req{ data_id, source };
            
            parallel_req.api_key(QUANDL_TESTKEY).as(json).limit(1000).column(2)
                .order(asc).timeframe(weekly).transform(diff);
            
//            std::cout << "\n    - P - - - > " << parallel_req.url() << std::endl;
            resp_data = parallel_req.make().get().str();
        });
}


TEST_CASE("Parallel requests and response fetching test")
{
    using namespace ql_fetcher;
    
    SECTION("Perform 2 requests in parallel")
    {
        const size_t n_lim{ 1000 };
        
        std::string ref_data_1{ make_data_request("EDF2016", "CME") },
                    ref_data_2{ make_data_request("EDF2015", "CME") };
        QUANDL_PAUSE
        std::string parallel_data_1, parallel_data_2;
        
        auto fut_1 = make_future_data_request("EDF2016", "CME", parallel_data_1);
        auto fut_2 = make_future_data_request("EDF2015", "CME", parallel_data_2);
        
        fut_1.get();
        fut_2.get();
        
        REQUIRE_FALSE(ref_data_1.empty());
        REQUIRE_FALSE(ref_data_2.empty());
        
        REQUIRE_FALSE(parallel_data_1.empty());
        REQUIRE_FALSE(parallel_data_2.empty());
        
        REQUIRE(parallel_data_1 == ref_data_1);
        REQUIRE(parallel_data_2 == ref_data_2);
    }
    QUANDL_PAUSE
    
    SECTION("Perform 5 requests in parallel")
    {
        const size_t n_lim{ 25 };
        
        std::string ref_data_1{ make_data_request("EDF2016", "CME") };
        std::string ref_data_2{ make_data_request("EDK2014", "CME") };
        QUANDL_PAUSE
        std::string ref_data_3{ make_data_request("EDZ2015", "CME") };
        std::string ref_data_4{ make_data_request("BZZ2015", "CME") };
        QUANDL_PAUSE
        std::string ref_data_5{ make_data_request("CLZ2015", "CME") };
        
        std::string parallel_data_1, parallel_data_2, parallel_data_3,
            parallel_data_4, parallel_data_5;
        
        auto fut_1 = make_future_data_request("EDF2016", "CME", parallel_data_1);
        auto fut_2 = make_future_data_request("EDK2014", "CME", parallel_data_2);
        QUANDL_PAUSE
        auto fut_3 = make_future_data_request("EDZ2015", "CME", parallel_data_3);
        auto fut_4 = make_future_data_request("BZZ2015", "CME", parallel_data_4);
        QUANDL_PAUSE
        auto fut_5 = make_future_data_request("CLZ2015", "CME", parallel_data_5);
        
        fut_1.get();
        fut_2.get();
        QUANDL_PAUSE
        fut_3.get();
        fut_4.get();
        QUANDL_PAUSE
        fut_5.get();
        
        REQUIRE_FALSE(ref_data_1.empty());
        REQUIRE_FALSE(ref_data_5.empty());
        
        REQUIRE_FALSE(parallel_data_1.empty());
        REQUIRE_FALSE(parallel_data_5.empty());
        
        REQUIRE(parallel_data_1 == ref_data_1);
        REQUIRE(parallel_data_2 == ref_data_2);
        REQUIRE(parallel_data_3 == ref_data_3);
        REQUIRE(parallel_data_4 == ref_data_4);
        REQUIRE(parallel_data_5 == ref_data_5);
    }
    QUANDL_PAUSE
}
