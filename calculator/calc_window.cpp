#include "calc_window.h"
#include <tchar.h>
#include <iostream>

#include "resource.h"


CCalculatorWindow::CCalculatorWindow()
:status(CALC_STATUS_IDLE)
{}

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
    case WM_INITDIALOG:
        return OnInit((LPVOID)lParam);
    case WM_CHAR:
        return OnChar((TCHAR)wParam, (DWORD)lParam);
    default:
        return FALSE;
	}
    
    return TRUE;
}

int CCalculatorWindow::OnChar(TCHAR Charatrer, DWORD KeyData)
{
    switch(Charatrer)
    {
    case '+':
        OnPlus();break;
    case '-':
        OnSub();break;
    case '*':
        OnMul();break;
    case '/':
        OnDiv();break;
    case 'c':
    case 'C':
        CleanAll();break;
    case VK_RETURN:
    case '=':
        OnEqual();break;
    }
    
    return TRUE;
}
 
int CCalculatorWindow::OnInit(LPVOID Param)
{
    HWND edit_handle;

    edit_handle = GetDlgItem(handle ,IDC_EDIT1);
    ::SendMessage(edit_handle, EM_SETLIMITTEXT, 20, 0);

    edit_control.attach(edit_handle);
    
    return TRUE;
}

bool CCalculatorWindow::StandardizeString(TCHAR* String, WPARAM &CaretStart, LPARAM &CaretEnd)
{
    #define CaretForward()\
        CaretStart = CaretStart > 1 ? CaretStart - 1 : 0; \
        CaretEnd = CaretEnd > 1 ? CaretEnd - 1 : 0;
        
    #define SkipThisCharacter() {\
        CaretForward()\
        result = true;\
        continue;}
        
    uint scan, put, length;
    bool dot, result, zero_front, zero_end;
    
    put = scan = 0;
    zero_front = zero_end = result = dot = false;
    
    for(;String[scan]; scan ++)
    {
        switch(String[scan])
        {
        case '.':
            zero_end = zero_front = true;
            if(dot)
                SkipThisCharacter()
            dot = true;
            break;
        default:
            if(!isdigit(String[scan]))
                SkipThisCharacter()
            else
            {
                if(!zero_end)
                {
                    if('0' == String[scan])
                        if(zero_front)
                            SkipThisCharacter()
                        else
                            zero_front = true;
                    else
                    {
                        if(zero_front)
                        {
                            put--;
                            CaretForward()
                            result = true;
                        }   
                        zero_end = zero_front = true;
                    }
                }
            }
        }
        
        String[put] = String[scan];
        put++;
    }
    
    String[put] = 0;
    
    length = lstrlen(String);
    
    if(0 == String - lstrchr(String, '.'))
    {
        tmemmove(String + 1, String, length + 1);
        String[0] = '0';
        length += 1;
        CaretStart ++; CaretEnd ++;
        result = true;
    }
    
    #undef SkipThisCharacter
    #undef CaretForward
    
    return result;
}

bool CCalculatorWindow::GetDigit(std::tstring &String)
{
    uint length;
    TCHAR* string;

    length = SendMessage(edit_control.getHandle(), EM_LINELENGTH, 0, 0);
    if(!length)
    {
        String.assign(TEXT("0"));
        return false;
    }
    
    string = new TCHAR[length + 3]; //terminator + two digit(possible)
    if(!string)
        return false;

    SendMessage(edit_control.getHandle(), WM_GETTEXT, length + 3, (LPARAM)string);
    
    FillDigitString(string);
    
    String.assign(string);
    
    delete string;
    
    return true;
}

bool CCalculatorWindow::FillDigitString(TCHAR* String)
{   
    if(0 == String - lstrstr(String, TEXT("0.")))
    {
        for(uint i = 2; String[i] ; i++)
        {
            if(String[i] != '0')
                return false;
        }
        
        String[1] = 0;
        return true;
    }

    return false;
}

