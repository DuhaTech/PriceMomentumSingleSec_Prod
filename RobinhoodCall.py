from RobinhoodAPI import Robinhood
import requests
import urllib
import json

def GetQuote(ticker):
    session = requests.session()
    session.headers = {
            "Accept": "*/*",
            "Accept-Encoding": "gzip, deflate",
            "Accept-Language": "en;q=1, fr;q=0.9, de;q=0.8, ja;q=0.7, nl;q=0.6, it;q=0.5",
            "Content-Type": "application/x-www-form-urlencoded; charset=utf-8",
            "X-Robinhood-API-Version": "1.0.0",
            "Connection": "keep-alive",
            "User-Agent": "Robinhood/823 (iPhone; iOS 7.1.2; Scale/2.00)"
        }
    data = { 'symbols' : ticker }
    res = session.get('https://api.robinhood.com/quotes/', params=data)
    if res.status_code == 200:
        ##return res.json()['results']
        return json.dumps(res.json()['results'][0])
    else:
        raise Exception("Could not retrieve quote: " + res.text)


def GetInstrument(ticker):
    x = Robinhood('jhuang0724','HjhCh6969!')
    data = x.instrument(ticker)
    return data

def GetBuyingPower():
    x = Robinhood('jhuang0724','HjhCh6969!')
    data = x.get_account_info()
    return data

def GetAccountNum():
    x = Robinhood('jhuang0724','HjhCh6969!')
    data = x.get_account_number()
    return data
