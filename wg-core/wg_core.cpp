#include "stdafx.h"
#include "wg_core.h"

/*********************************************[CoreDlgProc]*********************************************/

/**************************[CONSTRUCTORS / DESTRUCTORS]**************************/
CoreDlgProc::CoreDlgProc() :
	_callbacks(new CoreProcMap)
{
}
CoreDlgProc::~CoreDlgProc()
{
	if (_callbacks != nullptr)
	{
		_callbacks->clear();
		delete _callbacks;
	}
}

/**************************[PUBLIC METHODS]**************************/
void CoreDlgProc::set_callback(CoreFlagU32 callbackID, CoreCallback callback)
{
	//TODO: Verify callbackID as valid WindowsMessage
	if (callback == nullptr)
		return;
	(*_callbacks)[callbackID] = callback;
}
void CoreDlgProc::remove_callback(CoreFlagU32 callbackID)
{
	_callbacks->erase(callbackID);
}
CoreDlgProcHdl CoreDlgProc::get_proc_handle()
{
	return this->_proc;
}
/**************************[PRIVATE METHODS]**************************/
LRESULT CoreDlgProc::_proc(CoreDlgHdl hDlg, CoreFlagU32 msgFlags, CorePtrU wPrm, CorePtrL lPrm)
{
	auto callbackSearch = _callbacks->find(msgFlags);
	if (callbackSearch == _callbacks->end())
		return core::DefaultDialogProc(hDlg, msgFlags, wPrm, lPrm);

	CoreCallback callback = callbackSearch->second;
	return callback(hDlg, wPrm, lPrm);
}
/*******************************************************************************************************/


/*********************************************[core]*********************************************/
CoreAppHdl core::GetAppHandle()
{
	return GetModuleHandle(NULL);
}
LRESULT __stdcall core::DefaultDialogProc(CoreDlgHdl hDialog, CoreFlagU32 procFlags, CoreProcArgU uArgs, CoreProcArg args)
{
	return DefWindowProc(hDialog, procFlags, uArgs, args);
}
int CreateDlgContext(CoreCSTR contextName, CoreDlgProc* contextProc, CoreAppHdl appHdl, CoreFlagU32 contextFlags, HBRUSH bgData, HCURSOR cursorData, HICON iconData)
{
	WNDCLASSEX clsData;
	memset(&clsData, 0, sizeof(WNDCLASSEX));
	clsData.cbSize = sizeof(WNDCLASSEX);
	clsData.lpszClassName = contextName;
	clsData.lpfnWndProc = contextProc->get_proc_handle();

}
/************************************************************************************************/