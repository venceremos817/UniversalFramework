#pragma once

#include <SphereBoundaryVolume.h>
#include <OBBBoundaryVolume.h>
#include <CapsuleBoundaryVolume.h>
#include <RectBoundaryVolume.h>
#include <PlaneBoundaryVolume.h>


class BoundaryVolume
{
public:
	/// <summary>
	/// �R���C�_�[�̌`
	/// </summary>
	enum ColliderKind
	{
		None,
		Sphere,
		OBB
	};

public:
	BoundaryVolume() :m_kind(ColliderKind::None) {}


	/// <summary>
	/// 2��Sphere���E�{�����[�����Ȃ���Sphere���E�{�����[�����擾
	/// </summary>
	/// <param name="sp1"></param>
	/// <param name="sp2"></param>
	/// <returns></returns>
	static SphereBoundaryVolume SphereEnclosingSphere(const SphereBoundaryVolume& sp0, const SphereBoundaryVolume& sp1);


	/// <summary>
	/// ���Ƌ��̏Փ˔���
	/// </summary>
	/// <param name="sp1"></param>
	/// <param name="sp2"></param>
	/// <returns></returns>
	static bool Sphere_Sphere(const SphereBoundaryVolume& sp1, const SphereBoundaryVolume& sp2);
	/// <summary>
	/// Sphere�Ɠ����Ȃ�Sphere�̏Փ˔���
	/// </summary>
	/// <param name="srcSphere">������</param>
	/// <param name="srcVelocity">�������̑��x</param>
	/// <param name="destSphere">�����Ȃ���</param>
	/// <param name="startTime">�J�n����</param>
	/// <param name="endTime">�I������</param>
	/// <param name="retHitTime">�q�b�g���Ԃ��i�[���ĕԂ�</param>
	/// <returns></returns>
	static bool CollisionTestSphereSphere(const SphereBoundaryVolume& srcSphere, const Float3& srcVelocity,
		const SphereBoundaryVolume& destSphere, float startTime, float endTime, float& retHhitTime);

	/// <summary>
	/// ����OBB�̏Փ˔���
	/// </summary>
	/// <param name="sp"></param>
	/// <param name="obb"></param>
	/// <param name="retClosestPos">�Őڋߓ_���i�[����Q��</param>
	/// <returns></returns>
	static bool Sphere_OBB(const SphereBoundaryVolume& sp, const OBBBoundaryVolume& obb, Float3& retClosestPos);
	/// <summary>
	/// Sphere�Ɠ����Ȃ�OBB�̏Փ˔���
	/// </summary>
	/// <param name="srcSphere">������</param>
	/// <param name="srcVelocity">�������̑��x</param>
	/// <param name="destOBB">�����Ȃ�OBB</param>
	/// <param name="startTime">�J�n����</param>
	/// <param name="endTime">�I������</param>
	/// <param name="retHitTime">�q�b�g���Ԃ��i�[���ĕԂ�</param>
	/// <returns></returns>
	static bool CollisionTestSphereOBB(const SphereBoundaryVolume& srcSphere, const Float3& srcVelocity,
		const OBBBoundaryVolume& destOBB, float startTime, float endTime, float& retHitTime);

	/// <summary>
	/// ���ƃJ�v�Z���̏Փ˔���
	/// </summary>
	/// <param name="sphere"></param>
	/// <param name="capsule"></param>
	/// <param name="retClosestPos">�Őڋߓ_���i�[���Ė߂�</param>
	/// <returns></returns>
	static bool SPHERE_CAPSULE(const SphereBoundaryVolume& sphere, const CapsuleBoundaryVolume& capsule, Float3& retClosestPos);
	/// <summary>
	/// Sphere�Ɠ����Ȃ��J�v�Z���̏Փ˔���
	/// </summary>
	/// <param name="srcSphere">������</param>
	/// <param name="srcVelocity">�������̑��x</param>
	/// <param name="destCapsule">�����Ȃ��J�v�Z��</param>
	/// <param name="startTime">�J�n����</param>
	/// <param name="endTime">�I������</param>
	/// <param name="retHitTime">�q�b�g���Ԃ��i�[���ĕԂ�</param>
	/// <returns></returns>
	static bool CollisionTestSphereCapsule(const SphereBoundaryVolume& srcSphere, const Float3& srcVelocity,
		const CapsuleBoundaryVolume& destCapsule, float startTime, float endTime, float& retHitTime);

