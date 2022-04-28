#pragma once

#include <GameObject.h>
#include <Collision.h>

class CollisionManager
	: public GameObject
{
public:
	CollisionManager(const std::shared_ptr<Stage>& pStage);
	virtual ~CollisionManager();


	/// <summary>
	/// 初期化処理
	/// </summary>
	//virtual void OnCreate()override;
	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void OnDestroy()override;

	virtual void Destroy()override;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void CollisionUpdate();		// 更新タイミングが特殊なため、Updateと名づけない

	/// <summary>
	/// 衝突判定後に呼び出し、衝突メッセージを送る
	/// </summary>
	virtual void SendCollisionMessage();


private:
	void CollisionSub(rsize_t srcIdx);

private:
	struct CollisionItem
	{
		std::shared_ptr<Collision>	pCollision;
		SphereBoundaryVolume enclosingSphereVol;
		float minX;
		float maxX;
		float minY;
		float maxY;
		float minZ;
		float maxZ;

		bool operator==(const CollisionItem& dest)const
		{
			if (this == &dest)
				return true;
			return false;
		}
	};
	struct Param;
	std::unique_ptr<Param>	m_param;
};