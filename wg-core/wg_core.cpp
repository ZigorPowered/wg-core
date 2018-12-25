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
void CoreDlgProc::set_callback(CoreFlagU callbackID, CoreCallback callback)
{
	//TODO: Verify callbackID as valid WindowsMessage
	if (callback == nullptr)
		return;
	(*_callbacks)[callbackID] = callback;
}
void CoreDlgProc::remove_callback(CoreFlagU callbackID)
{
	_callbacks->erase(callbackID);
}
CoreProcHdl CoreDlgProc::get_proc_handle()
{
	return this->_proc;
}
/**************************[PRIVATE METHODS]**************************/
LRESULT CoreDlgProc::_proc(CoreDlgHdl hDlg, CoreFlagU msgFlags, CorePtrU wPrm, CorePtrL lPrm)
{
	auto callbackSearch = _callbacks->find(msgFlags);
	if (callbackSearch == _callbacks->end())
		return core::DefaultDialogProc(hDlg, msgFlags, wPrm, lPrm);

	CoreCallback callback = callbackSearch->second;
	return callback(hDlg, wPrm, lPrm);
}
/*******************************************************************************************************/

/*********************************************[CoreDlgContext]*********************************************/

/**************************[CONSTRUCTORS / DESTRUCTORS]**************************/
CoreDlgContext::CoreDlgContext(CoreCSTR contextName) :
	_contextName(contextName), _data(new WNDCLASSEX)
{
	memset(_data, 0, sizeof(WNDCLASSEX));
	_data->cbSize = sizeof(WNDCLASSEX);
	_data->lpszClassName = _contextName;
	_data->hInstance = core::GetAppHandle();
	_data->lpfnWndProc = core::DefaultDialogProc;
}
CoreDlgContext::CoreDlgContext(CoreCSTR contextName, CoreDlgProc* contextProc) : 
	CoreDlgContext(contextName)
{
	_data->lpfnWndProc = contextProc->get_proc_handle();
}
CoreDlgContext::CoreDlgContext(CoreCSTR contextName, CoreDlgProc* contextProc, CoreAppHdl hApp, CoreFlagU contextStyles, HBRUSH bgData, HCURSOR cursorData, HICON iconData) :
	CoreDlgContext(contextName, contextProc)
{
	_data->hInstance = hApp;
	_data->style = contextStyles;
	_data->hbrBackground = bgData;
	_data->hCursor = cursorData;
	_data->hIcon = iconData;
}
CoreDlgContext::~CoreDlgContext()
{
	UnregisterClass(_contextName, _data->hInstance);
	delete _data;
}

/**************************[PUBLIC METHODS]**************************/
int CoreDlgContext::register_context()
{
	auto status = RegisterClassEx(_data);
	if (status == 0)
		return CORE_ERROR;
	return CORE_NO_ERROR;
}
int CoreDlgContext::set_context_data(CoreCSTR contextName, CoreDlgProc* contextProc, CoreAppHdl hApp, CoreFlagU contextStyles, HBRUSH bgData, HCURSOR cursorData, HICON iconData)
{
	//TODO: Check for register befoire changing
}
/**********************************************************************************************************/

/*********************************************[core]*********************************************/
CoreAppHdl core::GetAppHandle()
{
	return GetModuleHandle(NULL);
}
LRESULT __stdcall core::DefaultDialogProc(CoreDlgHdl hDialog, CoreFlagU procFlags, CoreProcArgU uArgs, CoreProcArg args)
{
	return DefWindowProc(hDialog, procFlags, uArgs, args);
}
/************************************************************************************************/