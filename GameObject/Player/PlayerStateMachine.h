#pragma once
#include <memory>
#include <StateMachine.h>
#include <Behavior/Behavior.h>
#include <InputDevice.h>

class Player;
//================================================================
//	ステートマシン
//================================================================

/// <summary>
/// プレイヤー通常ステート
/// </summary>
class PlayerDefaultState
	:public ObjectState<Player>
{
	PlayerDefaultState() {}

public:
	~PlayerDefaultState();

	/// <summary>
	/// シングルトンアクセサ
	/// </summary>
	/// <returns></returns>
	static std::shared_ptr<PlayerDefaultState> GetInstance();

	/// <summary>
	/// 開始時の処理
	/// </summary>
	/// <param name="pOwner"></param>
	/// <param name="prevState"></param>
	void OnEnter(const std::shared_ptr<Player>& pOwner, const std::shared_ptr<ObjectState<Player>>& prevState = nullptr) override;
	/// <summary>
	/// 更新時の処理
	/// </summary>
	/// <param name="pOwner"></param>
	void OnUpdate(const std::shared_ptr<Player>& pOwner) override;
	/// <summary>
	/// 終了時の処理
	/// </summary>
	/// <param name="pOwner"></param>
	/// <param name="nextState"></param>
	void OnExit(const std::shared_ptr<Player>& pOwner, const std::shared_ptr<ObjectState<Player>>& nextState = nullptr) override;

private:
};

/// <summary>
/// プレイヤージャンプステート
/// </summary>
class PlayerJumpState
	: public ObjectState<Player>
{
	PlayerJumpState() {}

public:
	~PlayerJumpState() {}

	/// <summary>
	/// シングルトンアクセサ
	/// </summary>
	/// <returns></returns>
	static std::shared_ptr<PlayerJumpState> GetInstance();

	/// <summary>
	/// ステート開始時の処理
	/// </summary>
	/// <param name="pOwner"></param>
	/// <param name="prevState"></param>
	void OnEnter(const std::shared_ptr<Player>& pOwner, const std::shared_ptr<ObjectState<Player>>& prevState = nullptr) override;
	/// <summary>
	/// 更新時の処理
	/// </summary>
	/// <param name="pOwner"></param>
	void OnUpdate(const std::shared_ptr<Player>& pOwner) override;
	/// <summary>
	/// 終了時の処理
	/// </summary>
	/// <param name="pOwner"></param>
	/// <param name="nextState"></param>
	void OnExit(const std::shared_ptr<Player>& pOwner, const std::shared_ptr<ObjectState<Player>>& nextState = nullptr) override;
};


class PlayerStepState
	: public ObjectState<Player>
{
private:
	PlayerStepState();
public:
	~PlayerStepState();

	/// <summary>
	/// シングルトンアクセサ
	/// </summary>
	/// <returns></returns>
	static std::shared_ptr<PlayerStepState> GetInstance();

	/// <summary>
	/// ステート開始時の処理
	/// </summary>
	/// <param name="pPlayer"></param>
	/// <param name="prevState"></param>
	void OnEnter(const std::shared_ptr<Player>& pOwner, const std::shared_ptr<ObjectState<Player>>& prevState = nullptr)override;
	/// <summary>
	/// ステート更新処理
	/// </summary>
	/// <param name="pOwner"></param>
	void OnUpdate(const std::shared_ptr<Player>& pOwner) override;
	/// <summary>
	/// ステート終了処理
	/// </summary>
	/// <param name="pOwner"></param>
	/// <param name="nextState"></param>
	void OnExit(const std::shared_ptr<Player>& pOwner, const std::shared_ptr<ObjectState<Player>>& nextState = nullptr) override;

private:
	/// <summary>
	/// カメラとゲームパッドから進行方向を算出
	/// </summary>
	/// <returns></returns>
	const Float3 GetStepDirectionByGamePad(const std::shared_ptr<Player>& player, const GamePadState& gamePad)const;
	/// <summary>
	/// カメラとキー入力から進行方向を算出
	/// </summary>
	/// <returns></returns>
	const Float3 GetStepDirectionByKeybord(const std::shared_ptr<Player>& player)const;
};


//================================================================
// ビヘイビア
//================================================================
/// <summary>
/// プレイヤーの通常時(移動時など)ビヘイビア
/// </summary>
class PlayerDefaultBehavior
	:public Behavior
{
public:
	explicit PlayerDefaultBehavior(const std::shared_ptr<GameObject>& pGameObject);
	virtual ~PlayerDefaultBehavior() {}

	/// <summary>
	/// 移動を実行
	/// </summary>
	void Move();


private:
	/// <summary>
	/// カメラとゲームパッドから進行方向を算出
	/// </summary>
	/// <returns></returns>
	const Float3 GetMoveVectorByGamePad(const GamePadState& gamePad)const;
	/// <summary>
	/// カメラとキー入力から進行方向を算出
	/// </summary>
	/// <returns></returns>
	const Float3 GetMoveVectorByKeybord()const;

private:
	float m_maxSpeed;
};

/// <summary>
/// 通常射撃ビヘイビア
/// プレイヤーの向いている方向に通常弾を飛ばす
/// </summary>
class PlayerShotBehavior
	:public Behavior
{
public:
	explicit PlayerShotBehavior(const std::shared_ptr<GameObject>& pGameObject) :Behavior(pGameObject) {}
	virtual ~PlayerShotBehavior() {}

	/// <summary>
	/// 射撃実行
	/// </summary>
	void Shot();
};

/// <summary>
/// 連射射撃ビヘイビア
/// </summary>
class PlayerRapidFireBehavior
	: public Behavior
{
public:
	explicit PlayerRapidFireBehavior(const std::shared_ptr<GameObject>& pGameObject);
	virtual ~PlayerRapidFireBehavior() {}

	/// <summary>
	/// 射撃
	/// </summary>
	void Shot(std::shared_ptr<Player> player);

private:
	float m_rate;		// 射撃レート(1秒当たりの射撃回数)
	float m_spanTime;
};

class PlayerHomingShotBehavior
	: public Behavior
{
public:
	explicit PlayerHomingShotBehavior(const std::shared_ptr<GameObject>& pGameObject);
	virtual ~PlayerHomingShotBehavior() {}

	/// <summary>
	/// 発射
	/// </summary>
	void Shot(std::shared_ptr<Player> player);

private:
	float m_rate;
	float m_spanTime;
};

/// <summary>
/// プレイヤーステップビヘイビア
/// </summary>
class PlayerStepBehavior
	: public Behavior
{
public:
	explicit PlayerStepBehavior(const std::shared_ptr<GameObject>& pGameObject);
	virtual ~PlayerStepBehavior();



	/// <summary>
	/// パラメータ初期化
	/// </summary>
	/// <param name="velocity">ステップする速度</param>
	/// <param name="stepMoveTime">ステップで移動する時間</param>
	/// <param name="rigidityTime">硬直時間</param>
	void Reset(const Float3& velocity, float stepMoveTime, float rigidityTime);
	/// <summary>
	/// 実行
	/// </summary>
	/// <returns>ステップ時間が終了したらfalse</returns>
	bool Execute();

private:
	struct Param;
	std::unique_ptr<Param> m_param;
};