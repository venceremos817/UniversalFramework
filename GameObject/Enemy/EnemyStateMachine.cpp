#include "EnemyStateMachine.h"
#include <App/App.h>
#include <Enemy/Enemy.h>
#include <Math_EX.h>
#include <Effekseer.h>

//==============================================
// エネミー通常ステート
//==============================================
#pragma region EnemyDefaultState
std::shared_ptr<EnemyDefaultState> EnemyDefaultState::GetInstance()
{
	static std::shared_ptr<EnemyDefaultState> pInstance = nullptr;	// 自分自身のインスタンス

	if (!pInstance)
	{
		// インスタンスが未生成なら生成
		pInstance = std::shared_ptr<EnemyDefaultState>(new EnemyDefaultState);
	}
	return pInstance;
}


void EnemyDefaultState::OnEnter(const std::shared_ptr<Enemy>& pOwner, const std::shared_ptr<ObjectState<Enemy>>& prevState)
{
}

void EnemyDefaultState::OnUpdate(const std::shared_ptr<Enemy>& pOwner)
{
	// 重力による移動
	auto pGravityBehavior = pOwner->GetBehavior<GravityBehavior>();
	pGravityBehavior->Execute();
}

void EnemyDefaultState::OnExit(const std::shared_ptr<Enemy>& pOwner, const std::shared_ptr<ObjectState<Enemy>>& nextState)
{
}
#pragma endregion

//==============================================
// エネミー徘徊ステート
//==============================================
#pragma region EnemyWanderState
std::shared_ptr<EnemyWanderState> EnemyWanderState::GetInstance()
{
	static std::shared_ptr<EnemyWanderState> pInstance = nullptr;	// 自分自身のインスタンス

	if (!pInstance)
	{
		// インスタンスが未生成なら生成
		pInstance = std::shared_ptr<EnemyWanderState>(new EnemyWanderState);
	}
	return pInstance;
}

void EnemyWanderState::OnEnter(const std::shared_ptr<Enemy>& pOwner, const std::shared_ptr<ObjectState<Enemy>>& prevState)
{
}

void EnemyWanderState::OnUpdate(const std::shared_ptr<Enemy>& pOwner)
{
	// 重力
	auto pGravityBehavior = pOwner->GetBehavior<GravityBehavior>();
	pGravityBehavior->Execute();
	// 徘徊
	auto pWanderBehavior = pOwner->GetBehavior<WanderBehavior>();
	pWanderBehavior->Execute();

	// 進行方向を向く
	pOwner->GetBehavior<CommonBehavior>()->RotToVecotr(pOwner->GetTransform()->GetVelocity(), 1.0f);
}

void EnemyWanderState::OnExit(const std::shared_ptr<Enemy>& pOwner, const std::shared_ptr<ObjectState<Enemy>>& nextState)
{
}
#pragma endregion

//=================================================
// エネミー被撃破ステート
//=================================================
#pragma region EnemyDestroyState

std::shared_ptr<EnemyDestroyState> EnemyDestroyState::GetInstance()
{
	static std::shared_ptr<EnemyDestroyState> pInstance = nullptr;	// 自分自身のインスタンス

	if (!pInstance)
	{
		// インスタンスが未生成なら生成
		pInstance = std::shared_ptr<EnemyDestroyState>(new EnemyDestroyState);
	}
	return pInstance;
}

void EnemyDestroyState::OnEnter(const std::shared_ptr<Enemy>& pOwner, const std::shared_ptr<ObjectState<Enemy>>& prevState)
{
	// EnemyManagerから外す
	EnemyManager::GetInstance()->EraseEnemy(pOwner);

	auto defeatedBehavior = pOwner->GetBehavior<EnemyDefeatedBehavior>();
	defeatedBehavior->StartDefeat(pOwner);
}

void EnemyDestroyState::OnUpdate(const std::shared_ptr<Enemy>& pOwner)
{
	auto defeatedBehavior = pOwner->GetBehavior<EnemyDefeatedBehavior>();
	defeatedBehavior->Update(pOwner);
}

void EnemyDestroyState::OnExit(const std::shared_ptr<Enemy>& pOwner, const std::shared_ptr<ObjectState<Enemy>>& nextState)
{
}


#pragma endregion

//==============================================
// 徘徊ビヘイビア
//==============================================
#pragma region WonderBehaivior
struct WanderBehavior::Param
{
	Float3 wanderCenter;	// 徘徊中心
	float wanderRadius;		// 徘徊半径
	Float3 targetPos;		// 目標地点
	float timeToMove;		// 移動する時間(目的地点到達するまでの時間)

	Param()
		: wanderCenter()
		, wanderRadius(30.0f)
		, targetPos()
		, timeToMove(0.0f)
	{}
};

WanderBehavior::WanderBehavior(const std::shared_ptr<GameObject>& pGameObject)
	: Behavior(pGameObject)
	, m_pParam(new Param())
{
}

WanderBehavior::~WanderBehavior()
{
}

void WanderBehavior::SetWanderCenter(const Float3 & center)
{
	m_pParam->wanderCenter = center;
}

void WanderBehavior::SetWanderRadius(const float radius)
{
	m_pParam->wanderRadius = radius;
}

void WanderBehavior::SetRandomTargetPos()
{
	float radius = RandZeroToOne() * m_pParam->wanderRadius;
	float random = RandZeroToOne()*DirectX::XM_2PI;
	m_pParam->targetPos = m_pParam->wanderCenter + (Float3(cos(random), 0.0f, sin(random)) * radius);
	float distance = (GetGameObject()->GetTransform()->GetWorldPos() - m_pParam->targetPos).Length();
	m_pParam->timeToMove = distance / 5.0f;
}

void WanderBehavior::Execute()
{
	const float elapsedTime = APP->GetElapsedTime();
	m_pParam->timeToMove -= elapsedTime;
	if (m_pParam->timeToMove < 0.0f)
	{
		SetRandomTargetPos();
	}
	Float3 pos = GetGameObject()->GetTransform()->GetWorldPos();
	pos.y = 0.0f;
	Float3 velo = (m_pParam->targetPos - pos).normalized() * 5.0f;// *elapsedTime;
	GetGameObject()->GetRigidBody()->SetVelocity(velo);
}

#pragma endregion


EnemyDefeatedBehavior::EnemyDefeatedBehavior(const std::shared_ptr<GameObject>& pGameObject)
	: Behavior(pGameObject)
{
}

EnemyDefeatedBehavior::~EnemyDefeatedBehavior()
{
}

void EnemyDefeatedBehavior::StartDefeat(const std::shared_ptr<Enemy>& pOwner)
{
	auto col = pOwner->GetCollision();
	col->SetActive(false);
	auto rb = pOwner->GetRigidBody();
	rb->SetVelocityZero();
	rb->SetGravityVelocityZero();

	auto destroyEffect = pOwner->GetDestroyEffect();
	if (destroyEffect)
	{
		// 撃破エフェクト発生
		destroyEffect->Play(false, true, pOwner->GetTransform()->GetWorldPos());

		auto effectTerm = destroyEffect->GetEffect()->GetEffectResource()->CalculateTerm();
	}
}

void EnemyDefeatedBehavior::Update(const std::shared_ptr<Enemy>& pOwner)
{
	auto transform = pOwner->GetTransform();
	auto curScale = transform->GetScale();
	transform->SetScale(curScale*0.9f);

	if (!pOwner->GetDestroyEffect()->IsPlaying())
	{
		pOwner->Destroy();
	}
}
