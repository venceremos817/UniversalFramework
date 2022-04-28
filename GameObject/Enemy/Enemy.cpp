#include "Enemy.h"
#include <App.h>
#include <algorithm>
#include <GameObject.h>
#include <EnemyComponent.h>
#include <Stage.h>
#include <CollisionSphere.h>
#include <RigidBody.h>
#include <DrawComponent/DrawComponent.h>


struct Enemy::Param
{
	std::shared_ptr<StateMachine<Enemy>> stateMachine;	// ステートマシン
	std::shared_ptr<EffekseerComponent> destroyEffect;

	Param()
	{};
};

Enemy::Enemy(const std::shared_ptr<Stage>& pStage)
	: GameObject(pStage, ObjectType::Enemy)
	, m_param(new Param())
{
	AddTag("enemy");
}

Enemy::~Enemy()
{
}


void Enemy::OnCreate()
{
	// エネミーマネージャーに追加
	EnemyManager::GetInstance()->AddEnemy(GetSharedPtr<Enemy>());
	//--- コンポーネントの追加 ---
	auto enemy = AddComponent<EnemyComponent>(50.0f);
	auto rb = AddComponent<RigidBody>();
	auto col = AddComponent<CollisionCapsule>();
	auto capsuleDraw = AddComponent<BasicCBDrawComponent>();
	m_param->destroyEffect = AddComponent<EffekseerComponent>();

	//--- コンポーネントの設定 ---
	GetTransform()->SetWorldPos(0, 1, 0);
	capsuleDraw->SetShader<VS_PNTTg_BUMP, PS_PNTTg_BUMP>();
	capsuleDraw->SetMeshResource(APP->GetResource<MeshResource>("DEFAULT_PNTTG_CAPSULE"));
	capsuleDraw->SetTextureResource("CHARACTER_TEXTURE", 0);
	capsuleDraw->SetTextureResource("CHARACTER_NORMAL_TEXTURE", 1);
	capsuleDraw->SetDiffuse(Float4(2.5f, 2.5f, 0.7f, 1.0f));
	m_param->destroyEffect->SetEffect(APP->GetResource<EffekseerEffectResource>("BOMB_EFFECT"));

	// ステートマシンの設定
	m_param->stateMachine = std::make_shared<StateMachine<Enemy>>(GetSharedPtr<Enemy>());
	m_param->stateMachine->ChangeState(EnemyWanderState::GetInstance());
}

void Enemy::Update()
{
	// ステートマシンによる更新
	m_param->stateMachine->Update();
}

void Enemy::OnDestroy()
{
}

void Enemy::Destroy()
{
	auto enemy = GetStage()->AddGameObject<Enemy>();
	enemy->GetTransform()->ResetWorldPos(Float3((rand() % 30 - 30.0f), 10.0f, (rand() % 30 - 30.0f)));

	GetStage()->RemoveGameObject(GetSharedPtr<Enemy>());
}

void Enemy::OnCollisionEnter(std::vector<std::shared_ptr<GameObject>>& destObjVec)
{
}

const std::shared_ptr<EffekseerComponent>& Enemy::GetDestroyEffect() const
{
	return m_param->destroyEffect;
}

const std::shared_ptr<StateMachine<Enemy>>& Enemy::GetStateMachine() const
{
	return m_param->stateMachine;
}




//=============================================
//	EnemyManager
//=============================================
#pragma region EnemyManager

EnemyManager::EnemyManager()
{
	m_enemiesVector.clear();
}

EnemyManager::~EnemyManager()
{
	m_enemiesVector.clear();
}

std::shared_ptr<EnemyManager> EnemyManager::GetInstance()
{
	static std::shared_ptr<EnemyManager> pInstance = nullptr;
	if (pInstance == nullptr)
	{
		pInstance = std::shared_ptr<EnemyManager>(new EnemyManager);
	}
	return pInstance;
}

std::shared_ptr<Enemy> EnemyManager::AddEnemy(std::shared_ptr<Enemy> pEnemy)
{
	m_enemiesVector.push_back(pEnemy);
	return pEnemy;
}

void EnemyManager::EraseEnemy(const std::shared_ptr<Enemy>& pEnemy)
{
	for (auto it = m_enemiesVector.begin(); it != m_enemiesVector.end(); ++it)
	{
		if (*it == pEnemy)
		{
			m_enemiesVector.erase(it);
			break;
		}
	}
}

const EnemyVector EnemyManager::GetEnemyVector() const
{
	return m_enemiesVector;
}

const std::shared_ptr<Enemy> EnemyManager::GetNearlestEnemy(const Float3 & base) const
{
	auto enemyVec = GetEnemyVectorByDistance(base);
	if (!enemyVec.empty())
		return enemyVec[0];
	return nullptr;
}

const EnemyVector EnemyManager::GetEnemyVectorByDistance(const Float3& base)const
{
	EnemyVector temp = m_enemiesVector;
	auto distSort = [&](const std::shared_ptr<Enemy> a, const std::shared_ptr<Enemy> b)->bool
	{
		float aDist = Float3::Distance(a->m_pTransform->GetPos(), base);
		float bDist = Float3::Distance(b->m_pTransform->GetPos(), base);
		return aDist < bDist;
	};
	std::sort(temp.begin(), temp.end(), distSort);

	return temp;
}

void EnemyManager::ClearVector()
{
	m_enemiesVector.clear();
}
#pragma endregion	EnemyManager
