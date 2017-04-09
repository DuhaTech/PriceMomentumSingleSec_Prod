#ifndef STRATEGY_HPP
#define STRATEGY_HPP
#include <string>

using namespace std;

namespace strategy
{
    class Strategy
    {
        public:
            virtual void DetectStg() = 0;
			virtual void StgSta() = 0;
			virtual void Run() = 0;
			virtual void Output() = 0;
			virtual void FetchMktData() = 0;
			virtual void CleanEntryPoint() = 0;
			virtual void CleanMktData() = 0;
			virtual void ExecTrade() = 0;
    };
}

#endif // STRATEGY_HPP
