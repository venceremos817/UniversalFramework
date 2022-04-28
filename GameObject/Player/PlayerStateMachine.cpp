#include "PlayerStateMachine.h"
#include <Utility/Math_EX.h>
#include <Player/Player.h>
#include <App.h>
#include <Player/Bullet/PlayerBullet.h>
#include <Stage.h>
#include <Ray.h>
#include <Enemy/Enemy.h>
#include <DrawComponent/DrawComponent.h>

const float bulletSpeed = 10000.0f;


//====================================================
//	�f�t�H���g�X�e�[�g
//====================================================
#pragma region PlayerDefaultState
PlayerDefaultState::~PlayerDefaultState()
{
}

std::shared_ptr<PlayerDefaultState> PlayerDefaultState::GetInstance()
{
	static std::shared_ptr<PlayerDefaultState> pInstance;
	if (!pInstance)
	{
		// �C���X�^���X�������Ȃ琶��
		pInstance = std::shared_ptr<PlayerDefaultState>(new PlayerDefaultState);
	}
	return pInstance;
}

void PlayerDefaultState::OnEnter(const std::shared_ptr<Player>& pOwner, const std::shared_ptr<ObjectState<Player>>& prevState)
{

}

void PlayerDefaultState::OnUpdate(const std::shared_ptr<Player>& pOwner)
{
	// �L�[���͂ɂ��ړ�
	auto pDefBehavior = pOwner->GetBehavior<PlayerDefaultBehavior>();
	pDefBehavior->Move();

	// �Q�[���p�b�h����̓���
	auto gamePad = INPUTDEVICE.GetGamePadState()[0];
	// �L�[�{�[�h�}�E�X����̓���
	auto keyBord = INPUTDEVICE.GetKeybordMouseState();
	// �ˌ�
	if (gamePad.IsPress(XINPUT_GAMEPAD_RIGHT_SHOULDER) ||
		keyBord.IsKeyPress(VK_LBUTTON))
	{
		auto shotBehavior = pOwner->GetBehavior<PlayerRapidFireBehavior>();
		shotBehavior->Shot(pOwner);
	}
	if (gamePad.GetRightTrigger() ||
		keyBord.IsKeyPress(VK_RBUTTON))
	{
		auto beamBehavior = pOwner->GetBehavior<PlayerHomingShotBehavior>();
		beamBehavior->Shot(pOwner);
	}

	// �W�����v�X�e�[�g�Ɉȍ~
	if (gamePad.IsTrigger(XINPUT_GAMEPAD_LEFT_SHOULDER) ||
		keyBord.IsKeyTrigger(VK_SPACE))
	{
		pOwner->ChangeState(PlayerJumpState::GetInstance());
	}

	// �X�e�b�v�X�e�[�g�Ɉȍ~
	if (gamePad.IsTrigger(XINPUT_GAMEPAD_B) ||
		keyBord.IsKeyTrigger(VK_LSHIFT))
	{
		pOwner->ChangeState(PlayerStepState::GetInstance());
	}

	// �d�͂�������
	auto pGarvityBehavior = pOwner->GetBehavior<GravityBehavior>();
	pGarvityBehavior->Execute();
}

void PlayerDefaultState::OnExit(const std::shared_ptr<Player>& pOwner, const std::shared_ptr<ObjectState<Player>>& nextState)
{
}

#pragma endregion


//====================================================
//	�W�����v�X�e�[�g
//====================================================
#pragma region PlayerJumpState

std::shared_ptr<PlayerJumpState> PlayerJumpState::GetInstance()
{
	static std::shared_ptr<PlayerJumpState> pInstance;
	if (!pInstance)
	{
		// �C���X�^���X�������Ȃ琶��
		pInstance = std::shared_ptr<PlayerJumpState>(new PlayerJumpState);
	}
	return pInstance;
}

void PlayerJumpState::OnEnter(const std::shared_ptr<Player>& pOwner, const std::shared_ptr<ObjectState<Player>>& prevState)
{
	auto pGravityBehavior = pOwner->GetBehavior<GravityBehavior>();
	pGravityBehavior->Jump(Float3(0.0f, 8.0f, 0.0f));
}