	/// <summary>
	/// �J�v�Z����OBB�̏Փ˔���
	/// </summary>
	/// <param name="capsule">�J�v�Z��</param>
	/// <param name="obb">OBB</param>
	/// <param name="retClosestPos">�Őڋߓ_���i�[���Ė߂�</param>
	/// <returns></returns>
	static bool CAPSULE_OBB(const CapsuleBoundaryVolume& capsule, const OBBBoundaryVolume& obb, Float3& retClosestPos);
	/// <summary>
	/// �J�v�Z���Ɠ����Ȃ�OBB�̏Փ˔���
	/// </summary>
	/// <param name="srcCapVol">�����J�v�Z��</param>
	/// <param name="srcVelocity">�����J�v�Z���̑��x</param>
	/// <param name="destOBBVol">�����Ȃ�OBB</param>
	/// <param name="startTime">�J�n����</param>
	/// <param name="endTime">�I������</param>
	/// <param name="retHitTime">�q�b�g�������Ԃ��i�[���ĕԂ�</param>
	/// <returns></returns>
	static bool CollisionTestCapsuleOBB(const CapsuleBoundaryVolume& srcCapVol, const Float3& srcVelocity,
		const OBBBoundaryVolume& destOBBVol, float startTime, float endTime, float& retHitTime);

	/// <summary>
	/// �J�v�Z���ƃJ�v�Z���̏Փ˔���
	/// </summary>
	/// <param name="capA">�J�v�Z��A</param>
	/// <param name="capB">�J�v�Z��B</param>
	/// <param name="retClosestPosA">�J�v�Z��A�̍Őڋߓ_���i�[���ĕԂ�</param>
	/// <param name="retClosestPosB">�J�v�Z��B�̍Őڋߓ_���i�[���ĕԂ�</param>
	/// <returns></returns>
	static bool CAPSULE_CAPSULE(const CapsuleBoundaryVolume& capA, const CapsuleBoundaryVolume& capB, Float3& retClosestPosA, Float3& retClosestPosB);

	/// <summary>
	/// �J�v�Z���Ɠ����Ȃ��J�v�Z���̏Փ˔���
	/// </summary>
	/// <param name="srcCapVol">�����J�v�Z��</param>
	/// <param name="srcVelocity">�����J�v�Z���̑��x</param>
	/// <param name="destCapVol">�����Ȃ��J�v�Z��</param>
	/// <param name="startTime">�J�n����</param>
	/// <param name="endTime">�I������</param>
	/// <param name="retHitTime">�q�b�g�������Ԃ��i�[���ĕԂ�</param>
	/// <returns></returns>
	static bool CollisionTestCapsuleCapsule(const CapsuleBoundaryVolume& srcCapVol, const Float3& srcVelocity,
		const CapsuleBoundaryVolume& destCapVol, float startTime, float endTime, float& hitTime);

	/// <summary>
	/// ����AB�ƕ��ʂ̏Փ˔���
	/// </summary>
	/// <param name="a">�����J�n�_</param>
	/// <param name="b">�����I�_</param>
	/// <param name="plane">���ʋ��E�{�����[��</param>
	/// <param name="outT">�Őڋߓ_�̐�����̈ʒu�̊������i�[���ĕԂ�</param>
	/// <param name="outClosestPos">�Փ˂��Ă���ꍇ�A�Őڋߓ_���i�[���ĕԂ�</param>
	/// <returns></returns>
	static bool SEGMENT_PLANE(const Float3& a, const Float3& b, const PlaneBoundaryVolume& plane, float& outT, Float3& outClosestPos);

