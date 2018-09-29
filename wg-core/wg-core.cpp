#include "stdafx.h"

/**********************************************
FUNC DECLARATIONS
**********************************************/
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wPrm, LPARAM lPrm);
BOOL InitWndCls(LPCTSTR clsName, WNDPROC clsProc);
VOID SetWndClsStyle(HWND hClsInst, UINT styleFlags, HBRUSH bgBrush, HICON icon, HCURSOR cursor);

/**********************************************
GLOBAL VARIABLES
**********************************************/
HINSTANCE hAppInst;

/**********************************************
ENTRY POINT
**********************************************/
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	//Declare Unused Params
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
	if ((hAppInst = hInstance) == NULL)
		return -1;

    MSG appMsg;
	HWND hMainWin;

	if (!InitWndCls(_T("CLS_MAINWIN"), MainWndProc))
		return -1;
	
	hMainWin = CreateWindowEx(0, _T("CLS_MAINWIN"), _T("WGCore Main"), WS_OVERLAPPEDWINDOW, 0, 0, 1600, 800, 0, 0, hAppInst, 0);
	if (hMainWin == NULL)
	{
		UnregisterClass(_T("CLS_MAINWIN"), hAppInst);
		return -1;
	}
	SetWndClsStyle(hMainWin, CS_OWNDC | CS_HREDRAW | CS_VREDRAW, 0, LoadIcon(0, IDI_APPLICATION), LoadCursor(0, IDC_ARROW));


	ShowWindow(hMainWin, SW_SHOW);
	UpdateWindow(hMainWin);
    
    while (GetMessage(&appMsg, nullptr, 0, 0))
    {
            TranslateMessage(&appMsg);
            DispatchMessage(&appMsg);
    }

    return (int) appMsg.wParam;
}

BOOL InitWndCls(LPCTSTR clsName, WNDPROC clsProc)
{
	WNDCLASSEX newWndCls;
	memset(&newWndCls, 0, sizeof(WNDCLASSEX));
	newWndCls.cbSize = sizeof(WNDCLASSEX);
	newWndCls.hInstance = hAppInst;
	newWndCls.lpszClassName = clsName;
	newWndCls.lpfnWndProc = clsProc;
	return (RegisterClassEx(&newWndCls) == 0) ? false : true;
}
VOID SetWndClsStyle(HWND hClsInst, UINT styleFlags, HBRUSH bgBrush, HICON icon, HCURSOR cursor)
{
	SetClassLong(hClsInst, GCL_STYLE, (LONG)styleFlags);
	SetClassLong(hClsInst, GCLP_HBRBACKGROUND, (LONG)bgBrush);
	SetClassLong(hClsInst, GCLP_HICON, (LONG)icon);
	SetClassLong(hClsInst, GCL_HCURSOR, (LONG)cursor);
}



/**********************************************
MainWin Proc
**********************************************/
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wPrm, LPARAM lPrm)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, msg, wPrm, lPrm);
    }
    return 0;
}
