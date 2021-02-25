// FinClip.cpp : Defines the entry point for the application.
//
#include "framework.h"
#include "FinClip.h"
#include "finclip-sdk.h"
#include<iostream>

#pragma comment(lib, "FinClipSDK.lib")

#define MAX_LOADSTRING 100
using namespace std;
// Global Variables:
HINSTANCE hInst;
HWND gHwnd;// current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
HWND hWnd_appkey;
HWND hWnd_secret;
HWND hWnd_appid;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_FINCLIP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FINCLIP));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FINCLIP));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable
	DWORD dwStyle = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX; //���ô�����ʽ 

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, (WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_DLGFRAME) & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX,
		CW_USEDEFAULT, 0, 500, 650, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}
	gHwnd = hWnd;
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

std::string utf8_encode(const std::wstring& wstr, int CP = CP_UTF8)
{
	if (wstr.empty()) return std::string();
	int size_needed = WideCharToMultiByte(CP, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}

// Convert an UTF8 string to a wide Unicode String
std::wstring utf8_decode(const std::string& str, int CP = CP_UTF8)
{
	if (str.empty()) return std::wstring();
	int size_needed = MultiByteToWideChar(CP, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}
void init_finclipsdk(int server_type,std::wstring wappkey,std::wstring wsecret) {
	std::string appkey = utf8_encode(wappkey);
	std::string secret = utf8_encode(wsecret);
	FinConfig config = {
		server_type,
		"https://api.finclip.com",
		"/api/v1/mop",
		appkey.c_str(),
		secret.c_str(),
		"",
		1
	};
	IFinConfigPacker* configpacker = NewFinConfigPacker();
	configpacker->AddConfig(config);
	Initialize(hInst, configpacker);
}


void finclip_applet_callback(int ret,const char*) {
	//MessageBox(nullptr, L"oooo", L"title", 0);
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hwndButtonCintinue;

	switch (message)
	{
	case WM_COMMAND:
		if (LOWORD(wParam) == IDM_START_APPLET) {
			WCHAR key[1024];
			GetWindowText(hWnd_appkey, key, 1023);
			WCHAR secret[1024];
			GetWindowText(hWnd_secret, secret, 1023);
			WCHAR appid[1024];
			GetWindowText(hWnd_appid, appid, 1023);
			std::wstring wappkey(key);
			std::wstring wsecret(secret);
			std::wstring wappid(appid);
			if (wappkey.length() == 0) {
				MessageBox(NULL, L"������appKey", L"��ʾ", 0);
				return 0;
			}
			if (wsecret.length() == 0) {
				MessageBox(NULL, L"������Secret", L"��ʾ", 0);
				return 0;
			}
			if (wappid.length() == 0) {
				MessageBox(NULL, L"������appid", L"��ʾ", 0);
				return 0;
			}
			int server_type = 1;
			init_finclipsdk(server_type,wappkey, wsecret);
			IFinPacker* packer = NewFinPacker();
			packer->BeginPacker();
			packer->AddField("appId");
			packer->AddValue("appId");
			packer->EndPacker();
			StartApplet(server_type, utf8_encode(wappid).c_str(), packer, finclip_applet_callback);
			packer->Release();
		}
		break;
	case WM_PAINT:
		//{
		//    PAINTSTRUCT ps;
		//    HDC hdc = BeginPaint(hWnd, &ps);
		//    // TODO: Add any drawing code that uses hdc here...
		//    EndPaint(hWnd, &ps);
		//}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CREATE:
	{
		 CreateWindowW(
			L"static",  //��̬�ı��������
			L"AppKEY",  //�ؼ����ı�
			WS_CHILD /*�Ӵ���*/ | WS_VISIBLE /*����ʱ��ʾ*/ | WS_BORDER /*���߿�*/,
			20, 20, 60, 30,  hWnd/*�����ھ��*/,
			(HMENU)1,  //Ϊ�ؼ�ָ��һ��Ψһ��ʶ��
			((LPCREATESTRUCT)lParam)->hInstance,  //��ǰʵ�����
			NULL
		);

		CreateWindowW(
			L"static",  //��̬�ı��������
			L"Secret",  //�ؼ����ı�
			WS_CHILD /*�Ӵ���*/ | WS_VISIBLE /*����ʱ��ʾ*/ | WS_BORDER /*���߿�*/,
			20, 60, 60, 30, hWnd/*�����ھ��*/,
			(HMENU)2,  //Ϊ�ؼ�ָ��һ��Ψһ��ʶ��
			((LPCREATESTRUCT)lParam)->hInstance,  //��ǰʵ�����
			NULL
		);

		CreateWindowW(
			L"static",  //��̬�ı��������
			L"appid",  //�ؼ����ı�
			WS_CHILD /*�Ӵ���*/ | WS_VISIBLE /*����ʱ��ʾ*/ | WS_BORDER /*���߿�*/,
			20, 100, 60, 30, hWnd/*�����ھ��*/,
			(HMENU)2,  //Ϊ�ؼ�ָ��һ��Ψһ��ʶ��
			((LPCREATESTRUCT)lParam)->hInstance,  //��ǰʵ�����
			NULL
		);
		hWnd_appkey = CreateWindowW( L"EDIT", L"22LyZEib0gLTQdU3MUauAfJ/xujwNfM6OvvEqQyH4igA",
			WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT,
			100, 20, 250, 30,
			hWnd,
			(HMENU)IDM_APPLET_APPKEY, ((LPCREATESTRUCT)lParam)->hInstance, NULL);


		hWnd_secret = CreateWindowW(L"EDIT", L"703b9026be3d6bc5",
			WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT,
			100, 60, 250, 30,
			hWnd,
			(HMENU)IDM_APPLET_SECRET, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

		hWnd_appid = CreateWindowW(L"EDIT", L"5ea0401463cb900001d73865",
			WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT,
			100, 100, 250, 30,
			hWnd,
			(HMENU)IDM_APPLET_APPID, ((LPCREATESTRUCT)lParam)->hInstance, NULL);




		HWND hwndButton = CreateWindowW(
			L"BUTTON",  // Predefined class; Unicode assumed 
			L"��С����",      // Button text 
			WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles 
			100,         // x position 
			250,         // y position 
			200,        // Button width
			50,        // Button height
			hWnd,     // Parent window
			(HMENU)IDM_START_APPLET,       // No menu.
			((LPCREATESTRUCT)lParam)->hInstance,//(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), //hInst,
			NULL);

	}
	break;
	default:
		return DefWindowProcW(hWnd, message, wParam, lParam);
	}
	return 0;
}