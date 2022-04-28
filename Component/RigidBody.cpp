#include "RigidBody.h"
#include <App.h>



struct RigidBody::Param
{
	Float3	velocity;			// ���x
	Float3	gravityVelocity;	// �d�͑��x
	float	maxSpeed;			// �ō����x
	Float3	minVelocity;		// X,Y,Z���ꂼ��̍Œᑬ�x(����������ƒ�~����)
	Float3	angularVelocity;	// ��]���x
	float	mass;	// ����
	float	reflection;	// �����W��
	Float3	force;	// ���݂̃t�H�[�X
	float	frictionForce;		// �����̃t�H�[�X
	float	maxForce;			// �ő�t�H�[�X

	// �R���X�g���N�^
	Param()
		: velocity(0.0f, 0.0f, 0.0f)
		, gravityVelocity(0.0f, 0.0f, 0.0f)
		, maxSpeed(10.0f)
		, minVelocity(0.01f, 0.01f, 0.01f)
		, angularVelocity(0, 0, 0)
		, mass(1.0f)
		, reflection(0.5f)
		, force(0, 0, 0)
		, frictionForce(0.5f)
		, maxForce(30.0f)
	{}
};

RigidBody::RigidBody(const std::shared_ptr<GameObject>& pGameObject)
	: Component(pGameObject)
	, m_pParam(new Param())
{
	SetProcessingOrder(ProcessingOrder::ComponentProcessingOrder::RigidBody);
}

RigidBody::~RigidBody()
{
}

void RigidBody::Update()
{
	float elapsedTime = APP->GetElapsedTime();
	UpdateFromTime(elapsedTime);
}

const Float3 & RigidBody::GetVelocity() const
{
	return m_pParam->velocity;
}

const Float3& RigidBody::SetVelocity(const Float3 & velocity)
{
	m_pParam->velocity = velocity;
	if (abs(m_pParam->velocity.x) < abs(m_pParam->minVelocity.x))
	{
		m_pParam->velocity.x = 0;
	}
	if (abs(m_pParam->velocity.y) < abs(m_pParam->minVelocity.y))
	{
		m_pParam->velocity.y = 0;
	}
	if (abs(m_pParam->velocity.z) < abs(m_pParam->minVelocity.z))
	{
		m_pParam->velocity.z = 0;
	}
	return m_pParam->velocity;
}

const Float3 & RigidBody::SetVelocity(float x, float y, float z)
{
	return SetVelocity(Float3(x, y, z));
}

void RigidBody::SetVelocityZero()
{
	SetVelocity(Float3(0.0f, 0.0f, 0.0f));
}

const Float3 & RigidBody::GetGravityVelocity() const
{
	return m_pParam->gravityVelocity;
}

void RigidBody::SetGravityVelocity(const Float3 & velocity)
{
	m_pParam->gravityVelocity = velocity;
}

void RigidBody::SetGravityVelocity(float x, float y, float z)
{
	SetGravityVelocity(Float3(x, y, z));
}

void RigidBody::SetGravityVelocityZero()
{
	SetGravityVelocity(Float3(0.0f, 0.0f, 0.0f));
}

float RigidBody::GetMaxSpeed() const
{
	return m_pParam->maxSpeed;
}

void RigidBody::SetMaxSpeed(float speed)
{
	m_pParam->maxSpeed = speed;
}

const Float3& RigidBody::GetMinVelocity() const
{
	return m_pParam->minVelocity;
}

void RigidBody::SetMinVelocity(const Float3 & velocity)
{
	m_pParam->minVelocity = velocity;
}

void RigidBody::SetMinVelocity(float x, float y, float z)
{
	SetMinVelocity(Float3(x, y, z));
}

const Float3 & RigidBody::GetAngularVelocity() const
{
	return m_pParam->angularVelocity;
}

void RigidBody::SetAngularVelocity(const Float3 & angularVelocity)
{
	m_pParam->angularVelocity = angularVelocity;
}

void RigidBody::SetAngularVelocity(float x, float y, float z)
{
	SetAngularVelocity(Float3(x, y, z));
}

float RigidBody::GetMass() const
{
	return m_pParam->mass;
}

void RigidBody::SetMass(float mass)
{
	if (mass < 0.0001f)
		mass = 0.0001f;
	m_pParam->mass = mass;
}

float RigidBody::GetReflection() const
{
	return m_pParam->reflection;
}

void RigidBody::SetReflection(float reflection)
{
	m_pParam->reflection = reflection;
}

const Float3 & RigidBody::GetForce() const
{
	return m_pParam->force;
}

void RigidBody::SetForce(const Float3& force)
{
	m_pParam->force = force;
}

void RigidBody::SetForce(float x, float y, float z)
{
	SetForce(Float3(x, y, z));
}

float RigidBody::GetFrictionForce() const
{
	return m_pParam->frictionForce;
}

void RigidBody::SetFrictionForce(float frictionF)
{
	m_pParam->frictionForce = frictionF;
}

float RigidBody::GetMaxForce() const
{
	return m_pParam->maxForce;
}

void RigidBody::SetMaxForce(float f)
{
	m_pParam->maxForce = f;
}

void RigidBody::UpdateFromTime(float calcTime)
{
	// �t�H�[�X���ύX����Ă�����
	if (m_pParam->force.Length() > 0.0f)
	{
		// �t�H�[�X���ۂ߂�
		m_pParam->force = DirectX::XMVector3ClampLength(m_pParam->force, 0.0f, m_pParam->maxForce);
		// ���ʂ��v�Z���ĉ����x�����߂�
		Float3 acc = m_pParam->force / m_pParam->mass;
		m_pParam->velocity += acc * calcTime;
	}

	// ���C���������߂�
	Float3 frictionForce = m_pParam->velocity * -1.0f;
	frictionForce *= m_pParam->frictionForce;
	Float3 frictionAcc = frictionForce / m_pParam->mass;
	m_pParam->velocity += frictionAcc * calcTime;

	// �Œᑬ�x�ȉ��ɂȂ����瑬�x0
	if (abs(m_pParam->velocity.x) < abs(m_pParam->minVelocity.x))
		m_pParam->velocity.x = 0.0f;
	if (abs(m_pParam->velocity.y) < abs(m_pParam->minVelocity.y))
		m_pParam->velocity.y = 0.0f;
	if (abs(m_pParam->velocity.z) < abs(m_pParam->minVelocity.z))
		m_pParam->velocity.z = 0.0f;

	auto transform = GetGameObject()->GetTransform();
	Quaternion qtSpan;
	Quaternion quat = transform->GetQuaternion();
	qtSpan = DirectX::XMQuaternionRotationRollPitchYawFromVector(m_pParam->angularVelocity * calcTime);
	quat *= qtSpan;
	quat.Normalize();

	// �e�����x��K�p
	Float3 pos = transform->GetWorldPos();
	pos += m_pParam->velocity * calcTime;
	pos += m_pParam->gravityVelocity * calcTime;
	transform->SetQuaternion(quat);
	transform->SetWorldPos(pos);
}
