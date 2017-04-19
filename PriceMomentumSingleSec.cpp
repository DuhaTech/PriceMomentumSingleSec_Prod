#include "PriceMomentumSingleSec.hpp"

//std::mutex PriceMomentumSingleSec::mutex_;
//atomic<int> PriceMomentumSingleSec::timeWindowForExit;
//atomic<float> PriceMomentumSingleSec::entryQuant(0);
//atomic<double> PriceMomentumSingleSec::entryPrice(0);
//atomic<PositionStatus> PriceMomentumSingleSec::posStatus(ReadyForNewTrade);
//atomic<float> PriceMomentumSingleSec::orderPrice(0);
//PriceMomentumSingleSec::PriceMomentumSingleSec(){}

PriceMomentumSingleSec::PriceMomentumSingleSec(string ticker_, unsigned int bo_len_, float chgspd_,float chgdur_,int negResponse, float retTrg_, int timeWindowForExit_):
    ticker(ticker_),bo_len(bo_len_),chgspd_trg(chgspd_), chgdur_trg(chgdur_),negResponse_trg(negResponse), retTrg(retTrg_), timeWindowForExit(timeWindowForExit_),
    mktData(), cleanEntryVect(),side(""), type("limit"),time_in_force("gfd"), trigger("immediate"),entryVect()
{
    instrumentId = pyRobinAPI.GetInstrument(ticker);
    posStatus = ReadyForNewTrade;
}

PriceMomentumSingleSec::PriceMomentumSingleSec(const PriceMomentumSingleSec& obj)
{

}

PriceMomentumSingleSec::PriceMomentumSingleSec(string ticker_)
{
    robinhoodAPI.Init();
    ticker = ticker_;

    //read config data
    //std::cout<<"read config file"<<endl;
    ifstream configFile;

    pythonMutex.lock();
    configFile.open("config.txt");

//    configFile.seekg(0,configFile.end);
//    int len = configFile.tellg();
//    configFile.seekg(0,configFile.beg);
//    char* buffer = new char[len];
//    configFile.read(buffer,len);

    std::string content( (std::istreambuf_iterator<char>(configFile) ),
                         (std::istreambuf_iterator<char>()    ) );

    configFile.close();
    pythonMutex.unlock();

    Document configs;
    configs.Parse(content.c_str());
    //std::cout<<content<<endl;
    // delete[] buffer;
    //std::cout<<"close config file"<<endl;


    //initialize configurable variables
    //ticker = configs["ticker"].GetString();
    chgspd_trg = configs["targetchangespeed"].GetFloat();
    chgdur_trg = configs["targetchangeduration"].GetFloat();
    retTrg = configs["targetreturn"].GetFloat();
    timeWindowForExit = configs["timewindowforexit"].GetInt();

    //instrumentId = pyRobinAPI.GetInstrument(ticker);
    posStatus = ReadyForNewTrade;

}

void PriceMomentumSingleSec::StgSta()
{
}

void PriceMomentumSingleSec::Run()
{
    std::thread strategyMain(&PriceMomentumSingleSec::ExecTrade, this);
    std::thread strategyFetchMktData(&PriceMomentumSingleSec::FetchMktData, this);
    //std::thread strategyOpenNewPosition(&PriceMomentumSingleSec::OpenNewPosition, this);
    //std::thread strategyCloseLongPosition(&PriceMomentumSingleSec::CloseLongPosition, this);

    strategyMain.join();
    strategyFetchMktData.join();
    //strategyOpenNewPosition.join();
    //strategyCloseLongPosition.join();

}

