#include "Transform.h"
#include <XMFLOAT_EX.h>
#include <App.h>

struct Transform::Param
{
	// 再計算が必要かどうか判定するようフラグ
	bool bInit = false;
	bool isChanged;
	bool isPreChanged;

	// ワールドマトリクス
	Float4x4	preWorldM;
	Float4x4	worldM;
	// スケーリング
	Float3	preScale;
	Float3	scale;
	// 重心
	Float3	prePivod;
	Float3	pivod;
	// 回転
	Quaternion	preQuat;
	Quaternion	quat;
	// 座標
	Float3	prePos;
	Float3	pos;


	Param()
		: scale(1.0f, 1.0f, 1.0f)
		, preScale(1.0f, 1.0f, 1.0f)
		, isChanged(true)
		, isPreChanged(true)
	{}
};

Transform::Transform(const std::shared_ptr<GameObject>& pGameObject)
	: Component(pGameObject)
	, m_param(new Param)
{
	SetProcessingOrder(ProcessingOrder::ComponentProcessingOrder::Transform);
}

Transform::~Transform()
{
}

void Transform::Update()
{
	if (!m_param->bInit)
	{
		SetToBefore();
		m_param->bInit = true;
	}
}

Float3 Transform::GetPreScale() const
{
	return m_param->preScale;
}

Float3 Transform::GetScale() const
{
	return m_param->scale;
}

void Transform::SetScale(const Float3 & scale)
{
	m_param->isChanged = true;
	m_param->scale = scale;
}

void Transform::SetScale(float x, float y, float z)
{
	SetScale(Float3(x, y, z));
}

Float3 Transform::GetPrePivod() const
{
	return m_param->prePivod;
}

Float3 Transform::GetPivod() const
{
	return m_param->pivod;
}

void Transform::SetPivod(const Float3 & pivot)
{
	m_param->isChanged = true;
	m_param->pivod = pivot;
}

void Transform::SetPivot(float x, float y, float z)
{
	SetPivod(Float3(x, y, z));
}

Quaternion Transform::GetPreQuat() const
{
	return m_param->preQuat;
}

Quaternion Transform::GetQuaternion() const
{
	return m_param->quat;
}

void Transform::SetQuaternion(const Quaternion & quat)
{
	m_param->isChanged = true;
	m_param->quat = quat;
	m_param->quat.Normalize();
}

Float3 Transform::GetPreRotation() const
{
	return m_param->preQuat.ToRotVec();
}

Float3 Transform::GetRotation() const
{
	return m_param->quat.ToRotVec();
}

void Transform::SetRotation(const Float3 & rot)
{
	//m_param->isChanged = true;
	Quaternion quat;
	quat.RotationRollPitchYawFromVector(rot);
	SetQuaternion(quat);
}

void Transform::SetRotation(float x, float y, float z)
{
	SetRotation(Float3(x, y, z));
}

Float3 Transform::GetPrePos() const
{
	return m_param->prePos;
}

Float3 Transform::GetPos() const
{
	return m_param->pos;
}

void Transform::SetPos(const Float3 & pos)
{
	m_param->isChanged = true;
	m_param->pos = pos;
}

void Transform::SetPos(float x, float y, float z)
{
	SetPos(Float3(x, y, z));
}

void Transform::ResetPos(const Float3 & pos)
{
	m_param->isChanged = true;
	m_param->pos = pos;
	m_param->isPreChanged = true;
	m_param->prePos = pos;
}

Float3 Transform::GetWorldPos()
{
	// ワールドマトリクスから平行移動を抽出して返す
	Float3 pos = GetWorldMatrix().ExtractTransform();
	return pos;
}

void Transform::SetWorldPos(const Float3 & pos)
{
	Float3 setPos = pos;
	SetPos(setPos);
}

void Transform::SetWorldPos(float x, float y, float z)
{
	SetWorldPos(Float3(x, y, z));
}

void Transform::ResetWorldPos(const Float3 & pos)
{
	Float3 setPos = pos;
	ResetPos(setPos);
}

