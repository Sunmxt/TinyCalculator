#include "slight_wnd.h"
#include "logic.h"
#include <iostream>


class CCalculatorEditControl : public Slight::CWinNativeView
{
    #define EM_CALC_DISPLAY_RESULT (WM_USER + 1)
    
public:
    LRESULT handleMessage(UINT message, WPARAM wParam, LPARAM lParam);
    
    CCalculatorEditControl();
    
protected:
    uint status;
    
    #define CALC_EDIT_CTL_IDLE              0
    #define CALC_EDIT_CTL_DISPLAY_RESULT    1
};

class CCalculatorWindow : public Slight::CWinNativeView
{
    
public:
    CCalculatorWindow();
    
    LRESULT handleMessage(UINT message ,WPARAM wParam ,LPARAM lParam);
    
    int OnControlNotify(UINT NotifyCode, UINT ID, HWND Handle);
    int OnInit(LPVOID Param);
    int OnEditChange(HWND Handle);
    int OnPlus();
    int OnSub();
    int OnMul();
    int OnDiv();
    int OnEqual();
    int OnRegPlus();
    int OnRegSub();
    int OnRegStore();
    int OnRegShow();
    int OnChar(TCHAR Charatrer, DWORD KeyData);
    
    int Calculate(std::tstring Number);
    int CleanAll();
    int CleanCurrent();
    
    bool StandardizeString(TCHAR* String, WPARAM &CaretStart, LPARAM &CaretEnd);
    bool GetDigit(std::tstring &String);
    bool FillDigitString(TCHAR* String);

protected:
    CCalculator calc;
    CCalculator reg;
    
    CCalculatorEditControl edit_control;
    
    byte status;
    #define CALC_STATUS_IDLE    0
    #define CALC_PLUS_WAIT      1
    #define CALC_SUB_WAIT       2
    #define CALC_MUL_WAIT       3
    #define CALC_DIV_WAIT       4
    
};


