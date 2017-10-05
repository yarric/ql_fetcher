//
//  request_params.hpp
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

#ifndef ql_request_params_h
#define ql_request_params_h

#include "vendor_specific.hpp"
#include <string>

namespace ql_fetcher
{
    typedef struct
    {
        std::string _database_code;
        std::string _dataset_code;
        
        size_t _limit{ 0 };
        size_t _column_index{ 0 };
        
        std::string _start_date;
        std::string _end_date;
        
        sort_order _order{ desc };
        
        sampling_freq _timeframe{ none };
        
        calc_kind _transform{ no_calc };
        
        std::string _api_key;
        
        result_kind _fetch_as{ json };
        
        void update(bool yes_no) { _updated |= yes_no; }
        
        bool updated() const { return _updated; }
        
    private:
        bool _updated{ true };
    } request_params;
    
    std::string form_request(const request_params&);
}

#endif /* ql_request_params_h */
