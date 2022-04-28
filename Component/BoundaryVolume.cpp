#include "BoundaryVolume.h"
#include <XMFLOAT_EX.h>



SphereBoundaryVolume BoundaryVolume::SphereEnclosingSphere(const SphereBoundaryVolume & sp0, const SphereBoundaryVolume & sp1)
{
	const float EPSILON = 1.175494e-37f;
	SphereBoundaryVolume retSphereVol;
	Float3 d = sp1.m_center - sp0.m_center;
	float dist = Float3::Dot(d, d);
	float f = sp1.m_radius - sp0.m_radius;
	Float4 vec(f);
	vec = DirectX::XMVectorPow(vec, Float4(2.0f));
	if (vec.x >= dist)
	{
		if (sp1.m_radius >= sp0.m_radius)
		{
			retSphereVol = sp1;
		}
		else
		{
			retSphereVol = sp0;
		}
	}
	else
	{
		vec = Float4(dist);
		vec = DirectX::XMVectorSqrt(vec);
		dist = vec.x;
		retSphereVol.m_radius = (dist + sp0.m_radius + sp1.m_radius) * 0.5f;
		retSphereVol.m_center = sp0.m_center;
		if (dist > EPSILON)
		{
			retSphereVol.m_center += Float3(((retSphereVol.m_radius - sp0.m_radius) / dist) * d);
		}
	}

	return retSphereVol;
}

bool BoundaryVolume::Sphere_Sphere(const SphereBoundaryVolume & sp1, const SphereBoundaryVolume & sp2)
{
	Float3 dist = sp1.m_center - sp2.m_center;
	float dist2 = Float3::Dot(dist, dist);
	float sumRadius = sp1.m_radius + sp2.m_radius;
	return dist2 <= sumRadius * sumRadius;
}

bool BoundaryVolume::CollisionTestSphereSphere(const SphereBoundaryVolume & srcSphere, const Float3 & srcVelocity, const SphereBoundaryVolume & destSphere, float startTime, float endTime, float & retHitTime)
{
	const float EPSILON = 0.005f;
	// �ċA�ɂ���Ĉړ��̊Ԃ��Ԃ���
	// �����ԗp��SphereCollider
	SphereBoundaryVolume srcSp2;		
	float mid = (startTime + endTime) * 0.5f;	// start��end�̊Ԃ̎���
	srcSp2.m_center = srcSphere.m_center + srcVelocity * mid;	// �����i�񂾈ʒu
	srcSp2.m_radius = (mid - startTime) * srcVelocity.Length() + srcSphere.m_radius;
	if (!Sphere_Sphere(srcSp2, destSphere))
	{
		return false;
	}
	if (endTime - startTime < EPSILON)	// �\���ɕ�Ԃ����Ȃ�
	{
		retHitTime = startTime;	// �����������������ԂɂȂ�
		return true;
	}
	// �O���̎��ԂōċA
	if (CollisionTestSphereSphere(srcSphere, srcVelocity, destSphere, startTime, mid, retHitTime))
	{
		return true;
	}
	// �O���œ������Ă��Ȃ��̂Ō㔼�̎��ԂōċA
	return CollisionTestSphereSphere(srcSphere, srcVelocity, destSphere, mid, endTime, retHitTime);
}

bool BoundaryVolume::Sphere_OBB(const SphereBoundaryVolume & sp, const OBBBoundaryVolume & obb, Float3 & retClosestPos)
{
	ClosestPtPointOBB(sp.m_center, obb, retClosestPos);	// ���̒��S�_��OBB�̍Őڋߓ_�����߂�
	Float3 v = retClosestPos - sp.m_center;	// ���̒��S���Őڋߓ_�x�N�g��
	return Float3::Dot(v, v) <= sp.m_radius * sp.m_radius;
}

