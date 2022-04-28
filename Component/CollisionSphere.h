#pragma once
#include <Collision.h>



class CollisionSphere
	: public Collision
{
public:
	explicit CollisionSphere(const std::shared_ptr<GameObject>& pGameObject);
	virtual ~CollisionSphere();



	/// <summary>
	/// �_�u���f�X�p�b�`�֐�	�Փ˔���𑊎�ɌĂ�ł��炤
	/// </summary>
	/// <param name="pSrcCol"></param>
	 void CollisionCall(const std::shared_ptr<Collision>& pSrcCol) override;

	//========== �Q�b�^�[�E�Z�b�^�[ ==========
	/// <summary>
	/// �쐬���̔��a���擾
	/// </summary>
	/// <returns></returns>
	float GetMakedRadius()const;
	/// <summary>
	/// �쐬���̔��a��ݒ�
	/// </summary>
	/// <param name="radius"></param>
	void SetMakedRadius(float radius);
	/// <summary>
	/// �쐬���̒��a���擾
	/// </summary>
	/// <returns></returns>
	float GetMakedDiameter()const;
	/// <summary>
	/// �쐬���̒��a��ݒ�
	/// </summary>
	/// <param name="diameter"></param>
	/// <returns></returns>
	void SetMakeDiameter(float diameter);
	/// <summary>
	/// �v�Z��XYZ�̂ǂ̃X�P�[�����g�p���邩���擾
	/// </summary>
	/// <returns></returns>
	CalcScaling GetCalcScaling()const;
	/// <summary>
	/// �v�Z��XYZ�̂ǂ̃X�P�[�����g�p���邩��ݒ�
	/// </summary>
	/// <param name="s"></param>
	void SetCalcScaling(CalcScaling s);
	/// <summary>
	/// ���݂�Sphere���E�{�����[�����擾
	/// </summary>
	/// <returns></returns>
	SphereBoundaryVolume GetSphereVolume()const;
	/// <summary>
	/// 1�^�[���O��Sphere���E�{�����[�����擾
	/// </summary>
	/// <returns></returns>
	SphereBoundaryVolume GetBeforeSphereVolume()const;

	/// <summary>
	/// Sphere�{�����[�����擾
	/// </summary>
	/// <returns></returns>
	SphereBoundaryVolume GetWrappedSphereVol()const override;

	/// <summary>
	/// 1�O�ƌ��݂̘A������Sphere���E�{�����[�����擾
	/// </summary>
	/// <returns></returns>
	SphereBoundaryVolume GetEnclosingSphereVol() const override;




	//========== �Փ˃e�X�g ==========
	/// <summary>
	/// Sphere�Ƃ̏Փ˃e�X�g
	/// </summary>
	/// <param name="pDestSphere"></param>
	void CollisionTest(const std::shared_ptr<CollisionSphere>& pDestSphere)override;
	/// <summary>
	/// OBB�Ƃ̏Փ˃e�X�g (��]�ɂ��G�X�P�[�v���Ή�)
	/// </summary>
	/// <param name="pDestOBB"></param>
	void CollisionTest(const std::shared_ptr<CollisionOBB>& pDestOBB)override;
	/// <summary>
	/// Capsule�Ƃ̏Փ˃e�X�g
	/// </summary>
	/// <param name="pDestCapsule"></param>
	void CollisionTest(const std::shared_ptr<CollisionCapsule>& pDestCapsule)override;
	/// <summary>
	/// Rect�Ƃ̏Փ˃e�X�g
	/// </summary>
	/// <param name="pDestRect"></param>
	void CollisionTest(const std::shared_ptr<CollisionRect>& pDestRect)override;

	//========== �G�X�P�[�v���� ==========
	/// <summary>
	/// Sphere����G�X�P�[�v
	/// </summary>
	/// <param name="pDestSphere"></param>
	void CollisionEscape(const std::shared_ptr<CollisionSphere>& pDestSphere)override;
	/// <summary>
	/// OBB����G�X�P�[�v
	/// </summary>
	/// <param name="pDestOBB"></param>
	void CollisionEscape(const std::shared_ptr<CollisionOBB>& pDestOBB)override;
	/// <summary>
	/// Capsule����G�X�P�[�v
	/// </summary>
	/// <param name="pDestCapsule"></param>
	void CollisionEscape(const std::shared_ptr<CollisionCapsule>& pDestCapsule)override;
	/// <summary>
	/// Rect����G�X�P�[�v
	/// </summary>
	/// <param name="pDestRect"></param>
	void CollisionEscape(const std::shared_ptr<CollisionRect>& pDestRect)override;


	/// <summary>
	/// �Փ˖@�����擾
	/// </summary>
	/// <param name="retNormal">�Փ˖@�����i�[���Ė߂�</param>
	void GetHitNormal(const std::shared_ptr<CollisionSphere>& pDestSphere, Float3 & retNormal) const override;
	void GetHitNormal(const std::shared_ptr<CollisionOBB>& pDestOBB, Float3 & retNormal) const override;
	void GetHitNormal(const std::shared_ptr<CollisionCapsule>& pDestCapsule, Float3 & retNormal) const override;
	void GetHitNormal(const std::shared_ptr<CollisionRect>& pDestRect, Float3 & retNormal) const override;

	/// <summary>
	/// 1�O�̃^�[������̎w�莞�Ԃ̈ʒu�ɖ߂�
	/// </summary>
	/// <param name="totalVelocity">�g�[�^�����x(RigidBody��Gravity..etc����������)</param>
	/// <param name="spanTime">����</param>
	void BackToBefore(const Float3& totalVelocity, float spanTime)override;


private:
	struct Param;
	std::unique_ptr<Param> m_pParam;


	// Collision ����Čp������܂���



};