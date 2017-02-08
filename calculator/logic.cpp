#include <string>

/*
    计算逻辑实现
    
    输入：以字符串(std::string)表示的数字
    输出：以字符串(std::string)表示的结果
    
    
    例如：
        比如计算 3*2 ，则通过assign方法，设置数字3，然后使用multiply（传入数字2）完成计算。
*/

class CCalculator
{
public:
    CCalculator();
    ~CCalculator();
    
    void assign(std::string Number);    
        /* 
            设置运算的第一个数字
        */
        
    void plus(std::string Number);      //加
    void sub(std::string Number);       //减
    void multiply(std::string Number);  //乘
    void division(std::string Number);  //除
    
    void getResult(std::string &Number); //结果

}

CCalculator::CCalculator()
{
}

CCalculator::~CCalculator()
{
}

void CCalculator::assign(std::string Number)
{
}

void CCalculator::plus(std::string Number)
{
}

void CCalculator::sub(std::string Number)
{
}

void CCalculator::multiply(std::string Number)
{
}

void CCalculator::division(std::string Number)
{
}