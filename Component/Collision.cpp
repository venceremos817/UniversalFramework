#include "Collision.h"
#include <Transform.h>
#include <GameObject.h>
#include <RigidBody.h>


struct Collision::Param
{
	bool	isTrigger;				// �Փˎ����蔲���邩�ǂ���
	bool	isFixed;				// �Î~�I�u�W�F�N�g���ǂ��� (�Փ˔���͍s�����A���ꎩ�͕̂s��)
	float	escapeAdjustedValue;	// �G�X�P�[�v�����̒����l
	float	escapeSlideValue;		// Slide���̃G�X�P�[�v�����̒����l
	HitAction hitAction;			// �Փ˂����Ƃ��̓���
	// �Փˑ���(��g���K�[
	std::vector<std::shared_ptr<GameObject>>	hitCollisionObjVec;		// �q�b�g�����I�u�W�F�N�g(��g���K�[
	std::vector<std::shared_ptr<GameObject>>	beforeCollisionObjVec;	// 1�O�Ƀq�b�g�����I�u�W�F�N�g(��g���K�[�łȂ�
	std::vector<std::shared_ptr<GameObject>>	tempCollisionObjVec;	// �ėp�q�b�g�����I�u�W�F�N�g(��g���K�[�łȂ�
	// �Փˑ���(�g���K�[
	std::vector<std::shared_ptr<GameObject>>	hitTriggerObjVec;		// �q�b�g�����I�u�W�F�N�g(�g���K�[
	std::vector<std::shared_ptr<GameObject>>	beforeTriggerObjVec;	// 1�O�Ƀq�b�g�����I�u�W�F�N�g(�g���K�[
	std::vector<std::shared_ptr<GameObject>>	tempTriggerObjVec;		// �ėp�q�b�g�I�u�W�F�N�g(�g���K�[

	inline Float3 Slide(const Float3& vec, const Float3& normal)
	{
		float len = Float3::Dot(vec, normal);	// vec�Ɩ@�����璼�s���̒��������߂�
		Float3 contact = normal * len;	// ���̒����ɐL�΂�
		return (vec - contact);	// �X���C�h��������͌��݂̃x�N�g����������Z
	}

	Param()
		: isTrigger(false)
		, isFixed(false)
		, escapeAdjustedValue(0.0f)
		, escapeSlideValue(0.0f)
		, hitAction(HitAction::Auto)
	{}
};


Collision::Collision(const std::shared_ptr<GameObject>& pGameObject)
	: Component(pGameObject)
	, m_pParam(new Param())
{
}

Collision::~Collision()
{
}

bool Collision::IsTrigger() const
{
	return m_pParam->isTrigger;
}

void Collision::SetIsTrigger(bool isTrigger)
{
	m_pParam->isTrigger = isTrigger;
}

bool Collision::IsFixed() const
{
	return m_pParam->isFixed;
}

void Collision::SetFixed(bool isFixed)
{
	m_pParam->isFixed = isFixed;
}

float Collision::GetEscapeAdjustedValue() const
{
	return m_pParam->escapeAdjustedValue;
}

void Collision::SetEscapeAdjustedValue(float adjustedVal)
{
	m_pParam->escapeAdjustedValue = adjustedVal;
}

float Collision::GetEscapeSlideValue() const
{
	return m_pParam->escapeSlideValue;
}

void Collision::SetEscapeSlideValue(float slideVal)
{
	m_pParam->escapeSlideValue = slideVal;
}

Collision::HitAction Collision::GetHitAction() const
{
	return m_pParam->hitAction;
}

void Collision::SetHitAction(HitAction hitAction)
{
	m_pParam->hitAction = hitAction;
}

void Collision::AddHitObject(const std::shared_ptr<GameObject>& pDestObject)
{
	// ���g���g���K�[���ǂ����ŐU�蕪��
	if (IsTrigger())
		m_pParam->hitTriggerObjVec.push_back(pDestObject);
	else
		m_pParam->hitCollisionObjVec.push_back(pDestObject);
}

void Collision::SetToBeforeHitObject()
{
	// �g���K�[
	m_pParam->beforeTriggerObjVec.resize(m_pParam->hitTriggerObjVec.size());
	m_pParam->beforeTriggerObjVec = m_pParam->hitTriggerObjVec;
	// ��g���K�[
	m_pParam->beforeCollisionObjVec.resize(m_pParam->hitCollisionObjVec.size());
	m_pParam->beforeCollisionObjVec = m_pParam->hitCollisionObjVec;
}

