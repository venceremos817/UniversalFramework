#pragma once


#include <GameObject.h>
//#include <PlayerCamera.h>
//#include <Transform.h>
#include <RigidBody.h>
#include <Player/PlayerStateMachine.h>
#include <DrawComponent/DrawComponent.h>
#include <EffekseerComponent.h>

class Player
	: public GameObject
{
public:
	Player(const std::shared_ptr<Stage>& pStage);
	virtual ~Player();

	void OnCreate()override;
	void Update()override;
	void LateUpdate()override;
	void OnDestroy()override;

	virtual void Destroy()override;

	/// <summary>
	/// ステートを変更
	/// </summary>
	/// <param name="pState"></param>
	void ChangeState(const std::shared_ptr<ObjectState<Player>>& pState);

	/// <summary>
	/// 非トリガーコリジョンイベント(衝突開始時
	/// </summary>
	/// <param name="destObjVec"></param>
	virtual void OnCollisionEnter(std::vector<std::shared_ptr<GameObject>>& destObjVec)override;

	/// <summary>
	/// エイムアシスト力を設定
	/// </summary>
	/// <param name="assist"></param>
	void SetAimAssist(float assist);
	/// <summary>
	/// エイムアイシスと力を取得
	/// </summary>
	/// <returns></returns>
	float GetAimAssist()const;

	std::shared_ptr<EffekseerComponent>& GetStepWaveEffect()const;

private:
	/// <summary>
	/// 地面と衝突時
	/// </summary>
	void OnHitGround();

private:
	struct Param;
	std::unique_ptr<Param> m_param;
};