void PlayerJumpState::OnUpdate(const std::shared_ptr<Player>& pOwner)
{
	// �L�[���͂ɂ��ړ�
	auto pDefBehavior = pOwner->GetBehavior<PlayerDefaultBehavior>();
	pDefBehavior->Move();

	// �Q�[���p�b�h����̓���
	auto gamePad = INPUTDEVICE.GetGamePadState()[0];
	// �L�[�{�[�h�}�E�X����̓���
	auto keyBord = INPUTDEVICE.GetKeybordMouseState();
	// �ˌ�
	if (gamePad.IsPress(XINPUT_GAMEPAD_RIGHT_SHOULDER) ||
		keyBord.IsKeyPress(VK_LBUTTON))
	{
		auto shotBehavior = pOwner->GetBehavior<PlayerRapidFireBehavior>();
		shotBehavior->Shot(pOwner);
	}
	if (gamePad.GetRightTrigger() ||
		keyBord.IsKeyPress(VK_RBUTTON))
	{
		auto beamBehavior = pOwner->GetBehavior<PlayerHomingShotBehavior>();
		beamBehavior->Shot(pOwner);
	}

	// �d�͂�������
	auto pGarvityBehavior = pOwner->GetBehavior<GravityBehavior>();
	pGarvityBehavior->Execute();
}

void PlayerJumpState::OnExit(const std::shared_ptr<Player>& pOwner, const std::shared_ptr<ObjectState<Player>>& nextState)
{
}

#pragma endregion

//=============================================================
// �X�e�b�v�X�e�[�g
//=============================================================
#pragma region PlayerStepState

PlayerStepState::PlayerStepState()
{
}

PlayerStepState::~PlayerStepState()
{
}

std::shared_ptr<PlayerStepState> PlayerStepState::GetInstance()
{
	static std::shared_ptr<PlayerStepState> pInstance = nullptr;
	if (!pInstance)
	{
		pInstance = std::shared_ptr<PlayerStepState>(new PlayerStepState);
	}
	return pInstance;
}

void PlayerStepState::OnEnter(const std::shared_ptr<Player>& pOwner, const std::shared_ptr<ObjectState<Player>>& prevState)
{
	// �i�s�����̎擾
	Float3 stepDirection;
	auto gamePad = INPUTDEVICE.GetGamePadState()[0];
	if (gamePad.IsConected())
	{
		// �Q�[���p�b�h�œ���
		stepDirection = GetStepDirectionByGamePad(pOwner, gamePad);
	}
	else
	{
		// �L�[�{�[�h�œ���
		stepDirection = GetStepDirectionByKeybord(pOwner);
	}
	// ���͂���������ΑO���ɃX�e�b�v
	if (stepDirection.Length() < 0.1f)
	{
		stepDirection = pOwner->GetTransform()->GetForward();
	}
	stepDirection.y = 0.0f;
	stepDirection.Normalize();

	auto stepBehavior = pOwner->GetBehavior<PlayerStepBehavior>();
	auto rb = pOwner->GetRigidBody();
	stepDirection *= rb->GetMaxForce();

	stepBehavior->Reset(stepDirection, 0.1f, 0.005f);

	auto stepEff = pOwner->GetStepWaveEffect();
	auto transform = pOwner->GetTransform();
	auto quat = transform->GetQuaternion();
	//quat.Normalize();
	DirectX::XMVECTOR axisV;
	Float4 axis;
	float angle;
	DirectX::XMQuaternionToAxisAngle(&axisV, &angle, quat);
	axis = DirectX::XMVector3Normalize(axisV);
	stepEff->Play(false, false, transform->GetWorldPos());
	stepEff->SetRotation(Float3(axis), angle);
}

void PlayerStepState::OnUpdate(const std::shared_ptr<Player>& pOwner)
{
	auto stepBehavior = pOwner->GetBehavior<PlayerStepBehavior>();

	if (!stepBehavior->Execute())
		pOwner->ChangeState(PlayerDefaultState::GetInstance());
}

void PlayerStepState::OnExit(const std::shared_ptr<Player>& pOwner, const std::shared_ptr<ObjectState<Player>>& nextState)
{
}

const Float3 PlayerStepState::GetStepDirectionByGamePad(const std::shared_ptr<Player>& player, const GamePadState & gamePad) const
{
	Float3 angle(0.0f);
	Float2 input = gamePad.GetThumbL();
	if (input.Length() != 0.0f)
	{
		auto pCamera = player->GetStage()->GetView()->GetTargetCamera();
		// �i�s�����̌������v�Z
		Float3 front = player->GetTransform()->GetWorldPos() - pCamera->GetPos();
		front.y = 0.0f;
		front.Normalize();
		// �i�s��������̊p�x���Z�o
		float frontAngle = atan2f(front.z, front.x);
		// �Q�[���p�b�h�̌����v�Z
		Float2 moveVec(input.x, input.y);
		float moveSize = moveVec.Length();
		// �Q�[���p�b�h�̌�������p�x���Z�o
		float cntrlAngle = atan2f(-moveVec.x, moveVec.y);
		// �g�[�^���̊p�x���Z�o
		float totalAngle = frontAngle + cntrlAngle;
		// �p�x����x�N�g�����쐬
		angle = Float3(cos(totalAngle), 0.0f, sin(totalAngle));
		angle.Normalize();
		angle *= moveSize;
		angle.y = 0.0f;
	}
	return angle;
}

