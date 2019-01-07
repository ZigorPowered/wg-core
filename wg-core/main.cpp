#include "stdafx.h"
#include "wg_core.h"

WGCptr64 QuitWindow(WGCdlg_h winHdl, WGCFlag32 quitFlags, WGCptr32 arg1 = WGC_UNUSED_PARAM, WGCptr64 arg2 = WGC_UNUSED_PARAM)
{
	DestroyWindow(winHdl);
	PostQuitMessage(0);
	return WGC_CALLBACK_RESULT(0);
}

int __stdcall WinMain(HINSTANCE hCurrent, HINSTANCE hPrev, LPSTR cmd, int nCmd)
{
	WGCapp_h hMainApp = CORE::GetAppHandle();

	WGCcstr winContextName = WGC_CSTR("WindowContext");
	CORE::CreateDlgContext(winContextName, hMainApp, CORE::DefaultDlgProc);

	WGCDialog mainWin(winContextName, hMainApp, 100, 100, 800, 600, WGC_CSTR("Main Window"), WS_OVERLAPPEDWINDOW, 0, 0, 0);
	WGCDialog toolWin(winContextName, hMainApp, 200, 200, 900, 400, WGC_CSTR("Tool Box"), WS_OVERLAPPEDWINDOW, 0, 0, 0);

	WGCDialogProc mainWinProc;
	mainWinProc.set_callback(WGC_EVENT_CLOSE_DLG, QuitWindow);

	WGCEventWatch appEventWatch;
	appEventWatch.register_dialog_proc(mainWin.get_handle(), &mainWinProc);

	if (mainWin.start() == WGC_ERROR)
		return 0;

	while (appEventWatch.is_active())
		appEventWatch.update();


	return 0;
}