//
//  vendor_specific.hpp
//  ql_fetcher
//
//  Created by Yaric Shkurat on 25/09/2017.
//  Copyright © 2017 Yarrik Sh. All rights reserved.
//

#ifndef ql_vendor_specific_hpp
#define ql_vendor_specific_hpp

#include <sstream>
#include <string>

namespace ql_fetcher
{
    enum sort_order
    {
        desc = 0,
        asc = 1
    };
    
    enum sampling_freq
    {
        none = 0,
        daily = 1,
        weekly = 2,
        monthly = 3,
        quarterly = 4,
        annual = 5
    };
    
    enum calc_kind
    {
        no_calc = 0,
        diff = 1,
        rdiff = 2,
        rdiff_from = 3,
        cumul = 4,
        normalize = 5
    };
    
    enum result_kind
    {
        json = 0,
        csv = 1
    };
    
    std::string to_string(calc_kind);
    
    std::string to_string(result_kind);
    
    std::string to_string(sort_order);
    
    std::string to_string(sampling_freq);
    
    bool no_data_returned(const std::string& response);
}



#ifdef QL_FETCHER_IMPL
#include "request_params.hpp"

#define QL_DATASET_PATH   "https://www.quandl.com/api/v3/datasets/"

namespace ql_fetcher
{
    static const char * sort_orders_str[] = {"desc", "asc"};
    
    static const char * sampling_freqs_str[] =
        {"none", "daily", "weekly", "monthly", "quarterly", "annual"};
    
    static const char * calc_kinds_str[] =
        {"none", "diff", "rdiff", "rdiff_from", "cumul", "normalize"};
    
    static const char * result_kinds_str[] = {"json", "csv"};
    
    std::string to_string(sort_order order)
    {
        if (order <= asc)
            return std::string(sort_orders_str[order]);
        else
            return "";
    }
    
    std::string to_string(sampling_freq freq)
    {
        if (freq <= annual)
            return std::string(sampling_freqs_str[freq]);
        else
            return "";
    }
    
    std::string to_string(calc_kind calc)
    {
        if (calc <= normalize)
            return std::string(calc_kinds_str[calc]);
        else
            return "";
    }
    
    std::string to_string(result_kind res_kind)
    {
        if (res_kind <= csv)
            return std::string(result_kinds_str[res_kind]);
        else
            return "";
    }
    
    std::string form_request(const request_params& params)
    {
        std::stringstream request_url;
        
        request_url << QL_DATASET_PATH
        << params._database_code << "/"
        << params._dataset_code
        << "." << to_string(params._fetch_as) << "?";
        
        if (params._limit > 0)
            request_url << "limit=" << params._limit << "&";
        
        if (params._column_index > 0)
            request_url << "column_index=" << params._column_index << "&";
        
        if (!params._start_date.empty())
            request_url << "start_date=" << params._start_date << "&";
        
        if (!params._end_date.empty())
            request_url << "end_date=" << params._end_date << "&";
        
        request_url
        << "order=" << to_string(params._order) << "&"
        << "collapse=" << to_string(params._timeframe) << "&"
        << "transform=" << to_string(params._transform);
        
        if (!params._api_key.empty())
            request_url << "&" << "api_key=" << params._api_key;
        
        return request_url.str();
    }
    
    bool no_data_returned(const std::string& response)
    {
        return response.find("quandl_error") != std::string::npos;
    }
}
#endif // QL_FETCHER_IMPL

#endif /* ql_vendor_specific_hpp */
