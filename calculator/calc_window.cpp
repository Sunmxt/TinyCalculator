#include "calc_window.h"
#include <tchar.h>
#include <iostream>

#ifdef _UNICODE
	#define lsprintf swprintf
    #define tout    wcout
#else
	#define lsprintf sprintf
    #define tout cout
#endif

LRESULT CCalculatorWindow::handleMessage(UINT message ,WPARAM wParam ,LPARAM lParam)
{
    switch(message)
	{
	case WM_CLOSE:
		DestroyWindow(handle);break;
	case WM_DESTROY:
		PostQuitMessage(0);break;
	case WM_COMMAND:
		return OnControlNotify(HIWORD(wParam), LOWORD(wParam), (HWND)lParam);
	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
    
    return 0;
}

int CCalculatorWindow::OnControlNotify(UINT NotifyCode, UINT ID, HWND Handle)
{
#ifdef _DEBUG
	TCHAR string[100];
	lsprintf(string, TEXT("NotifyCode = 0x%x, ID = 0x%x, Handle = 0x%x\n") ,NotifyCode, ID, Handle);
    std::wcout << string;
#endif
	return 1;
}