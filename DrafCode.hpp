
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <curl/curl.h>
#include <iostream>
#include <memory>
#include <fstream>
#include <RobinhoodAPI.hpp>
#include <Python.h>


using namespace Robinhood;

//struct response {
//  char *ptr;
//  size_t len;
//};

//void init_string(struct response *s) {
//  s->len = 0;
//  s->ptr = (char*) malloc(s->len+1);
//  if (s->ptr == NULL) {
//    fprintf(stderr, "malloc() failed\n");
//    exit(EXIT_FAILURE);
//  }
//  s->ptr[0] = '\0';
//}

//size_t writefunc(void *ptr, size_t size, size_t nmemb,  struct response *s)
//{
//  size_t new_len = s->len + size*nmemb;
//  s->ptr = (char*)realloc(s->ptr, new_len+1);
//  if (s->ptr == NULL) {
//    fprintf(stderr, "realloc() failed\n");
//    exit(EXIT_FAILURE);
//  }
//  std::memcpy(s->ptr+s->len, ptr, size*nmemb);
//  s->ptr[new_len] = '\0';
//  s->len = new_len;
//
//  return size*nmemb;
//}
std::string PythonCall();


int main(void)
{

//std::ifstream read("\\\\JinhuaHuang\\YahooFinanceData\\Quotes\\CommonStock\\IBM.csv"); /* \\\\192.168.1.121\\YahooFinanceData\\Quotes\\CommonStock\\*/
char buffer[1000];
FILE* file;
size_t bytesRead;

//file = fopen("/media/jinhuacolin/E151901325CCC29F/IBM.csv","rb");
//bytesRead = fread(buffer,sizeof(char), sizeof(buffer),file);

//std::cout<<buffer;


RobinhoodAPI* rh = new RobinhoodAPI();
rh->Init();
//std::string ret = rh->PlaceOrder("WKHS",2,"buy","limit","gfd","immediate",1.0);
//std::string ret = rh->GetIntrument();

//std::string ret = rh->CancelOrder("https://api.robinhood.com/orders/f85150aa-6e4e-4d1c-bff3-d7074084ee9c/cancel/");
//std::string ret =  rh->GetQuote("AVP");
//std::string ret =  rh->GetOrder("AVP");

//std::cout<<ret<<std::endl;

std::string quote = PythonCall();

std::cout << quote<<std::endl;

//std::ofstream file;
//file.open("token.txt");
//file<<ret;
//file.close();
//std::cout<<ret<<std::endl;




//
//  CURL *curl;
//  CURLcode res;
//  struct curl_slist* list = NULL;
//  list = curl_slist_append(list, "Accept:*/*");
//  list = curl_slist_append(list,"Accept-Encoding:gzip, deflate");
//  list = curl_slist_append(list,"Accept-Language:en;q=1, fr;q=0.9, de;q=0.8, ja;q=0.7, nl;q=0.6, it;q=0.5");
// list = curl_slist_append(list,"Content-Type: application/x-www-form-urlencoded; charset=utf-8");
// list = curl_slist_append(list,"X-Robinhood-API-Version:1.0.0");
// list = curl_slist_append(list,"Connection:keep-alive");
// list = curl_slist_append(list,"User-Agent: Robinhood/823 (iPhone; iOS 7.1.2; Scale/2.00)");
//list = curl_slist_append(list, "Authorization: Token fd1f9a3aa21835274184a0b01a0013191dc75661");
//list = curl_slist_append(list, "Symbols:AVP");

//std::string orderSpec = "symbol=AVP&price=1.0&quantity=1&side=buy&time_in_force=gfd&type=limit&trigger=immediate&account=https://api.robinhood.com/accounts/5QS63703/&instrument=https://api.robinhood.com/instruments/7da371f1-ca40-4c5d-ae4a-7d75c9437b73/";
//
//
//  char err[CURL_ERROR_SIZE];
//
//  curl_global_init(CURL_GLOBAL_DEFAULT);
//
//  curl = curl_easy_init();
//  if(curl) {
    //struct string s;
    //init_string(&s);
      //curl_easy_setopt(curl, CURLOPT_URL, "https://example.com/");
    //curl_easy_setopt(curl,CURLOPT_ERRORBUFFER, err);
    //curl_easy_setopt(curl, CURLOPT_URL, "http://chartapi.finance.yahoo.com/instrument/1.0/AVP/chartdata;type=quote;range=1d/csv");
 //curl_easy_setopt(curl, CURLOPT_URL, "https://api.robinhood.com/quotes/symbols=avp");
     //curl_easy_setopt(curl, CURLOPT_URL, "https://api.robinhood.com/api-token-auth/");
     //curl_easy_setopt(curl, CURLOPT_URL, "https://api.robinhood.com/user/investment_profile/");
    // curl_easy_setopt(curl, CURLOPT_URL, "https://connect.secure.wellsfargo.com/auth/login/");
   //curl_easy_setopt(curl, CURLOPT_URL, "https://api.robinhood.com/accounts/");
  //curl_easy_setopt(curl, CURLOPT_URL, "https://api.robinhood.com/orders/");
   //curl_easy_setopt(curl, CURLOPT_URL, "https://api.robinhood.com/quotes/IBM/");
   //curl_easy_setopt(curl, CURLOPT_URL, "https://api.robinhood.com/instruments/IBM/");
    //curl_easy_setopt(curl,CURLOPT_USERPWD, "[jhuang07244]:[HjhCH6969!]");
//    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");
//     curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "cookies.txt");
//      curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "cookies.txt");
//    curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1 );
//   curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
//    curl_easy_setopt(curl, CURLOPT_POST, 1);
//curl_easy_setopt(curl, CURLOPT_POSTFIELDS, orderSpec.c_str());
    //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "username=jhuang0724&password=HjhCh6969!");
    //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "username=fd1f9a3aa21835274184a0b01a0013191dc75661");
  //curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
   //curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);

        //curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);


#ifdef SKIP_PEER_VERIFICATION
    /*
     * If you want to connect to a site who isn't using a certificate that is
     * signed by one of the certs in the CA bundle you have, you can skip the
     * verification of the server's certificate. This makes the connection
     * A LOT LESS SECURE.
     *
     * If you have a CA cert for the server stored someplace else than in the
     * default bundle, then the CURLOPT_CAPATH option might come handy for
     * you.
     */
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERIFICATION
    /*
     * If the site you're connecting to uses a different host name that what
     * they have mentioned in their server certificate's commonName (or
     * subjectAltName) fields, libcurl will refuse to connect. You can skip
     * this check, but this will make the connection less secure.
     */
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif

    /* Perform the request, res will get the return code */
//curl_easy_setopt(curl, CURLOPT_URL, "https://api.robinhood.com/orders/?cursor=cD0yMDE2LTAxLTIwKzE5JTNBMzUlM0EwMC4yMjEyOTQlMkIwMCUzQTAw");
//    err[0] = 0;
//    res = curl_easy_perform(curl);


   //res = curl_easy_perform(curl);
    //std::cout<<"after perform"<<std::endl;

    /* Check for errors */
//    if(res != CURLE_OK)
//    {
//    std::cout<<"error"<<std::endl;
//      fprintf(stderr, "curl_easy_perform() failed: %s\n",
//              curl_easy_strerror(res));
//    }

//std::string token(s.ptr);

    //std::ofstream file;
   // file.open("token.txt");
    //file<<token;
    //file.close();

    //std::cout<<s.len<<std::endl;
    //std::cout<<(s.ptr)<<std::endl;



    /* always cleanup */
    //curl_easy_cleanup(curl);

  //}

  //curl_global_cleanup();
  getchar();


  return 0;
}

std::string PythonCall()
{
	// Set PYTHONPATH TO working directory
   setenv("PYTHONPATH",".",1);

   PyObject *pName, *pModule, *pDict, *pFunc, *pValue, *presult;


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

   const char* result;

   if (PyCallable_Check(pFunc))
   {
       pValue=Py_BuildValue("(s)","AVP");
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
   //printf("Result is %d\n",PyInt_AsLong(presult));
   Py_DECREF(pValue);

   // Clean up
   Py_DECREF(pModule);
   Py_DECREF(pName);

   // Finish the Python Interpreter
   Py_Finalize();
   return std::string(result);

}