void PriceMomentumSingleSec::ExecTrade()
{
    while(mktData.size() <= 2)
    {
        usleep(10000000);
    }

    string date_benchM = "";
    string date = "";
    string dateTime = "";
    float preHigh = std::numeric_limits<float>::min();
    float preLow = std::numeric_limits<float>::max();
    float curHigh = 0;
    float curLow = 0;
    float price = 0;
    float price_TrendBegin = 0;
    long int preTimeStamp = 0;
    long int timeStamp_trendBegin = 0;
    long int timeStamp;
    long int timeStamp_secondLast;
    long int preCriticalTS = 0;
    long int curCriticalTS = 0;
    CriticalPoint highLow = Neutral;
    string preTrend = "" ;
    string curTrend = "";
    bool sameDay = false;
    long int dur = 0;
    float chgspd = 0.0;
    float prePrice = 0.0;
    float entryOrderPrice = 0;
    float buyingPower = 0;
    ofstream tradeLog;
    list<MarketDataPoint>::iterator it_first = mktData.begin();
    list<MarketDataPoint>::iterator it_secondLast;
    list<MarketDataPoint>::iterator it_last;
    CriticalPoint highLow_secondLast = Neutral;
    float price_SecondLast = 0.0;
    string fileName = ticker + "tradeLog.txt";

    tradeLog.open(fileName,ios::app);
    price_TrendBegin = it_first->GetQuotePrice();
    prePrice = it_first->GetQuotePrice();
    timeStamp_trendBegin = it_first->GetTimeStamp();
    date_benchM = (it_first->GetTimeStr()).substr(0,10);
    preLow = price_TrendBegin;
    preHigh = price_TrendBegin;

    //tradeLog<<timeStamp<<";"<<price_TrendBegin<<";"<<curTrend<<";"<<preTrend<<";"<<preHigh<<";"<<preLow<<";"<<preCriticalTS<<";"<<preTimeStamp<<";"<<price_TrendBegin<<"\n";
    //tradeLog<<std::flush;

    list<MarketDataPoint>::iterator it_second = std::next(it_first);
    if(it_second->GetQuotePrice() >= prePrice)
    {
        preTrend = "UP";
        curTrend = "UP";
        prePrice = it_second->GetQuotePrice();
    }
    else if(it_second->GetQuotePrice() <= prePrice)
    {
        preTrend = "DOWN";
        curTrend = "DOWN";
        prePrice = it_second->GetQuotePrice();
    }
    preTimeStamp = it_second->GetTimeStamp();

    //tradeLog<<it_second->GetTimeStamp()<<";"<<it_second->GetQuotePrice()<<";"<<curTrend<<";"<<preTrend<<";"<<preHigh<<";"<<preLow<<";"<<preCriticalTS<<";"<<preTimeStamp<<";"<<price_TrendBegin<<"\n";
    //tradeLog<<std::flush;

    while(1)
    {
        usleep(3000000);
        //auto mkIt = std::prev(mktData.end());
        //MarketDataPoint it = mktData.back();
        it_secondLast = prev(prev(mktData.end()));
        highLow_secondLast = it_secondLast->GetHighLow();
        price_SecondLast = it_secondLast->GetQuotePrice();
        timeStamp_secondLast = it_secondLast->GetTimeStamp();
        if(highLow_secondLast == 1)
        {
            preHigh = price_SecondLast;
            preCriticalTS = timeStamp_secondLast;
        }
        else if(highLow_secondLast == -1)
        {
            preLow = price_SecondLast;
            preCriticalTS = timeStamp_secondLast;
        }
        it_last = std::prev(mktData.end());
        dateTime = it_last->GetTimeStr();
        date = dateTime.substr(0,10);
        timeStamp = it_last->GetTimeStamp();
        highLow = it_last->GetHighLow();
        price = it_last->GetQuotePrice();
        buyingPower = 10;

        if(timeStamp > preTimeStamp)
        {
            //if price is between preHigh and preLow, the curTrend will stay what it is so far.
            if(price >= preHigh)
            {
                curTrend = "UP";
                //curHigh = price;
                //curCriticalTS = timeStamp;
            }
            //else if(highLow == -1 && price > preLow){
            //curTrend = "UP";
            //curLow = price;
            //curCriticalTS = timeStamp;
            //}
            //else if (highLow == 1 && price < preHigh){
            // curTrend = "DOWN";
            // curHigh = price;
            //curCriticalTS = timeStamp;
            // }
            else if (price <= preLow)
            {
                curTrend = "DOWN";
                //curLow = price;
                //curCriticalTS = timeStamp;
            }
            dur = timeStamp - timeStamp_trendBegin;
            chgspd = (price - price_TrendBegin)/price_TrendBegin/float(dur);

            tradeLog<<timeStamp<<";"<<price<<";"<<curTrend<<";"<<preTrend<<";"<<preHigh<<";"<<preLow<<";"<<preCriticalTS<<";"<<preTimeStamp<<";"<<price_TrendBegin<<"\n";
            tradeLog<<std::flush;
            //assert(preTrend != "" && curTrend != "");
            if(curTrend == preTrend && curTrend == "UP" && dur > chgdur_trg && chgspd > chgspd_trg && posStatus == ReadyForNewTrade && buyingPower >= price * util::sharesEachTrade)
            {
                //OpenNewPosition("buy","limit","gfd","immediate",price);
                side = "buy";
                posStatus = ProcessingNewTrade;
                orderPrice = price;
                tradeLog<<timeStamp<<";"<<side<<";"<<price<<util::sharesEachTrade<<"\n";
                tradeLog<<std::flush;
            }
            else if(curTrend == preTrend && curTrend == "DOWN" && dur > chgdur_trg && chgspd < -chgspd_trg)
            {
                //short selling is currently not available
                //entryVect.push_back(std::make_tuple("Short", dateTime, timeStamp, price,false));
                tradeLog<<timeStamp<<";"<<"short"<<";"<<price<<util::sharesEachTrade<<"\n";
                tradeLog<<std::flush;
            }
            //else if(highLow == 0 && dur > chgdur_trg && chgspd > chgspd_trg && posStatus == ReadyForNewTrade && buyingPower >= price * util::sharesEachTrade){
            //OpenNewPosition("buy","limit","gfd","immediate",price);
            //side = "buy";
            //posStatus = ProcessingNewTrade;
            //orderPrice = price;
            //tradeLog<<timeStamp<<";"<<side<<";"<<price<<util::sharesEachTrade<<"\n";
            //tradeLog.close();
            //}
            //else if(highLow == 0 && dur > chgdur_trg && chgspd < -chgspd_trg){
            //short selling is currently not available
            //entryVect.push_back(std::make_tuple("Short", dateTime, timeStamp, price,false));
            //}
            else if(preTrend != curTrend && curTrend == "UP")
            {
                price_TrendBegin = preLow;
                //preHigh = price_TrendBegin;
                timeStamp_trendBegin = preCriticalTS;
                preTrend = curTrend;
            }
            else if(preTrend != curTrend && curTrend == "DOWN")
            {
                price_TrendBegin = preHigh;
                //preLow = price_TrendBegin;
                timeStamp_trendBegin = preCriticalTS;
                preTrend = curTrend;
            }

            //preCriticalTS = curCriticalTS;
            //preHigh = curHigh;
            //preLow = curLow;
            preTimeStamp = timeStamp;
        }
    }
    tradeLog.close();
}