void Collision::ClearHitObject()
{
	// �g���K�[
	m_pParam->hitTriggerObjVec.clear();
	m_pParam->tempTriggerObjVec.clear();
	// ��g���K�[
	m_pParam->hitCollisionObjVec.clear();
	m_pParam->tempCollisionObjVec.clear();
}

std::vector<std::shared_ptr<GameObject>>& Collision::GetHitCollisionObjectVec()
{
	return m_pParam->hitCollisionObjVec;
}

std::vector<std::shared_ptr<GameObject>>& Collision::GetHitTriggerObjectVec()
{
	return m_pParam->hitTriggerObjVec;
}

bool Collision::IsHitCollisionObject(const std::shared_ptr<GameObject>& obj) const
{
	// �z���T������
	if (!m_pParam->tempCollisionObjVec.empty())
		for (auto& v : m_pParam->hitCollisionObjVec)
			if (v == obj)
				return true;	// �������true

	return false;	// ������Ȃ�����
}

bool Collision::IsHitTriggerObject(const std::shared_ptr<GameObject>& obj) const
{
	// �z���T������
	if (!m_pParam->tempTriggerObjVec.empty())
		for (auto& v : m_pParam->hitTriggerObjVec)
			if (v == obj)
				return true;

	return false;
}

std::vector<std::shared_ptr<GameObject>>& Collision::GetBeforeHitCollisionObjectVec()
{
	return m_pParam->beforeCollisionObjVec;
}

std::vector<std::shared_ptr<GameObject>>& Collision::GetBeforeHitTriggerObjectVec()
{
	return m_pParam->beforeTriggerObjVec;
}

std::vector<std::shared_ptr<GameObject>>& Collision::GetCollisionEnterObjectVec()
{
	m_pParam->tempCollisionObjVec.clear();
	for (auto v : m_pParam->hitCollisionObjVec)
	{
		// ���݃q�b�g���Ă���I�u�W�F�N�g���O�̃q�b�g���Ă���I�u�W�F�N�g�ɂȂ���΃Z�b�g
		FindIfCollisionVector(v, m_pParam->tempCollisionObjVec, m_pParam->beforeCollisionObjVec, false);
	}

	return m_pParam->tempCollisionObjVec;
}

std::vector<std::shared_ptr<GameObject>>& Collision::GetTriggerEnterObjectVec()
{
	m_pParam->tempTriggerObjVec.clear();
	for (auto v : m_pParam->hitTriggerObjVec)
	{
		// ���݃q�b�g���Ă���I�u�W�F�N�g���O�̃q�b�g���Ă���I�u�W�F�N�g�ɂȂ���΃Z�b�g
		FindIfCollisionVector(v, m_pParam->tempTriggerObjVec, m_pParam->beforeTriggerObjVec, false);
	}

	return m_pParam->tempTriggerObjVec;
}

std::vector<std::shared_ptr<GameObject>>& Collision::GetCollisionStayObjectVec()
{
	m_pParam->tempCollisionObjVec.clear();
	for (auto v : m_pParam->hitCollisionObjVec)
	{
		// ���݃q�b�g���Ă���I�u�W�F�N�g���O�̃q�b�g���Ă���I�u�W�F�N�g�ɑ��݂��Ă���΃Z�b�g
		FindIfCollisionVector(v, m_pParam->tempCollisionObjVec, m_pParam->beforeCollisionObjVec, true);
	}

	return m_pParam->tempCollisionObjVec;
}

std::vector<std::shared_ptr<GameObject>>& Collision::GetTriggerStayObjectVec()
{
	m_pParam->tempTriggerObjVec.clear();
	for (auto v : m_pParam->hitTriggerObjVec)
	{
		// ���݃q�b�g���Ă���I�u�W�F�N�g���O�̃q�b�g���Ă���I�u�W�F�N�g�ɑ��݂��Ă���΃Z�b�g
		FindIfCollisionVector(v, m_pParam->tempTriggerObjVec, m_pParam->beforeTriggerObjVec, true);
	}

	return m_pParam->tempTriggerObjVec;
}

std::vector<std::shared_ptr<GameObject>>& Collision::GetCollisionExitObjectVec()
{
	m_pParam->tempCollisionObjVec.clear();
	for (auto v : m_pParam->beforeCollisionObjVec)
	{
		// �O�Ƀq�b�g���Ă����I�u�W�F�N�g�����݃q�b�g���Ă���I�u�W�F�N�g�ɂȂ���΃Z�b�g
		FindIfCollisionVector(v, m_pParam->tempCollisionObjVec, m_pParam->hitCollisionObjVec, false);
	}

	return m_pParam->tempCollisionObjVec;
}

