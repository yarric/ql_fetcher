# A lightweight C++ library for downloading data from [Quandl](https://www.quandl.com)

## Installing

To install add the [ql_fetcher](https://raw.githubusercontent.com/yarric/ql_fetcher/master/release/ql_fetcher) to your project headers directory. You also have to link the [libcurl](https://curl.haxx.se/libcurl/) library to your project.

## Basic usage example

Let’s fetch 100 Low price points in ascending order of the EDF2017 future from Quandl’s CME data source and parse them with [json](https://github.com/nlohmann/json):

```c++
#define  QL_FETCHER_IMPL                            // define only once in your project
#include "ql_fetcher"
#include "json.hpp"                                 // see the link above
#include <iostream>
                                       
int main() 
{                                  
   using json = nlohmann::json;                     // for convinience 
   namespace ql = ql_fetcher;
   
   // ql::set_api_key(VALID_API_KEY);               // set the Quandl API key if you have one
   
   auto parsed_resp = ql::fetch("EDF2017", "CME")   // returns a ql::request
      .limit(100).column(3).order(ql::asc)
      .with([](ql::response& r)
      {
         return json::parse(r.str());
      }).get();                                     
                                                                       
   std::cout << " \n Some data point: "
      << parsed_resp["dataset"]["data"][4].dump() << std::endl;
                                 
   return 0; 
}
```

The output is:

```Some data point: ["2016-09-09",99.025]```

Here ```ql::fetch("EDF2017", “CME")``` creates a [```request```](https://github.com/yarric/ql_fetcher/blob/master/src/request.hpp) for the EDF2017 future from CME limited to 100 time points (by ```limit(100)```) of the Low price (specified by ```column(4)```) in ascending order (specified by ```order(ql::asc)```). The ```with(callback)``` call wraps the provided callback into a ```std::future<CallBackReturnType>``` which actually performs the request over the network and passes the response (either as a copy or reference depending on the type of the argument) as the argument of the callback. The call to ```str()``` method returns the data downloaded from Quandl and stored in [```response```](https://github.com/yarric/ql_fetcher/blob/master/src/response.hpp) as plain text which is then converted to JSON by ```json::parse(s)```, returned and stored in ```parsed_resp```.

All request parameters except for the name of the data set and the data source are optional and can be set in any order. For more information see the description of request parameters below.

## Description of request parameters

NOTE: the ```req``` is an object of the ```request``` class.

Parameter | Description | Usage | Values | [Corresponding Quandl parameter](https://docs.quandl.com/docs/parameters-2)
-- | -- | -- | -- | --
database | Identifier of the database | ```req.database(“CME”)``` | String type | database_code
dataset | Identifier of the returned dataset  | ```req.dataset(“EDF2018”)``` | String type | dataset_code
limit | Number of time points returned | ```req.limit(100)``` | Unsigned integer type | limit
column | Index of the returned column | ```req.column(4)``` | Unsigned integer type | column_index
begin | First date of the returned data | ```req.begin(“2015-12-25”)``` | Date string *yyyy-mm-dd* | start_date
end | Last date of the returned data | ```req.end(“2016-02-15”)``` | Date string *yyyy-mm-dd* | end_date
order | Ordering of the returned data with respect to date | ```req.order(ql::asc)``` | ```ql::desc```, ```ql::asc``` | order
timeframe | Sampling frequency of the returned data | ```req.timeframe(ql::weekly)``` | ```ql::none```, ```ql::daily```, ```ql::weekly```, ```ql::monthly```, ```ql::quarterly```, ```ql::annual``` | collapse
transform | Calculation to be performed on the data before downloading | ```req.transform(ql::diff)``` | ```ql::no_calc``` *(none)*, ```ql::diff```, ```ql::rdiff```, ```ql::rdiff_from```, ```ql::cumul```, ```ql::normalize``` | transform
api_key | The Quandl API key used for the request | ```req.api_key(“VALID_KEY”)``` | String type | api_key
