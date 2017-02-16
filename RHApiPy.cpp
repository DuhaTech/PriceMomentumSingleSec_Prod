#include "RHApiPy.hpp"

namespace Robinhood
{


	RHApiPy::RHApiPy()
	{
		    // Set PYTHONPATH TO working directory
   setenv("PYTHONPATH",".",1);

   // Initialize the Python Interpreter
   Py_Initialize();

    // Build the name object
   pName = PyString_FromString((char*)"RobinhoodCall");

   // Load the module object
   pModule = PyImport_Import(pName);


   // pDict is a borrowed reference
   pDict = PyModule_GetDict(pModule);


   // pFunc is also a borrowed reference
   pFunc = PyDict_GetItemString(pDict, (char*)"GetQuote");

//function to get instrument details
	pFunc_inst = PyDict_GetItemString(pDict, (char*)"GetInstrument");

	pFunc_buyingPower = PyDict_GetItemString(pDict, (char*)"GetBuyingPower");

	}

    RHApiPy::~RHApiPy()
    {
        Py_DECREF(pValue);

       // Clean up
       Py_DECREF(pModule);
       Py_DECREF(pName);

       // Finish the Python Interpreter
       Py_Finalize();


    }

    std::unique_ptr<Document> RHApiPy::GetQuote(std::string ticker)
    {
        const char* result;
        std::string resStr;

        if (PyCallable_Check(pFunc))
        {
            pValue=Py_BuildValue("(s)",ticker.c_str());
            PyErr_Print();
            presult=PyObject_CallObject(pFunc,pValue);
            PyErr_Print();
            PyObject* objectsRepresentation = PyObject_Repr(presult);
            result =PyString_AsString(objectsRepresentation);
        } else
        {
            PyErr_Print();
        }

        resStr = std::string(result);
        resStr = resStr.substr(1, resStr.size() - 2);
        /*
        std::vector<std::string> resVect;
        boost::split(resVect, resStr,boost::is_any_of(","));

        std::string preClose = resVect[0];
        double preCloseFlt = std::stod(preClose.substr(23,preClose.size()-1));

        std::string latestClose = resVect[1];
        double latestCloseFlt = std::stod(latestClose.substr(24,latestClose.size()-1));

        std::string latestBid = resVect[11];
        double latestBidFlt = std::stod(latestBid.substr(17,latestBid.size()-1));

        std::string latestAsk = resVect[6];
        double latestAskFlt = std::stod(latestAsk.substr(17,latestAsk.size()-1));

        std::string latestBidSize = resVect[13];
        double latestBidSizeFlt = std::stod(latestBidSize.substr(14,latestBidSize.size()-1));

        std::string latestAskSize = resVect[10];
        double latestAskSizeFlt = std::stod(latestAskSize.substr(14,latestAskSize.size()-1));

        //std::cout<<preCloseFlt<<std::endl<<latestCloseFlt<<std::endl<<latestBidFlt<<std::endl<<latestAskFlt<<std::endl
        //<<latestBidSizeFlt<<std::endl<<latestAskSizeFlt<<std::endl;
        std::vector<double> quotes = {preCloseFlt, latestCloseFlt,latestAskFlt, latestBidFlt, latestAskSizeFlt, latestBidSizeFlt};
        */
        //std::cout<<resStr;
        std::unique_ptr<Document> quotes (new Document());
        quotes->Parse(resStr.c_str());
        return quotes;

    }

    float RHApiPy::GetBuyingPower()
    {
        const char* result;
        std::string resStr;

        if (PyCallable_Check(pFunc_buyingPower))
        {
            //pValue=Py_BuildValue("(s)",ticker.c_str());
            PyErr_Print();
            presult=PyObject_CallObject(pFunc_buyingPower, nullptr);
            PyErr_Print();
            PyObject* objectsRepresentation = PyObject_Repr(presult);
            result =PyString_AsString(objectsRepresentation);
        } else
        {
            PyErr_Print();
        }


        resStr = std::string(result);
        resStr = resStr.substr(1, resStr.size() - 2);

        std::unique_ptr<Document> quotes (new Document());
        quotes->Parse(resStr.c_str());
        float buyingPower = std::stof(quotes->operator[]("buying_power").GetString());
        return buyingPower;
    }


    double RHApiPy::GetPreClosePrice(std::string ticker)
    {

        const char* result;
        std::string resStr;



       if (PyCallable_Check(pFunc))
       {
           pValue=Py_BuildValue("(s)",ticker.c_str());
           PyErr_Print();
           //printf("Let's give this a shot!\n");
           presult=PyObject_CallObject(pFunc,pValue);
           PyErr_Print();
           PyObject* objectsRepresentation = PyObject_Repr(presult);
           result =PyString_AsString(objectsRepresentation);
           //std::cout<<presult<<std::endl;
       } else
       {
           PyErr_Print();
       }


        resStr = std::string(result);

    std::vector<std::string> resVect;
    boost::split(resVect, resStr,boost::is_any_of(","));
    std::string preClose = resVect[3];
    double preCloseFlt = std::stod(preClose.substr(31,preClose.size()-1));
    return preCloseFlt;
    }


    std::string RHApiPy::GetInstrument(std::string ticker)
    {
        const char* result;
        std::string resStr;

        if (PyCallable_Check(pFunc_inst))
        {
           pValue=Py_BuildValue("(s)",ticker.c_str());
           PyErr_Print();
           presult=PyObject_CallObject(pFunc_inst,pValue);
           PyErr_Print();
           PyObject* objectsRepresentation = PyObject_Repr(presult);
           result =PyString_AsString(objectsRepresentation);
        } else
        {
           PyErr_Print();
        }

        resStr = std::string(result);
        std::vector<std::string> resVect;
        boost::split(resVect, resStr,boost::is_any_of(","));
        std::string instrument = resVect[4];
        instrument = instrument.substr(11,75);
        return instrument;
    }


}
