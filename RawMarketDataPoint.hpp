#ifndef RAWMARKETDATAPOINT_HPP
#define RAWMARKETDATAPOINT_HPP

namespace ds
{
    class RawMarketDataPoint
    {
    private:
        int year_;
        int month_;
        int day_;
        int hour_;
        int minute_;
        int second_;
        float quote_;
    public:
        void SetYear(int);
        void SetMonth(int);
        void SetDay(int);
        void SetHour(int);
        void SetMin(int);
        void SetSec(int);
        void SetQuote(float qutoe);
        int GetHour() const;
        int GetMon() const;
        int GetMin() const;
        int GetSec() const;
        float GetQuote() const;
    };
}

#endif // RAWMARKETDATAPOINT_HPP
