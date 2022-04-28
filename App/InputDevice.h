#pragma once
#include <Windows.h>
#include <Xinput.h>
#include <vector>
#include <XMFLOAT_EX.h>
#pragma comment (lib,"xinput.lib")

using USE_KEY_VEC = std::vector<DWORD>;

/// <summary>
/// �L�[�{�[�h�E�}�E�X�̏��
/// </summary>
class KeybordMouseState
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	KeybordMouseState();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~KeybordMouseState();

	/// <summary>
	/// �}�E�X�g�p�Ń}�E�X���N���C�A���g�̈���ɂ��邩�ǂ���
	/// </summary>
	/// <param name="hWnd">�E�B���h�E�n���h��</param>
	/// <param name="useKeyVec">�g�p����L�[�R�[�h�z��</param>
	/// <returns></returns>
	bool IsMouseEnable(HWND hWnd, const USE_KEY_VEC& useKeyVec);

	/// <summary>
	/// �L�[�{�[�h�̏�Ԃ��擾
	/// </summary>
	/// <param name="hWnd">�E�B���h�E�n���h��</param>
	/// <param name="useKeyVec">�g�p����L�[�R�[�h�z��</param>
	/// <returns></returns>
	bool GetKeyState(HWND hWnd, const USE_KEY_VEC& useKeyVec);

	/// <summary>
	/// �L�[��������Ă��邩
	/// </summary>
	/// <param name="key">�L�[�R�[�h</param>
	/// <returns></returns>
	bool IsKeyPress(DWORD key)const;
	/// <summary>
	/// �L�[�������ꂽ�u�Ԃꂽ��
	/// </summary>
	/// <param name="key"></param>
	/// <returns></returns>
	bool IsKeyTrigger(DWORD key)const;
	/// <summary>
	/// �L�[�������ꂽ�u�Ԃ�
	/// </summary>
	/// <param name="key"></param>
	/// <returns></returns>
	bool IsKeyRelease(DWORD key)const;

	/// <summary>
	/// �}�E�X�|�C���^���W���擾
	/// </summary>
	/// <returns></returns>
	const POINT& GetMouseClientPos()const;
	/// <summary>
	/// �}�E�X�|�C���^�̈ړ��ʂ��擾
	/// </summary>
	/// <returns></returns>
	POINT GetMouseMove()const;
	/// <summary>
	/// �}�E�X�z�C�[���̉�]�ʂ��擾
	/// </summary>
	/// <returns></returns>
	int GetMouseWheel();


private:
	/// <summary>
	/// �}�E�X���b�Z�[�W�t�b�N
	/// �}�E�X�_�u���N���b�N�E�z�C�[���𓾂邽�߂Ɏg��
	/// </summary>
	/// <param name="nCode"></param>
	/// <param name="wParam"></param>
	/// <param name="lParam"></param>
	/// <returns></returns>
	static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);

private:
	static const UINT MAX_KEYTABLE_CODE = 256;	// �L�[�����
	bool m_bPushKeyTbl[MAX_KEYTABLE_CODE];		// ���݂̃L�[�{�[�h�̏��
	bool m_bLastKeyTbl[MAX_KEYTABLE_CODE];		// �O��̃L�[�{�[�h�̏��
	bool m_bTriggerKeyTbl[MAX_KEYTABLE_CODE];	// �����ꂽ�u�Ԃ̃L�[�{�[�h
	bool m_bReleaseKeyTbl[MAX_KEYTABLE_CODE];	// �����ꂽ�u�Ԃ̃L�[�{�[�h
	bool m_keyMessageActive;					// �����̃L�[�C�x���g������

	POINT m_mouseClientPoint;		// �N���C�A���g��̃}�E�X�|�C���^���W
	POINT m_mouseMove;				// �}�E�X�|�C���^�̈ړ���
	int   m_mouseWheelValue;		// �}�E�X�z�C�[����]��
	bool  m_mouseDoubleClick[3];	// �}�E�X�_�u���N���b�N
	static int   m_mouseWheelCount;		// �}�E�X�z�C�[����]��
	static bool  m_mouseDoubleClickMessage[3];	// �}�E�X�_�u���N���b�N
	static HHOOK m_mouseHook;
};