int CCalculatorWindow::OnEditChange(HWND Handle)
{
    TCHAR *string;
    DWORD length;
    WPARAM pos_start;
    LPARAM pos_end;
    
    length = SendMessage(Handle, EM_LINELENGTH, 0, 0);
    if(!length)
        return 1;
    
    string = new TCHAR[length + 3];
    if(!string)
        return 0;
    
    SendMessage(Handle, WM_GETTEXT, length + 3, (LPARAM)string);
    SendMessage(Handle, EM_GETSEL, (WPARAM)&pos_start, (LPARAM)&pos_end);
    
    if(StandardizeString(string, pos_start, pos_end))
    {
        SendMessage(Handle, WM_SETTEXT, 0, (LPARAM)string);
        SendMessage(Handle, EM_SETSEL, pos_start, pos_end);
    }
    
    delete string;
    
    return 1;
}

int CCalculatorWindow::Calculate(std::tstring Number)
{
    switch(status)
    {
    case CALC_STATUS_IDLE:
        calc.assign(Number);
        SendMessage(edit_control.getHandle(), EM_CALC_DISPLAY_RESULT, 0, 0);
        return TRUE;
        
    case CALC_PLUS_WAIT:
        calc.plus(Number);
        break;
        
    case CALC_SUB_WAIT:
        calc.sub(Number);
        break;
        
    case CALC_MUL_WAIT:
        calc.multiply(Number);
        break;
        
    case CALC_DIV_WAIT:
        calc.division(Number);
        break;
    }
    
    calc.getResult(Number);
    SendMessage(edit_control.getHandle(), WM_SETTEXT, 0, (LPARAM)Number.c_str());
    
    return TRUE;
}

int CCalculatorWindow::OnPlus()
{
    std::tstring Number;
    GetDigit(Number);
    Calculate(Number);
    status = CALC_PLUS_WAIT;

    return TRUE;
}

int CCalculatorWindow::OnSub()
{
    std::tstring Number;
    GetDigit(Number);
    Calculate(Number);
    status = CALC_SUB_WAIT;
    
    return TRUE;
}

int CCalculatorWindow::OnMul()
{
    std::tstring Number;
    GetDigit(Number);
    Calculate(Number);
    status = CALC_MUL_WAIT;
    
    return TRUE;
}

int CCalculatorWindow::OnDiv()
{
    std::tstring Number;
    GetDigit(Number);
    Calculate(Number);
    status = CALC_DIV_WAIT;
    
    return TRUE;
}

int CCalculatorWindow::OnEqual()
{
    std::tstring Number;
    if(CALC_STATUS_IDLE != status)
    {
         GetDigit(Number);
         Calculate(Number);
         SendMessage(edit_control.getHandle(), EM_CALC_DISPLAY_RESULT, 0, 0);
         status = CALC_STATUS_IDLE;
    }

    return TRUE;
}

int CCalculatorWindow::OnRegPlus()
{
    std::tstring Number;
    GetDigit(Number);
    
    reg.plus(Number);
    return TRUE;
}

int CCalculatorWindow::OnRegSub()
{
    std::tstring Number;
    GetDigit(Number);
    
    reg.sub(Number);
    
    return TRUE;
}

int CCalculatorWindow::OnRegStore()
{
    std::tstring Number;
    GetDigit(Number);
    
    reg.assign(Number);
    return TRUE;
}

int CCalculatorWindow::OnRegShow()
{
    std::tstring Number;
    reg.getResult(Number);
    
    SendMessage(edit_control.getHandle(), WM_SETTEXT, 0, (LPARAM)Number.c_str());
    
    return TRUE;
}

int CCalculatorWindow::CleanAll()
{
    status = CALC_STATUS_IDLE;
    SendMessage(edit_control.getHandle(), WM_SETTEXT, 0 ,(LPARAM)TEXT(""));

    return TRUE;
}

int CCalculatorWindow::CleanCurrent()
{
    SendMessage(edit_control.getHandle(), WM_SETTEXT, 0 ,(LPARAM)TEXT(""));

    return TRUE;
}
    
