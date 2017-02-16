#ifndef BROKERUTIL_HPP
#define BROKERUTIL_HPP

namespace util
{
    extern float rhSellingFee; //selling fees from Robinhood
    extern int sharesEachTrade;

    enum PositionStatus
    {
        ReadyForNewTrade = 1,
        ProcessingNewTrade = 2,
        CloseLongPosition = 3,
        CloseShortPosition = 4,
    };
}

#endif // BROKERUTIL_HPP