void PriceMomentumSingleSec::OpenNewPosition()
{
    unique_ptr<Document> orderDetail(new Document());
    string orderId = "";
    std::clock_t start;
    clock_t end_;
    double duration = 0;
    string orderState ="";

    while(1)
    {
        usleep(100000);

        if(posStatus == ProcessingNewTrade)
        {

            std::cout<<"Processing New Trade"<<std::endl;
            //mutex_.lock();
            type = "limit";
            time_in_force = "gfd";
            trigger = "immediate";
            //mutex_.unlock();
            orderDetail.reset();
            orderDetail = robinhoodAPI.PlaceOrder(ticker,util::sharesEachTrade,side,type,time_in_force,trigger,orderPrice,instrumentId);
            orderId = orderDetail->operator[]("id").GetString();
            //vector<string> orderResponseList;
            //boost::split(orderResponseList, orderResponse, boost::is_any_of(","));
            //string orderId = (orderResponseList[5]).substr(6, 36);;
            start = clock();
            //orderDetail.reset(robinhoodAPI.GetOrderStatus(orderId).get());
// = (orderDetail)->operator[]("state").GetString();

            /** wait 10 seconds for the order to be filled.*/
            usleep(30000000);

            /** when the program comes here, it is either the order has been filled
            or timed out for filling the order. For either case, we will send a canceling
            order.
            */
            /**if order is not filled after 10 seconds, cancel the order*/
            //robinhoodAPI.CancelOrder(orderId);
            orderDetail.reset();
            orderDetail = robinhoodAPI.CancelOrder(orderId);
            orderState = orderDetail->operator[]("state").GetString();
            entryQuant = orderDetail->operator[]("cumulative_quantity").GetDouble();

            //set current portfolio status
            if(entryQuant > 0 && side == "buy")
            {
                posStatus = util::CloseLongPosition;
                entryPrice = orderDetail->operator[]("average_price").GetDouble();
            }
            else if (entryQuant > 0 && side == "sell")
            {
                posStatus = util::CloseShortPosition;
                entryPrice = orderDetail->operator[]("average_price").GetDouble();
            }
            else if(entryQuant == 0.0)
            {
                posStatus = ReadyForNewTrade;
            }
        }
    }
}