const Float3 PlayerStepState::GetStepDirectionByKeybord(const std::shared_ptr<Player>& player) const
{
	// ����
	auto keyBord = INPUTDEVICE.GetKeybordMouseState();
	float inputX = 0.0f;
	float inputZ = 0.0f;
	if (keyBord.IsKeyPress('W'))
		inputZ += 1.0f;
	if (keyBord.IsKeyPress('A'))
		inputX -= 1.0f;
	if (keyBord.IsKeyPress('S'))
		inputZ -= 1.0f;
	if (keyBord.IsKeyPress('D'))
		inputX += 1.0f;
	Float2 inputVec(inputX, inputZ);
	// �L�[���͂ɂ��p�x
	float inputVal = inputVec.Length();
	// ���͂��Ȃ���� 0�x�N�g����Ԃ��ďI��
	if (inputVal == 0.0f)
		return Float3(0.0f, 0.0f, 0.0f);

	float inputAngle = atan2f(-inputX, inputZ);

	auto pCamera = player->GetStage()->GetView()->GetTargetCamera();
	// �O��������͎��̐i�s����
	Float3 camearaForward = player->GetTransform()->GetPos() - pCamera->GetPos();
	camearaForward.y = 0.0f;
	camearaForward.Normalize();
	// �i�s�����̊p�x
	float cameraAngle = atan2f(camearaForward.z, camearaForward.x);

	// �i�s����
	Float3 moveVec;
	float totalAngle = inputAngle + cameraAngle;
	moveVec = Float3(cosf(totalAngle), 0.0f, sinf(totalAngle));
	moveVec.Normalize();
	moveVec *= inputVal;
	moveVec.y = 0.0f;

	return moveVec;
}


#pragma endregion


//====================================================
//	�f�t�H���g�r�w�C�r�A
//====================================================
#pragma region PlayerDefaultBehavior
PlayerDefaultBehavior::PlayerDefaultBehavior(const std::shared_ptr<GameObject>& pGameObject)
	: Behavior(pGameObject)
	, m_maxSpeed(10.0f)
{
}

void PlayerDefaultBehavior::Move()
{
	// �i�s�����̎擾
	Float3 moveVec;
	auto gamePad = INPUTDEVICE.GetGamePadState()[0];
	if (gamePad.IsConected())
	{
		// �Q�[���p�b�h�ňړ�
		moveVec = GetMoveVectorByGamePad(gamePad);
	}
	else
	{
		// �L�[�{�[�h�ňړ�
		moveVec = GetMoveVectorByKeybord();
	}

	auto pRb = GetGameObject()->GetRigidBody();
	Float3 velocity = pRb->GetVelocity();
	// ���͂����� ���� �㉺�̈ړ����łȂ�(�㏸�A���~���łȂ�)�Ȃ猸�����ďI��
	if (moveVec.Length() < 0.0f && velocity.y == 0.0f)
	{
		velocity *= pRb->GetFrictionForce();
		pRb->SetVelocity(velocity);
		return;
	}


	float elapsedTime = APP->GetElapsedTime();
	// �ڕW�ʒu
	//Float3 targetPos = moveVec * pRb->GetMaxSpeed();
	Float3 targetPos = moveVec * m_maxSpeed;
	// �ڕW�ʒu�Ɍ������t�H�[�X
	Float3 force = targetPos - velocity;
	force.y = 0.0f;
	force *= 1.5f;
	// �����x
	Float3 acc = force / pRb->GetMass();
	velocity += (acc * elapsedTime);
	//pRb->SetForce(acc);
	pRb->SetVelocity(velocity);

	// �i�s����������
	GetGameObject()->GetBehavior<CommonBehavior>()->RotToVecotr(velocity, 1.0f);
}

