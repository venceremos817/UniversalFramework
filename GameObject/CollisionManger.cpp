#include "CollisionManager.h"
#include <vector>
#include <algorithm>
#include <Stage.h>
#include <Collision.h>

struct CollisionManager::Param
{
	std::vector<CollisionItem>	collisionItemVec;
};

CollisionManager::CollisionManager(const std::shared_ptr<Stage>& pStage)
	: GameObject(pStage)
	, m_param(new Param)
{
	m_processOrder = ProcessingOrder::GameObjectProcessingOrder::Collision;
	AddTag("CollisionManger");
}

CollisionManager::~CollisionManager()
{

}

void CollisionManager::OnDestroy()
{
}

void CollisionManager::Destroy()
{
	GetStage()->RemoveGameObject(GetSharedPtr<CollisionManager>());
}

void CollisionManager::CollisionUpdate()
{
	//===== 現在有効なコリジョンを収集 =====
	m_param->collisionItemVec.clear();
	// ゲームオブジェクト配列を探索
	auto gameObjVec = GetStage()->GetGameObjectsVec();
	for (auto& obj : gameObjVec)
	{
		// ゲームオブジェクトが有効
		if (obj->IsActive())
		{
			std::shared_ptr<Collision> pCollision = nullptr;
			bool haveCollision = obj->TryGetComponent<Collision>(pCollision);
			// コリジョン持っている、かつそれが有効
			if (haveCollision && pCollision->IsActive())
			{
				CollisionItem item;
				item.pCollision = pCollision;
				item.enclosingSphereVol = pCollision->GetEnclosingSphereVol();
				item.minX = item.enclosingSphereVol.m_center.x - item.enclosingSphereVol.m_radius;
				item.maxX = item.enclosingSphereVol.m_center.x + item.enclosingSphereVol.m_radius;
				item.minY = item.enclosingSphereVol.m_center.y - item.enclosingSphereVol.m_radius;
				item.maxY = item.enclosingSphereVol.m_center.y + item.enclosingSphereVol.m_radius;
				item.minZ = item.enclosingSphereVol.m_center.z - item.enclosingSphereVol.m_radius;
				item.maxZ = item.enclosingSphereVol.m_center.z + item.enclosingSphereVol.m_radius;

				m_param->collisionItemVec.push_back(item);
			}
		}
	}


	// 速さでソート
	auto SortBySpeed = [&](CollisionItem& left, CollisionItem& right)->bool
	{
		Float3 leftVelocity = left.pCollision->GetGameObject()->GetTransform()->GetVelocity();
		Float3 rightVelocity = right.pCollision->GetGameObject()->GetTransform()->GetVelocity();
		float leftSpeed = leftVelocity.Length();
		float rightSpeed = rightVelocity.Length();

		return (leftSpeed < rightSpeed);
	};
	std::sort(m_param->collisionItemVec.begin(), m_param->collisionItemVec.end(), SortBySpeed);

	for (size_t i = 0; i < m_param->collisionItemVec.size(); ++i)
	{
		// 不動でなければ衝突判定実施
		if (!m_param->collisionItemVec[i].pCollision->IsFixed())
		{
			CollisionSub(i);
		}
	}
}

void CollisionManager::SendCollisionMessage()
{
	for (auto obj : GetStage()->GetGameObjectsVec())
	{
		auto collision = obj->GetCollision();
		if (collision && obj->IsActive())
			obj->OnRecivCollisionMessage();
	}
}

void CollisionManager::CollisionSub(rsize_t srcIdx)
{
	CollisionItem& srcItem = m_param->collisionItemVec[srcIdx];

	for (auto& destItem : m_param->collisionItemVec)
	{
		// 同じアイテムなら衝突判定を行わない
		if (srcItem == destItem)
			continue;
		//---- 衝突しない条件 ---
		if (srcItem.minX > destItem.maxX || srcItem.maxX < destItem.minX)
			continue;
		if (srcItem.minY > destItem.maxY || srcItem.maxY < destItem.minY)
			continue;
		if (srcItem.minZ > destItem.maxZ || srcItem.maxZ < destItem.minZ)
			continue;
		// このターンすでに衝突済み
		if (destItem.pCollision->IsHitCollisionObject(srcItem.pCollision->GetGameObject()))
			continue;

		// 衝突判定を行う(destにダブルデスパッチで呼び出してもらう
		destItem.pCollision->CollisionCall(srcItem.pCollision);
	}
}

