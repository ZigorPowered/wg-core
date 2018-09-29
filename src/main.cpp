#include "stdafx.h"
#include "wgi.h"
#include <vector>
#include <unordered_map>

typedef std::vector<HWND*> HandleList;

/****************************************************************
GLOBAL VARIABLES
****************************************************************/
HINSTANCE glInst;
std::vector<LPCTSTR> glClassNames;
std::unordered_map<LPCTSTR, HandleList*> glClassMap;

WGICamera glMainCam;
const float CAM_H_FOV = 180.0f;
const float CAM_V_FOV = 90.0f;
const float CAM_DIST = 10.0f;

std::vector<WGIBounds*> glBoundsList;
std::unordered_map<WGIBounds*, WGIMesh*> glMeshMap;
std::vector<WGIPtLight*> glLightList;
//****************************************************************

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL				RegWndCls(LPCTSTR clsName, WNDPROC clsProc, UINT clsStyle = 0, HICON clsIcon = LoadIcon(0, IDI_WINLOGO), HCURSOR clsCursor = LoadCursor(0, IDC_ARROW), HBRUSH clsBG = CreateSolidBrush(RGB(0,0,0)));
BOOL				AddWinHdl(LPCTSTR clsName, int xPos, int yPos, int winWidth, int winHeight, LPCTSTR winTitle, DWORD style, HWND* pRecall = 0, DWORD exStyle = 0, HWND hParent = 0, HMENU winID = 0);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	if (hInstance == NULL)
		return 0;
	glInst = hInstance;

	float tc = cos(3.0f * PI / 4.0f);

	wgi::SetCamera(&glMainCam, CAM_H_FOV, CAM_V_FOV, CAM_DIST);
	WGIMesh testPlane;
	testPlane.vertices.push_back({ -1.0f, 1.0f, 1.0f });
	testPlane.vertices.push_back({ 1.0f, 1.0f, 1.0f });
	testPlane.vertices.push_back({ 1.0f, 1.0f, -1.0f });
	testPlane.vertices.push_back({ -1.0f, 1.0f, -1.0f });

	WGIBounds testBounds;
	wgi::GetMeshBounds(&testPlane, &testBounds);
	glBoundsList.push_back(&testBounds);
	glMeshMap[&testBounds] = &testPlane;

	MSG appMsg;
	HWND hMainWin;
	if (!RegWndCls(_T("APPWND"), WndProc, CS_HREDRAW | CS_VREDRAW | CS_OWNDC))
		return 0;

	if (!AddWinHdl(_T("APPWND"), 10, 10, 1280, 720, _T("Ray Tracing Test"), WS_OVERLAPPEDWINDOW, &hMainWin))
	{
		UnregisterClass(_T("APPWND"), glInst);
		return 0;
	}

    while (GetMessage(&appMsg, nullptr, 0, 0))
    {
        TranslateMessage(&appMsg);
        DispatchMessage(&appMsg);
    }

    return (int)appMsg.wParam;
}

BOOL RegWndCls(LPCTSTR clsName, WNDPROC clsProc, UINT clsStyle, HICON clsIcon, HCURSOR clsCursor, HBRUSH clsBG)
{
	WNDCLASSEX newExCls;
	memset(&newExCls, 0, sizeof(WNDCLASSEX));
	newExCls.cbSize = sizeof(WNDCLASSEX);
	newExCls.hInstance = glInst;
	newExCls.lpszClassName = clsName;
	newExCls.lpfnWndProc = clsProc;
	newExCls.style = clsStyle;
	newExCls.hIcon = clsIcon;
	newExCls.hCursor = clsCursor;
	newExCls.hbrBackground = clsBG;
	if (RegisterClassEx(&newExCls) == 0)
		return FALSE;

	glClassNames.push_back(clsName);
	glClassMap[clsName] = new HandleList;
	return TRUE;
}

BOOL AddWinHdl(LPCTSTR clsName, int xPos, int yPos, int winWidth, int winHeight, LPCTSTR winTitle, DWORD style, HWND* pRecall, DWORD exStyle, HWND hParent, HMENU winID)
{
	HWND hNewWin = CreateWindowEx(exStyle, clsName, winTitle, style, xPos, yPos, winWidth, winHeight, hParent, winID, glInst, 0);
	if (hNewWin == NULL)
		return FALSE;
	
	(glClassMap[clsName])->push_back(&hNewWin);
	*pRecall = hNewWin;

	ShowWindow(hNewWin, SW_SHOW);
	UpdateWindow(hNewWin);
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
	case WM_PAINT:
	{
		RECT winRC;
		PAINTSTRUCT winPS;
		HDC winDC = BeginPaint(hWnd, &winPS);
		GetWindowRect(hWnd, &winRC);

		for (int x = 0; x < winRC.right; x++)
		{
			for (int y = 0; y < winRC.bottom; y++)
			{
				WGIRay rayIter;
				wgi::GetCamRay(&glMainCam, x, y, winRC.right, winRC.bottom, &(rayIter.rayVec));
				for (int i = 0; i < glBoundsList.size(); i++)
				{
					if (wgi::IsBoundsVisible(&rayIter, glBoundsList[i]))
					{
						WGIMesh* activeMesh = glMeshMap.at(glBoundsList[i]);
						for (int j = 0; j < activeMesh->vertices.size(); j++)
						{
							if (wgi::CheckRayCollision(&rayIter, activeMesh->vertices[j]))
								SetPixel(winDC, x, y, RGB(0, 255, 0));
						}
					}
				}
			}
		}

		EndPaint(hWnd, &winPS);
	}
	break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