const Float3 PlayerDefaultBehavior::GetMoveVectorByGamePad(const GamePadState & gamePad) const
{
	Float3 angle(0.0f);
	Float2 input = gamePad.GetThumbL();
	if (input.Length() != 0.0f)
	{
		//Camera* pCamera = APP->GetSceneInterface()->GetMainCamera();
		auto pCamera = GetGameObject()->GetStage()->GetView()->GetTargetCamera();
		// �i�s�����̌������v�Z
		Float3 front = GetGameObject()->GetTransform()->GetWorldPos() - pCamera->GetPos();
		front.y = 0.0f;
		front.Normalize();
		// �i�s��������̊p�x���Z�o
		float frontAngle = atan2f(front.z, front.x);
		// �Q�[���p�b�h�̌����v�Z
		Float2 moveVec(input.x, input.y);
		float moveSize = moveVec.Length();
		// �Q�[���p�b�h�̌�������p�x���Z�o
		float cntrlAngle = atan2f(-moveVec.x, moveVec.y);
		// �g�[�^���̊p�x���Z�o
		float totalAngle = frontAngle + cntrlAngle;
		// �p�x����x�N�g�����쐬
		angle = Float3(cos(totalAngle), 0.0f, sin(totalAngle));
		angle.Normalize();
		angle *= moveSize;
		angle.y = 0.0f;
	}
	return angle;
}

const Float3 PlayerDefaultBehavior::GetMoveVectorByKeybord() const
{
	// ����
	auto keyBord = INPUTDEVICE.GetKeybordMouseState();
	float inputX = 0.0f;
	float inputZ = 0.0f;
	if (keyBord.IsKeyPress('W'))
		inputZ += 1.0f;
	if (keyBord.IsKeyPress('A'))
		inputX -= 1.0f;
	if (keyBord.IsKeyPress('S'))
		inputZ -= 1.0f;
	if (keyBord.IsKeyPress('D'))
		inputX += 1.0f;
	Float2 inputVec(inputX, inputZ);
	// �L�[���͂ɂ��p�x
	float inputVal = inputVec.Length();
	// ���͂��Ȃ���� 0�x�N�g����Ԃ��ďI��
	if (inputVal == 0.0f)
		return Float3(0.0f, 0.0f, 0.0f);

	float inputAngle = atan2f(-inputX, inputZ);

	auto pCamera = GetGameObject()->GetStage()->GetView()->GetTargetCamera();
	// �O��������͎��̐i�s����
	Float3 camearaForward = GetGameObject()->GetTransform()->GetPos() - pCamera->GetPos();
	camearaForward.y = 0.0f;
	camearaForward.Normalize();
	// �i�s�����̊p�x
	float cameraAngle = atan2f(camearaForward.z, camearaForward.x);

	// �i�s����
	Float3 moveVec;
	float totalAngle = inputAngle + cameraAngle;
	moveVec = Float3(cosf(totalAngle), 0.0f, sinf(totalAngle));
	moveVec.Normalize();
	moveVec *= inputVal;
	moveVec.y = 0.0f;

	return moveVec;
}
#pragma endregion

//====================================================
//	�ˌ��r�w�C�r�A
//====================================================
#pragma region PlayerShotBehavior

PlayerRapidFireBehavior::PlayerRapidFireBehavior(const std::shared_ptr<GameObject>& pGameObject)
	: Behavior(pGameObject)
	, m_rate(1000.0f), m_spanTime(0.0f)
{
}

void PlayerRapidFireBehavior::Shot(std::shared_ptr<Player> player)
{
	// �o�ߎ��Ԃ����Z
	m_spanTime += APP->GetElapsedTime();

	float fps = (float)APP->GetStepTimer().GetFPS();
	if (fps / m_rate < m_spanTime)
	{
		auto transform = player->GetTransform();
		m_spanTime = 0.0f;
		// --- �_�������� ---
		auto mainCamera = player->GetStage()->GetView()->GetTargetCamera();
		// �f�t�H���g�̓J�����̒����_����
		Float3 shotDirection = mainCamera->GetLookDirection().normalized();
		// ���s���߂��Ȃ��悤��
		if (shotDirection.y < -0.2f)
		{
			shotDirection.y = -0.2f;
			shotDirection.Normalize();
		}
		// �J�������璍���_���������Ƀ��C���΂��A�G�l�~�[�ɂԂ���΂�����_��
		Ray ray(mainCamera->GetPos(), mainCamera->GetLookDirection());
		float distance;
		Float3 aimPos;
		// �߂����Ƀ��C�ƏՓ˂��Ă��Ȃ����T��
		for (auto enemy : EnemyManager::GetInstance()->GetEnemyVectorByDistance(transform->GetPos()))
		{
			auto enemySp = enemy->GetCollision()->GetWrappedSphereVol();
			enemySp.m_radius *= player->GetAimAssist();
			// ���C�ƃG�l�~�[�X�t�B�A���Ԃ����Ă���΂��̃G�l�~�[�Ɍ������Ĕ�΂�
			if (ray.IsHitSphere(enemySp, distance, aimPos))
			{
				auto enemyPos = enemy->GetTransform()->GetWorldPos();
				shotDirection = enemyPos - transform->GetWorldPos();
				shotDirection.Normalize();
				break;
			}
		}

		// �΂炯������
		Float3 recoil = Float3(RandZeroToOne() - 0.5f, RandZeroToOne() - 0.5f, RandZeroToOne() - 0.5f).normalized() * 0.01f;
		shotDirection += recoil;

		// ����
		Float3 shotPos = transform->GetPos() + shotDirection;
		auto bullet = player->GetStage()->AddGameObject<NormalBullet>();
		bullet->Shot(shotPos, shotDirection, bulletSpeed);
	}
}
#pragma endregion

