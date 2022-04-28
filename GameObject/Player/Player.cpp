#include "Player.h"
#include <App.h>
#include <Stage.h>
#include <CollisionCapsule.h>
#include <RigidBody.h>
#include <DrawComponent/DrawComponent.h>
#include <EffekseerComponent.h>
#include <StringSprite.h>


struct Player::Param
{
	std::shared_ptr<StateMachine<Player>>	stateMachine;	// �X�e�[�g�}�V��
	float aimAssist;	// �G�C���A�V�X�g��
	std::shared_ptr<EffekseerComponent> stepWaveEff;

	Param()
		: stateMachine(nullptr)
		, aimAssist(3.0f)
	{}
};

Player::Player(const std::shared_ptr<Stage>& pStage)
	: GameObject(pStage)
	, m_param(new Param)
{
	AddTag("player");
}

Player::~Player()
{
}

void Player::OnCreate()
{
	//--- �R���|�[�l���g�̒ǉ� ---
	auto rb = AddComponent<RigidBody>();
	auto col = AddComponent<CollisionCapsule>();
	auto capsuleDraw = AddComponent<BasicCBDrawComponent>();
	m_param->stepWaveEff = AddComponent<EffekseerComponent>();

	//--- �R���|�[�l���g�ݒ� ---
	m_pTransform->SetScale(1.0f, 1.0f, 1.0f);
	m_pTransform->SetPos(0.0f, 1.0f, -5.0f);
	m_pTransform->SetRotation(0.0f, 0.0f, 0.0f);
	rb->SetMaxSpeed(50.0f);
	rb->SetFrictionForce(1.0f);
	m_pCollision->SetHitAction(Collision::HitAction::Auto);
	m_pCollision->SetFixed(false);
	m_param->stepWaveEff->SetEffect(APP->GetResource<EffekseerEffectResource>("STEP_WAVE_EFFECT"));
	capsuleDraw->SetShader<VS_PNTTg_BUMP, PS_PNTTg_BUMP>();
	capsuleDraw->SetMeshResource(APP->GetResource<MeshResource>("DEFAULT_PNTTG_CAPSULE"));
	capsuleDraw->SetTextureResource("CHARACTER_TEXTURE", 0);
	capsuleDraw->SetTextureResource("CHARACTER_NORMAL_TEXTURE", 1);
	capsuleDraw->SetDiffuse(Float4(2.5f, 0.7f, 0.7f, 1.0f));

	// �X�e�[�g�}�V���̐ݒ�
	m_param->stateMachine = std::make_shared<StateMachine<Player>>(GetSharedPtr<Player>());
	ChangeState(PlayerDefaultState::GetInstance());
}

void Player::Update()
{
	// �X�e�[�g�}�V���̍X�V
	m_param->stateMachine->Update();

#ifdef _DEBUG
	auto input = APP->GetInputDevice().GetKeybordMouseState();
	auto draw = GetComponent<BasicCBDrawComponent>();
	auto dif = draw->GetDiffuse();
	if (input.IsKeyPress('I'))
	{
		if (input.IsKeyPress('K'))
			dif.x -= 0.001f;
		if (input.IsKeyPress('L'))
			dif.x += 0.001f;

		draw->SetDiffuse(dif);
	}
	if (input.IsKeyPress('O'))
	{
		if (input.IsKeyPress('K'))
			dif.y -= 0.001f;
		if (input.IsKeyPress('L'))
			dif.y += 0.001f;
		draw->SetDiffuse(dif);
	}
	if (input.IsKeyPress('P'))
	{
		if (input.IsKeyPress('K'))
			dif.z -= 0.001f;
		if (input.IsKeyPress('L'))
			dif.z += 0.001f;
		draw->SetDiffuse(dif);
	}
#endif // _DEBUG
}

void Player::LateUpdate()
{
#ifdef _DEBUG

#endif // DEBUG
}

void Player::OnDestroy()
{
}

void Player::Destroy()
{
	GetStage()->RemoveGameObject(GetSharedPtr<Player>());
}

void Player::ChangeState(const std::shared_ptr<ObjectState<Player>>& pState)
{
	m_param->stateMachine->ChangeState(pState);
}

void Player::OnCollisionEnter(std::vector<std::shared_ptr<GameObject>>& destObjVec)
{
	for (auto destObj : destObjVec)
	{
		auto destType = destObj->GetObjectType();

		switch (destType)
		{
		case GameObject::ObjectType::Enemy:
			break;
		case GameObject::ObjectType::Ground:
			OnHitGround();
			break;
		}
	}
}

void Player::SetAimAssist(float assist)
{
	m_param->aimAssist = assist;
}

float Player::GetAimAssist()const
{
	return m_param->aimAssist;
}

std::shared_ptr<EffekseerComponent>& Player::GetStepWaveEffect() const
{
	return m_param->stepWaveEff;
}

void Player::OnHitGround()
{
	// �W�����v���Ȃ�
	if (m_param->stateMachine->IsCurrentState(PlayerJumpState::GetInstance()))
	{
		// �ʏ�X�e�[�g(�W�����v�\)�Ɉȍ~
		ChangeState(PlayerDefaultState::GetInstance());
	}
}

