//
//  utils.hpp
//  ql_fetcher
//
//  Created by Yarrik Shkurat on 03/10/2017.
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

#ifndef ql_utils_hpp
#define ql_utils_hpp

#include "request.hpp"
#include <string>

namespace ql_fetcher
{
    request fetch(const std::string& data_code, const std::string& source_code);
    
    void set_api_key(const std::string&);
    
    std::string& get_api_key();
}



#ifdef QL_FETCHER_IMPL
namespace ql_fetcher
{
    std::string* _global_api_key = new std::string( "" );
    
    request fetch(const std::string& data_code, const std::string& source_code)
    {
        return request{data_code, source_code};
    }
    
    void set_api_key(const std::string& api_key)
    {
        *_global_api_key = api_key;
    }
    
    std::string& get_api_key()
    {
        return *_global_api_key;
    }
}
#endif // QL_FETCHER_IMPL

#endif /* ql_utils_hpp */