bool BoundaryVolume::CollisionTestSphereOBB(const SphereBoundaryVolume & srcSphere, const Float3 & srcVelocity, const OBBBoundaryVolume & destOBB, float startTime, float endTime, float & retHitTime)
{
	const float EPSILON = 0.005f;
	SphereBoundaryVolume srcSp2;
	float mid = (startTime + endTime) * 0.5f;
	srcSp2.m_center = srcSphere.m_center + srcVelocity * mid;
	srcSp2.m_radius = (mid - startTime) * srcVelocity.Length() + srcSphere.m_radius;
	Float3 closestPos;
	if (!Sphere_OBB(srcSp2, destOBB, closestPos))
	{
		return false;
	}
	if (endTime - startTime < EPSILON)
	{
		retHitTime = startTime;	// �����������ԓ���
		return true;
	}
	if (CollisionTestSphereOBB(srcSphere, srcVelocity, destOBB, startTime, mid, retHitTime))
		return true;
	return CollisionTestSphereOBB(srcSphere, srcVelocity, destOBB, mid, endTime, retHitTime);
}

bool BoundaryVolume::SPHERE_CAPSULE(const SphereBoundaryVolume & sphere, const CapsuleBoundaryVolume & capsule, Float3 & retClosestPos)
{
	float t;
	Float3 closestPos;
	// ���̒��S�ƃJ�v�Z�����[��[�����̍Őڋߓ_�����߂�
	ClosestPtPointSegment(sphere.m_center, capsule.m_bottomPos, capsule.m_topPos, t, closestPos);
	Float3 normal = sphere.m_center - closestPos;
	normal.Normalize();
	normal *= capsule.m_radius;
	// �Őڋߓ_���i�[
	retClosestPos = closestPos + normal;
	// �Փ˂��Ă��邩����
	float dist = SqrtDistPointSegment(capsule.m_bottomPos, capsule.m_topPos, sphere.m_center);
	float radius = sphere.m_radius + capsule.m_radius;
	return dist < radius * radius;
}

bool BoundaryVolume::CollisionTestSphereCapsule(const SphereBoundaryVolume & srcSphere, const Float3 & srcVelocity, const CapsuleBoundaryVolume & destCapsule, float startTime, float endTime, float & retHitTime)
{
	const float EPSILON = 0.005f;
	SphereBoundaryVolume srcSphere2;
	float mid = (startTime + endTime) * 0.5f;	// ���Ԃ̎���
	srcSphere2.m_center = srcSphere.m_center + srcVelocity * mid;
	srcSphere2.m_radius = (mid - startTime) * srcVelocity.Length() + srcSphere.m_radius;
	Float3 retVec;
	if (!BoundaryVolume::SPHERE_CAPSULE(srcSphere2, destCapsule, retVec))
		return false;	// �������Ă��Ȃ�
	// �\���ɍċA������
	if (endTime - startTime < EPSILON)
	{
		retHitTime = startTime;	// �q�b�g���ԓ���
		return true;
	}
	// �O�����ԂōċA
	if (CollisionTestSphereCapsule(srcSphere, srcVelocity, destCapsule, startTime, mid, retHitTime))
		return true;
	// �㔼���ԂōċA
	return CollisionTestSphereCapsule(srcSphere, srcVelocity, destCapsule, mid, endTime, retHitTime);
}

bool BoundaryVolume::CAPSULE_OBB(const CapsuleBoundaryVolume & capsule, const OBBBoundaryVolume & obb, Float3 & retClosestPos)
{
	// TODO : �m�F
	// �X�C�[�v�����鋅
	SphereBoundaryVolume startSphere, endSphere;
	startSphere.m_center = capsule.m_bottomPos;
	startSphere.m_radius = capsule.m_radius;
	endSphere.m_center = capsule.m_topPos;
	endSphere.m_radius = capsule.m_radius;
	// �e�_��OBB�̍Őڋߓ_���擾
	// �J�v�Z����OBB�̍Őڋߓ_���擾
	int flg;
	retClosestPos = ClosestPtCapsuleOBB(capsule, obb, flg);
	float hitTime;
	Float3 velocity = endSphere.m_center - startSphere.m_center;
	if (CollisionTestSphereOBB(startSphere, velocity, obb, 0.0f, 1.0f, hitTime))
		return true;
	return false;
}

