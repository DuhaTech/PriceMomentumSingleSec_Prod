from RobinhoodAPI import Robinhood

def GetQuote(ticker):
    x = Robinhood('jhuang0724','HjhCh6969!')
    data = x.get_quote(ticker)
    return data

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
