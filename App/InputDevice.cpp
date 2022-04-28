#include "InputDevice.h"

int   KeybordMouseState::m_mouseWheelCount = 0;
bool  KeybordMouseState::m_mouseDoubleClickMessage[3] = { false,false,false };
HHOOK KeybordMouseState::m_mouseHook = nullptr;

KeybordMouseState::KeybordMouseState()
	: m_keyMessageActive(false)
	, m_mouseClientPoint{ 0, 0 }
	, m_mouseMove{ 0, 0 }
	, m_mouseWheelValue(0)
{
	ZeroMemory(&m_bPushKeyTbl, sizeof(m_bPushKeyTbl));
	ZeroMemory(&m_bLastKeyTbl, sizeof(m_bLastKeyTbl));
	ZeroMemory(&m_bTriggerKeyTbl, sizeof(m_bTriggerKeyTbl));
	ZeroMemory(&m_bReleaseKeyTbl, sizeof(m_bReleaseKeyTbl));
	m_mouseHook = SetWindowsHookEx(WH_GETMESSAGE, MouseProc, 0, GetCurrentThreadId());
}

KeybordMouseState::~KeybordMouseState()
{
	UnhookWindowsHookEx(m_mouseHook);
}

bool KeybordMouseState::IsMouseEnable(HWND hWnd, const USE_KEY_VEC& useKeyVec)
{
	//m_mouseClientPoint = { 0,0 };
	// マウスポインタの座標を取得
	POINT point;
	GetCursorPos(&point);
	if (ScreenToClient(hWnd, &point))
	{
		// クライアント座標に変換出来たらマウス座標を設定
		m_mouseMove = POINT{ point.x - m_mouseClientPoint.x,point.y - m_mouseClientPoint.y };
		m_mouseClientPoint = point;
	}
	else
	{
		return false;
	}

	// マウスホイール
	m_mouseWheelValue = m_mouseWheelCount;
	m_mouseWheelCount = 0;

	// マウスボタンを使うかどうかを調べる
	std::vector<DWORD> mouseButton = { VK_LBUTTON,VK_MBUTTON,VK_RBUTTON };
	bool mouseChk = false;
	int mouseButtonIdx = 0;
	for (auto chk : mouseButton)
	{
		auto it = std::find(useKeyVec.begin(), useKeyVec.end(), chk);
		if (it != useKeyVec.end())
		{
			// ダブルクリック
			m_mouseDoubleClick[mouseButtonIdx] = m_mouseDoubleClickMessage[mouseButtonIdx];
			m_mouseDoubleClickMessage[mouseButtonIdx] = false;

			mouseChk = true;
			break;
		}
		++mouseButtonIdx;
	}

	// マウスボタン使用かつポインタがクライアント領域内ならtrue
	if (mouseChk)
	{
		RECT rc;
		GetClientRect(hWnd, &rc);
		if (PtInRect(&rc, point))
			return true;
	}
	return false;
}

bool KeybordMouseState::GetKeyState(HWND hWnd, const USE_KEY_VEC& useKeyVec)
{
	const int push = 0x8000;

	m_keyMessageActive = false;
	// 1つ前にコピー
	CopyMemory(m_bLastKeyTbl, m_bPushKeyTbl, sizeof(m_bLastKeyTbl));
	// キーボードの状態を初期化
	ZeroMemory(&m_bPushKeyTbl, sizeof(m_bPushKeyTbl));
	ZeroMemory(&m_bTriggerKeyTbl, sizeof(m_bTriggerKeyTbl));
	ZeroMemory(&m_bReleaseKeyTbl, sizeof(m_bReleaseKeyTbl));
	// マウスが有効かチェック
	bool mouseEnable = IsMouseEnable(hWnd, useKeyVec);
	// 自身にフォーカスがなければfalse
	if (GetFocus() != hWnd)
		return false;

	// HACK : Escキーが押された場合はfalseでリターン
	SHORT ret = GetAsyncKeyState((int)VK_ESCAPE);
	if (ret & push)
		return false;

	size_t size = useKeyVec.size();
	for (size_t i = 0; i < size; ++i)
	{
		// 調べるキー
		auto key = useKeyVec[i];

		// マウスが無効なら無視
		if (key == VK_LBUTTON || key == VK_RBUTTON || key == VK_MBUTTON)
			if (!mouseEnable)
				continue;

		ret = GetAsyncKeyState((int)key);
		if (ret & push)
		{
			m_bPushKeyTbl[key] = true;
			m_keyMessageActive = true;
			// 前回押されていない
			if (!m_bLastKeyTbl[key])
				m_bTriggerKeyTbl[key] = true;
		}
		else
		{
			// 前回は押されていた
			if (m_bLastKeyTbl[key])
			{
				m_bReleaseKeyTbl[key] = true;
				m_keyMessageActive = true;
			}
		}
	}
	return m_keyMessageActive;
}

bool KeybordMouseState::IsKeyPress(DWORD key)const
{
	return m_bPushKeyTbl[key];
}

bool KeybordMouseState::IsKeyTrigger(DWORD key)const
{
	//		(現在と前のどちらかだけ押されている)       かつ 現在は押されてる
	//return (m_bPushKeyTbl[key] ^ m_bLastKeyTbl[key]) & m_bPushKeyTbl[key];
	return m_bTriggerKeyTbl[key];
}

bool KeybordMouseState::IsKeyRelease(DWORD key)const
{
	//			(現在と前のどちらかだけ押されている)	かつ 前は押されてた
	//return (m_bPushKeyTbl[key] ^ m_bLastKeyTbl[key]) & m_bLastKeyTbl[key];
	return m_bReleaseKeyTbl[key];
}

const POINT & KeybordMouseState::GetMouseClientPos() const
{
	return m_mouseClientPoint;
}

