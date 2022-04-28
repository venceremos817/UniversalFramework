#include <Windows.h>
#include <crtdbg.h>
#include <string>

#include <App/App.h>

//===== �萔 =====
const char*	pClassName = "Class Name";
const char*	pWndTitle = "Game";

//===== �v���g�^�C�v�錾 =====
ATOM MyRegisterClass(HINSTANCE hInstance);
HWND InitInstance(HINSTANCE hInstance, int nCmdShow, bool isFullScreen, int clientWidth, int clientHeight);
int	MainLoop(HINSTANCE hInstance, HWND hWnd, bool isFullScreen, int clientWidth, int clientHeight);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);



/// <summary>
/// �G���g���|�C���g
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

	// ���������[�N���o�p
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// �E�B���h�E�̕��ƍ���
	int clientWidth = 1280;
	int clientHeight = 720;
	// �t���X�N���[���ɂ��邩��ݒ�
	bool isFullScreen = false;

	// �E�B���h�E�N���X��o�^
	MyRegisterClass(hInstance);

	// �A�v���P�[�V�����̏�����
	HWND hWnd = InitInstance(hInstance, nCmdShow, isFullScreen, clientWidth, clientHeight);
	if (!hWnd)
	{
		return FALSE;
	}


	return MainLoop(hInstance, hWnd, isFullScreen, clientWidth, clientHeight);
}


/// <summary>
/// �E�B���h�E�N���X��o�^����
/// </summary>
/// <param name="hInstance"></param>
/// <returns></returns>
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(wcex));

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;	// �E�B���h�E�X�^�C��
	wcex.lpfnWndProc = WndProc;				// �E�B���h�E�v���V�[�W����ݒ�
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;				// �E�B���h�E�v���V�[�W��������C���X�^���X�n���h��
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);		// �^�X�N�o�[�ɕ\������A�C�R��
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);		// �J�[�\���A�C�R��
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	// �E�B���h�E�w�i�F
	wcex.lpszMenuName = nullptr;			// �f�t�H���g���j���[��
	wcex.lpszClassName = pClassName;		// �E�B���h�E�N���X�������ʂ��閼�O
	wcex.hIconSm = wcex.hIcon;				// 16*16�A�C�R��

	// �E�B���h�E�N���X����o�^���ĕԂ�
	return RegisterClassEx(&wcex);
}


/// <summary>
/// ���C���E�B���h�E���쐬����
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

	// �E�B���h�E�̍쐬
	if (isFullScreen)
	{
		// ��ʑS�̂̕��ƍ������擾
		clientWidth = GetSystemMetrics(SM_CXSCREEN);
		clientHeight = GetSystemMetrics(SM_CYSCREEN);
		hWnd = CreateWindowEx(
			WS_EX_OVERLAPPEDWINDOW,		// �ǉ��ŃE�B���h�E�X�^�C�����w��
			pClassName,					// �쐬����E�B���h�E�N���X���w��
			pWndTitle,					// �L���v�V�����ɕ\�����閼�O
			WS_POPUP,					// ��{�̃E�B���h�E�X�^�C��
			0,							// �E�B���h�E�\���ʒuX
			0,							// �E�C���h�E�\���ʒuY
			clientWidth,				// �t���X�N���[���E�B���h�E�̉���
			clientHeight,				// �t���X�N���[���E�C���h�E�̍���
			nullptr,					// �e�E�C���h�E�̃n���h���i�Ȃ��j
			nullptr,					// ���j���[��q�E�C���h�E�̃n���h��
			hInstance,					// �A�v���P�[�V�����C���X�^���X�̃n���h��
			nullptr						// �E�C���h�E�̍쐬�f�[�^
		);
	}
	else
	{
		// �E�B���h�E�̃T�C�Y����
		RECT rc = { 0,0,clientWidth,clientHeight };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
		hWnd = CreateWindowEx(
			WS_EX_OVERLAPPEDWINDOW,		// �ǉ��ŃE�B���h�E�X�^�C�����w��
			pClassName,					// �쐬����E�B���h�E�N���X���w��
			pWndTitle,					// �L���v�V�����ɕ\�����閼�O
			WS_OVERLAPPEDWINDOW,		// ��{�̃E�B���h�E�X�^�C��
			CW_USEDEFAULT,				// �E�B���h�E�\���ʒuX
			CW_USEDEFAULT,				// �E�C���h�E�\���ʒuY
			rc.right - rc.left,			// �t���X�N���[���E�B���h�E�̉���
			rc.bottom - rc.top,			// �t���X�N���[���E�C���h�E�̍���
			nullptr,					// �e�E�C���h�E�̃n���h���i�Ȃ��j
			nullptr,					// ���j���[��q�E�C���h�E�̃n���h��
			hInstance,					// �A�v���P�[�V�����C���X�^���X�̃n���h��
			nullptr						// �E�C���h�E�̍쐬�f�[�^
		);
	}

	if (!hWnd)
	{
		// �E�B���h�E�̍쐬�Ɏ��s
		MessageBox(nullptr, "�E�B���h�E�쐬�Ɏ��s���܂���", "Error", MB_OK);
		return 0;
	}

	// �E�B���h�E��\��
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}


/// <summary>
/// ���C�����[�v
/// </summary>
/// <param name="hInstance"></param>
/// <param name="hWnd"></param>
/// <param name="isFullScreen"></param>
/// <param name="clientWidth"></param>
/// <param name="clientHeight"></param>
/// <returns></returns>
int MainLoop(HINSTANCE hInstance, HWND hWnd, bool isFullScreen, int clientWidth, int clientHeight)
{
	// COM�̏�����
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
	{
		return -1;
	}

	// �A�v���P�[�V�����N���X�̍\�z
	App::CreateApp(hInstance, hWnd, isFullScreen, clientWidth, clientHeight);

	// �V�[���̍쐬
	auto scenePtr = APP->CreateScene<TestScene>();

	// ���b�Z�[�W���[�v
	MSG msg = {};
	// ���p����L�[�{�[�h����
	USE_KEY_VEC useKeyVec = { 'W', 'A', 'S', 'D','I','O','P','K','L', VK_LBUTTON, VK_RBUTTON, VK_SPACE, VK_LSHIFT, VK_UP, VK_DOWN };
	// ���C�����[�v
	while (WM_QUIT != msg.message)
	{
		if (!APP->ResetInpuState(hWnd, useKeyVec))
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				// �L�[�{�[�h�E�}�E�X�̏�Ԃ����Z�b�g
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		// �Q�[���X�V/�`�揈��
		APP->UpdateDraw(1);
	}

	// �A�v���P�[�V�����̍폜
	App::DeleteApp();
	// COM�̃����[�X
	CoUninitialize();

	return (int)msg.wParam;
}

/// <summary>
/// ���C���E�B���h�E�v���V�[�W��
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
