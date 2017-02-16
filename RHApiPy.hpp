#ifndef RHAPIPY_HPP
#define RHAPIPY_HPP

#include <memory>
#include <iostream>
#include <Python.h>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <string>
#include <boost/algorithm/string/regex.hpp>
#include "RHApiPy.hpp"
#include "rapidjson/document.h"
#include <string>

using namespace rapidjson;

namespace Robinhood
{
    class RHApiPy
    {
        public:
             std::unique_ptr<Document> GetQuote(std::string ticker);
             float GetBuyingPower();
             double GetPreClosePrice(std::string ticker);
             std::string GetInstrument(std::string ticker);
             RHApiPy();
             ~RHApiPy();
        private:
               PyObject *pName, *pModule, *pDict, *pFunc,*pValue, *presult, *pFunc_inst, *pFunc_buyingPower;
    };
}
#endif // PYROBINAPI_HPP
