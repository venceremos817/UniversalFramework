#pragma once
#include <memory>
#include <StateMachine.h>
#include <Behavior/Behavior.h>

class Enemy;

//=======================================================
//	ステートマシン
//=======================================================
/// <summary>
/// エネミー通常時ステート
/// </summary>
class EnemyDefaultState
	: public ObjectState<Enemy>
{
private:
	EnemyDefaultState() {}
public:
	virtual ~EnemyDefaultState() {}

	/// <summary>
	/// シングルトンアクセサ
	/// </summary>
	/// <returns></returns>
	static std::shared_ptr<EnemyDefaultState> GetInstance();


	/// <summary>
	/// 開始時の処理
	/// </summary>
	/// <param name="pOwner"></param>
	/// <param name="prevState"></param>
	virtual void OnEnter(const std::shared_ptr<Enemy>& pOwner, const std::shared_ptr<ObjectState<Enemy>>& prevState = nullptr) override;
	/// <summary>
	/// 更新時の処理
	/// </summary>
	/// <param name="pOwner"></param>
	virtual void OnUpdate(const std::shared_ptr<Enemy>& pOwner) override;
	/// <summary>
	/// 終了時の処理
	/// </summary>
	/// <param name="pOwner"></param>
	/// <param name="nextState"></param>
	virtual void OnExit(const std::shared_ptr<Enemy>& pOwner, const std::shared_ptr<ObjectState<Enemy>>& nextState = nullptr) override;

private:

};

/// <summary>
/// エネミー徘徊ステート
/// </summary>
class EnemyWanderState
	: public ObjectState<Enemy>
{
private:
	EnemyWanderState() {}
public:
	virtual ~EnemyWanderState() {}

	/// <summary>
	/// シングルトンアクセサ
	/// </summary>
	/// <returns></returns>
	static std::shared_ptr<EnemyWanderState> GetInstance();


	/// <summary>
	/// 開始時の処理
	/// </summary>
	/// <param name="pOwner"></param>
	/// <param name="prevState"></param>
	virtual void OnEnter(const std::shared_ptr<Enemy>& pOwner, const std::shared_ptr<ObjectState<Enemy>>& prevState = nullptr) override;
	/// <summary>
	/// 更新時の処理
	/// </summary>
	/// <param name="pOwner"></param>
	virtual void OnUpdate(const std::shared_ptr<Enemy>& pOwner) override;
	/// <summary>
	/// 終了時の処理
	/// </summary>
	/// <param name="pOwner"></param>
	/// <param name="nextState"></param>
	virtual void OnExit(const std::shared_ptr<Enemy>& pOwner, const std::shared_ptr<ObjectState<Enemy>>& nextState = nullptr) override;
};

/// <summary>
/// エネミー被撃破時ステート
/// </summary>
class EnemyDestroyState
	: public ObjectState<Enemy>
{
private:
	EnemyDestroyState() {};
public:
	virtual ~EnemyDestroyState() {};

	static std::shared_ptr<EnemyDestroyState> GetInstance();

	virtual void OnEnter(const std::shared_ptr<Enemy>& pOwner, const std::shared_ptr<ObjectState<Enemy>>& prevState = nullptr) override;

	virtual void OnUpdate(const std::shared_ptr<Enemy>& pOwner) override;

	virtual void OnExit(const std::shared_ptr<Enemy>& pOwner, const std::shared_ptr<ObjectState<Enemy>>& nextState = nullptr) override;

};

/// <summary>
/// 徘徊ビヘイビア
/// </summary>
class WanderBehavior
	: public Behavior
{
public:
	WanderBehavior(const std::shared_ptr<GameObject>& pGameObject);
	virtual ~WanderBehavior();

	/// <summary>
	/// 徘徊中心点を設定
	/// </summary>
	/// <param name="center"></param>
	void SetWanderCenter(const Float3& center);
	/// <summary>
	/// 徘徊半径を設定
	/// </summary>
	/// <param name="radius"></param>
	void SetWanderRadius(const float radius);
	/// <summary>
	/// 徘徊円までの距離を設定
	/// </summary>
	/// <param name="distance"></param>
	void SetWanderDistance(const float distance);
	/// <summary>
	/// ランダム変異の最大値
	/// </summary>
	/// <param name="jitter"></param>
	void SetWanderJitter(const float jitter);

	/// <summary>
	/// 目標地点を設定
	/// </summary>
	/// <param name="targetPos"></param>
	void SetRandomTargetPos();

	/// <summary>
	/// 実行
	/// </summary>
	void Execute();

private:
	Float3 Wander(const Float4x4& worldMat, const float wanderRadius, const float wanderDistance, const float wanderJitter, Float3& wanderTarget);
	bool AccumulateForce(Float3& force, const Float3& forceToAdd, const float maxForce);
private:
	struct Param;
	std::unique_ptr<Param> m_pParam;
};

/// <summary>
/// エネミー被撃破ビヘイビア
/// </summary>
class EnemyDefeatedBehavior
	: public Behavior
{
public:
	EnemyDefeatedBehavior(const std::shared_ptr<GameObject>& pGameObject);
	virtual ~EnemyDefeatedBehavior();

	void StartDefeat(const std::shared_ptr<Enemy>& pOwner);
	void Update(const std::shared_ptr<Enemy>& pOwner);
	
private:
};