bool BoundaryVolume::CollisionTestCapsuleOBB(const CapsuleBoundaryVolume & srcCapVol, const Float3 & srcVelocity, const OBBBoundaryVolume & destOBBVol, float startTime, float endTime, float & retHitTime)
{
	// TODO : ������
	const float EPSILON = 0.005f;
	CapsuleBoundaryVolume srcCapVol2 = srcCapVol;
	float mid = (startTime + endTime) * 0.5f;
	srcCapVol2.m_radius = (mid - startTime) * srcVelocity.Length() + srcCapVol.m_radius;
	float scale = srcCapVol2.m_radius / srcCapVol.m_radius;
	// ���S�����_�̃J�v�Z�����쐬
	CapsuleBoundaryVolume srcBaseCapVol = srcCapVol;
	srcBaseCapVol.SetCenter(Float3(0.0f));
	// ���_�J�v�Z���ŃX�P�[�����O
	Float4x4 scaleMatrix;
	scaleMatrix.Scale(Float3(scale));
	// �e���_���X�P�[�����O
	srcCapVol2.m_bottomPos = srcBaseCapVol.m_bottomPos * scaleMatrix;
	srcCapVol2.m_topPos = srcBaseCapVol.m_bottomPos * scaleMatrix;
	// ���S�ړ�
	srcCapVol2.SetCenter(srcCapVol.GetCenter() + srcVelocity * mid);
	Float3 closestPos;
	if (!CAPSULE_OBB(srcCapVol2, destOBBVol, closestPos))
		return false;

	// �\���ɍċA������
	if (endTime - startTime < EPSILON)
	{
		retHitTime = startTime;		// �Փˎ��ԓ���
		return true;
	}

	// �O�����ԂōċA
	if (CollisionTestCapsuleOBB(srcCapVol, srcVelocity, destOBBVol, startTime, mid, retHitTime))
		return true;
	// �㔼���ԂōċA
	return CollisionTestCapsuleOBB(srcBaseCapVol, srcVelocity, destOBBVol, mid, endTime, retHitTime);
}

bool BoundaryVolume::CAPSULE_CAPSULE(const CapsuleBoundaryVolume & capA, const CapsuleBoundaryVolume & capB, Float3 & retClosestPosA, Float3 & retClosestPosB)
{
	float s, t;
	float dist = ClosestPtSegmentSegment(capA.m_bottomPos, capA.m_topPos, capB.m_bottomPos, capB.m_topPos, s, t, retClosestPosA, retClosestPosB);
	float radius = capA.m_radius + capB.m_radius;
	return dist < radius * radius;
}

bool BoundaryVolume::CollisionTestCapsuleCapsule(const CapsuleBoundaryVolume & srcCapVol, const Float3 & srcVelocity, const CapsuleBoundaryVolume & destCapVol, float startTime, float endTime, float & retHitTime)
{
	const float EPSILON = 0.005f;

	CapsuleBoundaryVolume srcCapsuleVol2 = srcCapVol;
	float mid = (startTime + endTime) * 0.5f;
	srcCapsuleVol2.m_radius = (mid - startTime) * srcVelocity.Length() + srcCapVol.m_radius;
	float scale = srcCapsuleVol2.m_radius / srcCapVol.m_radius;

	// ���S�����_�̌��̃J�v�Z�����쐬
	CapsuleBoundaryVolume srcBaseCapsuleVol = srcCapVol;
	srcBaseCapsuleVol.SetCenter(Float3(0.0f));
	// ���_�J�v�Z�����X�P�[�����O
	Float4x4 scaleMat;
	scaleMat.Scale(Float3(scale));
	// �e���_���X�P�[�����O
	srcCapsuleVol2.m_bottomPos = srcBaseCapsuleVol.m_bottomPos * scaleMat;
	srcCapsuleVol2.m_topPos = srcBaseCapsuleVol.m_topPos * scaleMat;
	// ���S�_���ړ�
	srcCapsuleVol2.SetCenter(srcCapVol.GetCenter() + srcVelocity * mid);

	Float3 temp1, temp2;
	if (!BoundaryVolume::CAPSULE_CAPSULE(srcCapsuleVol2, destCapVol, temp1, temp2))
		return false;
	if (endTime - startTime < EPSILON)
	{
		retHitTime = startTime;
		return true;
	}
	if (CollisionTestCapsuleCapsule(srcCapVol, srcVelocity, destCapVol, startTime, mid, retHitTime))
		return true;
	return CollisionTestCapsuleCapsule(srcCapVol, srcVelocity, destCapVol, mid, endTime, retHitTime);
}

