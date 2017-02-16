#include "RawMarketDataPoint.hpp"

using namespace ds;

float RawMarketDataPoint::GetQuote() const
{
    return quote_;
}

int RawMarketDataPoint::GetHour() const
{
    return hour_;
}

int RawMarketDataPoint::GetMin() const
{
    return minute_;
}

int RawMarketDataPoint::GetSec() const
{
    return second_;
}

void RawMarketDataPoint::SetQuote(float quote)
{
    quote_ = quote;
}

void RawMarketDataPoint::SetHour(int hour)
{
    hour_ = hour;
}

void RawMarketDataPoint::SetMin(int minute)
{
    minute_ = minute;
}


void RawMarketDataPoint::SetSec(int sec)
{
    second_ = sec;
}

