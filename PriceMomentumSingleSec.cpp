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

void PriceMomentumSingleSec::StgSta()
{
}

void PriceMomentumSingleSec::Run()
{
    std::thread strategyMain(&PriceMomentumSingleSec::ExecTrade, this);
    std::thread strategySetMarketData(&PriceMomentumSingleSec::SetMktData, this);
    std::thread strategyOpenNewPosition(&PriceMomentumSingleSec::OpenNewPosition, this);
    std::thread strategyCloseLongPosition(&PriceMomentumSingleSec::CloseLongPosition, this);

    strategyMain.join();
    strategySetMarketData.join();
    strategyOpenNewPosition.join();
    strategyCloseLongPosition.join();

}

void PriceMomentumSingleSec::ExecTrade()
{
    while(mktData.size() < 2)
    {
        usleep(3000000);
    }

	string date_benchM = "";
	string date = "";
	string dateTime = "";
	float preHigh = std::numeric_limits<float>::min();
	float preLow = std::numeric_limits<float>::max();
	float curHigh = 0;
	float curLow = 0;
	float price = 0;
	float priceBase = 0;
	long int preTimeStamp = 0;
	long int timeStampBase = 0;
	long int timeStamp;
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

    list<MarketDataPoint>::iterator it_first = mktData.begin();
    priceBase = it_first->GetQuotePrice();
    prePrice = it_first->GetQuotePrice();
    timeStampBase = it_first->GetTimeStamp();
    date_benchM = (it_first->GetTimeStr()).substr(0,10);
    preLow = priceBase;
    preHigh = priceBase;

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


	while(1)
	{
	    //SetMktData();
	    //usleep(30000000);
	    auto it = prev(mktData.end());
        dateTime = it->GetTimeStr();
        date = dateTime.substr(0,10);
        timeStamp = it->GetTimeStamp();
        highLow = it->GetHighLow();
        price = it->GetQuotePrice();
        buyingPower = pyRobinAPI.GetBuyingPower();


        //if there is a new market data point
        if(timeStamp > preTimeStamp)
        {
            if((highLow == 1 && price > preHigh))
            {
                curTrend = "UP";
                curHigh = price;
                curCriticalTS = timeStamp;
            }
            else if(highLow == -1 && price > preLow)
            {
                curTrend = "UP";
                curLow = price;
                curCriticalTS = timeStamp;
            }
            else if (highLow == 1 && price < preHigh)
            {
                curTrend = "DOWN";
                curHigh = price;
                curCriticalTS = timeStamp;
            }
            else if (highLow == -1 && price < preLow)
            {
                curTrend = "DOWN";
                curLow = price;
                curCriticalTS = timeStamp;
            }

            dur = timeStamp - timeStampBase;
            chgspd = (price - priceBase)/priceBase/float(dur);

            //assert(preTrend != "" && curTrend != "");

            if(curTrend == preTrend && curTrend == "UP" && dur > chgdur_trg && chgspd > chgspd_trg && posStatus == ReadyForNewTrade && buyingPower >= price * util::sharesEachTrade)
            {
                //OpenNewPosition("buy","limit","gfd","immediate",price);
                side = "buy";
                posStatus = ProcessingNewTrade;
                orderPrice = price;
            }
            else if(curTrend == preTrend && curTrend == "DOWN" && dur > chgdur_trg && chgspd < -chgspd_trg)
            {
                //short selling is currently not available
                //entryVect.push_back(std::make_tuple("Short", dateTime, timeStamp, price,false));
            }
            else if(highLow == 0 && dur > chgdur_trg && chgspd > chgspd_trg && posStatus == ReadyForNewTrade && buyingPower >= price * util::sharesEachTrade)
            {
                //OpenNewPosition("buy","limit","gfd","immediate",price);
                side = "buy";
                posStatus = ProcessingNewTrade;
                orderPrice = price;

            }
            else if(highLow == 0 && dur > chgdur_trg && chgspd < -chgspd_trg)
            {
                //short selling is currently not available
                //entryVect.push_back(std::make_tuple("Short", dateTime, timeStamp, price,false));
            }
            else if(preTrend != curTrend && curTrend == "UP")
            {
                priceBase = preLow;
                preHigh = priceBase;
                timeStampBase = preCriticalTS;
                preTrend = curTrend;
            }
            else if(preTrend != curTrend && curTrend == "DOWN")
            {
                priceBase = preHigh;
                preLow = priceBase;
                timeStampBase = preCriticalTS;
                preTrend = curTrend;
            }

            preCriticalTS = curCriticalTS;
            preHigh = curHigh;
            preLow = curLow;

        }
	}
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

void PriceMomentumSingleSec::SetMktData()
{

    list<MarketDataPoint>::iterator lastIt;
    list<MarketDataPoint>::iterator secondLastIt;
    float lastQuote;
    float secondLastQuote;
    long int lastTimestamp;
    long int secondLastTimestamp;
    long int lastCriticalTimestamp = 0;
    bool newData = false;
    int len = mktData.size();
    float curQuote;
    long int curTimeStamp;
    int hour = 0;
    int minute = 0;
    int sec  = 0;
    string dateTime = "";
    unique_ptr<Document> quotes(new rapidjson::Document());
    MarketDataPoint dataP;


    while(1)
    {   quotes.reset();
        quotes = pyRobinAPI.GetQuote(ticker);
        len = mktData.size();
        curQuote = stof(quotes->operator[]("last_trade_price").GetString());
        dateTime = quotes->operator[]("updated_at").GetString();
        hour = stoi(dateTime.substr(11,2));
        minute = stoi(dateTime.substr(14,2));
        sec = stoi(dateTime.substr(17,2));
        curTimeStamp = hour * 3600 + minute * 60 + sec;


        /*if there is already data in the list, not include the new data point if it has the same
        timestamp of last data point in existing data list. */
        if(len != 0)
        {
            lastIt  = prev(mktData.end());
            lastTimestamp = lastIt->GetTimeStamp();
            if(curTimeStamp > lastTimestamp)
            {
                newData = true;

                //MarketDataPoint dataP;
                dataP.SetQuotePrice(curQuote);
                dataP.SetTimeStamp(curTimeStamp);
                dataP.SetTimeStr("");
                dataP.SetHighLow(Neutral);
                mktData.push_back(dataP);

                //std::cout<<curTimeStamp<<" "<<curQuote<<std::endl;
            }

        }
        //if there is no data in the list, always add the first data.
        else if(len == 0)
        {
            //MarketDataPoint dataP;
            dataP.SetQuotePrice(curQuote);
            dataP.SetTimeStamp(curTimeStamp);
            dataP.SetTimeStr("");
            dataP.SetHighLow(Neutral);
            mktData.push_back(dataP);
        }

        /*
        If there are at least three data points in the market data list,
        update the high/low of second last data point. The high/low of last
        data point is always neutral.
        */
        if(len > 2 && newData == true)
        {
            lastIt  = prev(mktData.end());
            secondLastIt = prev(lastIt);
            lastQuote = lastIt->GetQuotePrice();
            secondLastQuote = secondLastIt->GetQuotePrice();
            lastTimestamp = lastIt->GetTimeStamp();
            secondLastTimestamp = secondLastIt->GetTimeStamp();

            if (curQuote >= lastQuote && lastQuote < secondLastQuote && lastTimestamp - lastCriticalTimestamp > 50)
            {
                lastIt->SetHighLow(Low);
                lastCriticalTimestamp = curTimeStamp;
            }
            else if (curQuote > lastQuote && lastQuote <= secondLastQuote && lastTimestamp - lastCriticalTimestamp > 50)
            {
                lastIt->SetHighLow(Low);
                lastCriticalTimestamp = curTimeStamp;
            }
            else if (curQuote < lastQuote && lastQuote >= secondLastQuote && lastTimestamp - lastCriticalTimestamp > 50)
            {
                lastIt->SetHighLow(High);
                lastCriticalTimestamp = curTimeStamp;
            }
            else if (curQuote <= lastQuote && lastQuote > secondLastQuote && lastTimestamp - lastCriticalTimestamp > 50)
            {
                lastIt->SetHighLow(High);
                lastCriticalTimestamp = curTimeStamp;
            }
        }

        /* sleep for 60 seconds before fetching a new market data.
        Due to Robinhood constraint, only one data request can be made
        every 3 seconds. */
        usleep(3000000);
    }

}


PriceMomentumSingleSec::~PriceMomentumSingleSec()
{
    log.close();
    //cout<<"********************strategy destroy*************************"<<endl;
}


	void PriceMomentumSingleSec::DetectStg() {}
    void PriceMomentumSingleSec::Output() {}
    void PriceMomentumSingleSec::CleanEntryPoint(){}
    void PriceMomentumSingleSec::CleanMktData() {}


