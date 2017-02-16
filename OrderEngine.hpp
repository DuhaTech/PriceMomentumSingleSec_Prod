#ifndef ORDERENGINE_HPP
#define ORDERENGINE_HPP

namespace strategy
{
    class OrderEngine
    {
    public:
        int OpenNewPosition(string,string,string,string,float);
        int CloseLongPosition(string,string,string,string);
    };
}
#endif // ORDERENGINE_HPP
