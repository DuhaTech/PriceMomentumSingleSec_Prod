#ifndef MARKETDATAPOINT_HPP
#define MARKETDATAPOINT_HPP

#include <string>

using namespace std;

namespace ds
{
	enum CriticalPoint {High = 1, Low = -1, Neutral = 0};

	struct MarketDataPoint
	{
		private:
			long int timeStamp;
			string timeStr;
			float quotePrice;
			CriticalPoint highLow;
		public:
			void SetTimeStamp(long int timeStamp_);
			void SetQuotePrice(float quotePrice_);
			void SetHighLow(CriticalPoint highLow_);
			void SetTimeStr(string timeStr_);
			long int GetTimeStamp() const;
			float GetQuotePrice() const;
			CriticalPoint GetHighLow() const;
			string GetTimeStr() const;
	};

}

#endif // MARKETDATAPOINT_HPP
