#include <cmath>
#include "logic.h"


double CCalculator::StringToDouble(std::tstring Number)
{
    double num;
    num = 0;
    
    std::tstring::iterator it=Number.begin();
    std::tstring::iterator loc=Number.begin();
    
    for(loc;loc<Number.end();loc++)
    {
        if(*loc=='.') break;
    } 
    for(it;it<loc;it++)
    {
        num +=((*it)-48)*pow(10.0,loc-it-1);
    }
    if(it!=Number.end()) it++;
    while(it<Number.end())
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

void CCalculator::assign(std::tstring Number)
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
