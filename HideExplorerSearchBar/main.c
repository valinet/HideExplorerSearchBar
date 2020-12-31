#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Commctrl.h>

// https://stackoverflow.com/questions/30141592/how-do-i-find-a-handle-inside-a-control
HWND FindChildWindow(
	HWND hwndParent,
	wchar_t* lpszClass
)
{
	HWND hwnd = FindWindowEx(
		hwndParent,
		NULL,
		lpszClass,
		NULL
	);
	if (hwnd == NULL)
	{
		HWND hwndChild = FindWindowEx(
			hwndParent,
			NULL,
			NULL,
			NULL
		);
		while (hwndChild != NULL && hwnd == NULL)
		{
			hwnd = FindChildWindow(
				hwndChild,
				lpszClass
			);
			if (hwnd == NULL)
			{
				hwndChild = FindWindowEx(
					hwndParent,
					hwndChild,
					NULL,
					NULL
				);
			}
		}
	}
	return hwnd;
}

// https://github.com/Open-Shell/Open-Shell-Menu/blob/master/Src/ClassicExplorer/ExplorerBHO.cpp
VOID HideExplorerSearchBar(HWND hWnd)
{
	HWND band = NULL, rebar = NULL;
	band = FindChildWindow(
		hWnd,
		L"TravelBand"
	);
	if (!band)
	{
		return;
	}
	rebar = GetParent(band);
	if (!rebar)
	{
		return;
	}
	int idx = 0;
	idx = (int)SendMessage(
		rebar,
		RB_IDTOINDEX,
		4,
		0
	);
	if (idx >= 0)
	{
		SendMessage(
			rebar,
			RB_SHOWBAND,
			idx,
			FALSE
		);
	}
	idx = (int)SendMessage(
		rebar,
		RB_IDTOINDEX,
		5,
		0
	);
	if (idx >= 0)
	{
		SendMessage(
			rebar,
			RB_SHOWBAND,
			idx,
			TRUE
		);
	}
	RedrawWindow(
		rebar,
		NULL,
		NULL,
		RDW_UPDATENOW | RDW_ALLCHILDREN
	);
}

// https://stackoverflow.com/questions/4407631/is-there-windows-system-event-on-active-window-changed
VOID CALLBACK WinEventProcCallback(
	HWINEVENTHOOK hWinEventHook,
	DWORD dwEvent,
	HWND hwnd,
	LONG idObject,
	LONG idChild,
	DWORD dwEventThread,
	DWORD dwmsEventTime
)
{
	wchar_t szClassName[257];
	szClassName[0] = 0;
	GetClassName(
		hwnd,
		szClassName,
		257
	);
	if (wcscmp(szClassName, L"CabinetWClass") ||
		wcscmp(szClassName, L"#32770"))
	{
		Sleep(100);
		HideExplorerSearchBar(hwnd);
	}
}

int WINAPI wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	MSG msg;
	HWINEVENTHOOK hook;

	if (!SetProcessDpiAwarenessContext(
		DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2
	))
	{
		return 0;
	}
	if (!(hook = SetWinEventHook(
		EVENT_SYSTEM_FOREGROUND,
		EVENT_SYSTEM_FOREGROUND,
		NULL,
		WinEventProcCallback,
		0,
		0,
		WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS
	)))
	{
		return 0;
	}
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	UnhookWinEvent(hook);
	return 0;
}