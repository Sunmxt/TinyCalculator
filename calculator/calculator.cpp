#include "calc_window.h"
#include <iostream>
#include <commctrl.h>

#include "resource.h"

#pragma comment(lib, "comctl32.lib")
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

class CCalculatorApp : public Slight::CObject
{
public:
    static void DebugInit();
    static void DebugClean();
    
    
    CCalculatorApp();
    ~CCalculatorApp();
    
public:
    int exec();
    
};

#ifdef _DEBUG
    void CCalculatorApp::DebugInit()
    {
        HANDLE console_standard_output;
        DWORD written;
        TCHAR* debug_info = TEXT("[Tiny calculator debug information]\n");
        
        AllocConsole();
        
        console_standard_output = GetStdHandle(STD_OUTPUT_HANDLE);
        WriteConsole(console_standard_output, debug_info, lstrlen(debug_info), &written, NULL);

        freopen("CONOUT$", "wt", stdout);
        freopen("CONOUT$", "wt", stderr);
        freopen("CONIN$", "rt", stdin);
	
        std::cout << "[cout] connected.\n";
        std::cerr << "[cerr] connected.\n";
        std::clog << "[clog] connected.\n";
    }

    void CCalculatorApp::DebugClean()
    {
        FreeConsole();
        fclose(stdout);
        fclose(stdin);
        fclose(stderr);
    }
#endif

CCalculatorApp::CCalculatorApp()
{
    InitCommonControls();
    #ifdef _DEBUG
        DebugInit();
    #endif
}

CCalculatorApp::~CCalculatorApp()
{
    #ifdef _DEBUG
        DebugClean();
    #endif
}

int CCalculatorApp::exec()
{
    CCalculatorWindow wnd;
    
    wnd.createDialog(IDD_CALC);
	MSG message;

	while(GetMessage(&message, 0, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
    
    return 0;
}

int WINAPI WinMain(HINSTANCE hIns, HINSTANCE hPrevIns, LPSTR lpCmdLine, int nShowCmd)
{
    CCalculatorApp application;
	return application.exec();
}