bool BoundaryVolume::SEGMENT_PLANE(const Float3 & a, const Float3 & b, const PlaneBoundaryVolume & plane, float & outT, Float3 & outClosestPos)
{
	Float3 ab = b - a;
	outT = (plane.m_dotValue - Float3::Dot(plane.m_normal, a)) / Float3::Dot(plane.m_normal, ab);
	if (outT > 0.0f && outT < 1.0f)
	{
		outClosestPos = a + (ab * outT);
		return true;
	}

	return false;
}

bool BoundaryVolume::CAPSULE_RECT(const CapsuleBoundaryVolume & capsuleVol, const RectBoundaryVolume & rectVol, Float3 & retClosestPos)
{
	// �X�C�[�v�����鋅
	SphereBoundaryVolume startSpVol, endSpVol;
	// �������
	startSpVol.m_center = capsuleVol.m_bottomPos;
	startSpVol.m_radius = capsuleVol.m_radius;
	endSpVol.m_center = capsuleVol.m_topPos;
	endSpVol.m_radius = capsuleVol.m_radius;

	PlaneBoundaryVolume planeVol = rectVol.GetPlaneVolume();
	float t;
	Float3 closestPos;
	// �J�v�Z�������Ƌ�`���ʂ̍Őڋߓ_�ƁA�Őڋߓ_�̐�����ł̊������擾
	SEGMENT_PLANE(startSpVol.m_center, endSpVol.m_center, planeVol, t, closestPos);
	// ���̓_�ŏ�����
	Float3 center = startSpVol.m_center;

	if (t < 0)			// �J�n�_���O
		center = startSpVol.m_center;
	else if (t > 1.0f)	// �I���_����
		center = endSpVol.m_center;
	else				// ������Ɋ܂܂��
		center = closestPos;

	// center�͐���̖ʂƂ̍Őڋߓ_
	// ���Ƌ�`�̍Őڋߓ_�𓾂�
	ClosestPtPointRect(center, rectVol, retClosestPos);

	float hitTime;
	Float3 velocity = endSpVol.m_center - startSpVol.m_center;
	if (CollisionTestSphereRect(startSpVol, velocity, rectVol, 0.0f, 1.0f, hitTime))
		return true;
	return false;
}

bool BoundaryVolume::CollisionTestCapsuleRect(const CapsuleBoundaryVolume & srcCapVol, const Float3 & srcVelocity, const RectBoundaryVolume & destRectVol, float startTime, float endTime, float & retHitTime)
{
	const float EPSILON = 0.005f;

	float mid = (startTime + endTime) * 0.5f;
	CapsuleBoundaryVolume srcCapVol2 = srcCapVol;
	srcCapVol2.m_radius = (mid - startTime) * srcVelocity.Length() + srcCapVol.m_radius;
	float scale = srcCapVol2.m_radius / srcCapVol.m_radius;
	// ���S�_�����_�̌��̃J�v�Z�����쐬
	CapsuleBoundaryVolume srcBaseCapVol = srcCapVol;
	srcBaseCapVol.SetCenter(Float3(0.0f));
	// ���_�J�v�Z���ŃX�P�[�����O
	Float4x4 scaleMat;
	scaleMat.Scale(Float3(scale));
	// �e���_���X�P�[�����O
	srcCapVol2.m_bottomPos = srcBaseCapVol.m_bottomPos * scaleMat;
	srcCapVol2.m_topPos = srcBaseCapVol.m_topPos * scaleMat;
	// ���S���W���ړ�
	srcCapVol2.SetCenter(srcCapVol.GetCenter() + srcVelocity * mid);

	Float3 closestPos;
	if (!CAPSULE_RECT(srcCapVol2, destRectVol, closestPos))
		return false;

	// �\���ɍċA����������
	if (endTime - startTime < EPSILON)
	{
		retHitTime = startTime;
		return true;
	}

	// �O�����ԂōċA
	if (CollisionTestCapsuleRect(srcCapVol, srcVelocity, destRectVol, startTime, mid, retHitTime))
		return true;
	// �㔼���ԂōċA
	return CollisionTestCapsuleRect(srcCapVol, srcVelocity, destRectVol, mid, endTime, retHitTime);
}

