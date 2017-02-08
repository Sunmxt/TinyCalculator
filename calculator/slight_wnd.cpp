#include "slight_wnd.h"

namespace Slight {
    class CWindowThunk32
	{
	public:
		#pragma pack(push ,1)
		typedef struct Window_Thunk_Execute_Code 
		{
			BYTE mov_code_1;//mov dowrd ptr [handle] ,object_ptr
			BYTE mov_code_2;
			BYTE mov_code_3;
			BYTE mov_code_4;
			DWORD object_ptr;

			BYTE push_code;//push process_address
			WNDPROC process_address;

			BYTE ret_code;//ret (jmp)
		}ThunkCode;
		#pragma pack(pop)

	public:
		CWindowThunk32();
		~CWindowThunk32();

		WNDPROC setProcess(WNDPROC Process);
		DWORD setPointer(DWORD Pointer);
		
		bool lock();
		bool unlock();

		bool init();

		void* getThunk() const;
	private:
		ThunkCode *thunk;
	};

	template<int SizeOfPointer = sizeof(void*)>
	class CWindowThunkTrait
	{};

	template<>
	class CWindowThunkTrait<4>
	{
	public:
		typedef CWindowThunk32 ThunkType;
		typedef DWORD PointerDataType;
	};

	typedef struct Window_Create_Params 
	{
		LPVOID user;
		void* object;
	}WindowCreateParams;
}

using namespace Slight;
/////////////////////////////////////////////////////
CWindowThunk32::CWindowThunk32()
:thunk(0)
{
	init();
}

CWindowThunk32::~CWindowThunk32()
{
	if(thunk)
	{
		::VirtualFree(thunk ,sizeof(ThunkCode) ,MEM_DECOMMIT);
	}
}

bool CWindowThunk32::init()
{
	thunk = (ThunkCode*)VirtualAlloc(NULL ,sizeof(ThunkCode) ,MEM_COMMIT ,PAGE_EXECUTE_READWRITE);
	if(!thunk)
	{
		return false;
	}

	/*
		C7 44 24 04 78 56 34 12 mov         dword ptr [esp+4],12345678h
		68 78 56 34 12			push        12345678h 
		C3						ret    
	*/

	thunk -> mov_code_1 = 0xc7;
	thunk -> mov_code_2 = 0x44;
	thunk -> mov_code_3 = 0x24;
	thunk -> mov_code_4 = 0x04;
	thunk -> object_ptr = 0;

	thunk -> push_code = 0x68;
	thunk -> process_address = 0;

	thunk -> ret_code = 0xc3;

	return true;
}

WNDPROC CWindowThunk32::setProcess(WNDPROC Process)
{
	WNDPROC old;

	old = thunk -> process_address;
	thunk -> process_address = Process;

	return old;
}

DWORD CWindowThunk32::setPointer(DWORD Pointer)
{
	DWORD old;

	old = thunk -> object_ptr;
	thunk -> object_ptr = Pointer;

	return old;
}
		
bool CWindowThunk32::lock()
{
	DWORD old_protect;
	return ::VirtualProtect(thunk ,sizeof(ThunkCode) ,PAGE_EXECUTE_READ ,&old_protect);
}

bool CWindowThunk32::unlock()
{
	DWORD old_protect;
	return ::VirtualProtect(thunk ,sizeof(ThunkCode) ,PAGE_EXECUTE_READWRITE ,&old_protect);
}

void* CWindowThunk32::getThunk() const
{return thunk;}


LRESULT CALLBACK CWinNativeView::InitialWindowProcess1(HWND handle ,UINT message ,WPARAM wParam ,LPARAM lParam)
{
	typedef CWindowThunkTrait<>::ThunkType WindowThunk;
    WindowCreateParams *params;
    CWinNativeView *view;
        
	if(WM_NCCREATE == message)
	{
		LPCREATESTRUCT create_struct;
		

		create_struct = (LPCREATESTRUCT)lParam;
		params = (WindowCreateParams*)create_struct -> lpCreateParams;
		view = (CWinNativeView*)params -> object;
		create_struct -> lpCreateParams = params -> user;

		view -> handle = handle;

		::SetWindowLong(handle ,GWL_WNDPROC ,(LONG)((WindowThunk*)view -> private_data) -> getThunk());
		
		return view -> handleMessage(message ,wParam ,lParam);
	}
    else if(WM_INITDIALOG == message)
    {
        params = (WindowCreateParams*)lParam;
        view = (CWinNativeView*)params -> object;
        view -> handle = handle;
        
        ((WindowThunk*)view -> private_data) -> setProcess((WNDPROC)WindowProcess);
		((WindowThunk*)view -> private_data) -> lock();
        
        ::SetWindowLong(handle ,GWL_WNDPROC ,(LONG)((WindowThunk*)view -> private_data) -> getThunk());
    }

	return ::DefWindowProc(handle ,message ,wParam ,lParam);
}

LRESULT CALLBACK CWinNativeView::InitialWindowProcess2(CWinNativeView *view ,UINT message ,WPARAM wParam ,LPARAM lParam)
{
	typedef CWindowThunkTrait<>::ThunkType WindowThunk;
    WindowCreateParams *params;
    
	if(WM_CREATE == message)
	{
		LPCREATESTRUCT create_struct;
        
		create_struct = (LPCREATESTRUCT)lParam;
		params = (WindowCreateParams*)create_struct -> lpCreateParams;
		create_struct -> lpCreateParams = params -> user;
        
        ((WindowThunk*)view -> private_data) -> setProcess((WNDPROC)WindowProcess);
		((WindowThunk*)view -> private_data) -> lock();
	}
    
	return view -> handleMessage(message ,wParam ,lParam);
}

