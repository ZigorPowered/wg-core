#pragma once

#define CORE_ERROR -1;
#define CORE_NO_ERROR 0;

typedef UINT CoreUINT, CoreFlagU32;
typedef DWORD CoreULONG, CoreFlagU64;
typedef LPCTSTR CoreCSTR;
typedef HINSTANCE CoreAppHdl;
typedef WNDPROC CoreProc;
typedef HWND CoreDlgHdl;
typedef WPARAM CoreProcArgU, CorePtrU;
typedef LPARAM CoreProcArg, CorePtrL;

typedef LRESULT(*CoreCallback)(CoreDlgHdl, CorePtrU, CorePtrL);
typedef std::unordered_map<CoreFlagU32, CoreCallback> CoreProcMap;

class CoreDlgProc;
class CoreDlgContext;

typedef CoreProc(CoreDlgProc::* CoreDlgProcHdl)();

class CoreDlgProc
{
public:
	CoreDlgProc();
	~CoreDlgProc();

	void set_callback(CoreFlagU32 callbackID, CoreCallback callback);
	void remove_callback(CoreFlagU32 callbackID);
	CoreDlgProcHdl get_proc_handle();
private:
	CoreProcMap* _callbacks;
	LRESULT _proc(CoreDlgHdl hDlg, CoreFlagU32 msgFlags, CorePtrU wPrm, CorePtrL lPrm);
};

namespace core
{
	CoreAppHdl GetAppHandle();
	LRESULT __stdcall DefaultDialogProc(CoreDlgHdl hDialog, CoreFlagU32 procFlags, CoreProcArgU uArgs, CoreProcArg args);
	int CreateDlgContext(CoreCSTR contextName, CoreDlgProc* contextProc, CoreAppHdl appHdl, CoreFlagU32 contextFlags = 0, HBRUSH bgData = 0, HCURSOR cursorData = 0, HICON iconData = 0);
	CoreDlgHdl CreateDlg(HINSTANCE appHdl, CoreCSTR dlgContextName, CoreCSTR dlgTitle, int xPos, int yPos, int width, int height, CoreFlagU64 dlgStyle, HMENU dlgID = 0, HWND hParentDlg = 0);
}





