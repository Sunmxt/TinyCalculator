#ifndef SLIGHT_UI_WINDOWS_HEADER
#define SLIGHT_UI_WINDOWS_HEADER

#include "object.h"
namespace Slight 
{
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
    
    class CWinNativeView : public CObject
	{
	public:
		static LRESULT CALLBACK InitialWindowProcess1(HWND handle ,UINT message ,WPARAM wParam ,LPARAM lParam);
		static LRESULT CALLBACK InitialWindowProcess2(CWinNativeView *view ,UINT message ,WPARAM wParam ,LPARAM lParam);
        static LRESULT CALLBACK InitialDialogProcess(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK WindowProcess(CWinNativeView *View ,UINT message ,WPARAM wParam ,LPARAM lParam);
	public:
		CWinNativeView();
		~CWinNativeView();

		bool move(int X ,int Y);
		bool resize(int Width ,int Height);
		bool create(int X ,int Y ,int Width ,int Height ,wchar_t *Title);
        bool createDialog(UINT ResourceID);
		bool destroy();
		bool show();
		bool hide();
		
		HWND getHandle() const;

		HWND detach();
        bool attach(HWND Handle);

		int getWidth();
		int getHeight();

		virtual LRESULT handleMessage(UINT message ,WPARAM wParam ,LPARAM lParam);

	private:
		void *private_data;
        
        Slight::CWindowThunkTrait<>::ThunkType* alloc_thunk(bool UseVaild = false, WNDPROC Process = (WNDPROC)InitialWindowProcess2);
        bool free_thunk();

	protected:
		HWND handle;
        
        WNDPROC origin_process;
	};
    
    

}

#endif