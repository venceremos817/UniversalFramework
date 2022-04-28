#pragma once
#include <Windows.h>
#include <Xinput.h>
#include <vector>
#include <XMFLOAT_EX.h>
#pragma comment (lib,"xinput.lib")

using USE_KEY_VEC = std::vector<DWORD>;

/// <summary>
/// キーボード・マウスの状態
/// </summary>
class KeybordMouseState
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	KeybordMouseState();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~KeybordMouseState();

	/// <summary>
	/// マウス使用でマウスがクライアント領域内にあるかどうか
	/// </summary>
	/// <param name="hWnd">ウィンドウハンドル</param>
	/// <param name="useKeyVec">使用するキーコード配列</param>
	/// <returns></returns>
	bool IsMouseEnable(HWND hWnd, const USE_KEY_VEC& useKeyVec);

	/// <summary>
	/// キーボードの状態を取得
	/// </summary>
	/// <param name="hWnd">ウィンドウハンドル</param>
	/// <param name="useKeyVec">使用するキーコード配列</param>
	/// <returns></returns>
	bool GetKeyState(HWND hWnd, const USE_KEY_VEC& useKeyVec);

	/// <summary>
	/// キーが押されているか
	/// </summary>
	/// <param name="key">キーコード</param>
	/// <returns></returns>
	bool IsKeyPress(DWORD key)const;
	/// <summary>
	/// キーが押された瞬間れたか
	/// </summary>
	/// <param name="key"></param>
	/// <returns></returns>
	bool IsKeyTrigger(DWORD key)const;
	/// <summary>
	/// キーが離された瞬間か
	/// </summary>
	/// <param name="key"></param>
	/// <returns></returns>
	bool IsKeyRelease(DWORD key)const;

	/// <summary>
	/// マウスポインタ座標を取得
	/// </summary>
	/// <returns></returns>
	const POINT& GetMouseClientPos()const;
	/// <summary>
	/// マウスポインタの移動量を取得
	/// </summary>
	/// <returns></returns>
	POINT GetMouseMove()const;
	/// <summary>
	/// マウスホイールの回転量を取得
	/// </summary>
	/// <returns></returns>
	int GetMouseWheel();


private:
	/// <summary>
	/// マウスメッセージフック
	/// マウスダブルクリック・ホイールを得るために使う
	/// </summary>
	/// <param name="nCode"></param>
	/// <param name="wParam"></param>
	/// <param name="lParam"></param>
	/// <returns></returns>
	static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);

private:
	static const UINT MAX_KEYTABLE_CODE = 256;	// キー数上限
	bool m_bPushKeyTbl[MAX_KEYTABLE_CODE];		// 現在のキーボードの状態
	bool m_bLastKeyTbl[MAX_KEYTABLE_CODE];		// 前回のキーボードの状態
	bool m_bTriggerKeyTbl[MAX_KEYTABLE_CODE];	// 押された瞬間のキーボード
	bool m_bReleaseKeyTbl[MAX_KEYTABLE_CODE];	// 離された瞬間のキーボード
	bool m_keyMessageActive;					// 何かのキーイベントが発生

	POINT m_mouseClientPoint;		// クライアント上のマウスポインタ座標
	POINT m_mouseMove;				// マウスポインタの移動量
	int   m_mouseWheelValue;		// マウスホイール回転量
	bool  m_mouseDoubleClick[3];	// マウスダブルクリック
	static int   m_mouseWheelCount;		// マウスホイール回転量
	static bool  m_mouseDoubleClickMessage[3];	// マウスダブルクリック
	static HHOOK m_mouseHook;
};

/// <summary>
/// ゲームパッドの状態
/// </summary>
class GamePadState
	: public XINPUT_STATE
{
public:
	/// <summary>
	/// 接続されているか
	/// </summary>
	/// <returns></returns>
	bool IsConected();

	/// <summary>
	/// 状態を取得
	/// </summary>
	/// <param name="idx">識別番号</param>
	void ResetState(int idx);

	/// <summary>
	/// Lスティック取得
	/// </summary>
	/// <returns></returns>
	Float2 GetThumbL()const;
	/// <summary>
	/// Rスティック取得
	/// </summary>
	/// <returns></returns>
	Float2 GetThumbR()const;

	/// <summary>
	/// 押されているかどうか取得
	/// </summary>
	/// <param name="button"></param>
	/// <returns></returns>
	bool IsPress(WORD button);
	/// <summary>
	/// 押された瞬間かどうか取得
	/// </summary>
	/// <param name="button"></param>
	/// <returns></returns>
	bool IsTrigger(WORD button);
	/// <summary>
	/// 離された瞬間かどうか取得
	/// </summary>
	/// <param name="button"></param>
	/// <returns></returns>
	bool IsRelease(WORD button);

	BYTE GetRightTrigger();
	BYTE GetLeftTrigger();

private:
	/// <summary>
	/// デッドゾーンを適用
	/// </summary>
	void ApplyDeadZone();

private:
	WORD wButtons;		// ボタン状態
	BYTE bLeftTrigger;	// 左トリガー
	BYTE bRightTrigger;	// 右トリガー
	float fThumbRX;		// 右スティックX
	float fThumbRY;		// 右スティックY
	float fThumbLX;		// 左スティックX
	float fThumbLY;		// 左スティックY
	WORD wNowUpdateButtons;	// 状態が変わったときにフラグが立つボタン
	WORD wPressedButtons;	// 押された瞬間を所持するボタン
	WORD wReleasedButtons;	// 離された瞬間を所持するボタン
	WORD wLastButtons;		// 前回のボタン
	bool bConnected;		// 接続されているか
};

/// <summary>
/// キーボード・マウス・ゲームパッド等の入力機器を管理する
/// </summary>
class InputDevice
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	InputDevice();
	~InputDevice() {}

	/// <summary>
	/// マウスとキーボードの状態を取得
	/// </summary>
	/// <param name="hWnd">ウィンドウハンドル</param>
	/// <param name="useKeyVec">使用するキー</param>
	/// <returns></returns>
	bool ResetInputState(HWND hWnd, USE_KEY_VEC& useKeyVec);

	/// <summary>
	/// ゲームパッドの状態を取得
	/// </summary>
	void ResetControllerState();

	/// <summary>
	/// キーボードステートアクセサ
	/// </summary>
	/// <returns></returns>
	const KeybordMouseState& GetKeybordMouseState()const;
	/// <summary>
	/// ゲームパッドステートアクセサ
	/// </summary>
	/// <returns></returns>
	const std::vector<GamePadState>& GetGamePadState()const;

private:
	KeybordMouseState m_keybordMouseState;		// キーボード・マウスの状態
	static const DWORD MAX_CONTROLLER = 1;
	std::vector<GamePadState> m_gamePadStates;	// ゲームパッドの状態
};