bool BoundaryVolume::OBB_OBB(const OBBBoundaryVolume & obb1, const OBBBoundaryVolume & obb2)
{
	const float EPSILON = 1.175494e-37f;
	float R[3][3], AbsR[3][3];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			R[i][j] = Float3::Dot(obb1.m_rot[i], obb2.m_rot[j]);
			AbsR[i][j] = fabsf(R[i][j]) + EPSILON;
		}
	}
	Float3 t = obb2.m_center - obb1.m_center;
	t = Float3(Float3::Dot(t, obb1.m_rot[0]),
		Float3::Dot(t, obb1.m_rot[1]),
		Float3::Dot(t, obb1.m_rot[2]));
	//��L=A0, L=A1, L=A2����
	float ra, rb;
	for (int i = 0; i < 3; i++) {
		ra = obb1.m_size[i];
		rb = obb2.m_size[0] * AbsR[i][0] + obb2.m_size[1] * AbsR[i][1] + obb2.m_size[2] * AbsR[i][2];
		if (fabsf(t[i]) > ra + rb) {
			return false;
		}
	}
	//��L=B0, L=B1, L=B2����
	for (int i = 0; i < 3; i++) {
		ra = obb1.m_size[0] * AbsR[0][i] + obb1.m_size[1] * AbsR[1][i] + obb1.m_size[2] * AbsR[2][i];
		rb = obb2.m_size[i];
		if (fabsf(t[0] * R[0][i] + t[1] * R[1][i] + t[2] * R[2][i]) > ra + rb) {
			return false;
		}
	}
	//��L=A0 X B0����
	ra = obb1.m_size[1] * AbsR[2][0] + obb1.m_size[2] * AbsR[1][0];
	rb = obb2.m_size[1] * AbsR[0][2] + obb2.m_size[2] * AbsR[0][1];
	if (fabsf(t[2] * R[1][0] - t[1] * R[2][0]) > ra + rb) {
		return false;
	}
	//��L=A0 X B1����
	ra = obb1.m_size[1] * AbsR[2][1] + obb1.m_size[2] * AbsR[1][1];
	rb = obb2.m_size[0] * AbsR[0][2] + obb2.m_size[2] * AbsR[0][0];
	if (fabsf(t[2] * R[1][1] - t[1] * R[2][1]) > ra + rb) {
		return false;
	}
	//��L=A0 X B2����
	ra = obb1.m_size[1] * AbsR[2][2] + obb1.m_size[2] * AbsR[1][2];
	rb = obb2.m_size[0] * AbsR[0][1] + obb2.m_size[1] * AbsR[0][0];
	if (fabsf(t[2] * R[1][2] - t[1] * R[2][2]) > ra + rb) {
		return false;
	}
	//��L=A1 X B0����
	ra = obb1.m_size[0] * AbsR[2][0] + obb1.m_size[2] * AbsR[0][0];
	rb = obb2.m_size[1] * AbsR[1][2] + obb2.m_size[2] * AbsR[1][1];
	if (fabsf(t[0] * R[2][0] - t[2] * R[0][0]) > ra + rb) {
		return false;
	}
	//��L=A1 X B1����
	ra = obb1.m_size[0] * AbsR[2][1] + obb1.m_size[2] * AbsR[0][1];
	rb = obb2.m_size[0] * AbsR[1][2] + obb2.m_size[2] * AbsR[1][0];
	if (fabsf(t[0] * R[2][1] - t[2] * R[0][1]) > ra + rb) {
		return false;
	}
	//��L=A1 X B2����
	ra = obb1.m_size[0] * AbsR[2][2] + obb1.m_size[2] * AbsR[0][2];
	rb = obb2.m_size[0] * AbsR[1][1] + obb2.m_size[1] * AbsR[1][0];
	if (fabsf(t[0] * R[2][2] - t[2] * R[0][2]) > ra + rb) {
		return false;
	}
	//��L=A2 X B0����
	ra = obb1.m_size[0] * AbsR[1][0] + obb1.m_size[1] * AbsR[0][0];
	rb = obb2.m_size[1] * AbsR[2][2] + obb2.m_size[2] * AbsR[2][1];
	if (fabsf(t[1] * R[0][0] - t[0] * R[1][0]) > ra + rb) {
		return false;
	}
	//��L=A2 X B1����
	ra = obb1.m_size[0] * AbsR[1][1] + obb1.m_size[1] * AbsR[0][1];
	rb = obb2.m_size[0] * AbsR[2][2] + obb2.m_size[2] * AbsR[2][0];
	if (fabsf(t[1] * R[0][1] - t[0] * R[1][1]) > ra + rb) {
		return false;
	}
	//��L=A2 X B2����
	ra = obb1.m_size[0] * AbsR[1][2] + obb1.m_size[1] * AbsR[0][2];
	rb = obb2.m_size[0] * AbsR[2][1] + obb2.m_size[1] * AbsR[2][0];
	if (fabsf(t[1] * R[0][2] - t[0] * R[1][2]) > ra + rb) {
		return false;
	}
	return true;
}

