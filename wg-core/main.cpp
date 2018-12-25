#include "stdafx.h"
#include "wg_core.h"

int main(int argc, char* argv[])
{
	CoreAppHdl appHdl = core::GetAppHandle();
	CoreDlgProc mainWinProc;
	CoreDlgContext mainWinContext(L"CONTEXT_MAIN_WIN", 
									&mainWinProc,
									appHdl,
									0,
									CreateSolidBrush(RGB(100,100,100)),
									LoadCursor(0, IDC_ARROW),
									LoadIcon(0, IDI_APPLICATION));


	CoreDlgHdl hMainWin;
	return 0;
}