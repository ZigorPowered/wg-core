#pragma once

//#include "stdafx.h"

#define CORE_TMP_WINDOW			"CORE_TEMPLATE_BASIC_WINDOW"
#define CORE_RC_CURSOR_DEFAULT	"CORE_RC_DEFAULT_CURSOR"

typedef unsigned long COREint32, COREColor, COREFlag;
typedef LPCTSTR COREcstr, COREResource;

typedef HINSTANCE COREAppHandle;
typedef HMENU COREControlID;
typedef HWND COREControlHandle;

typedef std::unordered_map<COREResource, COREint32> COREResourceMap;

struct COREControlTemplate;

struct COREControlTemplate
{
	COREcstr templateID;
	COREControlID controlID;
	int xPos;
	int yPos;
	int width;
	int height;
	COREcstr title;
	COREColor bgColor;
	COREResource cursorType;
	COREResource defaultIcon;
	COREFlag templateFlags;
	COREFlag controlStyle;
	COREFlag controlExStyle;
};

extern COREResourceMap* CORE_RC_MAP;

void LoadResource(COREResource resourceID, const char* rcFile)
{
	FILE* pFile = fopen(rcFile, "r");
	if (pFile == NULL)
		return;

	char readBuffer[256];
	char* rcName;
	while (!feof(pFile))
	{
		fgets(readBuffer, 256, pFile);

		if (readBuffer[0] == '#')
		{
			rcName = &readBuffer[1];
			if (strcmp(rcName, (LPCSTR)resourceID) == 0)
			{

			}
		}
	}

}

COREAppHandle GetMainHandle()
{
	return GetModuleHandle(NULL);
}

void LoadTemplate(COREControlTemplate* targetTemplate, COREResource templateID)
{
}

COREControlHandle CreateControl(const COREControlTemplate* controlTemplate, COREControlHandle parentHdl, COREAppHandle mainHdl)
{
	if (mainHdl == NULL)
		mainHdl = GetMainHandle();

	WNDCLASSEX clsControl;
	memset(&clsControl, 0, sizeof(WNDCLASSEX));
	clsControl.cbSize = sizeof(WNDCLASSEX);
	clsControl.lpszClassName = controlTemplate->templateID;
	clsControl.lpfnWndProc = DefWindowProc;
	clsControl.hInstance = mainHdl;

	clsControl.style = controlTemplate->templateFlags;
	clsControl.hbrBackground = CreateSolidBrush(controlTemplate->bgColor);
	clsControl.hCursor = LoadCursor(0, controlTemplate->cursorType);
	clsControl.hIcon = LoadIcon(0, controlTemplate->defaultIcon);

	if (RegisterClassEx(&clsControl) == 0)
		return NULL;

	HWND hControl = CreateWindowEx(controlTemplate->controlExStyle,
									controlTemplate->templateID,
									controlTemplate->title,
									controlTemplate->controlStyle,
									controlTemplate->xPos,
									controlTemplate->yPos,
									controlTemplate->width,
									controlTemplate->height,
									parentHdl, controlTemplate->controlID, mainHdl, 0);

	return hControl;
}