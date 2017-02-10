#include <cmath>
#include "logic.h"


double CCalculator::StringToDouble(std::tstring Number)
{
    double num;
    num = 0;
    
    std::tstring::iterator it=Number.begin();
    std::tstring::iterator loc=Number.begin();
    
    for(loc;loc<Number.end();loc++)//确定小数点的位置付给迭代器loc
    {
        if(*loc=='.') break;
    } 
    for(it;it<loc;it++)//小数点前的数
    {
        num +=((*it)-48)*pow(10.0,loc-it-1);
    }
    if(it!=Number.end()) it++;//必要的判断否者溢出出错
    while(it<Number.end())//小数点后的数
    {
        num +=((*it)-48)*pow(10.0,loc-it);
        it++;
    }
    
    return num;
}

CCalculator::CCalculator()
:result(0)
{
}

CCalculator::~CCalculator()
{
}

void CCalculator::assign(std::tstring Number)//字符串转double型
{
	result = StringToDouble(Number);
}

void CCalculator::plus(std::tstring Number)
{
    result += StringToDouble(Number);
}

void CCalculator::plus(const CCalculator &Number)
{
    result += Number.result;
}

void CCalculator::sub(std::tstring Number)
{
	result -= StringToDouble(Number);
}

void CCalculator::sub(const CCalculator &Number)
{
    result -= Number.result;
}

void CCalculator::multiply(std::tstring Number)
{
    result *= StringToDouble(Number);
}

bool CCalculator::division(std::tstring Number)
{
    double div_num;
    
    div_num = StringToDouble(Number);
    if(0 == div_num)
    {
        setError(Slight::ERR_INVAILD_PARAMETER);
        return false;
    }
    
    result /= div_num;
    
    return true;
}

CCalculator& CCalculator::operator=(const CCalculator& other)
{
    result = other.result;
    return *this;
}

void CCalculator::getResult(std::tstring &Number)
{
    TCHAR *string;
    uint length;
    
    length = lsnprintf(0, 0, TEXT("%g"), result);
    string = new TCHAR[length + 1];
    
    length = lsnprintf(string, length, TEXT("%g"), result);
    string[length] = 0;

    Number.assign(string);
    
    delete string;
}
