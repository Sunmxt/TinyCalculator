#include "slight_wnd.h"

class CCalculatorWindow : public Slight::CWinNativeView
{
public:
    LRESULT handleMessage(UINT message ,WPARAM wParam ,LPARAM lParam);
    
    int OnControlNotify(UINT NotifyCode, UINT ID, HWND Handle);
};
