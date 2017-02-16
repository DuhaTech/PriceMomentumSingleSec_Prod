#ifndef STRATEGYEXECUTION_HPP
#define STRATEGYEXECUTION_HPP

#include "PriceMomentumSingleSec.hpp"

namespace strategy
{
    class StrategyExecution
    {
    protected:
        Strategy strategy;
        OrderEngine orderEngine;
    };
}


#endif // STRATEGYEXECUTION_HPP
