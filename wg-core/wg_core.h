#pragma once

#define WGC_ERROR			-1
#define WGC_NO_ERROR		0
#define WGC_UNUSED_PARAM	0

#define WGC_CSTR(s) _T(s)
#define WGC_MAKE_DLG_ID(i) (HMENU)i
#define WGC_CALLBACK_RESULT(n) (LRESULT)n

#define WGC_EVENT_QUIT			WM_QUIT
#define WGC_EVENT_CLOSE_DLG		WM_CLOSE
#define WGC_EVENT_PROC_USED		WM_USER + 1


typedef UINT WGCuint, WGCFlag32;
typedef LPCTSTR WGCcstr;
typedef DWORD WGCFlag64;
typedef WPARAM WGCptr_u, WGCptr32;
typedef LPARAM WGCptr_l, WGCptr64;

typedef HINSTANCE WGCapp_h;
typedef WNDPROC WGCproc;
typedef HWND WGCdlg_h;
typedef HMENU WGCdlg_id;

typedef HBRUSH WGCpalette_h;
typedef HICON WGCicon_rc;
typedef HCURSOR WGCcursor_rc;

typedef WGCptr64(*WGCcallback)(WGCdlg_h, WGCFlag32, WGCptr32, WGCptr64);

class WGCDlgProc;
class WGCDialog;
class WGCEventWatch;

namespace CORE
{
	WGCapp_h GetAppHandle()
	{
		return GetModuleHandle(0);
	}
	WGCptr64 CALLBACK DefaultDlgProc(WGCdlg_h dialogHdl, WGCFlag32 eventFlags, WGCptr32 uArgPtr, WGCptr64 lArgPtr)
	{
		if (eventFlags == WGC_EVENT_PROC_USED)
			return (WGCptr64)WGC_NO_ERROR;

		return DefWindowProc(dialogHdl, eventFlags, uArgPtr, lArgPtr);
	}
	int CreateDlgContext(WGCcstr contextName, WGCapp_h appHdl, WGCproc dlgProc, WGCFlag32 contextFlags = 0U, WGCpalette_h bgPalette = NULL, WGCcursor_rc cursorType = LoadCursor(0, IDC_ARROW), WGCicon_rc iconType = LoadIcon(0, IDI_APPLICATION))
	{
		WNDCLASSEX contextData;
		memset(&contextData, 0, sizeof(WNDCLASSEX));
		contextData.cbSize = sizeof(WNDCLASSEX);
		contextData.lpszClassName = contextName;
		contextData.hInstance = appHdl;
		contextData.lpfnWndProc = dlgProc;
		contextData.style = contextFlags;
		contextData.hbrBackground = bgPalette;
		contextData.hCursor = cursorType;
		contextData.hIcon = iconType;
		return (RegisterClassEx(&contextData) == 0) ? WGC_ERROR : WGC_NO_ERROR;
	}
}

class WGCDialogProc
{
public:
	WGCDialogProc() :
		_callbackMap(new std::unordered_map<WGCFlag32, WGCcallback>)
	{
	}
	~WGCDialogProc()
	{
		delete this->_callbackMap;
	}

	void set_callback(WGCFlag32 eventFlag, WGCcallback eventCallback)
	{
		(*this->_callbackMap)[eventFlag] = eventCallback;
	}
	WGCptr64 __stdcall handle_event(WGCdlg_h dialogHdl, WGCFlag32 eventFlags, WGCptr32 uArgPtr, WGCptr64 lArgPtr)
	{
		auto mapSearch = _callbackMap->find(eventFlags);

		if (mapSearch == _callbackMap->end())
			return CORE::DefaultDlgProc(dialogHdl, eventFlags, uArgPtr, lArgPtr);

		return (mapSearch->second)(dialogHdl, eventFlags, uArgPtr, lArgPtr);
	}
private:
	std::unordered_map<WGCFlag32, WGCcallback>* _callbackMap;
};

class WGCEventWatch
{
public:
	WGCEventWatch() :
		_eventData(new MSG), _procMap(new std::unordered_map<WGCdlg_h, WGCDialogProc*>)
	{
	}

	void register_dialog_proc(WGCdlg_h dialogHdl, WGCDialogProc* dlgProc)
	{
		(*this->_procMap)[dialogHdl] = dlgProc;
	}
	bool is_active()
	{
		return GetMessage(this->_eventData, 0, 0, 0) > 0;
	}
	int update()
	{
		if (!this->is_active())
			return WGC_ERROR;

		auto procSearch = _procMap->find(_eventData->hwnd);
		if (procSearch != _procMap->end())
		{
			procSearch->second->handle_event(_eventData->hwnd, _eventData->message, _eventData->wParam, _eventData->lParam);
			_eventData->message = WGC_EVENT_PROC_USED;
		}

		TranslateMessage(_eventData);
		DispatchMessage(_eventData);
		return WGC_NO_ERROR;
	}
private:
	MSG* _eventData;
	std::unordered_map<WGCdlg_h, WGCDialogProc*>* _procMap;
};

class WGCDialog
{
public:
	WGCDialog(WGCcstr contextName) :
		_contextName(contextName)
	{
		_handle = CreateWindowEx(0U, _contextName, _T("WGC Dialog"), WS_VISIBLE, 0, 0, 100, 100, NULL, 0, CORE::GetAppHandle(), NULL);
	}
	WGCDialog(WGCcstr contextName, WGCapp_h appHandle) : 
		_contextName(contextName)
	{
		_handle = CreateWindowEx(0U, _contextName, _T("WGC Dialog"), WS_VISIBLE, 0, 0, 100, 100, NULL, 0, appHandle, NULL);
	}
	WGCDialog(WGCcstr contextName, WGCapp_h appHandle, int xPos, int yPos, int width, int height, WGCcstr title) :
		_contextName(contextName)
	{
		_handle = CreateWindowEx(0U, _contextName, title, WS_VISIBLE, xPos, yPos, width, height, NULL, 0, appHandle, NULL);
	}
	WGCDialog(WGCcstr contextName, WGCapp_h appHandle, int xPos, int yPos, int width, int height, WGCcstr title, WGCFlag64 basicStyle, WGCFlag64 exStyle, WGCdlg_h hParentDlg, int idValue) :
		_contextName(contextName)
	{
		_handle = CreateWindowEx(exStyle, _contextName, title, basicStyle, xPos, yPos, width, height, hParentDlg, WGC_MAKE_DLG_ID(idValue), appHandle, NULL);
	}
	~WGCDialog()
	{
		DestroyWindow(this->_handle);
		this->_handle = NULL;
	}

	WGCdlg_h get_handle()
	{
		return this->_handle;
	}
	bool handle_is_valid()
	{
		return _handle != NULL;
	}
	int start()
	{
		if (_handle == NULL)
			return WGC_ERROR;

		ShowWindow(_handle, SW_SHOW);
		UpdateWindow(_handle);
		return WGC_NO_ERROR;
	}
private:
	WGCdlg_h _handle;
	WGCcstr _contextName;
};



