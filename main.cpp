#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <curl/curl.h>
#include <iostream>
#include <memory>
#include <fstream>
#include "RHApiCPP2.hpp"
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
RHApiCPP2 api;
api.Init();
std::cout<<api.GetQuote("AVP")<<"abc"<<endl;


    const unsigned int bo_len = 10*60; //number of minutes during which the price has been moving in the same direction
    const float chgspd_trg = 0.007 / 15.00 / 60.00;
    const int negResponse_trg = 5;
    const float retTrg = 0.1/100;
    const int timeWindowForExit = 10*60; //the wider this window, the more risky is the strategy
    const long int secToFormStrategy = 10*60;
    //PriceMomentumSingleSec priceMom("VXX",bo_len, chgspd_trg, secToFormStrategy,  negResponse_trg,  retTrg,  timeWindowForExit);


    ifstream file;
    file.open("tickerList.txt");
    file.seekg(0,file.end);
    int len = file.tellg();
    file.seekg(0,file.beg);
    char* buffer = new char[len];
    file.read(buffer,len);
    file.close();

    vector<string> tickerList;
    Document content;
    content.Parse(buffer);
    string tickers = content["ticker"].GetString();
    boost::split(tickerList,tickers, boost::is_any_of(","));
    delete[] buffer;

    vector<PriceMomentumSingleSec> objList;
    vector<thread*> threadList;

    //PriceMomentumSingleSec obj("AVP");
    //usleep(4000000);
    //PriceMomentumSingleSec obj2("CHK");
    //obj2.Run();
    //obj.Run();
    //std::thread t1(&PriceMomentumSingleSec::Run, &obj);
    //std::thread t2(&PriceMomentumSingleSec::Run, &obj2);
   // t1.join();
    //t2.join();

    for(auto it = tickerList.begin(); it != tickerList.end(); ++it)
    {
        std::cout<<*it<<endl;
        //PriceMomentumSingleSec* obj= new PriceMomentumSingleSec(*it);
        //objList.push_back(obj);
        //std::thread*  threadObj = new thread(&PriceMomentumSingleSec::Run, new PriceMomentumSingleSec(*it));
        //threadObj->join();
        //threadList.push_back(new thread(&PriceMomentumSingleSec::Run, new PriceMomentumSingleSec(*it)));
        //threadObj->join();
        //std::cout<<"end"<<endl;
    }

    for(auto it = threadList.begin(); it != threadList.end();++it)
    {
        //(*it)->join();
    }

    //vector<PriceMomentumSingleSec> objList;

    //priceMom.FetchMktData();
    //priceMom.ExecTrade();
    //RHApiPy py;

    /*
    std::thread strategyMain(&PriceMomentumSingleSec::ExecTrade, &priceMom);
    std::thread strategySetMarketData(&PriceMomentumSingleSec::SetMktData, &priceMom);
    std::thread strategyOpenNewPosition(&PriceMomentumSingleSec::OpenNewPosition, &priceMom);
    std::thread strategyCloseLongPosition(&PriceMomentumSingleSec::CloseLongPosition, &priceMom);

    strategyMain.join();
  strategySetMarketData.join();
    strategyOpenNewPosition.join();
    strategyCloseLongPosition.join();
    */


  getchar();


  return 0;
}
