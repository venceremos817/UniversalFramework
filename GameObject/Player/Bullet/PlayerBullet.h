#pragma once
#include <GameObject.h>

/// <summary>
/// 通常弾
/// まっすぐ飛んでいく
/// </summary>
class NormalBullet
	: public GameObject
{
public:
	NormalBullet(const std::shared_ptr<Stage>& pStage);
	~NormalBullet();

	/// <summary>
	/// 生成し、指定した方向に飛んでいく
	/// </summary>
	/// <param name="direction"></param>
	void Shot(const Float3& spawnPos, const Float3& direction, float speed);

	void OnCreate()override;
	void Update()override;

	virtual void Destroy()override;

	void OnTriggerEnter(std::vector<std::shared_ptr<GameObject>>& destObjVec)override;


private:
	void OnEnterEnemy(const std::shared_ptr<GameObject>& pEney);

private:
	struct Param;
	std::unique_ptr<Param> m_param;
};


class HomingBullet
	: public GameObject
{
public:
	HomingBullet(const std::shared_ptr<Stage>& pStage);
	~HomingBullet();

	/// <summary>
	/// 発射
	/// </summary>
	/// <param name="spawnPos">生成位置</param>
	/// <param name="velocity">初速度</param>
	/// <param name="target">ターゲット</param>
	/// <param name="speed">速さ</param>
	/// <param name="curvatureRadius">曲線半径</param>
	/// <param name="damping">空気抵抗</param>
	void Shot(const Float3& spawnPos, const Float3& velocity, std::shared_ptr<Transform> target, float speed, float curvatureRadius, float damping);

	void OnCreate()override;
	void Update()override;

	virtual void Destroy()override;

	void OnTriggerEnter(std::vector<std::shared_ptr<GameObject>>& destObjVec)override;

private:
	struct Param;
	std::unique_ptr<Param> m_param;
};