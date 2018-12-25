#pragma once

#define CORE_ERROR -1;
#define CORE_NO_ERROR 0;

typedef UINT CoreUINT, CoreFlagU;
typedef LPCTSTR CoreCSTR;
typedef HINSTANCE CoreAppHdl;
typedef WNDPROC CoreProcHdl;
typedef HWND CoreDlgHdl;
typedef WPARAM CoreProcArgU, CorePtrU;
typedef LPARAM CoreProcArg, CorePtrL;

typedef LRESULT(*CoreCallback)(CoreDlgHdl, CorePtrU, CorePtrL);
typedef std::unordered_map<CoreFlagU, CoreCallback> CoreProcMap;

class CoreDlgProc;
class CoreDlgContext;

class CoreDlgProc
{
public:
	CoreDlgProc();
	~CoreDlgProc();

	void set_callback(CoreFlagU callbackID, CoreCallback callback);
	void remove_callback(CoreFlagU callbackID);
	CoreProcHdl get_proc_handle();
private:
	CoreProcMap* _callbacks;
	LRESULT _proc(CoreDlgHdl hDlg, CoreFlagU msgFlags, CorePtrU wPrm, CorePtrL lPrm);
};

class CoreDlgContext
{
public:
	CoreDlgContext(CoreCSTR contextName);
	CoreDlgContext(CoreCSTR contextName, CoreDlgProc* contextProc);
	CoreDlgContext(CoreCSTR contextName, CoreDlgProc* contextProc, CoreAppHdl hApp, CoreFlagU contextStyles, HBRUSH bgData, HCURSOR cursorData, HICON iconData);
	~CoreDlgContext();

	int register_context();
	int set_context_data(CoreCSTR contextName, CoreDlgProc* contextProc, CoreAppHdl hApp, CoreFlagU contextStyles, HBRUSH bgData, HCURSOR cursorData, HICON iconData);
private:
	WNDCLASSEX* _data;
	CoreCSTR _contextName;
};

namespace core
{
	CoreAppHdl GetAppHandle();
	LRESULT __stdcall DefaultDialogProc(CoreDlgHdl hDialog, CoreFlagU procFlags, CoreProcArgU uArgs, CoreProcArg args);
}





