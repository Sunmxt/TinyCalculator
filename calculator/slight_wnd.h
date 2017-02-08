#ifndef SLIGHT_UI_WINDOWS_HEADER
#define SLIGHT_UI_WINDOWS_HEADER

#include "object.h"
namespace Slight 
{
    class CWinNativeView : public CObject
	{
	public:
		static LRESULT CALLBACK InitialWindowProcess1(HWND handle ,UINT message ,WPARAM wParam ,LPARAM lParam);
		static LRESULT CALLBACK InitialWindowProcess2(CWinNativeView *view ,UINT message ,WPARAM wParam ,LPARAM lParam);
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

		int getWidth();
		int getHeight();

		virtual LRESULT handleMessage(UINT message ,WPARAM wParam ,LPARAM lParam);

	private:
		void *private_data;

	protected:
		HWND handle;
	};
}

#endif