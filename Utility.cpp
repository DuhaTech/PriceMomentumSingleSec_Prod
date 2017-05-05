#include "Utility.hpp"

namespace utility
{
    std::string Utility::GetCurrentDateTimeStr()
    {
        currentTimeT = time(nullptr);
        gmtime_r(&currentTimeT, &currentTimeStruct);
         std::string currentTimeStr = std::to_string(currentTimeStruct.tm_mon + 1) + "/" + std::to_string(currentTimeStruct.tm_mday) +"/"
    +std::to_string(currentTimeStruct.tm_year + 1900 ) + " " + std::to_string(currentTimeStruct.tm_hour)+ ":" + std::to_string(currentTimeStruct.tm_min) + ":"
    +std::to_string(currentTimeStruct.tm_sec);
    return currentTimeStr;
    }

}