/// <summary>
/// �Q�[���p�b�h�̏��
/// </summary>
class GamePadState
	: public XINPUT_STATE
{
public:
	/// <summary>
	/// �ڑ�����Ă��邩
	/// </summary>
	/// <returns></returns>
	bool IsConected();

	/// <summary>
	/// ��Ԃ��擾
	/// </summary>
	/// <param name="idx">���ʔԍ�</param>
	void ResetState(int idx);

	/// <summary>
	/// L�X�e�B�b�N�擾
	/// </summary>
	/// <returns></returns>
	Float2 GetThumbL()const;
	/// <summary>
	/// R�X�e�B�b�N�擾
	/// </summary>
	/// <returns></returns>
	Float2 GetThumbR()const;

	/// <summary>
	/// ������Ă��邩�ǂ����擾
	/// </summary>
	/// <param name="button"></param>
	/// <returns></returns>
	bool IsPress(WORD button);
	/// <summary>
	/// �����ꂽ�u�Ԃ��ǂ����擾
	/// </summary>
	/// <param name="button"></param>
	/// <returns></returns>
	bool IsTrigger(WORD button);
	/// <summary>
	/// �����ꂽ�u�Ԃ��ǂ����擾
	/// </summary>
	/// <param name="button"></param>
	/// <returns></returns>
	bool IsRelease(WORD button);

	BYTE GetRightTrigger();
	BYTE GetLeftTrigger();

private:
	/// <summary>
	/// �f�b�h�]�[����K�p
	/// </summary>
	void ApplyDeadZone();

private:
	WORD wButtons;		// �{�^�����
	BYTE bLeftTrigger;	// ���g���K�[
	BYTE bRightTrigger;	// �E�g���K�[
	float fThumbRX;		// �E�X�e�B�b�NX
	float fThumbRY;		// �E�X�e�B�b�NY
	float fThumbLX;		// ���X�e�B�b�NX
	float fThumbLY;		// ���X�e�B�b�NY
	WORD wNowUpdateButtons;	// ��Ԃ��ς�����Ƃ��Ƀt���O�����{�^��
	WORD wPressedButtons;	// �����ꂽ�u�Ԃ���������{�^��
	WORD wReleasedButtons;	// �����ꂽ�u�Ԃ���������{�^��
	WORD wLastButtons;		// �O��̃{�^��
	bool bConnected;		// �ڑ�����Ă��邩
};

/// <summary>
/// �L�[�{�[�h�E�}�E�X�E�Q�[���p�b�h���̓��͋@����Ǘ�����
/// </summary>
class InputDevice
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	InputDevice();
	~InputDevice() {}

	/// <summary>
	/// �}�E�X�ƃL�[�{�[�h�̏�Ԃ��擾
	/// </summary>
	/// <param name="hWnd">�E�B���h�E�n���h��</param>
	/// <param name="useKeyVec">�g�p����L�[</param>
	/// <returns></returns>
	bool ResetInputState(HWND hWnd, USE_KEY_VEC& useKeyVec);

	/// <summary>
	/// �Q�[���p�b�h�̏�Ԃ��擾
	/// </summary>
	void ResetControllerState();

	/// <summary>
	/// �L�[�{�[�h�X�e�[�g�A�N�Z�T
	/// </summary>
	/// <returns></returns>
	const KeybordMouseState& GetKeybordMouseState()const;
	/// <summary>
	/// �Q�[���p�b�h�X�e�[�g�A�N�Z�T
	/// </summary>
	/// <returns></returns>
	const std::vector<GamePadState>& GetGamePadState()const;

private:
	KeybordMouseState m_keybordMouseState;		// �L�[�{�[�h�E�}�E�X�̏��
	static const DWORD MAX_CONTROLLER = 1;
	std::vector<GamePadState> m_gamePadStates;	// �Q�[���p�b�h�̏��
};