std::vector<std::shared_ptr<GameObject>>& Collision::GetTriggerExitObjectVec()
{
	m_pParam->tempTriggerObjVec.clear();
	for (auto v : m_pParam->beforeTriggerObjVec)
	{
		// �O�Ƀq�b�g���Ă����I�u�W�F�N�g�����݃q�b�g���Ă���I�u�W�F�N�g�ɂȂ���΃Z�b�g
		FindIfCollisionVector(v, m_pParam->tempTriggerObjVec, m_pParam->hitTriggerObjVec, false);
	}

	return m_pParam->tempTriggerObjVec;
}

void Collision::AfterCollision(const std::shared_ptr<Collision>& pDestCol, const Float3 & srcVelocity, const Float3 & destVelocity, const Float3 & hitNormal, float afterHitTime)
{
	auto gameObject = GetGameObject();
	auto transform = gameObject->GetTransform();
	bool horizontal = false;
	if (gameObject->FindBehavior<GravityBehavior>())
	{
		Float3 gravityNormal = gameObject->GetBehavior<GravityBehavior>()->GetGravity();
		gravityNormal.Normalize();
		Float3 v = (Float3)DirectX::XMVector3AngleBetweenNormals(gravityNormal, hitNormal);
		if (v.x <= DirectX::XM_PIDIV4)
			horizontal = true;
	}

	// �X���C�h����������v�Z
	Float3 slide = m_pParam->Slide(srcVelocity, hitNormal);
	transform->SetToBefore();
	if (GetHitAction() != HitAction::Stop && GetHitAction() != HitAction::None)
	{
		Float3 worldPos = transform->GetWorldPos() + slide * afterHitTime;
		if (!horizontal)
			worldPos += hitNormal * (-1.0f * GetEscapeSlideValue());
		transform->SetWorldPos(worldPos);
	}

	std::shared_ptr<RigidBody> pRb = nullptr;
	if (gameObject->TryGetComponent<RigidBody>(pRb))
	{
		switch (GetHitAction())
		{
		case HitAction::Slide:
		{
			Float3 destVelocity = pDestCol->GetGameObject()->GetTransform()->GetVelocity();
			slide = m_pParam->Slide(srcVelocity, hitNormal);
			pRb->SetVelocity(slide);
			// �d�͂�0�ɂ���
			pRb->SetGravityVelocityZero();
		}
		break;

		case HitAction::Auto:
		{
			if (horizontal)
			{
				// ����Ă���Ƃ��̓X���C�h������
				slide = m_pParam->Slide(srcVelocity, hitNormal);
				pRb->SetVelocity(slide);
				// �����ɏ�����Ƃ��͏d�͂�0�ɂ���
				pRb->SetGravityVelocityZero();
			}
			else
			{
				// ����Ă��Ȃ��Ƃ��͔���
				float resultPower = -(1.0f + pRb->GetReflection());
				std::shared_ptr<RigidBody> pDestRb = nullptr;
				if (pDestCol->GetGameObject()->TryGetComponent<RigidBody>(pDestRb))
				{
					Float3 relativeVelocity = srcVelocity - destVelocity;
					resultPower = (-(1.0f + pRb->GetReflection())*Float3::Dot(relativeVelocity, hitNormal)) /
						(Float3::Dot(hitNormal, hitNormal)*(1.0f / pRb->GetMass() + 1.0f / pDestRb->GetMass()));
				}
				else
				{
					Float3 relativeVelocity = srcVelocity;
					resultPower = (-(1.0f + pRb->GetReflection())*Float3::Dot(relativeVelocity, hitNormal)) /
						(Float3::Dot(hitNormal, hitNormal)*(1.0f / pRb->GetMass()));
				}
				Float3 velocity = pRb->GetVelocity();
				velocity += (hitNormal*resultPower) / pRb->GetMass();
				pRb->SetVelocity(velocity);
			}
		}
		break;

		case HitAction::Stop:
		{
			// ���x�A�d�͂�0�ɂ���
			pRb->SetVelocityZero();
			pRb->SetGravityVelocityZero();
		}
		break;
		}
	}
}

void Collision::FindIfCollisionVector(std::shared_ptr<GameObject> target, std::vector<std::shared_ptr<GameObject>>& setVec, const std::vector<std::shared_ptr<GameObject>>& scanVec, bool isFind)
{
	if (isFind)
	{
		for (auto& v : scanVec)
			if (target == v)
			{
				// ����������Z�b�g���ă��^�[��
				setVec.push_back(target);
				return;
			}
	}
	else
	{
		for (auto& v : scanVec)
			if (target == v)
				// ���������烊�^�[��
				return;

		// ������Ȃ�������Z�b�g
		setVec.push_back(target);
	}
}
