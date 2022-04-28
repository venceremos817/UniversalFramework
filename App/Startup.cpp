#include <Windows.h>
#include <crtdbg.h>
#include <string>

#include <App/App.h>

//===== 定数 =====
const char*	pClassName = "Class Name";
const char*	pWndTitle = "Game";

//===== プロトタイプ宣言 =====
ATOM MyRegisterClass(HINSTANCE hInstance);
HWND InitInstance(HINSTANCE hInstance, int nCmdShow, bool isFullScreen, int clientWidth, int clientHeight);
int	MainLoop(HINSTANCE hInstance, HWND hWnd, bool isFullScreen, int clientWidth, int clientHeight);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);



/// <summary>
/// エントリポイント
/// </summary>
/// <param name="hInstance"></param>
/// <param name="hPrevInstance"></param>
/// <param name="lpCmdLine"></param>
/// <param name="nCmdShow"></param>
/// <returns></returns>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//UNREFERENCED_PARAMETER(hPrevInstance);
	//UNREFERENCED_PARAMETER(lpCmdLine);

	// メモリリーク検出用
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// ウィンドウの幅と高さ
	int clientWidth = 1280;
	int clientHeight = 720;
	// フルスクリーンにするかを設定
	bool isFullScreen = false;

	// ウィンドウクラスを登録
	MyRegisterClass(hInstance);

	// アプリケーションの初期化
	HWND hWnd = InitInstance(hInstance, nCmdShow, isFullScreen, clientWidth, clientHeight);
	if (!hWnd)
	{
		return FALSE;
	}


	return MainLoop(hInstance, hWnd, isFullScreen, clientWidth, clientHeight);
}


/// <summary>
/// ウィンドウクラスを登録する
/// </summary>
/// <param name="hInstance"></param>
/// <returns></returns>
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(wcex));

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;	// ウィンドウスタイル
	wcex.lpfnWndProc = WndProc;				// ウィンドウプロシージャを設定
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;				// ウィンドウプロシージャがあるインスタンスハンドル
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);		// タスクバーに表示するアイコン
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);		// カーソルアイコン
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	// ウィンドウ背景色
	wcex.lpszMenuName = nullptr;			// デフォルトメニュー名
	wcex.lpszClassName = pClassName;		// ウィンドウクラス情報を識別する名前
	wcex.hIconSm = wcex.hIcon;				// 16*16アイコン

	// ウィンドウクラス情報を登録して返す
	return RegisterClassEx(&wcex);
}


/// <summary>
/// メインウィンドウを作成する
/// </summary>
/// <param name="hInstance"></param>
/// <param name="nCmdShow"></param>
/// <param name="isFullScreen"></param>
/// <param name="clientWidth"></param>
/// <param name="clientHeight"></param>
/// <returns></returns>
HWND InitInstance(HINSTANCE hInstance, int nCmdShow, bool isFullScreen, int clientWidth, int clientHeight)
{
	HWND hWnd = 0;

	// ウィンドウの作成
	if (isFullScreen)
	{
		// 画面全体の幅と高さを取得
		clientWidth = GetSystemMetrics(SM_CXSCREEN);
		clientHeight = GetSystemMetrics(SM_CYSCREEN);
		hWnd = CreateWindowEx(
			WS_EX_OVERLAPPEDWINDOW,		// 追加でウィンドウスタイルを指定
			pClassName,					// 作成するウィンドウクラスを指定
			pWndTitle,					// キャプションに表示する名前
			WS_POPUP,					// 基本のウィンドウスタイル
			0,							// ウィンドウ表示位置X
			0,							// ウインドウ表示位置Y
			clientWidth,				// フルスクリーンウィンドウの横幅
			clientHeight,				// フルスクリーンウインドウの高さ
			nullptr,					// 親ウインドウのハンドル（なし）
			nullptr,					// メニューや子ウインドウのハンドル
			hInstance,					// アプリケーションインスタンスのハンドル
			nullptr						// ウインドウの作成データ
		);
	}
	else
	{
		// ウィンドウのサイズ調整
		RECT rc = { 0,0,clientWidth,clientHeight };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
		hWnd = CreateWindowEx(
			WS_EX_OVERLAPPEDWINDOW,		// 追加でウィンドウスタイルを指定
			pClassName,					// 作成するウィンドウクラスを指定
			pWndTitle,					// キャプションに表示する名前
			WS_OVERLAPPEDWINDOW,		// 基本のウィンドウスタイル
			CW_USEDEFAULT,				// ウィンドウ表示位置X
			CW_USEDEFAULT,				// ウインドウ表示位置Y
			rc.right - rc.left,			// フルスクリーンウィンドウの横幅
			rc.bottom - rc.top,			// フルスクリーンウインドウの高さ
			nullptr,					// 親ウインドウのハンドル（なし）
			nullptr,					// メニューや子ウインドウのハンドル
			hInstance,					// アプリケーションインスタンスのハンドル
			nullptr						// ウインドウの作成データ
		);
	}

	if (!hWnd)
	{
		// ウィンドウの作成に失敗
		MessageBox(nullptr, "ウィンドウ作成に失敗しました", "Error", MB_OK);
		return 0;
	}

	// ウィンドウを表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}


/// <summary>
/// メインループ
/// </summary>
/// <param name="hInstance"></param>
/// <param name="hWnd"></param>
/// <param name="isFullScreen"></param>
/// <param name="clientWidth"></param>
/// <param name="clientHeight"></param>
/// <returns></returns>
int MainLoop(HINSTANCE hInstance, HWND hWnd, bool isFullScreen, int clientWidth, int clientHeight)
{
	// COMの初期化
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
	{
		return -1;
	}

	// アプリケーションクラスの構築
	App::CreateApp(hInstance, hWnd, isFullScreen, clientWidth, clientHeight);

	// シーンの作成
	auto scenePtr = APP->CreateScene<TestScene>();

	// メッセージループ
	MSG msg = {};
	// 利用するキーボード入力
	USE_KEY_VEC useKeyVec = { 'W', 'A', 'S', 'D','I','O','P','K','L', VK_LBUTTON, VK_RBUTTON, VK_SPACE, VK_LSHIFT, VK_UP, VK_DOWN };
	// メインループ
	while (WM_QUIT != msg.message)
	{
		if (!APP->ResetInpuState(hWnd, useKeyVec))
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				// キーボード・マウスの状態をリセット
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		// ゲーム更新/描画処理
		APP->UpdateDraw(1);
	}

	// アプリケーションの削除
	App::DeleteApp();
	// COMのリリース
	CoUninitialize();

	return (int)msg.wParam;
}

/// <summary>
/// メインウィンドウプロシージャ
/// </summary>
/// <param name="hWnd"></param>
/// <param name="message"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
/// <returns></returns>
LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			DestroyWindow(hWnd);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
