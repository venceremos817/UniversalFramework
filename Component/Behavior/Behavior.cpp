#include "Behavior.h"
#include <GameObject.h>
#include <App.h>

struct GravityBehavior::Param
{
	Float3 gravity;

	Param() :gravity(0.0f, -9.8f, 0.0f)
	{}
};

GravityBehavior::GravityBehavior(const std::shared_ptr<GameObject>& pGameObject)
	: Behavior(pGameObject)
	, m_param(new Param())
{
}

GravityBehavior::~GravityBehavior()
{
}

const Float3 & GravityBehavior::GetGravity() const
{
	return m_param->gravity;
}

void GravityBehavior::SetGravity(const Float3 & gravity)
{
	m_param->gravity = gravity;
}

void GravityBehavior::SetGravity(float x, float y, float z)
{
	SetGravity(Float3(x, y, z));
}

void GravityBehavior::UpdateFromTime(float time)
{
	auto pRb = GetGameObject()->GetRigidBody();
	Float3 gravityVel = pRb->GetGravityVelocity();
	gravityVel += m_param->gravity * time;
	pRb->SetGravityVelocity(gravityVel);
}

void GravityBehavior::Jump(const Float3 & startVelocity, float escapeSpan)
{
	auto pRb = GetGameObject()->GetRigidBody();
	Float3 velocity = pRb->GetGravityVelocity();
	velocity += startVelocity;
	pRb->SetGravityVelocity(velocity);

	// ’n–Ê‚É‚ß‚èž‚ñ‚Å‚¢‚é‚ÆƒWƒƒƒ“ƒv‚Å‚«‚È‚¢‚Ì‚Å•â³
	Float3 escapeVec = startVelocity;
	escapeVec *= escapeSpan;
	Float3 escapePos = GetGameObject()->GetTransform()->GetWorldPos();
	escapePos += escapeVec;
	GetGameObject()->GetTransform()->ResetWorldPos(escapePos);
}

void GravityBehavior::Execute()
{
	float elapsedTime = APP->GetElapsedTime();
	UpdateFromTime(elapsedTime);
}

void CommonBehavior::RotToVecotr(const Float3 & vector, float lerpFactor)
{
	if (lerpFactor <= 0.0f)
		return;

	if (vector.Length() <= 0.0f)
		return;

	Float3 temp = vector;
	temp.Normalize();
	float directionAngle = atan2f(temp.x, temp.z);
	Quaternion quat;
	quat.RotationRollPitchYawFromVector(Float3(0.0f, directionAngle, 0.0f));
	quat.Normalize();
	Quaternion curQuat = GetGameObject()->GetTransform()->GetQuaternion();
	if (lerpFactor >= 1.0f)
	{
		curQuat = quat;
	}
	else
	{
		curQuat = DirectX::XMQuaternionSlerp(curQuat, quat, lerpFactor);
	}

	GetGameObject()->GetTransform()->SetQuaternion(curQuat);
}
