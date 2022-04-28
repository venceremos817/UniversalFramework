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
	// �}�E�X�|�C���^�̍��W���擾
	POINT point;
	GetCursorPos(&point);
	if (ScreenToClient(hWnd, &point))
	{
		// �N���C�A���g���W�ɕϊ��o������}�E�X���W��ݒ�
		m_mouseMove = POINT{ point.x - m_mouseClientPoint.x,point.y - m_mouseClientPoint.y };
		m_mouseClientPoint = point;
	}
	else
	{
		return false;
	}

	// �}�E�X�z�C�[��
	m_mouseWheelValue = m_mouseWheelCount;
	m_mouseWheelCount = 0;

	// �}�E�X�{�^�����g�����ǂ����𒲂ׂ�
	std::vector<DWORD> mouseButton = { VK_LBUTTON,VK_MBUTTON,VK_RBUTTON };
	bool mouseChk = false;
	int mouseButtonIdx = 0;
	for (auto chk : mouseButton)
	{
		auto it = std::find(useKeyVec.begin(), useKeyVec.end(), chk);
		if (it != useKeyVec.end())
		{
			// �_�u���N���b�N
			m_mouseDoubleClick[mouseButtonIdx] = m_mouseDoubleClickMessage[mouseButtonIdx];
			m_mouseDoubleClickMessage[mouseButtonIdx] = false;

			mouseChk = true;
			break;
		}
		++mouseButtonIdx;
	}

	// �}�E�X�{�^���g�p���|�C���^���N���C�A���g�̈���Ȃ�true
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
	// 1�O�ɃR�s�[
	CopyMemory(m_bLastKeyTbl, m_bPushKeyTbl, sizeof(m_bLastKeyTbl));
	// �L�[�{�[�h�̏�Ԃ�������
	ZeroMemory(&m_bPushKeyTbl, sizeof(m_bPushKeyTbl));
	ZeroMemory(&m_bTriggerKeyTbl, sizeof(m_bTriggerKeyTbl));
	ZeroMemory(&m_bReleaseKeyTbl, sizeof(m_bReleaseKeyTbl));
	// �}�E�X���L�����`�F�b�N
	bool mouseEnable = IsMouseEnable(hWnd, useKeyVec);
	// ���g�Ƀt�H�[�J�X���Ȃ����false
	if (GetFocus() != hWnd)
		return false;

	// HACK : Esc�L�[�������ꂽ�ꍇ��false�Ń��^�[��
	SHORT ret = GetAsyncKeyState((int)VK_ESCAPE);
	if (ret & push)
		return false;

	size_t size = useKeyVec.size();
	for (size_t i = 0; i < size; ++i)
	{
		// ���ׂ�L�[
		auto key = useKeyVec[i];

		// �}�E�X�������Ȃ疳��
		if (key == VK_LBUTTON || key == VK_RBUTTON || key == VK_MBUTTON)
			if (!mouseEnable)
				continue;

		ret = GetAsyncKeyState((int)key);
		if (ret & push)
		{
			m_bPushKeyTbl[key] = true;
			m_keyMessageActive = true;
			// �O�񉟂���Ă��Ȃ�
			if (!m_bLastKeyTbl[key])
				m_bTriggerKeyTbl[key] = true;
		}
		else
		{
			// �O��͉�����Ă���
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
	//		(���݂ƑO�̂ǂ��炩����������Ă���)       ���� ���݂͉�����Ă�
	//return (m_bPushKeyTbl[key] ^ m_bLastKeyTbl[key]) & m_bPushKeyTbl[key];
	return m_bTriggerKeyTbl[key];
}

bool KeybordMouseState::IsKeyRelease(DWORD key)const
{
	//			(���݂ƑO�̂ǂ��炩����������Ă���)	���� �O�͉�����Ă�
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
		// �z�C�[��
	case WM_MOUSEWHEEL:
		m_mouseWheelCount += GET_WHEEL_DELTA_WPARAM(pMsg->wParam) / WHEEL_DELTA;
		break;
		// ���_�u���N���b�N
	case WM_LBUTTONDBLCLK:
		m_mouseDoubleClickMessage[0] = true;
		break;
		// ���_�u���N���b�N
	case WM_MBUTTONDBLCLK:
		m_mouseDoubleClickMessage[1] = true;
		break;
		// �E�_�u���N���b�N
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
	// 1�O�̏�Ԃ�ێ�
	wLastButtons = Gamepad.wButtons;
	// XInput���猻�݂̏�Ԃ��擾
	XINPUT_STATE workState;
	DWORD dwResult;
	dwResult = XInputGetState(idx, &workState);
	if (dwResult == ERROR_SUCCESS)
	{	// �Q�[���p�b�h���ڑ�����Ă���
		dwPacketNumber = workState.dwPacketNumber;
		Gamepad = workState.Gamepad;
		bConnected = true;
		// �{�^���̐ݒ�
		wButtons = Gamepad.wButtons;
		// �X�V�{�^��
		wNowUpdateButtons = Gamepad.wButtons;
		bLeftTrigger = Gamepad.bLeftTrigger;
		bRightTrigger = Gamepad.bRightTrigger;
		// �O�񂩂�ω����Ă���{�^���̃t���O�𗧂Ă�
		wNowUpdateButtons ^= wLastButtons;
		// �����ꂽ�u��
		wPressedButtons = wNowUpdateButtons & wButtons;
		// �����ꂽ�u��
		wReleasedButtons = wNowUpdateButtons & (wButtons ^ 0xffff);
		// �X�e�B�b�N�̃f�b�h�]�[���̐ݒ�
		ApplyDeadZone();
	}
	else
	{	// �ڑ�����Ă��Ȃ�
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