bool BoundaryVolume::CollisionTestOBBOBB(const OBBBoundaryVolume & srcOBB, const Float3 & srcVelocity, const OBBBoundaryVolume & destOBB, float startTime, float endTime, float & retHitTime)
{
	const float EPSILON = 0.005f;
	// �܂�OBB�����ɒu�������Ĕ��肷��(��������̑O�Ɍy��������s��
	SphereBoundaryVolume beforeSrcSphere;
	beforeSrcSphere.m_center = srcOBB.m_center;
	beforeSrcSphere.m_radius = srcOBB.m_size.Length();
	if (!CollisionTestSphereOBB(beforeSrcSphere, srcVelocity, destOBB, startTime, endTime, retHitTime))
	{
		// ���ƏՓ˂��Ȃ����OBB�ł��Փ˂��Ȃ�
		return false;
	}
	// OBB��OBB�Ő����ɔ��肷��
	return CollisionTestOBBOBBsub(srcOBB, srcVelocity, destOBB, startTime, endTime, retHitTime);
}

bool BoundaryVolume::CollisionTestOBBOBBsub(const OBBBoundaryVolume & srcOBB, const Float3 & srcVelocity, const OBBBoundaryVolume & destOBB, float startTime, float endTime, float & retHitTime)
{
	const float EPSILON = 0.005f;
	// �ړ��̊Ԃ��Ԃ���OBB
	OBBBoundaryVolume srcOBB2 = srcOBB;
	float mid = (startTime + endTime) * 0.5f;
	srcOBB2.m_center = srcOBB.m_center + srcVelocity * mid;
	// OBB�̊e�ӂ̒������g��
	const float calcTmp = (mid - startTime) * srcVelocity.Length();
	srcOBB2.m_size.x = calcTmp + srcOBB.m_size.x;
	srcOBB2.m_size.y = calcTmp + srcOBB.m_size.y;
	srcOBB2.m_size.z = calcTmp + srcOBB.m_size.z;
	if (!OBB_OBB(srcOBB2, destOBB))
	{
		return false;
	}
	if (endTime - startTime < EPSILON)	// �\���ɕ�Ԃ����ΏI��
	{
		retHitTime = startTime;	// �������Փˎ��ԂƂ��ē���
		return true;
	}

	// �O�����Ԃŕ�ԍċA
	if (CollisionTestOBBOBBsub(srcOBB, srcVelocity, destOBB, startTime, mid, retHitTime))
	{
		return true;
	}
	// �㔼���Ԃŕ�ԍċA
	return CollisionTestOBBOBBsub(srcOBB, srcVelocity, destOBB, mid, endTime, retHitTime);
}

bool BoundaryVolume::SPHERE_RECT(const SphereBoundaryVolume & sphere, const RectBoundaryVolume & rect, Float3 & retClosestPos)
{
	// ���Ƌ�`�̍Őڋߓ_���擾
	ClosestPtPointRect(sphere.m_center, rect, retClosestPos);
	if ((sphere.m_center - retClosestPos).Length() < sphere.m_radius)
		return true;
	return false;
}

bool BoundaryVolume::CollisionTestSphereRect(const SphereBoundaryVolume & srcSphere, const Float3 & srcVelocity, const RectBoundaryVolume & destRect, float startTime, float endTime, float & retHitTime)
{
	const float EPSILON = 0.005f;
	SphereBoundaryVolume srcSphere2;
	float mid = (startTime + endTime) * 0.5f;	// ���Ԏ���
	srcSphere2.m_center = srcSphere.m_center + srcVelocity * mid;
	srcSphere2.m_radius = (mid - startTime) * srcVelocity.Length() + srcSphere.m_radius;
	Float3 retVec;
	if (!SPHERE_RECT(srcSphere2, destRect, retVec))
		return false;
	// �\���ċA������������
	if (endTime - startTime < EPSILON)
	{
		retHitTime = startTime;
		return true;
	}
	// �O�����ԂōċA
	if (CollisionTestSphereRect(srcSphere, srcVelocity, destRect, startTime, mid, retHitTime))
		return true;
	// �㔼���ԂōċA
	return CollisionTestSphereRect(srcSphere, srcVelocity, destRect, mid, endTime, retHitTime);
}

