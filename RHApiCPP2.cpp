#include "RHApiCPP2.hpp"

using namespace rapidjson;

namespace Robinhood
{
 //CURL * RobinhoodAPI::curl = curl_easy_init();
 //struct response RobinhoodAPI::resStr =  NULL;
 //CURLcode RobinhoodAPI::resCode =  CURLE_OK;


    void RHApiCPP2::Init()
    {
        curl = curl_easy_init();
        resCode =  CURLE_OK;
        //std::string RobinhoodAPI::token = "jfd";
        //token ="sjdf";

        //CURL * RHApiCPP::curl = curl_easy_init();
        //nit_string(&resStr);

        struct curl_slist* list = NULL;
        list = curl_slist_append(list, "Accept:*/*");
        list = curl_slist_append(list,"Accept-Encoding:utf-8, gzip, deflate");
        list = curl_slist_append(list,"Accept-Language:en;q=1, fr;q=0.9, de;q=0.8, ja;q=0.7, nl;q=0.6, it;q=0.5");
        list = curl_slist_append(list,"Content-Type: application/x-www-form-urlencoded; charset=utf-8");
        list = curl_slist_append(list,"X-Robinhood-API-Version:1.0.0");
        list = curl_slist_append(list,"Connection:keep-alive");
        list = curl_slist_append(list,"User-Agent: Robinhood/823 (iPhone; iOS 7.1.2; Scale/2.00)");
        //list = curl_slist_append(list, "Authorization: Token ec3cbc33141ab9782e3d047ed6edcbd30b5c89ac");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "cookies.txt");
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "cookies.txt");
        curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);

        //GetToken();
        //list = curl_slist_append(list, ("Authorization: Token " + token).c_str());
    }





    size_t RHApiCPP2::writefunc(void *ptr, size_t size, size_t nmemb,  struct response *s)
    {
        if(ptr == nullptr || ptr == NULL)
        {
            std::cout<<"null pointer"<<std::endl;
        }
size_t new_len = s->len + size*nmemb;
  s->ptr = (char*)realloc(s->ptr, new_len+1);

  if (s->ptr == NULL) {
    fprintf(stderr, "realloc() failed\n");
    exit(EXIT_FAILURE);
  }
  std::memcpy(s->ptr+s->len, ptr, size*nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  return size*nmemb;

    }

    void RHApiCPP2::init_string(struct response *s)
    {
        s->len = 0;
        s->ptr = (char*) malloc(s->len+1);
        if (s->ptr == NULL)
        {
            std::cout<<"malloc() failed"<<std::endl;
            fprintf(stderr, "malloc() failed\n");
            exit(EXIT_FAILURE);
        }
        s->ptr[0] = '\0';
    }




    std::string RHApiCPP2::GetQuote(std::string ticker)
    {
        //curl_easy_setopt(curl, CURLOPT_POST, 0);
        struct response resStr;
        init_string(&resStr);
        std::string responseData = "NULL";
        std::string para = "symbol=AVP";
        //std::string url = "https://api.robinhood.com/quotes/ONEY/";
        //std::string url = "https://api.robinhood.com/quotes/AVP/";
        std::string url = "https://example.com/";
        //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, para.c_str());
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resStr);
        //curl_easy_setopt(curl,CURLOPT_HTTPGET,1);
        //curl_easy_setopt(curl, CURLOPT_POST, 1);

        resCode = curl_easy_perform(curl);
        //std::cout << resCode;
        responseData = std::string(resStr.ptr);
        if(resCode == CURLE_OK)
        {
            //std::cout << " okay"<<std::endl;
            std::cout << responseData<<std::endl;
            //std::cout << (resStr.len);
        }
        else
            {std::cout << "not okay"<<std::endl;}

        return "";
    }

}