	/// <summary>
	/// �J�v�Z���Ƌ�`�̏Փ˔���
	/// </summary>
	/// <param name="capsuleVol">�J�v�Z�����E�{�����[��</param>
	/// <param name="rectVol">��`���E�{�����[��</param>
	/// <param name="retClosestPos">�Őڋߓ_���i�[���ĕԂ�</param>
	/// <returns></returns>
	static bool CAPSULE_RECT(const CapsuleBoundaryVolume& capsuleVol, const RectBoundaryVolume& rectVol, Float3& retClosestPos);
	/// <summary>
	/// �J�v�Z���Ɠ����Ȃ���`�̏Փ˔���
	/// </summary>
	/// <param name="srcCapVol">�����J�v�Z��</param>
	/// <param name="srcVelocity">�����J�v�Z���̑��x</param>
	/// <param name="destRectVol">�����Ȃ���`</param>
	/// <param name="startTime">�J�n����</param>
	/// <param name="endTime">�I������</param>
	/// <param name="retHitTime">�q�b�g�������Ԃ��i�[���ĕԂ�</param>
	/// <returns></returns>
	static bool CollisionTestCapsuleRect(const CapsuleBoundaryVolume& srcCapVol, const Float3& srcVelocity,
		const RectBoundaryVolume& destRectVol, float startTime, float endTime, float& retHitTime);

	/// <summary>
	/// OBB��OBB�̏Փ˔���
	/// </summary>
	/// <param name="obb1"></param>
	/// <param name="obb2"></param>
	/// <returns></returns>
	static bool OBB_OBB(const OBBBoundaryVolume& obb1, const OBBBoundaryVolume& obb2);
	/// <summary>
	/// OBB�Ɠ����Ȃ�OBB�̏Փ˔���
	/// (���x���d�����A������Ȃ��Ƒ��߂ɔ��f�ł���
	/// </summary>
	/// <param name="srcOBB">����OBB</param>
	/// <param name="srcVelocity">����OBB�̑��x</param>
	/// <param name="destOBB">�����Ȃ�OBB</param>
	/// <param name="startTime">�J�n����</param>
	/// <param name="endTime">�I������</param>
	/// <param name="retHitTime">�q�b�g���Ԃ��i�[���ĕԂ�</param>
	/// <returns></returns>
	static bool CollisionTestOBBOBB(const OBBBoundaryVolume& srcOBB, const Float3& srcVelocity,
		const OBBBoundaryVolume& destOBB, float startTime, float endTime, float& retHitTime);
	/// <summary>
	/// OBB�Ɠ����Ȃ�OBB�̏Փ˔��� (����
	/// </summary>
	/// <param name="srcOBB">����OBB</param>
	/// <param name="srcVelocity">����OBB�̑��x</param>
	/// <param name="destOBB">�����Ȃ�OBB</param>
	/// <param name="startTime">�J�n����</param>
	/// <param name="endTime">�I������</param>
	/// <param name="retHitTime">�q�b�g���Ԃ��i�[���ĕԂ�</param>
	/// <returns></returns>
	static bool CollisionTestOBBOBBsub(const OBBBoundaryVolume& srcOBB, const Float3& srcVelocity,
		const OBBBoundaryVolume& destOBB, float startTime, float endTime, float& retHitTime);

	/// <summary>
	/// Sphere��Rect�̏Փ˔���
	/// </summary>
	/// <param name="sphere"></param>
	/// <param name="rect"></param>
	/// <param name="retClosestPos">�Őڋߓ_���i�[���ĕԂ�</param>
	/// <returns></returns>
	static bool SPHERE_RECT(const SphereBoundaryVolume& sphere, const RectBoundaryVolume& rect, Float3& retClosestPos);
	/// <summary>
	/// Sphere�Ɠ����Ȃ�Rect�̏Փ˔���
	/// </summary>
	/// <param name="srcSphere">������</param>
	/// <param name="srcVelocity">�������̑��x</param>
	/// <param name="destRect">�����Ȃ���`</param>
	/// <param name="startTime">�J�n����</param>
	/// <param name="endTime">�I������</param>
	/// <param name="retHitTime">�q�b�g���Ԃ��i�[���ĕԂ�</param>
	/// <returns></returns>
	static bool CollisionTestSphereRect(const SphereBoundaryVolume& srcSphere, const Float3& srcVelocity,
		const RectBoundaryVolume& destRect, float startTime, float endTime, float& retHitTime);

	/// <summary>
	/// OBB��PLANE�̏Փ˔���
	/// </summary>
	/// <param name="obb"></param>
	/// <param name="plane"></param>
	/// <returns></returns>
	static bool OBB_PLANE(const OBBBoundaryVolume& obb, const PlaneBoundaryVolume& plane);