/** a trade must be filled in order to close a position. This method will keep looping until a trade
has been filled to close a position.*/
void PriceMomentumSingleSec::CloseLongPosition()
{
    while(1)
    {
        usleep(100000);
        if(posStatus == util::CloseLongPosition)
        {


            std::cout<<"Closing long position"<<std::endl;
            //mutex_.lock();
            type = "limit";
            time_in_force = "gfd";
            trigger = "immediate";
            //mutex_.unlock();

            //sleep for one minute before closing out a position
            //usleep(60000000);
            //SetMktData();
            //auto it = prev(mktData.end());
            //float price = it->GetQuotePrice();
            clock_t tradeStart = clock();
            clock_t tradeEnd;
            float tradeDur = 0;
            int tradeStatus = 0; //a trade could have multiple orders, including those unsuccessful one, canceled ones.
            //string orderResponse;// = robinhoodAPI.PlaceOrder(ticker,util::sharesEachTrade,direction,type,life,delay,price,instrumentId);
            vector<string> orderResponseList;
            //boost::split(orderResponseList, orderResponse, boost::is_any_of(","));
            string orderId;// = (orderResponseList[5]).substr(6, 36);;
            std::clock_t orderStart;// = clock();
            clock_t orderEnd;
            float orderDur = 0;
            string orderState = "";// = robinhoodAPI->GetOrderStatus(orderId);
            std::unique_ptr<Document> quotes(new Document());
            float latestBidPrice = 0.0;
            unique_ptr<Document> orderDetail(new Document());
            float latestFilledQuant = 0;
            float cumulativeFilledQuant = 0;
            float ret = 0;

            while(ret < retTrg && tradeDur <= timeWindowForExit)
            {
                //fetch latest bid price data
                quotes.reset();
                quotes = pyRobinAPI.GetQuote(ticker);
                latestBidPrice = stof(quotes->operator[]("bid_price").GetString());

                /**calculate return*/
                ret = (latestBidPrice - entryPrice) / entryPrice;

                tradeEnd = clock();
                tradeDur = tradeEnd - tradeStart;

            }
            /**place an order to close the position if the return is
            higher the targeted return; or the time out for filling the order*/
            while(cumulativeFilledQuant < entryQuant)
            {
                orderDetail = robinhoodAPI.PlaceOrder(ticker,entryQuant - cumulativeFilledQuant,"sell",type,time_in_force,trigger,latestBidPrice,instrumentId);
                orderId = orderDetail->operator[]("id").GetString();
                orderDetail.reset();
                orderDetail = robinhoodAPI.GetOrderStatus(orderId);
                orderState = orderDetail->operator[]("state").GetString();
                latestFilledQuant = orderDetail->operator[]("cumulative_quantity").GetDouble();
                orderStart = clock();

                /* wait for 10 seconds to fill the order */
                usleep(10000000);
                orderDetail.reset();
                orderDetail = robinhoodAPI.CancelOrder(orderId);
                latestFilledQuant = orderDetail->operator[]("cumulative_quantity").GetDouble();
                cumulativeFilledQuant += latestFilledQuant;

                //fetch latest bid price data
                quotes.reset(pyRobinAPI.GetQuote(ticker).get());
                latestBidPrice = quotes->operator[]("bid_price").GetDouble();
            }

            posStatus = ReadyForNewTrade;
            entryQuant = 0;
        }
    }
}

