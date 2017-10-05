//
//  request.hpp
//  ql_fetcher
//
//  Created by Yarrik Shkurat on 21/09/2017.
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

#ifndef ql_request_hpp
#define ql_request_hpp

#include "request_params.hpp"
#include "response.hpp"
#include "networking.hpp"
#include <string>
#include <future>
#include <functional>


namespace ql_fetcher
{
    std::string& get_api_key();
    
    class request
    {
    public:
        request(const std::string& data_code, const std::string& source_code)
        {
            dataset(data_code);
            database(source_code);
            params._api_key = get_api_key();
        }
        
        request& database(const std::string& source_code)
        {
            params.update(params._database_code != source_code);
            
            params._database_code = source_code;
            return *this;
        }
        
        request& dataset(const std::string& data_code)
        {
            params.update(params._dataset_code != data_code);
            
            params._dataset_code = data_code;
            return *this;
        }
        
        request& limit(size_t n)
        {
            params.update(params._limit != n);
            
            params._limit = n;
            return *this;
        }
        
        request& column(size_t n)
        {
            params.update(params._column_index != n);
            
            params._column_index = n;
            return *this;
        }
        
        request& begin(const std::string& yyyy_mm_dd)
        {
            params.update(params._start_date != yyyy_mm_dd);
            
            params._start_date = yyyy_mm_dd;
            return *this;
        }
        
        request& end(const std::string& yyyy_mm_dd)
        {
            params.update(params._end_date != yyyy_mm_dd);
            
            params._end_date = yyyy_mm_dd;
            return *this;
        }
        
        request& order(sort_order _order)
        {
            params.update(params._order != _order);
            
            params._order = _order;
            return *this;
        }
        
        request& timeframe(sampling_freq freq)
        {
            params.update(params._timeframe != freq);
            
            params._timeframe = freq;
            return *this;
        }
        
        request& transform(calc_kind calc)
        {
            params.update(params._transform != calc);
            
            params._transform = calc;
            return *this;
        }
        
        request& api_key(const std::string& key)
        {
            params.update(params._api_key != key);
            
            params._api_key = key;
            return *this;
        }
        
        request& as(result_kind data_kind)
        {
            params.update(params._fetch_as != data_kind);
            
            params._fetch_as = data_kind;
            return *this;
        }
        
        const std::string& database() const
        {
            return params._database_code;
        }
        
        const std::string& dataset() const
        {
            return params._dataset_code;
        }
        
        size_t limit() const
        {
            return params._limit;
        }
        
        size_t column() const
        {
            return params._column_index;
        }
        
        const std::string& begin() const
        {
            return params._start_date;
        }
        
        const std::string& end() const
        {
            return params._end_date;
        }
        
        sort_order order() const
        {
            return params._order;
        }
        
        sampling_freq timeframe() const
        {
            return params._timeframe;
        }
        
        calc_kind transform() const
        {
            return params._transform;
        }
        
        const std::string& api_key() const
        {
            return params._api_key;
        }
        
        std::string url() const
        {
            return form_request(params);
        }
        
        template <typename CallBack>
        auto with(CallBack cb)
        {
            return std::async(std::launch::deferred,
            [=] {
                return cb(make().get());
            });
        }
        
        template <typename CallBack, typename FallBack>
        auto with_or(CallBack cb, FallBack fb)
        {
            return with([=](response& resp)
            {
                if (resp.empty() || resp.fail())
                    return fb(resp);
                else
                    return cb(resp);
            });
        }
        
        std::future<response&> make()
        {
            if (params.updated())
                cached_resp = response{ fetch_raw(url()) };
            
            return std::async(std::launch::async,
            [&] () -> response& {
                return cached_resp;
            });
        }
        
    private:
        request_params params;
        response cached_resp { "" };
    };
}

#endif /* ql_request_hpp */