int CCalculatorWindow::OnControlNotify(UINT NotifyCode, UINT ID, HWND Handle)
{
#ifdef _DEBUG
	TCHAR string[100];
	lsprintf(string, TEXT("[Controls] NotifyCode = 0x%x, ID = 0x%x, Handle = 0x%x\n") ,NotifyCode, ID, Handle);
    std::wcout << string;
#endif

    switch(ID)
    {
    case IDC_PLUS:
        return OnPlus();break;
    case IDC_SUB:
        return OnSub();break;           
    case IDC_MUL:
        return OnMul();break;                        
    case IDC_DIV:
        return OnDiv();break;                         
    case IDC_CLEAR:
        CleanAll();
        break;
    case IDC_CLEAR_CUR:
        CleanCurrent();
        break;
    case IDC_NUM_9:
        SendMessage(edit_control.getHandle(), WM_CHAR, (WPARAM)'9', 0);
        break;
    case IDC_NUM_1:
        SendMessage(edit_control.getHandle(), WM_CHAR, (WPARAM)'1', 0);
        break;                       
    case IDC_NUM_3:
        SendMessage(edit_control.getHandle(), WM_CHAR, (WPARAM)'3', 0);
        break;                      
    case IDC_NUM_2:
        SendMessage(edit_control.getHandle(), WM_CHAR, (WPARAM)'2', 0);
        break;                       
    case IDC_NUM_5:
        SendMessage(edit_control.getHandle(), WM_CHAR, (WPARAM)'5', 0);
        break;                       
    case IDC_NUM_0:
        SendMessage(edit_control.getHandle(), WM_CHAR, (WPARAM)'0', 0);
        break;                       
    case IDC_NUM_4:
        SendMessage(edit_control.getHandle(), WM_CHAR, (WPARAM)'4', 0);
        break;                       
    case IDC_NUM_7:
        SendMessage(edit_control.getHandle(), WM_CHAR, (WPARAM)'7', 0);
        break;                       
    case IDC_NUM_6:
        SendMessage(edit_control.getHandle(), WM_CHAR, (WPARAM)'6', 0);
        break;                       
    case IDC_NUM_8:
        SendMessage(edit_control.getHandle(), WM_CHAR, (WPARAM)'8', 0);
        break;                       
    case IDC_EQUAL:
        OnEqual();
        break;                                        
    case IDC_MEM_PLUS:
        OnRegPlus();
        break;                    
    case IDC_MEM_SUB:
        OnRegSub();
        break;                     
    case IDC_MEM_CLEAR:
        reg.assign(std::tstring(TEXT("0")));
        break;               
    case IDC_MEM_READ:
        OnRegShow();
        break;
    case IDC_MEM_STORE:
        OnRegStore();
        break;
    case IDC_DOT:
        SendMessage(edit_control.getHandle(), WM_CHAR, (WPARAM)'.', 0);
        break;
        
    case IDC_BACKSPACE:
        SendMessage(edit_control.getHandle(), WM_CHAR, (WPARAM)0x08/* Backspace */, 0);
        break;
    case IDC_EDIT1:
        return OnEditChange(Handle);
    }
	return TRUE;
}

/////////////////////////////////////////////

CCalculatorEditControl::CCalculatorEditControl()
:status(CALC_EDIT_CTL_IDLE)
{
}

LRESULT CCalculatorEditControl::handleMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
    TCHAR *string;
    DWORD length;
    uint scan;
    
    //character filter
    switch(message)
    {
    case EM_CALC_DISPLAY_RESULT:
        status = CALC_EDIT_CTL_DISPLAY_RESULT;
        return 0;
        
    case WM_CHAR:
        switch(wParam)
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            break;
        case '+':
        case '-':
        case '*':
        case '/':
        case '=':
        case VK_RETURN:
        case 'c':
        case 'C':
            SendMessage(::GetParent(handle), WM_CHAR, wParam, lParam);
            return 0;
            
        case 0x08: //backspace
            break;
        case '.':
            length = SendMessage(handle, EM_LINELENGTH, 0, 0);
            if(!length)
                break;
    
            string = new TCHAR[length + 1];
            if(!string)
                return 0;
    
            SendMessage(handle, WM_GETTEXT, length + 1, (LPARAM)string);
            
            for(scan = 0 ; string[scan] ; scan++)
                if('.' == string[scan])
                {
                    delete string;
                    return 0;
                }
                
            delete string;
            
            break;
        
        default:
            return 0;
        }
        if(CALC_EDIT_CTL_DISPLAY_RESULT == status)
        {
            status = CALC_EDIT_CTL_IDLE;
            SendMessage(handle, WM_SETTEXT, 0, (LPARAM)TEXT(""));
        }
        break;
    }
    
    return origin_process(handle, message, wParam, lParam);
}