bool BoundaryVolume::OBB_PLANE(const OBBBoundaryVolume & obb, const PlaneBoundaryVolume & plane)
{
	float r = obb.m_size.x * abs(Float3::Dot(plane.m_normal, obb.m_rot[0]))
		+ obb.m_size.y * abs(Float3::Dot(plane.m_normal, obb.m_rot[1]))
		+ obb.m_size.z * abs(Float3::Dot(plane.m_normal, obb.m_rot[2]));
	float s = Float3::Dot(plane.m_normal, obb.m_center) - plane.m_dotValue;
	return abs(s) <= r;	
}

bool BoundaryVolume::OBB_RECT(const OBBBoundaryVolume & obb, const RectBoundaryVolume & rect)
{
	if (OBB_PLANE(obb, rect.GetPlaneVolume()))
	{
		// ���ʂƌ������Ă����ꍇ��OBB�Ɣ���
		OBBBoundaryVolume obb2(Float3(rect.m_baseXSize, rect.m_baseYSize, 1.0f), rect.m_matrix);
		return OBB_OBB(obb, obb2);
	}
	return false;
}

bool BoundaryVolume::CollisionTestOBBRect(const OBBBoundaryVolume & srcOBB, const Float3 & srcVelocity, const RectBoundaryVolume & destRect, float startTime, float endTime, float & hitTime)
{
	const float EPSILON = 0.005f;
	OBBBoundaryVolume srcOBB2;
	float mid = (startTime + endTime) * 0.5f;
	srcOBB2.m_center = srcOBB.m_center + srcVelocity * mid;
	// OBB�̊e�ӂ̒������g��
	float calcTemp = (mid - startTime) * srcVelocity.Length();
	srcOBB2.m_size.x = calcTemp + srcOBB.m_size.x;
	srcOBB2.m_size.y = calcTemp + srcOBB.m_size.y;
	srcOBB2.m_size.z = calcTemp + srcOBB.m_size.z;

	if (!OBB_RECT(srcOBB2, destRect))
		return false;

	if (endTime - startTime < EPSILON)
	{
		hitTime = startTime;
		return true;
	}
	if (CollisionTestOBBRect(srcOBB, srcVelocity, destRect, startTime, mid, hitTime))
		return true;
	return CollisionTestOBBRect(srcOBB, srcVelocity, destRect, mid, endTime, hitTime);
}

void BoundaryVolume::ClosestPtPointOBB(const Float3 & point, const OBBBoundaryVolume & obb, Float3 & retClosestPos)
{
	Float3 d = point - obb.m_center;
	retClosestPos = obb.m_center;
	float dist;
	for (int i = 0; i < 3; i++)
	{
		dist = Float3::Dot(d, obb.m_rot[i]);
		if (dist > obb.m_size[i])
		{
			dist = obb.m_size[i];
		}
		if (dist < -obb.m_size[i])
		{
			dist = -obb.m_size[i];
		}
		retClosestPos += obb.m_rot[i] * dist;
	}
}

void BoundaryVolume::ClosestPtPointSegment(const Float3 & c, const Float3 & a, const Float3 & b, float & retT, Float3 & retClosestPos)
{
	Float3 ab = b - a;
	retT = Float3::Dot(c - a, ab) / Float3::Dot(ab, ab);
	if (retT < 0.0f)
		retT = 0.0f;
	if (retT > 1.0f)
		retT = 1.0f;
	retClosestPos = a + (ab * retT);
}

