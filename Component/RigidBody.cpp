#include "RigidBody.h"
#include <App.h>



struct RigidBody::Param
{
	Float3	velocity;			// 速度
	Float3	gravityVelocity;	// 重力速度
	float	maxSpeed;			// 最高速度
	Float3	minVelocity;		// X,Y,Zそれぞれの最低速度(これを下回ると停止する)
	Float3	angularVelocity;	// 回転速度
	float	mass;	// 質量
	float	reflection;	// 反発係数
	Float3	force;	// 現在のフォース
	float	frictionForce;		// 減速のフォース
	float	maxForce;			// 最大フォース

	// コンストラクタ
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
	// フォースが変更されていたら
	if (m_pParam->force.Length() > 0.0f)
	{
		// フォースを丸める
		m_pParam->force = DirectX::XMVector3ClampLength(m_pParam->force, 0.0f, m_pParam->maxForce);
		// 質量を計算して加速度を求める
		Float3 acc = m_pParam->force / m_pParam->mass;
		m_pParam->velocity += acc * calcTime;
	}

	// 摩擦加速を求める
	Float3 frictionForce = m_pParam->velocity * -1.0f;
	frictionForce *= m_pParam->frictionForce;
	Float3 frictionAcc = frictionForce / m_pParam->mass;
	m_pParam->velocity += frictionAcc * calcTime;

	// 最低速度以下になったら速度0
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

	// 各加速度を適用
	Float3 pos = transform->GetWorldPos();
	pos += m_pParam->velocity * calcTime;
	pos += m_pParam->gravityVelocity * calcTime;
	transform->SetQuaternion(quat);
	transform->SetWorldPos(pos);
}
