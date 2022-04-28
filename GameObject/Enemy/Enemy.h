#pragma once

#include <GameObject.h>
#include <Enemy/EnemyStateMachine.h>
#include <EffekseerComponent.h>

class Enemy
	: public GameObject
{
public:
	Enemy(const std::shared_ptr<Stage>& pStage);
	virtual ~Enemy();

	void OnCreate()override;
	void Update()override;
	void OnDestroy()override;

	virtual void Destroy()override;

	void OnCollisionEnter(std::vector<std::shared_ptr<GameObject>>& destObjVec)override;

	/// <summary>
	/// 被撃破時のエフェクトを取得
	/// </summary>
	/// <returns></returns>
	const std::shared_ptr<EffekseerComponent>& GetDestroyEffect()const;

	const std::shared_ptr<StateMachine<Enemy>>& GetStateMachine()const;

private:
	struct Param;
	std::unique_ptr<Param> m_param;
};




/// <summary>
/// エネミーを管理する
/// シングルトン
/// </summary>
using EnemyVector = std::vector<std::shared_ptr<Enemy>>;
class EnemyManager
{
private:
	EnemyManager();
public:
	virtual ~EnemyManager();

	/// <summary>
	/// シングルトンアクセサ
	/// </summary>
	/// <returns></returns>
	static std::shared_ptr<EnemyManager> GetInstance();

	/// <summary>
	/// 管理に追加
	/// </summary>
	std::shared_ptr<Enemy> AddEnemy(std::shared_ptr<Enemy> pEnemy);

	/// <summary>
	/// 管理から外す
	/// </summary>
	/// <param name="pEnemy"></param>
	void EraseEnemy(const std::shared_ptr<Enemy>& pEnemy);

	/// <summary>
	/// 管理しているエネミー配列を取得
	/// </summary>
	const EnemyVector GetEnemyVector()const;

	/// <summary>
	/// 基準点から一番近いエネミーを取得
	/// </summary>
	/// <returns></returns>
	const std::shared_ptr<Enemy> GetNearlestEnemy(const Float3& base)const;

	/// <summary>
	/// 管理しているエネミーを基準点から近い順にソートした配列を取得
	/// </summary>
	/// <param name="base">基準点</param>
	/// <returns></returns>
	const EnemyVector GetEnemyVectorByDistance(const Float3& base)const;

	/// <summary>
	/// 管理している配列をクリア
	/// </summary>
	void ClearVector();

private:
	EnemyVector	m_enemiesVector;	// 管理するエネミーを保持する配列
};

