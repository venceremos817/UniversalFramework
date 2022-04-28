#pragma once
#include <Collision.h>

class CollisionRect
	: public Collision
{
public:
	explicit CollisionRect(const std::shared_ptr<GameObject>& pGameObject);
	virtual ~CollisionRect();

	/// <summary>
	/// �_�u���f�X�p�b�`�֐�	�Փ˔���𑊎�ɌĂ�ł��炤
	/// </summary>
	/// <param name="srcCol"></param>
	void CollisionCall(const std::shared_ptr<Collision>& pSrcCol) override;


	/// <summary>
	/// ���݂�Rect���E�{�����[�����擾
	/// </summary>
	/// <returns></returns>
	RectBoundaryVolume GetRectVol()const;
	/// <summary>
	/// 1�^�[���O��Rect���E�{�����[�����擾
	/// </summary>
	/// <returns></returns>
	RectBoundaryVolume GetBeforeRectVol()const;

	/// <summary>
	/// �쐬����1�ӂ̒������擾
	/// </summary>
	/// <returns></returns>
	float GetMakedSize()const;

	/// <summary>
	/// ��ݍ���Sphere���E�{�����[�����擾
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
	void CollisionTest(const std::shared_ptr<CollisionSphere>& pDestSphere) override {}
	/// <summary>
	/// OBB�Ƃ̏Փ˃e�X�g
	/// </summary>
	/// <param name="pDestOBB"></param>
	void CollisionTest(const std::shared_ptr<CollisionOBB>& pDestOBB) override {}
	/// <summary>
	/// Capusule�Ƃ̏Փ˃e�X�g
	/// </summary>
	/// <param name="pDestCapsule"></param>
	void CollisionTest(const std::shared_ptr<CollisionCapsule>& pDestCapsule) override {}
	/// <summary>
	/// Rect�Ƃ̏Փ˃e�X�g
	/// </summary>
	/// <param name="pDestRect"></param>
	void CollisionTest(const std::shared_ptr<CollisionRect>& pDestRect) override {}

	//========== �G�X�P�[�v���� ==========
	/// <summary>
	/// Sphere����G�X�P�[�v
	/// </summary>
	/// <param name="pDestCol"></param>
	void CollisionEscape(const std::shared_ptr<CollisionSphere>& pDestSphere) override {}
	/// <summary>
	/// OBB����G�X�P�[�v
	/// </summary>
	/// <param name="pDestOBB"></param>
	void CollisionEscape(const std::shared_ptr<CollisionOBB>& pDestOBB) override {}
	/// <summary>
	/// Capsule����G�X�P�[�v
	/// </summary>
	/// <param name="pDestCapsule"></param>
	void CollisionEscape(const std::shared_ptr<CollisionCapsule>& pDestCapsule) override {}
	/// <summary>
	/// Rect����G�X�P�[�v
	/// </summary>
	/// <param name="pDestRect"></param>
	void CollisionEscape(const std::shared_ptr<CollisionRect>& pDestRect) override {}

	/// <summary>
	/// �Փ˖@�����擾
	/// </summary>
	/// <param name="pDestSphere"></param>
	/// <param name="retNormal"></param>
	void GetHitNormal(const std::shared_ptr<CollisionSphere>& pDestSphere, Float3 & retNormal) const override {}
	void GetHitNormal(const std::shared_ptr<CollisionOBB>& pDestOBB, Float3 & retNormal) const override {}
	void GetHitNormal(const std::shared_ptr<CollisionCapsule>& pDestCapsule, Float3 & retNormal) const override {}
	void GetHitNormal(const std::shared_ptr<CollisionRect>& pDestRect, Float3 & retNormal) const override {}


	void BackToBefore(const Float3 & totalVelocity, float spanTime) override {}

private:
	struct Param;
	std::unique_ptr<Param> m_pParam;
};