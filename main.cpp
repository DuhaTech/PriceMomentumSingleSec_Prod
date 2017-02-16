#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <curl/curl.h>
#include <iostream>
#include <memory>
#include <fstream>
#include "RHApiCPP.hpp"
#include <Python.h>
#include "RHApiPy.hpp"
#include <unistd.h> //for sleep function
#include <cmath> //math functions
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string.hpp>
#include "PriceMomentumSingleSec.hpp"
#include <thread>

using namespace Robinhood;
using namespace strategy;
using namespace std;
using namespace rapidjson;

int main(void)
{
//RHApiPy api;
//std::cout<<api.GetBuyingPower()<<endl;


    unsigned int bo_len = 10*60; //number of minutes during which the price has been moving in the same direction
    float chgspd_trg = 0.007 / 15.00 / 60.00;
    int negResponse_trg = 5;
    float retTrg = 0.1/100;
    int timeWindowForExit = 10*60; //the wider this window, the more risky is the strategy
    long int secToFormStrategy = 10*60;
    PriceMomentumSingleSec priceMom("AVP",bo_len, chgspd_trg, secToFormStrategy,  negResponse_trg,  retTrg,  timeWindowForExit);
    //priceMom.ExecTrade();

    //priceMom.ExecTrade();
    //RHApiPy py;

    std::thread strategyMain(&PriceMomentumSingleSec::ExecTrade, &priceMom);
    std::thread strategySetMarketData(&PriceMomentumSingleSec::SetMktData, &priceMom);
    std::thread strategyOpenNewPosition(&PriceMomentumSingleSec::OpenNewPosition, &priceMom);
    std::thread strategyCloseLongPosition(&PriceMomentumSingleSec::CloseLongPosition, &priceMom);

    strategyMain.join();
  strategySetMarketData.join();
    strategyOpenNewPosition.join();
    strategyCloseLongPosition.join();


  getchar();


  return 0;
}
