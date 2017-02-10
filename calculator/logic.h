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
    static double StringToDouble(std::tstring Number);
    
    void plus(std::tstring Number);
    void plus(const CCalculator &Number);
    void sub(std::tstring Number);
    void sub(const CCalculator &Number);
     
    void multiply(std::tstring Number);
    bool division(std::tstring Number);
    
    void getResult(std::tstring &Number);

    CCalculator& operator=(const CCalculator& other);
};