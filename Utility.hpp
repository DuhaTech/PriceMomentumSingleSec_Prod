#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <ctime>
#include <string>

namespace utility
{
    class Utility
    {
    private:
        time_t currentTimeT;
        struct tm currentTimeStruct;
    public:
        std::string GetCurrentDateTimeStr();
    };
}

#endif // UTILITY_HPP