LRESULT CALLBACK CWinNativeView::WindowProcess(CWinNativeView *view ,UINT message ,WPARAM wParam ,LPARAM lParam)
{
	LRESULT return_value;

	return_value = view -> handleMessage(message ,wParam ,lParam);

	if(WM_NCDESTROY == message)
	{
		view -> detach();
	}

	return return_value;
}

LRESULT CWinNativeView::handleMessage(UINT message ,WPARAM wParam ,LPARAM lParam)
{
	return ::DefWindowProc(handle ,message ,wParam ,lParam);
}

CWinNativeView::CWinNativeView()
:handle(0) ,private_data(0)
{}

CWinNativeView::~CWinNativeView()
{
	if(handle)
	{
		destroy();
	}
	if(private_data)
	{
		delete private_data;
	}
}

bool CWinNativeView::create(int X ,int Y ,int Width ,int Height ,wchar_t *Title)
{
	typedef CWindowThunkTrait<>::ThunkType WindowThunk;
	typedef CWindowThunkTrait<>::PointerDataType PointerData;

	if(!Title)
	{
		setError(ERR_INVAILD_PARAMETER);
		return false;
	}

	WindowThunk *thunk;
	WindowCreateParams params;

	if(!private_data)
	{
		private_data = new WindowThunk;
	}
	thunk = (WindowThunk*)private_data;

	thunk -> setProcess((WNDPROC)InitialWindowProcess2);
	thunk -> setPointer((PointerData)this);

	params.object = this;
	params.user = NULL;

	handle = ::CreateWindowExW(WS_EX_OVERLAPPEDWINDOW ,L"Slight_UI_0" ,Title ,WS_OVERLAPPEDWINDOW ,X ,Y 
								,Width ,Height ,NULL ,NULL ,::GetModuleHandle(NULL) ,&params);

	if(!handle)
	{
		setError(ERR_OS_ERROR);
		return false;
	}

	::ShowWindow(handle ,SW_SHOW);
	::UpdateWindow(handle);

	return true;
}

bool CWinNativeView::createDialog(UINT ResourceID)
{
    typedef CWindowThunkTrait<>::ThunkType WindowThunk;
	typedef CWindowThunkTrait<>::PointerDataType PointerData;

	if(!ResourceID)
	{
		setError(ERR_INVAILD_PARAMETER);
		return false;
	}
    
    WindowThunk *thunk;
	WindowCreateParams params;

	if(!private_data)
	{
		private_data = new WindowThunk;
	}
	thunk = (WindowThunk*)private_data;

	thunk -> setProcess((WNDPROC)InitialWindowProcess2);
	thunk -> setPointer((PointerData)this);

	params.object = this;
	params.user = NULL;
    
    handle = ::CreateDialogParam(::GetModuleHandle(NULL), MAKEINTRESOURCE(ResourceID), NULL, (DLGPROC)InitialWindowProcess1, (LPARAM)&params);

    if(!handle)
	{
		setError(ERR_OS_ERROR);
		return false;
	}
    
    ::ShowWindow(handle ,SW_SHOW);
	
	return true;
}

HWND CWinNativeView::getHandle() const
{
	return handle;
}

HWND CWinNativeView::detach()
{
	HWND old;

	old = handle;
	handle = 0;

	return old;
}

bool CWinNativeView::move(int X ,int Y)
{
	RECT rectangle;

	if(!::GetWindowRect(handle ,&rectangle))
	{
		setError(ERR_OS_ERROR);
		return false;
	}

	if(!::MoveWindow(handle ,X ,Y
					,rectangle.right - rectangle.left
					,rectangle.bottom - rectangle.top
					,TRUE))
	{
		setError(ERR_OS_ERROR);
		return false;
	}

	return true;
}

bool CWinNativeView::resize(int Width ,int Height)
{
	RECT rectangle;

	if(!::GetWindowRect(handle ,&rectangle))
	{
		setError(ERR_OS_ERROR);
		return false;
	}

	if(!::MoveWindow(handle 
					,rectangle.left ,rectangle.top
					,Width ,Height ,TRUE))
	{
		setError(ERR_OS_ERROR);
		return false;
	}

	return true;
}

bool CWinNativeView::destroy()
{
	return ::DestroyWindow(handle);
}

bool CWinNativeView::show()
{
	return ::ShowWindow(handle ,SW_SHOW);
}

bool CWinNativeView::hide()
{
	return ::ShowWindow(handle ,SW_HIDE);
}


int CWinNativeView::getWidth()
{
	RECT rectangle;

	if(!::GetWindowRect(handle ,&rectangle))
	{
		setError(ERR_OS_ERROR);
		return -1;
	}

	return rectangle.right - rectangle.left;
}

int CWinNativeView::getHeight()
{
	RECT rectangle;

	if(!::GetWindowRect(handle ,&rectangle))
	{
		setError(ERR_OS_ERROR);
		return false;
	}

	return rectangle.bottom - rectangle.top;
}