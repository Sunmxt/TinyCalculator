#include "slight_wnd.h"
#include <iostream>

#ifdef _UNICODE
	#define lsprintf swprintf
    #define tout    wcout
    #define tstring wstring
    #define tmemmove wmemmove
    #define lstrchr wcschr
    #define lstrrchr wcsrchr
#else
	#define lsprintf sprintf
    #define tout cout
    #define tstring string
    #define tmemmove memmove
    #define lstrchr strchr
    #define lstrrchr strrchr
#endif


class CCalculatorEditControl : public Slight::CWinNativeView
{
public:
    LRESULT handleMessage(UINT message, WPARAM wParam, LPARAM lParam);
};

class CCalculatorWindow : public Slight::CWinNativeView
{
    
public:
    CCalculatorWindow();
    
    LRESULT handleMessage(UINT message ,WPARAM wParam ,LPARAM lParam);
    
    int OnControlNotify(UINT NotifyCode, UINT ID, HWND Handle);
    int OnInit(LPVOID Param);
    int OnEditChange(HWND Handle);
    
    bool StandardizeString(TCHAR* String);
    bool SaveFirstDigit();
    bool FillDigitString(TCHAR* String);

protected:
    std::tstring digit;
    
    CCalculatorEditControl edit_control;
};