	/// <summary>
	/// OBB�Ƌ�`�̏Փ˔���
	/// </summary>
	/// <param name="obb"></param>
	/// <param name="rect"></param>
	/// <returns></returns>
	static bool OBB_RECT(const OBBBoundaryVolume& obb, const RectBoundaryVolume& rect);
	/// <summary>
	/// OBB�Ɠ����Ȃ�Rect�̏Փ˔���
	/// </summary>
	/// <param name="srcOBB">����OBB</param>
	/// <param name="srcVelocity">����OBB�̑��x</param>
	/// <param name="destRect">�����Ȃ���`</param>
	/// <param name="startTime">�J�n����</param>
	/// <param name="endTime">�I������</param>
	/// <param name="hitTime">�q�b�g���Ԃ��i�[���ĕԂ�</param>
	/// <returns></returns>
	static bool CollisionTestOBBRect(const OBBBoundaryVolume& srcOBB, const Float3& srcVelocity,
		const RectBoundaryVolume& destRect, float startTime, float endTime, float& hitTime);

	/// <summary>
	/// point���猩��OBB�̍Őڋߓ_�����߂�
	/// </summary>
	/// <param name="point"></param>
	/// <param name="obb"></param>
	/// <param name="retClosestPos">�Őڋߓ_���i�[����Q��</param>
	static void ClosestPtPointOBB(const Float3& point, const OBBBoundaryVolume& obb, Float3& retClosestPos);

	/// <summary>
	/// �_�Ɛ����̍Őڋߓ_�����߂�
	/// </summary>
	/// <param name="c">�_</param>
	/// <param name="a">�����J�n�_</param>
	/// <param name="b">�����I���_</param>
	/// <param name="retT">�Őڋߓ_�̐�����̈ʒu�̊������i�[���ĕԂ�</param>
	/// <param name="retClosestPos">�Őڋߓ_���i�[���ĕԂ�</param>
	static void ClosestPtPointSegment(const Float3& c, const Float3& a, const Float3& b, float& retT, Float3& retClosestPos);

	/// <summary>
	/// �J�v�Z����OBB�̍Őڋߓ_���擾
	/// </summary>
	/// <param name="capsule">�J�v�Z��</param>
	/// <param name="obb">OBB</param>
	/// <param name="retFlg">2�̋��Ƃ̈ʒu�֌W</param>
	/// <returns></returns>
	static Float3 ClosestPtCapsuleOBB(const CapsuleBoundaryVolume& capsule, const OBBBoundaryVolume& obb, int& retFlg);

	/// <summary>
	/// �_�Ƌ�`�̍Őڋߓ_�����߂�
	/// </summary>
	/// <param name="point">�_</param>
	/// <param name="rect">��`</param>
	/// <param name="retClosestPos">�Őڋߓ_���i�[���ĕԂ�</param>
	static void ClosestPtPointRect(const Float3& point, const RectBoundaryVolume& rect, Float3& retClosestPos);

	/// <summary>
	/// �������m�̍Őڋߓ_�����߂�
	/// </summary>
	/// <param name="sA">����A�̎n�_</param>
	/// <param name="eA">����A�̏I�_</param>
	/// <param name="sB">����B�̎n�_</param>
	/// <param name="eB">����B�̏I�_</param>
	/// <param name="retS">����A�̍Őڋߓ_�̃p�����[�^</param>
	/// <param name="retT">����B�̍Őڋߓ_�̃p�����[�^</param>
	/// <param name="retClosestPosA">�Őڋߓ_A</param>
	/// <param name="retClosestPosB">�Őڋߓ_B</param>
	/// <returns>�Őڋߓ_PosA,PosB���v�Z���A���̃p�����[�^s,t���v�Z���A�������m�̋����̕�����Ԃ�</returns>
	static float ClosestPtSegmentSegment(const Float3& sA, const Float3& eA, const Float3& sB, const Float3& eB, 
		float& retS, float& retT, Float3& retClosestPosA, Float3& retClosestPosB);

	/// <summary>
	/// �_�Ɛ����̋����̕������擾
	/// </summary>
	/// <param name="a">�����J�n�_</param>
	/// <param name="b">�����I���_</param>
	/// <param name="c">�_</param>
	/// <returns></returns>
	static float SqrtDistPointSegment(const Float3& a, const Float3& b, const Float3& c);


public:
	ColliderKind m_kind;
};


