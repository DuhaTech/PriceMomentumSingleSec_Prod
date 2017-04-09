#ifndef RHAPICPP_HPP2
#define RHAPICPP_HPP2

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <curl/curl.h>
#include <iostream>
#include <memory>
#include <fstream>
#include <boost/algorithm/string/regex.hpp> //for splitting a string
#include <boost/algorithm/string.hpp> //for splitting a string
#include <rapidjson/document.h>
#include "RHApiCPP.hpp"

using namespace rapidjson;
using namespace Robinhood;

namespace Robinhood
{


    class RHApiCPP2
    {
        public:
             std::string token;
             std::string account;
             CURL *curl;
             CURLcode resCode;
             bool authentication = false;

        public:
             void Login();
             std::string GetQuote(std::string symbol);

             void Init();
             void init_string(struct response *s);
             static size_t writefunc(void *ptr, size_t size, size_t nmemb,  struct response *s);

            //static std::string GetQuote(std::string ticker);
            //static size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s);
            //RobinhoodAPI();


    };
}
#endif // ROBINHOODAPI_HPP