#pragma region PlayerBeamBehavior

PlayerHomingShotBehavior::PlayerHomingShotBehavior(const std::shared_ptr<GameObject>& pGameObject)
	: Behavior(pGameObject)
	, m_rate(1000.0f), m_spanTime(0.0f)
{
}

void PlayerHomingShotBehavior::Shot(std::shared_ptr<Player> player)
{
	// �o�ߎ��Ԃ����Z
	m_spanTime += APP->GetElapsedTime();
	float fps = (float)APP->GetStepTimer().GetFPS();

	if (fps / m_rate > m_spanTime)
		return;

	m_spanTime = 0.0f;

	auto transform = player->GetTransform();
	auto camera = player->GetStage()->GetView()->GetTargetCamera();

	// --- �^�[�Q�b�g��T��
	Ray ray(camera->GetPos(), camera->GetLookDirection());
	float distance;
	Float3 aimPos;
	std::shared_ptr<Transform> target;
	// �߂����Ƀ��C�ƏՓ˂��Ă��Ȃ����T��
	for (auto enemy : EnemyManager::GetInstance()->GetEnemyVectorByDistance(transform->GetPos()))
	{
		auto enemySp = enemy->GetCollision()->GetWrappedSphereVol();
		enemySp.m_radius *= player->GetAimAssist();
		// ���C�ƃG�l�~�[�X�t�B�A���Ԃ����Ă���΂��̃G�l�~�[�Ɍ������Ĕ�΂�
		if (ray.IsHitSphere(enemySp, distance, aimPos))
		{
			target = enemy->GetTransform();
			break;
		}
	}

	if (target)
	{
		Float3 velocity = Float3(RandZeroToOne() - 0.5f, RandZeroToOne(), RandZeroToOne() - 0.5f).normalized();
		Float3 spawnPos = transform->GetPos() + velocity;
		velocity *= 20.0f;
		auto beam = player->GetStage()->AddGameObject<HomingBullet>();
		beam->Shot(spawnPos, velocity, target, 1.0f, 0.01f, 0.2f);
	}
}


#pragma endregion


//===========================================
// �X�e�b�v�r�w�C�r�A
//===========================================
#pragma region PlayerStepBehavior
struct PlayerStepBehavior::Param
{
	Float3 velocity;	// ����
	float remainingTime;// �X�e�b�v�ňړ�����c�莞��
	float rigidityTime;	// �d������

	Param()
		:velocity(), remainingTime(0.0f), rigidityTime(0.0f)
	{}
};

PlayerStepBehavior::PlayerStepBehavior(const std::shared_ptr<GameObject>& pGameObject)
	: Behavior(pGameObject)
	, m_param(new Param)
{
}

PlayerStepBehavior::~PlayerStepBehavior()
{
}

void PlayerStepBehavior::Reset(const Float3 & velocity, float stepMoveTime, float rigidityTime)
{
	m_param->velocity = velocity;
	m_param->remainingTime = stepMoveTime;
	m_param->rigidityTime = rigidityTime;
}

bool PlayerStepBehavior::Execute()
{
	float elapsedTime = APP->GetElapsedTime();
	// �X�e�b�v���Ԍo��
	m_param->remainingTime -= elapsedTime;
	if (m_param->remainingTime > 0.0f)
	{
		// �͂�������
		auto rb = GetGameObject()->GetRigidBody();
		rb->SetVelocity(m_param->velocity);
		return true;
	}
	else if (m_param->rigidityTime > 0.0f)
	{
		// �d�����Ԍo��
		m_param->rigidityTime -= elapsedTime;
		return true;
	}

	return false;
}

#pragma endregion

