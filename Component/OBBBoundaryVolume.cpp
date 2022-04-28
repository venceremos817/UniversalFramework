#include "OBBBoundaryVolume.h"

OBBBoundaryVolume::OBBBoundaryVolume()
{
	Float3 sizeVec(1.0f, 1.0f, 1.0f);
	Float4x4 mat(DirectX::XMMatrixIdentity());
	Create(sizeVec, mat);
}

OBBBoundaryVolume::OBBBoundaryVolume(const Float3 & size, const Float3 & rot, Float3 & center)
	: m_center(center)
	, m_size(size)
{
	m_size *= 0.5f;
	// 回転を得る
	Float3 localRot = rot;
	localRot.Normalize();
	Float4x4 rotM(DirectX::XMMatrixRotationRollPitchYawFromVector(localRot));
	m_rot[0] = Float3(rotM._11, rotM._12, rotM._13);
	m_rot[1] = Float3(rotM._21, rotM._22, rotM._23);
	m_rot[2] = Float3(rotM._31, rotM._32, rotM._33);
}

OBBBoundaryVolume::OBBBoundaryVolume(const Float3 & size, const Quaternion & quat, const Float3 & center)
	: m_center(center)
	, m_size(size)
{
	m_size *= 0.5f;
	// 回転を得る
	Quaternion localQuat = quat;
	// クォータニオンを正規化
	localQuat.Normalize();
	// クォータニオンから回転行列を作成
	Float4x4 rotM(DirectX::XMMatrixRotationQuaternion(quat));
	//DirectX::XMFLOAT4X4 rotM=Transform::GetRot4x4()
	m_rot[0] =Float3(rotM._11, rotM._12, rotM._13);
	m_rot[1] =Float3(rotM._21, rotM._22, rotM._23);
	m_rot[2] =Float3(rotM._31, rotM._32, rotM._33);
}

OBBBoundaryVolume::OBBBoundaryVolume(const Float3 & size, const Float4x4 matrix)
{
	m_center.x = matrix._41;
	m_center.y = matrix._42;
	m_center.z = matrix._43;
	Float3 vecX(matrix._11, matrix._12, matrix._13);
	Float3 vecY(matrix._21, matrix._22, matrix._23);
	Float3 vecZ(matrix._31, matrix._32, matrix._33);
	m_size = Float3(
		size.x * vecX.Length(),
		size.y * vecY.Length(),
		size.z * vecZ.Length()
	);
	m_size *= 0.5f;
	// 回転を得る
	m_rot[0] = vecX.Normalize();
	m_rot[1] = vecY.Normalize();
	m_rot[2] = vecZ.Normalize();
}

SphereBoundaryVolume OBBBoundaryVolume::GetWrappedSphereVolume()
{
	SphereBoundaryVolume retSphereVol;
	retSphereVol.m_radius = m_size.Length();
	retSphereVol.m_center = m_center;
	return retSphereVol;
}

void OBBBoundaryVolume::Create(const Float3 & size, const Float4x4 & matrix)
{
	m_center.x = matrix._41;
	m_center.y = matrix._42;
	m_center.z = matrix._43;
	Float3 VecX(matrix._11, matrix._12, matrix._13);
	Float3 VecY(matrix._21, matrix._22, matrix._23);
	Float3 VecZ(matrix._31, matrix._32, matrix._33);
	m_size = Float3(
		size.x * VecX.Length(),
		size.y * VecY.Length(),
		size.z * VecZ.Length()
	);
	m_size *= 0.5f;
	//回転を得る
	m_rot[0] = VecX.Normalize();
	m_rot[1] = VecY.Normalize();
	m_rot[2] = VecZ.Normalize();
}

Float4x4 OBBBoundaryVolume::GetRotMatrix() const
{
	Float4x4 ret;
	ret._11 = m_rot[0].x;
	ret._12 = m_rot[0].y;
	ret._13 = m_rot[0].z;
	ret._21 = m_rot[1].x;
	ret._22 = m_rot[1].y;
	ret._23 = m_rot[1].z;
	ret._31 = m_rot[2].x;
	ret._32 = m_rot[2].y;
	ret._33 = m_rot[2].z;
	return ret;
}

Float4x4 OBBBoundaryVolume::GetWorldMatrix() const
{
	Float4x4 retM, posM;
	retM = DirectX::XMMatrixScaling(m_size.x * 2.0f, m_size.y * 2.0f, m_size.z * 2.0f);
	retM = DirectX::XMMatrixMultiply(retM, GetRotMatrix());
	posM = DirectX::XMMatrixTranslation(m_center.x, m_center.y, m_center.z);
	retM = DirectX::XMMatrixMultiply(retM, posM);

	return retM;
}

float OBBBoundaryVolume::GetMaxSize() const
{
	return GetHalfMaxSize() * 2.0f;
}

float OBBBoundaryVolume::GetHalfMaxSize() const
{
	float ret = m_size.x;
	if (ret < m_size.y)
		ret = m_size.y;
	if (ret < m_size.z)
		ret = m_size.z;
	return ret;
}

float OBBBoundaryVolume::GetMinSize() const
{
	return GetHalfMinSize() * 2.0f;
}

float OBBBoundaryVolume::GetHalfMinSize() const
{
	float ret = m_size.x;
	if (ret > m_size.y)
		ret = m_size.y;
	if (ret > m_size.z)
		ret = m_size.z;
	return ret;
}