void PriceMomentumSingleSec::FetchMktData()
{

    list<MarketDataPoint>::iterator lastIt;
    list<MarketDataPoint>::iterator secondLastIt;
    float curQuote;
    float lastQuote;
    float secondLastQuote;
    long int curTimeStamp = 0;
    long int lastTimestamp = 0;
    long int secondLastTimestamp = 0;
    long int lastCriticalTimestamp = 0;
    long int criticalPointSpan = 0;
    bool newData = false;
    int mktDataLen = mktData.size();
    int hour = 0;
    int minute = 0;
    int sec  = 0;
    string dateTime = "";
    shared_ptr<Document> quotes(new rapidjson::Document());
    MarketDataPoint dataP;
    ofstream dataLog;
    dataLog.open("data.txt", ios::app);
    //std::cout<<"mark"<<std::endl;

    while(1)
    {
        quotes.reset();

        //pythonMutex.lock();
        //std::lock_guard<std::mutex> lock(pythonMutex);
        //std::cout<<ticker<<std::endl;
        quotes = robinhoodAPI.GetQuote(ticker);
        //std::cout<<ticker<<": "<<quotes->operator[]("last_trade_price").GetString()<<std::endl;
        //std::cout<<"land mark"<<std::endl;
        //pythonMutex.unlock();

        mktDataLen = mktData.size();
        curQuote = stof(quotes->operator[]("last_trade_price").GetString());
        dateTime = quotes->operator[]("updated_at").GetString();
        hour = stoi(dateTime.substr(11,2));
        minute = stoi(dateTime.substr(14,2));
        sec = stoi(dateTime.substr(17,2));
        curTimeStamp = hour * 3600 + minute * 60 + sec;

        //If there is already data in the list, not include the new data point if it has the same
        //timestamp as last data point in existing data list.
        if(mktDataLen > 2)
        {
            lastIt  = prev(mktData.end());
            lastTimestamp = lastIt->GetTimeStamp();

            if(curTimeStamp > lastTimestamp + 50)
            {
                newData = true;
                dataP.SetQuotePrice(curQuote);
                dataP.SetTimeStamp(curTimeStamp);
                dataP.SetTimeStr("");
                dataP.SetHighLow(Neutral);
            }
        }
        //If there is no data in the list, always add the first data.
        else if(mktDataLen <= 2 && curTimeStamp > lastTimestamp + 50)
        {
            dataP.SetQuotePrice(curQuote);
            dataP.SetTimeStamp(curTimeStamp);
            dataP.SetTimeStr("");
            dataP.SetHighLow(Neutral);
            mktData.push_back(dataP);
            lastTimestamp = curTimeStamp;
            dataLog<<ticker<<";"<<curTimeStamp<<";"<<curQuote<<";"<<0<<"\n";
            dataLog<<std::flush;
        }

        //If there are at least three data points in the market data list,
        //update the high/low of second last data point. The high/low of last
        //data point is always neutral.
        if(mktDataLen > 2 && newData == true)
        {
            secondLastIt = prev(lastIt);
            lastQuote = lastIt->GetQuotePrice();
            secondLastQuote = secondLastIt->GetQuotePrice();
            lastTimestamp = lastIt->GetTimeStamp();
            secondLastTimestamp = secondLastIt->GetTimeStamp();
            criticalPointSpan = lastTimestamp - lastCriticalTimestamp;

            if (curQuote >= lastQuote && lastQuote < secondLastQuote && criticalPointSpan > 50)
            {
                lastIt->SetHighLow(Low);
                lastCriticalTimestamp = curTimeStamp;
            }
            else if (curQuote > lastQuote && lastQuote <= secondLastQuote && criticalPointSpan > 50)
            {
                lastIt->SetHighLow(Low);
                lastCriticalTimestamp = curTimeStamp;
            }
            else if (curQuote < lastQuote && lastQuote >= secondLastQuote && criticalPointSpan > 50)
            {
                lastIt->SetHighLow(High);
                lastCriticalTimestamp = curTimeStamp;
            }
            else if (curQuote <= lastQuote && lastQuote > secondLastQuote && criticalPointSpan > 50)
            {
                lastIt->SetHighLow(High);
                lastCriticalTimestamp = curTimeStamp;
            }
            mktData.push_back(dataP);
            dataLog<<ticker<<";"<<lastTimestamp<<";"<<lastQuote<<";"<<lastIt->GetHighLow()<<"\n";
            dataLog<<std::flush;
            newData = false;
        }
        //Sleep for 60 seconds before fetching a new market data.
        //Due to Robinhood constraint, only one data request can be made
        //every 40 seconds.

        usleep(30000000);
    }
    dataLog.close();
}

PriceMomentumSingleSec::~PriceMomentumSingleSec()
{
    log.close();
    //cout<<"********************strategy destroy*************************"<<endl;
}


void PriceMomentumSingleSec::DetectStg() {}
void PriceMomentumSingleSec::Output() {}
void PriceMomentumSingleSec::CleanEntryPoint() {}
void PriceMomentumSingleSec::CleanMktData() {}

