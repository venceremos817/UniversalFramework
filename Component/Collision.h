#pragma once

#include <vector>
#include <Component.h>
//#include <GameObject.h>
#include <BoundaryVolume.h>

class CollisionSphere;
class CollisionOBB;
class CollisionCapsule;
class CollisionRect;


/// <summary>
/// �Փ˔���R���|�[�l���g�̒��ېe�N���X
/// </summary>
class Collision
	: public Component
{
public:
	/// <summary>
	/// �Փ˂����Ƃ��̓���
	/// </summary>
	enum class HitAction
	{
		None,	// �������Ȃ�
		Slide,	// �X���C�h
		Auto,	// �I�[�g�v�Z
		Stop,	// �X�g�b�v
	};

	/// <summary>
	/// �X�P�[����1�������ꍇ�̌v�Z�Ɏg�p����X�P�[��
	/// </summary>
	enum class CalcScaling
	{
		XScale,
		YScale,
		ZScale
	};


public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pGameObj"></param>
	explicit Collision(const std::shared_ptr<GameObject>& pGameObject);
	~Collision();


	//========== �p�����[�^�Q�b�^�[�E�Z�b�^�[ ==========
	/// <summary>
	/// ���蔲���邩�ǂ������擾
	/// </summary>
	/// <returns></returns>
	bool IsTrigger()const;
	/// <summary>
	/// ���蔲���邩�ǂ�����ݒ�
	/// </summary>
	void SetIsTrigger(bool isTrigger);
	/// <summary>
	/// �Œ�Փ˃I�u�W�F�N�g���ǂ������擾
	/// </summary>
	/// <returns></returns>
	bool IsFixed()const;
	/// <summary>
	/// �Œ�Փ˃I�u�W�F�N�g���ǂ�����ݒ�
	/// </summary>
	/// <param name="isFixed"></param>
	void SetFixed(bool isFixed);

	/// <summary>
	/// �G�X�P�[�v�����̒����l���擾
	/// </summary>
	/// <returns></returns>
	float GetEscapeAdjustedValue()const;
	/// <summary>
	/// �G�X�P�[�v�����̒����l��ݒ�
	/// </summary>
	/// <param name="f"></param>
	void SetEscapeAdjustedValue(float adjustedVal);

	/// <summary>
	/// �G�X�P�[�vSlide�����̒����l���擾
	/// </summary>
	/// <returns></returns>
	float GetEscapeSlideValue()const;
	/// <summary>
	/// �G�X�P�[�vSlide�����̒����l��ݒ�
	/// </summary>
	/// <param name="f"></param>
	void SetEscapeSlideValue(float slideVal);

	/// <summary>
	/// �Փ˂����Ƃ��̓�����擾
	/// </summary>
	/// <returns></returns>
	HitAction GetHitAction()const;
	/// <summary>
	/// �Փ˂����Ƃ��̓����ݒ�
	/// </summary>
	/// <param name="hitAction"></param>
	void SetHitAction(HitAction hitAction);

	//========== �q�b�g�I�u�W�F�N�g�z�񑀍� ==========
	/// <summary>
	/// �q�b�g�I�u�W�F�N�g��ǉ�
	/// </summary>
	/// <param name="pDestObject"></param>
	void AddHitObject(const std::shared_ptr<GameObject>& pDestObject);
	/// <summary>
	/// 1�O�̃q�b�g�I�u�W�F�N�g�ɃR�s�[
	/// </summary>
	void SetToBeforeHitObject();
	/// <summary>
	/// �q�b�g�I�u�W�F�N�g���N���A
	/// </summary>
	void ClearHitObject();
	/// <summary>
	/// �q�b�g�I�u�W�F�N�g�z����擾
	/// </summary>
	/// <returns></returns>
	std::vector<std::shared_ptr<GameObject>>& GetHitCollisionObjectVec();
	std::vector<std::shared_ptr<GameObject>>& GetHitTriggerObjectVec();
	/// <summary>
	/// �q�b�g�I�u�W�F�N�g�z��ɓ����Ă��邩�ǂ���
	/// </summary>
	/// <param name="obj"></param>
	/// <returns></returns>
	bool IsHitCollisionObject(const std::shared_ptr<GameObject>& obj)const;
	bool IsHitTriggerObject(const std::shared_ptr<GameObject>& obj)const;
	/// <summary>
	/// 1�O�̃q�b�g�I�u�W�F�N�g�z����擾
	/// </summary>
	/// <returns></returns>
	std::vector<std::shared_ptr<GameObject>>& GetBeforeHitCollisionObjectVec();
	std::vector<std::shared_ptr<GameObject>>& GetBeforeHitTriggerObjectVec();
	/// <summary>
	/// �V�����q�b�g�����I�u�W�F�N�g�z����擾
	/// </summary>
	/// <returns></returns>
	std::vector<std::shared_ptr<GameObject>>& GetCollisionEnterObjectVec();
	std::vector<std::shared_ptr<GameObject>>& GetTriggerEnterObjectVec();
	/// <summary>
	/// �Փ˂������Ă���q�b�g�I�u�W�F�N�g�z����擾
	/// </summary>
	/// <returns></returns>
	std::vector<std::shared_ptr<GameObject>>& GetCollisionStayObjectVec();
	std::vector<std::shared_ptr<GameObject>>& GetTriggerStayObjectVec();
	/// <summary>
	/// �Փ˂��痣�ꂽ�q�b�g�I�u�W�F�N�g�z����擾
	/// </summary>
	/// <returns></returns>
	std::vector<std::shared_ptr<GameObject>>& GetCollisionExitObjectVec();
	std::vector<std::shared_ptr<GameObject>>& GetTriggerExitObjectVec();


	/// <summary>
	/// �_�u���f�X�p�b�`�֐�	�Փ˔���𑊎�ɌĂ�ł��炤
	/// </summary>
	/// <param name="srcCol"></param>
	virtual void CollisionCall(const std::shared_ptr<Collision>& pSrcCol) = 0;


	//========== �Փ˃e�X�g ==========
	/// <summary>
	/// Sphere�Ƃ̏Փ˃e�X�g
	/// </summary>
	/// <param name="pDestSphere"></param>
	virtual void CollisionTest(const std::shared_ptr<CollisionSphere>& pDestSphere) = 0;
	/// <summary>
	/// OBB�Ƃ̏Փ˃e�X�g
	/// </summary>
	/// <param name="pDestOBB"></param>
	virtual void CollisionTest(const std::shared_ptr<CollisionOBB>& pDestOBB) = 0;
	/// <summary>
	/// Capusule�Ƃ̏Փ˃e�X�g
	/// </summary>
	/// <param name="pDestCapsule"></param>
	virtual void CollisionTest(const std::shared_ptr<CollisionCapsule>& pDestCapsule) = 0;
	/// <summary>
	/// Rect�Ƃ̏Փ˃e�X�g
	/// </summary>
	/// <param name="pDestRect"></param>
	virtual void CollisionTest(const std::shared_ptr<CollisionRect>& pDestRect) = 0;

	//========== �G�X�P�[�v���� ==========
	/// <summary>
	/// Sphere����G�X�P�[�v
	/// </summary>
	/// <param name="pDestCol"></param>
	virtual void CollisionEscape(const std::shared_ptr<CollisionSphere>& pDestSphere) = 0;
	/// <summary>
	/// OBB����G�X�P�[�v (��]�ɂ��G�X�P�[�v���Ή�)
	/// </summary>
	/// <param name="pDestOBB"></param>
	virtual void CollisionEscape(const std::shared_ptr<CollisionOBB>& pDestOBB) = 0;
	/// <summary>
	/// Capsule����G�X�P�[�v
	/// </summary>
	/// <param name="pDestCapsule"></param>
	virtual void CollisionEscape(const std::shared_ptr<CollisionCapsule>& pDestCapsule) = 0;
	/// <summary>
	/// Rect����G�X�P�[�v
	/// </summary>
	/// <param name="pDestRect"></param>
	virtual void CollisionEscape(const std::shared_ptr<CollisionRect>& pDestRect) = 0;

	/// <summary>
	/// �Փ˖@�����擾
	/// </summary>
	/// <param name="rNormal">�Փ˖@�����i�[���Ė߂�</param>
	virtual void GetHitNormal(const std::shared_ptr<CollisionSphere>& pDestSphere, Float3& retNormal)const = 0;
	virtual void GetHitNormal(const std::shared_ptr<CollisionOBB>& pDestOBB, Float3& retNormal)const = 0;
	virtual void GetHitNormal(const std::shared_ptr<CollisionCapsule>& pDestCapsule, Float3& retNormal)const = 0;
	virtual void GetHitNormal(const std::shared_ptr<CollisionRect>& pDestRect, Float3& retNormal)const = 0;


	/// <summary>
	/// 1�O�̃^�[������̎w�莞�Ԃ̈ʒu�ɖ߂�
	/// </summary>
	/// <param name="totalVelocity">�g�[�^�����x(RigidBody��Gravity..etc����������)</param>
	/// <param name="spanTime">����</param>
	virtual void BackToBefore(const Float3& totalVelocity, float spanTime) = 0;

	/// <summary>
	/// �Փ˔����̏���
	/// </summary>
	/// <param name="destCol">�����Collision</param>
	/// <param name="srcVelocity">�����̑��x</param>
	/// <param name="destVelocity">����̑��x</param>
	/// <param name="hitNormal">�Փ˖@��</param>
	/// <param name="afterHitTime">�Փˌ�̃^�[������</param>
	void AfterCollision(const std::shared_ptr<Collision>& pDestCol, const Float3& srcVelocity,
		const Float3& destVelocity, const Float3& hitNormal, float afterHitTime);

	/// <summary>
	/// �Փ˔���㏈���Ăяo���p�e���v���[�g1
	/// </summary>
	/// <param name="srcVelocity">src�̑��x</param>
	/// <param name="destVelocity">dest�̑��x</param>
	/// <param name="elapsedTime">�^�[���o�ߎ���</param>
	/// <param name="hitTime">�Փˎ���</param>
	/// <param name="srceColl">src�R���W����</param>
	/// <param name="destColl">dest�R���W����</param>
	template<class SrcColType, class DestColType>
	void AfterCollisionTemplate_1(const Float3& srcVelocity, const Float3& destVelocity,
		float elapsedTime, float hitTime, const std::shared_ptr<SrcColType>& pSrcColl, const std::shared_ptr<DestColType>& pDestColl)
	{
		// �Փˑ����o�^
		AddHitObject(pDestColl->GetGameObject());

		// �����܂��͑��肪�g���K�[�Ȃ�Փˌ�̃G�X�P�[�v���͕K�v�Ȃ��̂Ń��^�[��
		if (IsTrigger() || pDestColl->IsTrigger())
		{
			// ���肪fixed�łȂ���Α���̏Փˑ���ɓo�^
			if (!pDestColl->IsFixed())
				pDestColl->AddHitObject(GetGameObject());
			return;
		}

		// ���Ԓ���
		if (hitTime <= 0.0f)
			hitTime = 0.0f;
		if (hitTime >= elapsedTime)
			hitTime = elapsedTime;

		float afterHitTime = elapsedTime - hitTime;
		bool useSrcAfter = false;
		bool useDestAfter = false;


		if (GetHitAction() != HitAction::None)
		{
			// ���㏈�����L���Ȃ�ՓˑO�܂Ŗ߂�
			BackToBefore(srcVelocity, hitTime);
			useSrcAfter = true;
		}

		if (!pDestColl->IsFixed())
		{
			// ���肪fixed�łȂ���Α���̏Փˑ���ɓo�^
			pDestColl->AddHitObject(GetGameObject());
			if (pDestColl->GetHitAction() != HitAction::None)
			{
				// ���㏈�����L���Ȃ�ՓˑO�܂Ŗ߂�
				pDestColl->BackToBefore(destVelocity, hitTime);
				useDestAfter = true;
			}
		}
		else
		{
			// ���肪fixed�Ȃ玩�ȑޔ������ł͂܂��Փ˂��Ă���\��������̂Ŕ�������ăG�X�P�[�v����
			if (GetHitAction() != HitAction::None)
				CollisionEscape(pDestColl);
		}

		if (useSrcAfter)
		{
			Float3 hitNormal;
			GetHitNormal(pDestColl, hitNormal);
			AfterCollision(pDestColl, srcVelocity, destVelocity, hitNormal, afterHitTime);
		}
		if (useDestAfter)
		{
			Float3 hitNormal;
			GetHitNormal(pSrcColl, hitNormal);
			pDestColl->GetHitNormal(pSrcColl, hitNormal);
			pDestColl->AfterCollision(pSrcColl, destVelocity, srcVelocity, hitNormal, afterHitTime);
		}
	}
	/// <summary>
	/// �Փ˔���㏈���Ăяo���p�e���v���[�g2
	/// </summary>
	/// <param name="srcVelocity">src�̑��x</param>
	/// <param name="destVelocity">dest�̑��x</param>
	/// <param name="elapsedTime">�^�[���o�ߎ���</param>
	/// <param name="hitTime">�Փˎ���</param>
	/// <param name="srceColl">src�R���W����</param>
	/// <param name="destColl">dest�R���W����</param>
	template<class SrcColType, class DestColType>
	void AfterCollisionTemplate_2(const Float3& srcVelocity, const Float3& destVelocity,
		float elapsedTime, float hitTime, const std::shared_ptr<SrcColType>& pSrcColl, const std::shared_ptr<DestColType>& pDestColl)
	{
		// �Փˑ����o�^
		AddHitObject(pDestColl->GetGameObject());

		// �����܂��͑��肪�g���K�[�Ȃ�Փˌ�̃G�X�P�[�v���͕K�v�Ȃ��̂Ń��^�[��
		if (IsTrigger() || pDestColl->IsTrigger())
		{
			pDestColl->AddHitObject(GetGameObject());
			return;
		}

		// �����܂��͑��肪�g���K�[�Ȃ�Փˌ�̃G�X�P�[�v���͕K�v�Ȃ��̂Ń��^�[��
		if (IsTrigger() || pDestColl->IsTrigger())
			return;

		// ���Ԓ���
		if (hitTime <= 0.0f)
			hitTime = 0.0f;
		if (hitTime >= elapsedTime)
			hitTime = elapsedTime;

		float afterHitTime = elapsedTime - hitTime;
		bool useSrcAfter = false;

		if (GetHitAction() != HitAction::None)
		{
			// ���㏈�����L���Ȃ�ՓˑO�܂Ŗ߂�
			BackToBefore(srcVelocity, hitTime);
			useSrcAfter = true;
		}
		if (GetHitAction() != HitAction::None)
		{
			CollisionEscape(pDestColl);
		}
		if (useSrcAfter)
		{
			Float3 hitNormal;
			GetHitNormal(pDestColl, hitNormal);
			AfterCollision(pDestColl, srcVelocity, destVelocity, hitNormal, afterHitTime);
		}
	}



	/// <summary>
	/// ��ݍ���Sphere���E�{�����[�����擾
	/// </summary>
	/// <returns></returns>
	virtual SphereBoundaryVolume GetWrappedSphereVol()const = 0;

	/// <summary>
	/// 1�O�ƌ��݂̘A������Sphere���E�{�����[�����擾
	/// </summary>
	/// <returns></returns>
	virtual SphereBoundaryVolume GetEnclosingSphereVol()const = 0;

	/// <summary>
	/// �����Ƒ���̘A������Sphere���E�{�����[���̏Փ˔���
	/// </summary>
	/// <param name="pDestCol"></param>
	/// <returns></returns>
	template<class T>
	bool CollisionWrappedSphere(const std::shared_ptr<T>& pDestCol)
	{
		SphereBoundaryVolume src = GetEnclosingSphereVol();
		SphereBoundaryVolume dest = pDestCol->GetEnclosingSphereVol();
		return (BoundaryVolume::Sphere_Sphere(src, dest));
	}




private:
	/// <summary>
	/// �Փ˔z������؂��ăe���|�����z��ɃZ�b�g����
	/// </summary>
	/// <param name="target">���؂���I�u�W�F�N�g</param>
	/// <param name="setVec">�Z�b�g����e���|�����z��</param>
	/// <param name="scanVec">�X�L��������z��</param>
	/// <param name="isFind">����������Z�b�g���邩�ǂ��� (false�̏ꍇ�͌�����Ȃ�������Z�b�g����)</param>
	void FindIfCollisionVector(std::shared_ptr<GameObject> target, std::vector<std::shared_ptr<GameObject>>& setVec, const std::vector<std::shared_ptr<GameObject>>& scanVec, bool isFind);


private:
	struct Param;
	std::unique_ptr<Param> m_pParam;
};


