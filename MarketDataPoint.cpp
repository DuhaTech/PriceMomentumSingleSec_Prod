#include "MarketDataPoint.hpp"

using namespace ds;


void MarketDataPoint::SetTimeStamp(long int timeStamp_){timeStamp = timeStamp_;}
void MarketDataPoint::SetQuotePrice(float quotePrice_){quotePrice = quotePrice_;}
void MarketDataPoint::SetHighLow(CriticalPoint highLow_){highLow = highLow_;}
void MarketDataPoint::SetTimeStr(string timeStr_){timeStr = timeStr_;}
long int MarketDataPoint::GetTimeStamp() const{return timeStamp;}
float MarketDataPoint::GetQuotePrice() const{return quotePrice;}
CriticalPoint MarketDataPoint::GetHighLow() const{return highLow;}
string MarketDataPoint::GetTimeStr() const {return timeStr;}