Float4x4 Transform::GetPreWolrdMatrix()
{
	if (m_param->isPreChanged )
	{
		m_param->preWorldM.AffineTransformation(
			m_param->preScale,
			m_param->prePivod,
			m_param->preQuat,
			m_param->prePos
		);
		m_param->isPreChanged = false;
	}
	return m_param->preWorldM;
}

const Float4x4 & Transform::GetWorldMatrix()
{
	// パラメータに変更があれば行列作り直し
	if (m_param->isChanged)
	{
		m_param->worldM.AffineTransformation(m_param->scale, m_param->pivod, m_param->quat, m_param->pos);
		m_param->isChanged = false;
	}
	return m_param->worldM;
}

const Float4x4 & Transform::Get2DWorldMatrix()
{
	// パラメータに変更があれば行列作り直し
	if (m_param->isChanged)
	{
		m_param->scale.z = 1.0f;
		Float4 tempZ(m_param->pos.z);
		tempZ = DirectX::XMVector4ClampLength(tempZ, 0.0f, 1.0f);
		m_param->pos.z = tempZ.z;
		m_param->pivod.z = 0.0f;
		m_param->worldM.AffineTransformation(
			m_param->scale,
			m_param->pivod,
			m_param->quat,
			m_param->pos
		);
		m_param->isChanged = false;
	}
	return m_param->worldM;
}

Float3 Transform::GetVelocity() const
{
	float elapsedTime = APP->GetElapsedTime();
	Float3 velocity = m_param->pos - m_param->prePos;
	velocity /= elapsedTime;
	return velocity;
}

void Transform::SetToBefore()
{
	if (m_param->preScale != m_param->scale)
	{
		m_param->isPreChanged = true;
		m_param->preScale = m_param->scale;
	}
	if (m_param->prePivod != m_param->pivod)
	{
		m_param->isPreChanged = true;
		m_param->prePivod = m_param->pivod;
	}
	if (m_param->preQuat != m_param->quat)
	{
		m_param->isPreChanged = true;
		m_param->preQuat = m_param->quat;
	}
	if (m_param->prePos != m_param->pos)
	{
		m_param->isPreChanged = true;
		m_param->prePos = m_param->pos;
	}
}

void Transform::LookAt(const Float3 & lookPos)
{
	Float3 Z = lookPos - GetWorldPos();	Z.Normalize();
	Float3 Y = GetUp();
	Float3 X = Float3::Cross(Y, Z);	X.Normalize();
	Y = Float3::Cross(Z, X);		Y.Normalize();

	Float4x4 rotM;
	rotM._11 = X.x; rotM._12 = X.y; rotM._13 = X.z; rotM._14 = 0.0f;
	rotM._21 = Y.x; rotM._22 = Y.y; rotM._23 = Y.z; rotM._24 = 0.0f;
	rotM._31 = Z.x; rotM._32 = Z.y; rotM._33 = Z.z; rotM._34 = 0.0f;
	rotM._41 = 0.0f; rotM._42 = 0.0f; rotM._43 = 0.0f; rotM._44 = 1.0f;

	SetQuaternion(rotM.ExtractQuaternion());
}

Float3 Transform::GetForward()
{
	Float3 ret = GetWorldMatrix().ExtractRotZ();
	ret.Normalize();
	return ret;
}

Float3 Transform::GetUp()
{
	Float3 ret = GetWorldMatrix().ExtractRotY();
	ret.Normalize();
	return ret;
}

Float3 Transform::GetRight()
{
	Float3 ret = GetWorldMatrix().ExtractRotZ();
	ret.Normalize();
	return ret;
}

bool Transform::IsSameWorldMatrix(const Float4x4 & mat)
{
	return GetWorldMatrix().equalInt(mat);
}

bool Transform::IsSameBeforeWorldMatrix(const Float4x4 & mat)
{
	return GetPreWolrdMatrix().equalInt(mat);
}