POINT KeybordMouseState::GetMouseMove()const
{
	return m_mouseMove;
}

int KeybordMouseState::GetMouseWheel()
{
	return m_mouseWheelValue;
}

LRESULT CALLBACK KeybordMouseState::MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0)
	{
		return CallNextHookEx(m_mouseHook, nCode, wParam, lParam);
	}

	MSG* pMsg = reinterpret_cast<MSG*>(lParam);
	switch (pMsg->message)
	{
		// ホイール
	case WM_MOUSEWHEEL:
		m_mouseWheelCount += GET_WHEEL_DELTA_WPARAM(pMsg->wParam) / WHEEL_DELTA;
		break;
		// 左ダブルクリック
	case WM_LBUTTONDBLCLK:
		m_mouseDoubleClickMessage[0] = true;
		break;
		// 中ダブルクリック
	case WM_MBUTTONDBLCLK:
		m_mouseDoubleClickMessage[1] = true;
		break;
		// 右ダブルクリック
	case WM_RBUTTONDBLCLK:
		m_mouseDoubleClickMessage[2] = true;
		break;
	}
	return CallNextHookEx(m_mouseHook, nCode, wParam, lParam);
}

bool GamePadState::IsConected()
{
	return bConnected;
}

void GamePadState::ResetState(int idx)
{
	// 1つ前の状態を保持
	wLastButtons = Gamepad.wButtons;
	// XInputから現在の状態を取得
	XINPUT_STATE workState;
	DWORD dwResult;
	dwResult = XInputGetState(idx, &workState);
	if (dwResult == ERROR_SUCCESS)
	{	// ゲームパッドが接続されている
		dwPacketNumber = workState.dwPacketNumber;
		Gamepad = workState.Gamepad;
		bConnected = true;
		// ボタンの設定
		wButtons = Gamepad.wButtons;
		// 更新ボタン
		wNowUpdateButtons = Gamepad.wButtons;
		bLeftTrigger = Gamepad.bLeftTrigger;
		bRightTrigger = Gamepad.bRightTrigger;
		// 前回から変化しているボタンのフラグを立てる
		wNowUpdateButtons ^= wLastButtons;
		// 押された瞬間
		wPressedButtons = wNowUpdateButtons & wButtons;
		// 離された瞬間
		wReleasedButtons = wNowUpdateButtons & (wButtons ^ 0xffff);
		// スティックのデッドゾーンの設定
		ApplyDeadZone();
	}
	else
	{	// 接続されていない
		bConnected = false;
	}
}

Float2 GamePadState::GetThumbL() const
{
	return Float2(fThumbLX, fThumbLY);
}

Float2 GamePadState::GetThumbR() const
{
	return Float2(fThumbRX, fThumbRY);
}

bool GamePadState::IsPress(WORD button)
{
	return wButtons & button;
}

bool GamePadState::IsTrigger(WORD button)
{
	return wPressedButtons & button;
}

bool GamePadState::IsRelease(WORD button)
{
	return wReleasedButtons & button;
}

BYTE GamePadState::GetRightTrigger()
{
	return bRightTrigger;
}

BYTE GamePadState::GetLeftTrigger()
{
	return bLeftTrigger;
}

void GamePadState::ApplyDeadZone()
{
	if ((Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		Gamepad.sThumbLX = 0;
		Gamepad.sThumbLY = 0;
	}
	if (Gamepad.sThumbLX >= 0)
	{
		fThumbLX = (float)Gamepad.sThumbLX / (float)SHRT_MAX;
	}
	else
	{
		fThumbLX = (float)Gamepad.sThumbLX / (float)(SHRT_MAX + 1);
	}
	if (Gamepad.sThumbLY >= 0)
	{
		fThumbLY = (float)Gamepad.sThumbLY / (float)SHRT_MAX;
	}
	else
	{
		fThumbLY = (float)Gamepad.sThumbLY / (float)(SHRT_MAX + 1);
	}
	if ((Gamepad.sThumbRX<XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
		(Gamepad.sThumbRY<XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			Gamepad.sThumbRY>-XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
	{
		Gamepad.sThumbRX = 0;
		Gamepad.sThumbRY = 0;
	}
	if (Gamepad.sThumbRX >= 0)
	{
		fThumbRX = (float)Gamepad.sThumbRX / (float)SHRT_MAX;
	}
	else
	{
		fThumbRX = (float)Gamepad.sThumbRX / (float)(SHRT_MAX + 1);
	}
	if (Gamepad.sThumbRY >= 0)
	{
		fThumbRY = (float)Gamepad.sThumbRY / (float)SHRT_MAX;
	}
	else
	{
		fThumbRY = (float)Gamepad.sThumbRY / (float)(SHRT_MAX + 1);
	}
}


InputDevice::InputDevice()
	: m_keybordMouseState()
	, m_gamePadStates(MAX_CONTROLLER)
{
	for (DWORD i = 0; i < MAX_CONTROLLER; ++i)
	{
		ZeroMemory(&m_gamePadStates[i], sizeof(GamePadState));
	}
}

bool InputDevice::ResetInputState(HWND hWnd, USE_KEY_VEC & useKeyVec)
{
	return m_keybordMouseState.GetKeyState(hWnd, useKeyVec);
}

void InputDevice::ResetControllerState()
{
	for (DWORD i = 0; i < MAX_CONTROLLER; ++i)
	{
		auto& gamePad = m_gamePadStates[i];
		gamePad.ResetState(i);
	}
}

const KeybordMouseState & InputDevice::GetKeybordMouseState() const
{
	return m_keybordMouseState;
}

const std::vector<GamePadState>& InputDevice::GetGamePadState() const
{
	return m_gamePadStates;
}

