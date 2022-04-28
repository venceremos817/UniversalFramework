#include "EnemyStateMachine.h"
#include <App/App.h>
#include <Enemy/Enemy.h>
#include <Math_EX.h>
#include <Effekseer.h>

//==============================================
// �G�l�~�[�ʏ�X�e�[�g
//==============================================
#pragma region EnemyDefaultState
std::shared_ptr<EnemyDefaultState> EnemyDefaultState::GetInstance()
{
	static std::shared_ptr<EnemyDefaultState> pInstance = nullptr;	// �������g�̃C���X�^���X

	if (!pInstance)
	{
		// �C���X�^���X���������Ȃ琶��
		pInstance = std::shared_ptr<EnemyDefaultState>(new EnemyDefaultState);
	}
	return pInstance;
}


void EnemyDefaultState::OnEnter(const std::shared_ptr<Enemy>& pOwner, const std::shared_ptr<ObjectState<Enemy>>& prevState)
{
}

void EnemyDefaultState::OnUpdate(const std::shared_ptr<Enemy>& pOwner)
{
	// �d�͂ɂ��ړ�
	auto pGravityBehavior = pOwner->GetBehavior<GravityBehavior>();
	pGravityBehavior->Execute();
}

void EnemyDefaultState::OnExit(const std::shared_ptr<Enemy>& pOwner, const std::shared_ptr<ObjectState<Enemy>>& nextState)
{
}
#pragma endregion

//==============================================
// �G�l�~�[�p�j�X�e�[�g
//==============================================
#pragma region EnemyWanderState
std::shared_ptr<EnemyWanderState> EnemyWanderState::GetInstance()
{
	static std::shared_ptr<EnemyWanderState> pInstance = nullptr;	// �������g�̃C���X�^���X

	if (!pInstance)
	{
		// �C���X�^���X���������Ȃ琶��
		pInstance = std::shared_ptr<EnemyWanderState>(new EnemyWanderState);
	}
	return pInstance;
}

void EnemyWanderState::OnEnter(const std::shared_ptr<Enemy>& pOwner, const std::shared_ptr<ObjectState<Enemy>>& prevState)
{
}

void EnemyWanderState::OnUpdate(const std::shared_ptr<Enemy>& pOwner)
{
	// �d��
	auto pGravityBehavior = pOwner->GetBehavior<GravityBehavior>();
	pGravityBehavior->Execute();
	// �p�j
	auto pWanderBehavior = pOwner->GetBehavior<WanderBehavior>();
	pWanderBehavior->Execute();

	// �i�s����������
	pOwner->GetBehavior<CommonBehavior>()->RotToVecotr(pOwner->GetTransform()->GetVelocity(), 1.0f);
}

void EnemyWanderState::OnExit(const std::shared_ptr<Enemy>& pOwner, const std::shared_ptr<ObjectState<Enemy>>& nextState)
{
}
#pragma endregion

//=================================================
// �G�l�~�[�팂�j�X�e�[�g
//=================================================
#pragma region EnemyDestroyState

std::shared_ptr<EnemyDestroyState> EnemyDestroyState::GetInstance()
{
	static std::shared_ptr<EnemyDestroyState> pInstance = nullptr;	// �������g�̃C���X�^���X

	if (!pInstance)
	{
		// �C���X�^���X���������Ȃ琶��
		pInstance = std::shared_ptr<EnemyDestroyState>(new EnemyDestroyState);
	}
	return pInstance;
}

void EnemyDestroyState::OnEnter(const std::shared_ptr<Enemy>& pOwner, const std::shared_ptr<ObjectState<Enemy>>& prevState)
{
	// EnemyManager����O��
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
// �p�j�r�w�C�r�A
//==============================================
#pragma region WonderBehaivior
struct WanderBehavior::Param
{
	Float3 wanderCenter;	// �p�j���S
	float wanderRadius;		// �p�j���a
	Float3 targetPos;		// �ڕW�n�_
	float timeToMove;		// �ړ����鎞��(�ړI�n�_���B����܂ł̎���)

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
		// ���j�G�t�F�N�g����
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