Float3 BoundaryVolume::ClosestPtCapsuleOBB(const CapsuleBoundaryVolume & capsule, const OBBBoundaryVolume & obb, int & retFlg)
{
	SphereBoundaryVolume sphereVol;
	sphereVol.m_center = capsule.m_bottomPos;
	sphereVol.m_radius = capsule.m_radius;
	Float3 closestPos;
	// �X�^�[�g�ʒu�ł̍Őڋߓ_
	Sphere_OBB(sphereVol, obb, closestPos);

	Float3 base = capsule.m_topPos - capsule.m_bottomPos;
	base.Normalize();
	Float3 dest = closestPos - capsule.m_bottomPos;
	float dot = Float3::Dot(base, dest);
	if (dot < 0)
	{
		// �X�^�[�g�ʒu�̋��̂̊O��	(�Őڋߓ_�͗L��
		retFlg = -1;	
		return closestPos;
	}
	float size = (capsule.m_topPos - capsule.m_bottomPos).Length();
	if (dot > size)
	{
		// �I�_����ɂ���
		sphereVol.m_center = capsule.m_topPos;
		Sphere_OBB(sphereVol, obb, closestPos);
		// �I�_�ōŐڋߓ_���Ƃ�
		retFlg = 1;
		return closestPos;
	}
	// ���S��OBB�̍Őڋߓ_���擾
	ClosestPtPointOBB(capsule.GetCenter(), obb, closestPos);
	float t;
	Float3 segPoint;
	ClosestPtPointSegment(closestPos, capsule.m_bottomPos, capsule.m_topPos, t, segPoint);
	Float3 span = closestPos - segPoint;
	span.Normalize();
	span *= capsule.m_radius;
	segPoint += span;
	closestPos = segPoint;
	retFlg = 0;
	return closestPos;
}

void BoundaryVolume::ClosestPtPointRect(const Float3 & point, const RectBoundaryVolume & rect, Float3 & retClosestPos)
{
	Float3 d = point - rect.m_center;
	retClosestPos = rect.m_center;
	for (int i = 0; i < 2; ++i)
	{
		float dist = Float3::Dot(d, rect.m_rot[i]);
		if (dist > rect.m_UVec[i])
			dist = rect.m_UVec[i];
		if (dist < -rect.m_UVec[i])
			dist =- rect.m_UVec[i];
		retClosestPos += rect.m_rot[i] * dist;
	}
}

float BoundaryVolume::ClosestPtSegmentSegment(const Float3& p1, const Float3& q1, const Float3& p2, const Float3& q2, float& s, float& t, Float3& c1, Float3& c2)
{
	// HACK : ����s��
	const float EPSILON = 1.175494e-37f;
	Float3 d1 = q1 - p1;
	Float3 d2 = q2 - p2;
	Float3 r = p1 - p2;
	float a = Float3::Dot(d1, d1);
	float e = Float3::Dot(d2, d2);
	float f = Float3::Dot(d2, r);

	if (f <= EPSILON && e <= EPSILON)
	{
		s = t = 0.0f;
		c1 = p1;
		c2 = p2;
		return Float3::Dot(c1 - c2, c1 - c2);
	}
	if (a <= EPSILON)
	{
		s = 0.0f;
		t = f / e;
		t = Clamp(t, 0.0f, 1.0f);
	}
	else
	{
		float c = Float3::Dot(d1, r);
		if (e <= EPSILON)
		{
			t = 0.0f;
			s = Clamp(-c / a, 0.0f, 1.0f);
		}
		else
		{
			float b = Float3::Dot(d1, d2);
			float denom = a * e - b * b;
			if (denom != 0.0f)
			{
				s = Clamp((b * f - c * e) / denom, 0.0f, 1.0f);
			}
			else
			{
				s = 0.0f;
			}
			t = (b * s + f) / e;
			if (t < 0.0f)
			{
				t = 0.0f;
				s = Clamp(-c / a, 0.0f, 1.0f);
			}
			else if (t > 1.0f)
			{
				t = 1.0f;
				s = Clamp((b - c) / a, 0.0f, 1.0f);
			}
		}
	}
	c1 = p1 + d1 * s;
	c2 = p2 + d2 * t;
	return Float3::Dot(c1 - c2, c1 - c2);
}

float BoundaryVolume::SqrtDistPointSegment(const Float3 & a, const Float3 & b, const Float3 & c)
{
	Float3 ab = b - a;
	Float3 ac = c - a;
	Float3 bc = c - a;
	float e = Float3::Dot(ac, ab);
	if (e <= 0.0f)
		return Float3::Dot(ac, ac);
	float f = Float3::Dot(ab, ab);
	if (e >= f)
		return Float3::Dot(bc, bc);
	return Float3::Dot(ac, ac) - e * e / f;
}
