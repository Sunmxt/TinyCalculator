#include "slight_wnd.h"

class CCalculatorEditControl : public Slight::CWinNativeView
{
public:
    LRESULT handleMessage(UINT message, WPARAM wParam, LPARAM lParam);
};

class CCalculatorWindow : public Slight::CWinNativeView
{
public:
    LRESULT handleMessage(UINT message ,WPARAM wParam ,LPARAM lParam);
    
    int OnControlNotify(UINT NotifyCode, UINT ID, HWND Handle);
    int OnInit(LPVOID Param);
    int OnEditChange(HWND Handle);
    
    bool StandardizeString(TCHAR* String);
    
protected:
    HWND edit_handle;
    
    CCalculatorEditControl edit_control;
};


