#include <iostream>
#include "object.h"

class CCalculator : public Slight::CObject
{
private:
    double result;
    
public:
    CCalculator();
    ~CCalculator();
    
    void assign(std::tstring Number);    
        /* 
            设置运算的第一个数字
        */
    static double StringToDouble(std::tstring Number);
    
    void plus(std::tstring Number);      //加
    void plus(const CCalculator &Number);
    void sub(std::tstring Number);       //减
    void sub(const CCalculator &Number);
     
    void multiply(std::tstring Number);  //乘
    bool division(std::tstring Number);  //除
    
    void getResult(std::tstring &Number); //结果

    CCalculator& operator=(const CCalculator& other);
};