void PriceMomentumSingleSec::FetchMktData2()
{
    list<MarketDataPoint>::iterator lastIt;
    list<MarketDataPoint>::iterator secondLastIt;
    float curQuote;
    float lastQuote;
    float secondLastQuote;
    long int curTimeStamp;
    long int lastTimestamp = 0;
    long int secondLastTimestamp;
    long int lastCriticalTimestamp = 0;
    long int criticalPointSpan = 0;
    bool newData = false;
    int mktDataLen = mktData.size();
    int hour = 0;
    int minute = 0;
    int sec  = 0;
    string dateTime = "";
    unique_ptr<Document> quotes(new rapidjson::Document());
    MarketDataPoint dataP;
    string fileName = ticker + "data.txt";
    ofstream dataLog;
    dataLog.open(fileName, ios::app);
    ifstream file;
    file.open("AVP.csv");


    while(!file.eof())
    {
        string line;
        getline(file, line);
        //std::cout<<line<<endl;
        vector<string> lineSplit;
        boost::split(lineSplit, line, boost::is_any_of(","));
        mktDataLen = mktData.size();
        //curQuote = stof(quotes->operator[]("last_trade_price").GetString());
        curQuote = stof(lineSplit[4]);
        //dateTime = quotes->operator[]("updated_at").GetString();
        //hour = stoi(dateTime.substr(11,2));
        //minute = stoi(dateTime.substr(14,2));
        // sec = stoi(dateTime.substr(17,2));
        curTimeStamp = stoi(lineSplit[0]);
        //cout<<curTimeStamp<<endl;

        //If there is already data in the list, not include the new data point if it has the same
        //timestamp as last data point in existing data list.
        if(mktDataLen > 2)
        {
            lastIt  = prev(mktData.end());
            lastTimestamp = lastIt->GetTimeStamp();

            if(curTimeStamp > lastTimestamp)
            {
                //cout<<"here"<<endl;
                newData = true;
                dataP.SetQuotePrice(curQuote);
                dataP.SetTimeStamp(curTimeStamp);
                dataP.SetTimeStr("");
                dataP.SetHighLow(Neutral);
            }
        }
        //If there is no data in the list, always add the first data.
        else if(mktDataLen <= 2)
        {
            dataP.SetQuotePrice(curQuote);
            dataP.SetTimeStamp(curTimeStamp);
            dataP.SetTimeStr("");
            dataP.SetHighLow(Neutral);
            mktData.push_back(dataP);
            //dataLog<<curTimeStamp<<";"<<curQuote<<"\n";
            //dataLog<<std::flush;
            dataLog<<curTimeStamp<<";"<<curQuote<<";"<<0<<"\n";
            dataLog<<std::flush;
        }

        //If there are at least three data points in the market data list,
        //update the high/low of second last data point. The high/low of last
        //data point is always neutral.
        //std::cout<<mktDataLen<<endl;
        if(mktDataLen > 2 && newData == true)
        {
            //std::cout<<123<<endl;
            secondLastIt = prev(lastIt);
            lastQuote = lastIt->GetQuotePrice();
            secondLastQuote = secondLastIt->GetQuotePrice();
            lastTimestamp = lastIt->GetTimeStamp();
            secondLastTimestamp = secondLastIt->GetTimeStamp();
            criticalPointSpan = lastTimestamp - lastCriticalTimestamp;

            if (curQuote >= lastQuote && lastQuote < secondLastQuote && criticalPointSpan > 50)
            {
                lastIt->SetHighLow(Low);
                lastCriticalTimestamp = curTimeStamp;
            }
            else if (curQuote > lastQuote && lastQuote <= secondLastQuote && criticalPointSpan > 50)
            {
                lastIt->SetHighLow(Low);
                lastCriticalTimestamp = curTimeStamp;
            }
            else if (curQuote < lastQuote && lastQuote >= secondLastQuote && criticalPointSpan > 50)
            {
                lastIt->SetHighLow(High);
                lastCriticalTimestamp = curTimeStamp;
            }
            else if (curQuote <= lastQuote && lastQuote > secondLastQuote && criticalPointSpan > 50)
            {
                lastIt->SetHighLow(High);
                lastCriticalTimestamp = curTimeStamp;
            }

            //dataLog<<curTimeStamp<<";"<<curQuote<<"\n";
            // dataLog<<std::flush;
            dataLog<<lastTimestamp<<";"<<lastQuote<<";"<<lastIt->GetHighLow()<<"\n";
            dataLog<<std::flush;
            mktData.push_back(dataP);
        }
        //Sleep for 60 seconds before fetching a new market data.
        //Due to Robinhood constraint, only one data request can be made
        //every 3 seconds.
        usleep(10000000);
    }
    dataLog.close();
}
