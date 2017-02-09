#include "calc_window.h"
#include <tchar.h>
#include <iostream>

#include "resource.h"


CCalculatorWindow::CCalculatorWindow()
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
    default:
        //return ::DefWindowProc(handle, message, wParam, lParam);
        return FALSE;
	}
    
    return TRUE;
}

int CCalculatorWindow::OnInit(LPVOID Param)
{
    HWND edit_handle;

    edit_handle = GetDlgItem(handle ,IDC_EDIT1);
    ::SendMessage(edit_handle, EM_SETLIMITTEXT, 20, 0);

    edit_control.attach(edit_handle);
    return FALSE;
}

bool CCalculatorWindow::StandardizeString(TCHAR* String)
{
    uint scan, put;
    bool dot, result;
    
    put = scan = 0;
    result = dot = false;
    
    for(;String[scan]; scan++)
    {
        switch(String[scan])
        {
        case '.':
            if(dot)
            {
                result = true;
                continue;
            }
            dot = true;
            break;
        default:
            if(!isdigit(String[scan]))
            {
                result = true;
                continue;
            }
        }
        
        String[put] = String[scan];
        put++;
    }
    
    String[put] = 0;
    
    return result;
}

bool CCalculatorWindow::SaveFirstDigit()
{
    uint length;
    TCHAR* string;

    length = SendMessage(edit_control.getHandle(), EM_LINELENGTH, 0, 0);
    if(!length)
        return false;
    
    string = new TCHAR[length + 2];
    if(!string)
        return false;
    
    SendMessage(edit_control.getHandle(), WM_GETTEXT, length + 1, (LPARAM)string);
    
    FillDigitString(string);
    
    digit.append(string);
    
    delete string;
    
    return true;
}

bool CCalculatorWindow::FillDigitString(TCHAR* String)
{
    uint length;
    bool dot_front, result;
    
    result = 0;
    length = lstrlen(String);
    dot_front = true;
    
   /* if(0 == String - lstrchr(String, '.'))
    {
        dot_front = true;
        tmemmove(String + 1, String, length + 1);
        String[0] = '0';
        length += 1;
    }
    
    
    if(String - lstrrchr(String, '.'))*/

    return result;
}

int CCalculatorWindow::OnEditChange(HWND Handle)
{
    TCHAR *string;
    DWORD length;
    
    length = SendMessage(Handle, EM_LINELENGTH, 0, 0);
    if(!length)
        return 1;
    
    string = new TCHAR[length + 1];
    if(!string)
        return 0;
    
    SendMessage(Handle, WM_GETTEXT, length + 1, (LPARAM)string);
    
    if(StandardizeString(string))
        SendMessage(Handle, WM_SETTEXT, 0, (LPARAM)string);
    
    delete string;
    
    return 1;
}

int CCalculatorWindow::OnControlNotify(UINT NotifyCode, UINT ID, HWND Handle)
{
#ifdef _DEBUG
	TCHAR string[100];
	lsprintf(string, TEXT("NotifyCode = 0x%x, ID = 0x%x, Handle = 0x%x\n") ,NotifyCode, ID, Handle);
    std::wcout << string;
#endif

    switch(ID)
    {
    case IDC_PLUS:break;
    case IDC_SUB:break;           
    case IDC_MUL:break;                        
    case IDC_DIV:break;                         
    case IDC_CLEAR:break;             
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
        break;                                        
    case IDC_MEM_PLUS:break;                    
    case IDC_MEM_SUB:break;                     
    case IDC_MEM_CLEAR:break;                   
    case IDC_MEM_READ:break;
    case IDC_DOT:
        SendMessage(edit_control.getHandle(), WM_CHAR, (WPARAM)'.', 0);
        break;
    case IDC_EDIT1:
        return OnEditChange(Handle);
    }
	return TRUE;
}

/////////////////////////////////////////////

LRESULT CCalculatorEditControl::handleMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
    TCHAR *string;
    DWORD length;
    uint scan;
    
    //character filter
    switch(message)
    {
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
        case 0x08: //backspace
            break;
        case '.':
            length = SendMessage(handle, EM_LINELENGTH, 0, 0);
            if(!length)
                return 1;
    
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
        break;
    }
    
    return origin_process(handle, message, wParam, lParam);
}