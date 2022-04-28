#pragma once

#include <DirectXMath.h>
#include <cmath>

struct Quaternion;
struct Float3;
struct Float4x4;

#pragma region XMFLOAT4
//------------------------------------------------------------------
//	XMFLOAT4
//------------------------------------------------------------------

struct Float4
	: public DirectX::XMFLOAT4
{
	inline Float4();
	inline Float4(float x, float y, float z, float w);
	inline Float4(const DirectX::XMVECTOR& v);
	inline Float4(float val);

	//=== 演算子 ===
	inline Float4& operator =(const DirectX::XMFLOAT3& f3)
	{
		x = f3.x;
		y = f3.y;
		z = f3.z;
		w = 0.0f;
		return *this;
	}
	inline const Float4 operator +(const Float4& v)const
	{
		return (Float4)DirectX::XMVectorAdd(*this, v);
	}
	inline Float4& operator +=(const Float4& v)
	{
		*this = *this + v;
		return *this;
	}
	inline const Float4 operator *(const Float4& v)const
	{
		return (Float4)DirectX::XMVectorMultiply(*this, v);
	}

	//=== キャスト ===
	inline operator DirectX::XMVECTOR()const;

	//=== 要素を取得 ===
	inline float operator[](int idx)const;
};

#pragma endregion

#pragma region XMFLOAT3
//------------------------------------------------------------------
//	XMFLOAT3
//------------------------------------------------------------------

struct Float3
	: public DirectX::XMFLOAT3
{
	inline Float3();
	inline Float3(float x, float y, float z);
	explicit inline Float3(float s);
	explicit inline Float3(const DirectX::XMVECTOR& v);
	explicit inline Float3(const DirectX::XMFLOAT3& v);

	//=== 演算子 ===
	inline Float3& operator =(const DirectX::XMVECTOR& v);
	inline Float3& operator =(const Float3& v);
	inline const Float3 operator +(const Float3& v)const;
	inline Float3& operator +=(const Float3& v);
	inline const Float3 operator -(const Float3& v)const;
	inline Float3& operator -=(const Float3& v);
	inline const Float3 operator *(float f)const;
	inline const Float3 operator *(const Float4x4& mat)const;
	inline Float3& operator *=(const float f);
	inline Float3& operator *=(const Float4x4& mat);
	inline const Float3 operator /(const float f)const;
	inline const Float3& operator /=(const float f);
	inline bool operator==(const Float3& val)const;
	inline bool operator!=(const Float3& val)const;
	inline float operator [](int idx)const;

	//=== キャスト ===
	inline operator DirectX::XMVECTOR()const;


	/// <summary>
	/// 正規化する
	/// </summary>
	/// <returns></returns>
	inline Float3& Normalize();
	_NODISCARD inline Float3 normalized()const;
	/// <summary>
	/// 長さを取得
	/// </summary>
	/// <returns></returns>
	inline float Length()const;

	/// <summary>
	/// 内積を求める
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	static inline float Dot(const Float3& v1, const Float3& v2);
	/// <summary>
	/// 外積を求める
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	static inline Float3 Cross(const Float3& v1, const Float3& v2);
	/// <summary>
	/// 2点間の距離を求める
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	static inline float Distance(const Float3& v1, const Float3& v2);
};

#pragma endregion

#pragma region XMFLOAT2
//------------------------------------------------------------------
//	XMFLOAT2
//------------------------------------------------------------------
struct Float2
	: public DirectX::XMFLOAT2
{
	inline Float2();
	inline Float2(float x, float y);
	explicit inline Float2(float s);
	explicit inline Float2(const DirectX::XMVECTOR& v);
	explicit inline Float2(const DirectX::XMFLOAT2& v);

	//=== 演算子 ===
	inline Float2& operator =(const DirectX::XMVECTOR& v);
	inline Float2& operator =(const Float2& v);
	inline const Float2 operator +(const Float2& v)const;
	inline Float2& operator +=(const Float2& v);
	inline const Float2& operator -(const Float2& v)const;
	inline Float2& operator -=(const Float2& v);
	inline const Float2 operator *(float s)const;
	inline Float2& operator *=(float s);
	inline const Float2 operator /(float s)const;
	inline const Float2& operator /=(float s);
	inline float operator [](int idx)const;

	//=== キャスト ===
	inline operator DirectX::XMVECTOR()const;

	/// <summary>
	/// 正規化する
	/// </summary>
	/// <returns></returns>
	inline Float2& Normalize();

	inline float Length()const;

	/// <summary>
	/// 内積を求める
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	static inline float Dot(const Float2& v1, const Float2& v2);

	/// <summary>
	/// 外積を求める
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	static inline Float2 Cross(const Float2& v1, const Float2& v2);
};

#pragma endregion

#pragma region Quaternion
struct Quaternion
	: public DirectX::XMFLOAT4
{
public:
	//=== コンストラクタ ===
	inline Quaternion();
	inline Quaternion(const Quaternion& qt);
	inline Quaternion(const DirectX::XMFLOAT4& v);
	inline Quaternion(const Float4& q);
	inline Quaternion(const Float3& v, float rad);
	inline Quaternion(float _x, float _y, float _z, float _w);
	explicit inline Quaternion(const DirectX::XMVECTOR& v);
	explicit inline Quaternion(float val);
	virtual ~Quaternion() {}


	//=== 演算子 ===
	inline Quaternion& operator =(const Quaternion& q);
	inline Quaternion& operator =(const DirectX::XMVECTOR& v);
	inline const Quaternion operator *(const Quaternion& q)const;
	inline Quaternion& operator *=(const Quaternion& q);
	inline bool operator ==(const Quaternion& q)const;
	inline bool operator !=(const Quaternion& q)const;


	//=== キャスト ===
	inline operator DirectX::XMVECTOR()const;


	/// <summary>
	/// 単位クォータニオンを設定
	/// </summary>
	/// <returns></returns>
	inline Quaternion& Identity();
	/// <summary>
	/// 逆クォータニオンを設定
	/// </summary>
	/// <returns></returns>
	inline Quaternion& Inverse();
	/// <summary>
	/// 正規化して設定
	/// </summary>
	/// <returns></returns>
	inline Quaternion& Normalize();

	/// <summary>
	/// 指定の軸回転のクォータニオンを設定
	/// </summary>
	/// <param name="radian">回転量</param>
	/// <param name="unitVec">回転軸</param>
	/// <returns></returns>
	inline Quaternion& Rotaion(float radian, const Float3& unitVec);

	/// <summary>
	/// 回転ベクトルからクォータニオンを設定
	/// </summary>
	/// <param name="rotV"></param>
	/// <returns></returns>
	inline Quaternion& RotationRollPitchYawFromVector(const Float3& rotV);



	/// <summary>
	/// 回転ベクトルに変換して取得
	/// </summary>
	/// <returns></returns>
	inline const Float3 ToRotVec()const;
};
#pragma endregion

#pragma region XMFLOAT4X4
//------------------------------------------------------------------
//	XMFLOAT4X4
//------------------------------------------------------------------  

struct Float4x4
	: public DirectX::XMFLOAT4X4
{
	inline Float4x4();
	inline Float4x4(const Float4x4& mat);
	explicit inline Float4x4(const Quaternion& quat);
	explicit inline Float4x4(const DirectX::XMMATRIX& mat);

	//=== 演算子 ===
	inline Float4x4& operator =(const DirectX::XMMATRIX& mat);
	inline Float4x4& operator =(const DirectX::XMFLOAT4X4& mat);
	inline Float4x4& operator =(const Float4x4& mat);
	inline const Float3 operator *(const Float3& v)const;
	inline const Float4x4 operator *(const Float4x4& mat)const;
	inline Float4x4& operator *=(const Float4x4& mat);
	inline const Float4 operator [](int major)const;


	/// <summary>
	/// 指定のmajorの取得
	/// </summary>
	/// <param name="major"></param>
	/// <returns></returns>
	inline const Float4 GetMajor(int major)const;



	/// <summary>
	/// 同値チェック(UINTで比較)
	/// </summary>
	/// <param name="dest"></param>
	/// <returns></returns>
	inline bool equalInt(const Float4x4& dest)const;

	//=== キャスト ===
	inline operator DirectX::XMMATRIX() const;



	/// <summary>
	/// 単位行列を設定
	/// </summary>
	inline Float4x4& Identity();

	/// <summary>
	/// 行列のスケーリングを1.0に設定
	/// </summary>
	/// <returns></returns>
	inline Float4x4& ScaleIdentity();


	/// <summary>
	/// 行列から各値を抽出する
	/// </summary>
	inline void	 Decompose(Float3& rScale, Quaternion& rQt, Float3& rPos)const;
	/// <summary>
	/// 行列から平行移動を抽出
	/// </summary>
	/// <returns></returns>
	inline Float3 ExtractTransform()const;
	/// <summary>
	/// 行列からスケーリングを抽出
	/// </summary>
	/// <returns></returns>
	inline Float3 ExtractScale()const;
	/// <summary>
	/// 行列からクォータニオンを抽出
	/// </summary>
	inline Quaternion ExtractQuaternion()const;


	/// <summary>
	/// X軸傾きを取得
	/// </summary>
	/// <returns></returns>
	inline Float3 ExtractRotX()const;
	/// <summary>
	/// Y軸傾きを取得
	/// </summary>
	/// <returns></returns>
	inline Float3 ExtractRotY()const;
	/// <summary>
	/// Z軸傾きを取得
	/// </summary>
	/// <returns></returns>
	inline Float3 ExtractRotZ()const;




	/// <summary>
	/// スケーリング行列を設定
	/// </summary>
	/// <param name="scaleVec"></param>
	/// <returns></returns>
	inline Float4x4& Scale(const Float3& scaleVec);


	/// <summary>
	/// アフィン変換を行う
	/// </summary>
	/// <param name="scale">スケーリング</param>
	/// <param name="rotOrigin">回転の中心</param>
	/// <param name="unitQuat">クォータニオン</param>
	/// <param name="translation">移動</param>
	/// <returns></returns>
	inline Float4x4& AffineTransformation(const Float3& scale, const Float3& rotOrigin, const Quaternion& unitQuat, const Float3& translation);

	/// <summary>
	/// str変換(Scale,Translation,Rotationの順で掛け算)
	/// </summary>
	/// <param name="scale">スケーリング</param>
	/// <param name="translation">移動</param>
	/// <param name="unitQuat">クォータニオン</param>
	/// <returns></returns>
	inline Float4x4& strTransformation(const Float3& scale, const Float3& translation, const Quaternion& unitQuat);


};
#pragma endregion


#pragma region XMVECTOR
//------------------------------------------------------------------
//	XMVECTOR
//------------------------------------------------------------------
inline DirectX::XMVECTOR XM_CALLCONV operator+(DirectX::FXMVECTOR v1, DirectX::FXMVECTOR v2)
{
	return DirectX::XMVectorAdd(v1, v2);
}

inline DirectX::XMVECTOR XM_CALLCONV operator-(DirectX::FXMVECTOR v1, DirectX::FXMVECTOR v2)
{
	return DirectX::XMVectorSubtract(v1, v2);
}

inline DirectX::XMVECTOR XM_CALLCONV operator*(DirectX::FXMVECTOR v1, DirectX::FXMVECTOR v2)
{
	return DirectX::XMVectorMultiply(v1, v2);
}

inline DirectX::XMVECTOR XM_CALLCONV operator*(DirectX::FXMVECTOR v, const float s)
{
	return DirectX::XMVectorScale(v, s);
}


#pragma endregion


//DirectX::XMFLOAT3 GetAngleBetweenPoints(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2);
#pragma region Float4

inline Float4::Float4()
	: DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)
{
}

inline Float4::Float4(float x, float y, float z, float w)
	: DirectX::XMFLOAT4(x, y, z, w)
{
}

inline Float4::Float4(const DirectX::XMVECTOR & v)
	: DirectX::XMFLOAT4()
{
	DirectX::XMStoreFloat4((DirectX::XMFLOAT4*)this, v);
}

inline Float4::Float4(float val)
	: DirectX::XMFLOAT4(val, val, val, val)
{
}

inline Float4::operator DirectX::XMVECTOR()const
{
	DirectX::XMVECTOR retV = DirectX::XMLoadFloat4(this);
	return retV;
}

inline float Float4::operator[](int idx) const
{
	return *(&x + idx);
}

#pragma endregion

#pragma region Float3

inline Float3::Float3()
	: DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)
{
}

inline Float3::Float3(float x, float y, float z)
	: DirectX::XMFLOAT3(x, y, z)
{
}

inline Float3::Float3(float s)
	: DirectX::XMFLOAT3(s, s, s)
{
}

inline Float3::Float3(const DirectX::XMVECTOR & v)
	: DirectX::XMFLOAT3()
{
	DirectX::XMStoreFloat3((DirectX::XMFLOAT3*)this, v);
}

inline Float3::Float3(const DirectX::XMFLOAT3 & v)
	: DirectX::XMFLOAT3(v)
{
}

inline Float3 & Float3::operator=(const DirectX::XMVECTOR & v)
{
	DirectX::XMStoreFloat3(this, v);
	return *this;
}

inline Float3 & Float3::operator=(const Float3 & v)
{
	if (this != &v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}
	return *this;
}

inline const Float3 Float3::operator+(const Float3 & v) const
{
	return (Float3)DirectX::XMVectorAdd(*this, v);
}

inline Float3 & Float3::operator+=(const Float3 & v)
{
	*this = *this + v;
	return *this;
}

inline const Float3 Float3::operator-(const Float3 & v) const
{
	return (Float3)DirectX::XMVectorSubtract(*this, v);
}

inline Float3 & Float3::operator-=(const Float3 & v)
{
	*this = *this - v;
	return *this;
}

inline const Float3 Float3::operator*(float f) const
{
	Float3 temp(f, f, f);
	return (Float3)DirectX::XMVectorMultiply(*this, temp);
}

inline const Float3 Float3::operator*(const Float4x4 & mat) const
{
	return (Float3)DirectX::XMVector3Transform(*this, mat);
}

inline Float3 & Float3::operator*=(const float f)
{
	*this = *this * f;
	return *this;
}

inline Float3 & Float3::operator*=(const Float4x4 & mat)
{
	*this = *this * mat;
	return *this;
}

inline const Float3 Float3::operator/(const float f)const
{
	Float3 temp(DirectX::XMVectorReciprocal(Float3(f, f, f)));
	return (Float3)DirectX::XMVectorMultiply(*this, temp);
}

inline const Float3 & Float3::operator/=(const float f)
{
	*this = *this / f;
	return *this;
}

inline bool Float3::operator==(const Float3 & val) const
{
	return DirectX::XMVector3Equal(*this, val);
}

inline bool Float3::operator!=(const Float3 & val) const
{
	return !DirectX::XMVector3Equal(*this, val);
}

inline float Float3::operator[](int idx) const
{
	return *(&x + idx);
}

inline Float3::operator DirectX::XMVECTOR() const
{
	DirectX::XMVECTOR retV = DirectX::XMLoadFloat3(this);
	return retV;
}

inline Float3 & Float3::Normalize()
{
	*this = (Float3)DirectX::XMVector3Normalize(*this);
	return *this;
}

inline Float3 Float3::normalized() const
{
	Float3 temp = (Float3)DirectX::XMVector3Normalize(*this);
	return temp;
}

inline float Float3::Length()const
{
	return ((Float3)DirectX::XMVector3Length(*this)).x;
}

float inline Float3::Dot(const Float3 & v1, const Float3 & v2)
{
	return ((Float3)DirectX::XMVector3Dot(v1, v2)).x;
}

Float3 inline Float3::Cross(const Float3 & v1, const Float3 & v2)
{
	return (Float3)DirectX::XMVector3Cross(v1, v2);
}

inline float Float3::Distance(const Float3& v1, const Float3& v2)
{
	Float3 temp = v1 - v2;
	return temp.Length();
}

#pragma endregion

#pragma region Float2
inline Float2::Float2()
	: DirectX::XMFLOAT2(0.0f, 0.0f)
{
}

inline Float2::Float2(float x, float y)
	: DirectX::XMFLOAT2(x, y)
{
}

inline Float2::Float2(float s)
	: DirectX::XMFLOAT2(s, s)
{
}

inline Float2::Float2(const DirectX::XMVECTOR& v)
	: DirectX::XMFLOAT2()
{
	DirectX::XMStoreFloat2((DirectX::XMFLOAT2*)this, v);
}

inline Float2::Float2(const DirectX::XMFLOAT2& v)
	: DirectX::XMFLOAT2(v)
{
}

//=== 演算子 ===
inline Float2& Float2::operator =(const DirectX::XMVECTOR& v)
{
	DirectX::XMStoreFloat2(this, v);
	return *this;
}

inline Float2& Float2::operator =(const Float2& v)
{
	if (this != &v)
	{
		x = v.x;
		y = v.y;
	}
	return *this;
}

inline const Float2 Float2::operator +(const Float2& v)const
{
	return (Float2)DirectX::XMVectorAdd(*this, v);
}

inline Float2& Float2::operator +=(const Float2& v)
{
	*this = *this + v;
	return *this;
}

inline const Float2& Float2::operator -(const Float2& v)const
{
	return (Float2)DirectX::XMVectorSubtract(*this, v);
}

inline Float2& Float2::operator -=(const Float2& v)
{
	*this = *this - v;
	return *this;
}

inline const Float2 Float2::operator *(float s)const
{
	Float2 temp(s, s);
	return (Float2)DirectX::XMVectorMultiply(*this, temp);
}

inline Float2& Float2::operator *=(float s)
{
	*this = *this * s;
	return *this;
}

inline const Float2 Float2::operator /(float s)const
{
	Float2 temp(DirectX::XMVectorReciprocal(Float2(s, s)));
	return (Float2)DirectX::XMVectorMultiply(*this, temp);
}

inline const Float2& Float2::operator /=(float s)
{
	*this = *this / s;
	return *this;
}

inline float Float2::operator [](int idx)const
{
	return *(&x + idx);
}

//=== キャスト ===
inline Float2::operator DirectX::XMVECTOR()const
{
	DirectX::XMVECTOR ret = DirectX::XMLoadFloat2(this);
	return ret;
}

inline Float2& Float2::Normalize()
{
	*this = (Float2)DirectX::XMVector2Normalize(*this);
	return *this;
}

inline float Float2::Length()const
{
	return ((Float2)DirectX::XMVector2Length(*this)).x;
}

inline float Float2::Dot(const Float2& v1, const Float2& v2)
{
	return ((Float2)DirectX::XMVector2Dot(v1, v2)).x;
}

inline Float2 Float2::Cross(const Float2& v1, const Float2& v2)
{
	return (Float2)DirectX::XMVector2Cross(v1, v2);
}
#pragma endregion


#pragma region Quaternion
inline Quaternion::Quaternion()
{
	Identity();
}

inline Quaternion::Quaternion(const Quaternion & qt)
	: DirectX::XMFLOAT4(qt.x, qt.y, qt.z, qt.w)
{
}

inline Quaternion::Quaternion(const DirectX::XMFLOAT4 & v)
	: DirectX::XMFLOAT4(v)
{
}

inline Quaternion::Quaternion(const DirectX::XMVECTOR& v)
	: DirectX::XMFLOAT4()
{
	DirectX::XMStoreFloat4(this, v);
}

inline Quaternion::Quaternion(const Float4& q)
	: DirectX::XMFLOAT4(q)
{
}

inline Quaternion::Quaternion(const Float3& v, float rad)
{
	*this = (Quaternion)DirectX::XMQuaternionRotationAxis(v, rad);
}

inline Quaternion::Quaternion(float _x, float _y, float _z, float _w)
	: DirectX::XMFLOAT4(_x, _y, _z, _w)
{
}

inline Quaternion::Quaternion(float val)
	: DirectX::XMFLOAT4(val, val, val, val)
{
}

inline Quaternion & Quaternion::operator=(const Quaternion & q)
{
	if (this != &q)
	{
		x = q.x;
		y = q.y;
		z = q.z;
		w = q.w;
	}
	return *this;
}

inline Quaternion & Quaternion::operator=(const DirectX::XMVECTOR & v)
{
	DirectX::XMStoreFloat4((DirectX::XMFLOAT4*)this, v);
	return *this;
}

inline const Quaternion Quaternion::operator*(const Quaternion & q) const
{
	return (Quaternion)DirectX::XMQuaternionMultiply(*this, q);
}

inline Quaternion & Quaternion::operator*=(const Quaternion & q)
{
	*this = *this * q;
	return *this;
}

inline bool Quaternion::operator==(const Quaternion & q) const
{
	return DirectX::XMQuaternionEqual(*this, q);
}

inline bool Quaternion::operator!=(const Quaternion & q) const
{
	return !DirectX::XMQuaternionEqual(*this, q);
}

inline Quaternion::operator DirectX::XMVECTOR()const
{
	DirectX::XMFLOAT4 temp = *this;
	DirectX::XMVECTOR v = DirectX::XMLoadFloat4(&temp);
	return v;
}

inline Quaternion & Quaternion::Identity()
{
	*this = (Quaternion)DirectX::XMQuaternionIdentity();
	return *this;
}

inline Quaternion& Quaternion::Inverse()
{
	*this = (Quaternion)DirectX::XMQuaternionInverse(*this);
	return *this;
}

inline Quaternion & Quaternion::Normalize()
{
	*this = (Quaternion)DirectX::XMQuaternionNormalize(*this);
	return *this;
}

inline Quaternion & Quaternion::Rotaion(float radian, const Float3 & unitVec)
{
	*this = (Quaternion)DirectX::XMQuaternionRotationAxis(unitVec, radian);
	return *this;
}

inline Quaternion & Quaternion::RotationRollPitchYawFromVector(const Float3 & rotV)
{
	*this = (Quaternion)DirectX::XMQuaternionRotationRollPitchYawFromVector(rotV);
	return *this;
}

inline const Float3 Quaternion::ToRotVec() const
{
	Quaternion temp = *this;
	temp.Normalize();
	Float4x4 mat(temp);
	Float3 rot;
	if (mat._32 == 1.0f)
	{
		rot.x = DirectX::XM_PIDIV2;
		rot.y = 0.0f;
		rot.z = -atan2(mat._21, mat._11);
	}
	else if (mat._32 == -1.0f)
	{
		rot.x = -DirectX::XM_PIDIV2;
		rot.y = 0.0f;
		rot.z = -atan2(mat._21, mat._11);
	}
	else
	{
		rot.x = -asin(mat._32);
		rot.y = -atan2(-mat._31, mat._33);
		rot.z = atan2(mat._12, mat._11);
	}
	return rot;
}


#pragma endregion

#pragma region Float4x4

inline Float4x4::Float4x4()
	: DirectX::XMFLOAT4X4()
{
	Identity();
}

inline Float4x4::Float4x4(const Float4x4 & mat)
	: DirectX::XMFLOAT4X4(mat)
{
}

inline Float4x4::Float4x4(const Quaternion & quat)
	: DirectX::XMFLOAT4X4()
{
	*this = (Float4x4)DirectX::XMMatrixRotationQuaternion(DirectX::XMVECTOR(quat));
}

inline Float4x4::Float4x4(const DirectX::XMMATRIX & mat)
	: DirectX::XMFLOAT4X4()
{
	DirectX::XMMATRIX tempM = mat;
	DirectX::XMStoreFloat4x4((DirectX::XMFLOAT4X4*)this, tempM);
}

inline Float4x4 & Float4x4::operator=(const DirectX::XMMATRIX & mat)
{
	DirectX::XMStoreFloat4x4((DirectX::XMFLOAT4X4*)this, mat);
	return *this;
}

inline Float4x4 & Float4x4::operator=(const DirectX::XMFLOAT4X4 & mat)
{
	(DirectX::XMFLOAT4X4)*this = mat;
	return *this;
}

inline Float4x4 & Float4x4::operator=(const Float4x4 & mat)
{
	if (this != &mat)
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				m[i][j] = mat[i][j];

	return *this;
}

inline const Float3 Float4x4::operator*(const Float3 & v) const
{
	return (Float3)DirectX::XMVector3Transform(v, *this);
}

inline const Float4x4 Float4x4::operator*(const Float4x4 & mat) const
{
	return (Float4x4)DirectX::XMMatrixMultiply(*this, mat);
}

inline Float4x4 & Float4x4::operator*=(const Float4x4 & mat)
{
	*this = *this * mat;
	return *this;
}

inline const Float4 Float4x4::operator[](int major) const
{
	return (Float4&)(m[major][0]);
}

inline const Float4 Float4x4::GetMajor(int major) const
{
	Float4 ret(m[major][0], m[major][1], m[major][2], m[major][3]);
	return ret;
}


inline Float4x4 & Float4x4::Identity()
{
	*this = (Float4x4)DirectX::XMMatrixIdentity();
	return *this;
}

inline Float4x4 & Float4x4::ScaleIdentity()
{
	Float3 scale, pos;
	Quaternion qt;
	Decompose(scale, qt, pos);
	scale = Float3(1.0f, 1.0f, 1.0f);
	return AffineTransformation(scale, Float3(0.0f, 0.0f, 0.0f), qt, pos);
}

inline void Float4x4::Decompose(Float3 & rScale, Quaternion & rQt, Float3 & rPos) const
{
	DirectX::XMVECTOR scale, qt, pos;
	if (DirectX::XMMatrixDecompose(&scale, &qt, &pos, *this))
	{
		rScale = scale;
		rQt = qt;
		rPos = pos;
	}
	else
	{
		// スケーリングがマイナスの場合
		auto XLen = Float3(_11, _12, _13).Length();
		auto YLen = Float3(_21, _22, _23).Length();
		auto ZLen = Float3(_31, _32, _33).Length();
		//スケーリングが0の要素がある
		if (XLen == 0.0f) {
			XLen = 0.0001f;
		}
		if (YLen == 0.0f) {
			YLen = 0.0001f;
		}
		if (ZLen == 0.0f) {
			ZLen = 0.0001f;
		}
		rScale = Float3(XLen, YLen, ZLen);
		rPos = Float3(_41, _42, _43);

		Float3 vX = Float3(_11, _12, _13) / XLen;
		Float3 vY = Float3(_21, _22, _23) / XLen;
		Float3 vZ = Float3(_31, _32, _33) / XLen;

		Float4x4 retM;
		retM.Identity();
		retM._11 = vX.x;
		retM._12 = vX.y;
		retM._13 = vX.z;

		retM._21 = vY.x;
		retM._22 = vY.y;
		retM._23 = vY.z;

		retM._31 = vZ.x;
		retM._32 = vZ.y;
		retM._33 = vZ.z;

		rQt = (Quaternion)DirectX::XMQuaternionRotationMatrix(retM);
	}
}

inline Float3 Float4x4::ExtractTransform() const
{
	return Float3(_41, _42, _43);
}

inline Float3 Float4x4::ExtractScale() const
{
	Float3 scale, pos;
	Quaternion qt;
	Decompose(scale, qt, pos);
	return scale;
}

inline Quaternion Float4x4::ExtractQuaternion() const
{
	Float3 scale, pos;
	Quaternion qt;
	Decompose(scale, qt, pos);
	return qt;
}

inline Float3 Float4x4::ExtractRotX() const
{
	Float3 ret(_11, _12, _13);
	Float3 scale = ExtractScale();
	ret /= scale.x;

	return ret;
}

inline Float3 Float4x4::ExtractRotY() const
{
	Float3 ret(_21, _22, _23);
	Float3 scale = ExtractScale();
	ret /= scale.y;

	return ret;
}

inline Float3 Float4x4::ExtractRotZ() const
{
	Float3 ret(_31, _32, _33);
	Float3 scale = ExtractScale();
	ret /= scale.z;

	return ret;
}

inline Float4x4 & Float4x4::Scale(const Float3 & scaleVec)
{
	*this = (Float4x4)DirectX::XMMatrixScalingFromVector(scaleVec);
	return *this;
}

inline Float4x4 & Float4x4::AffineTransformation(const Float3 & scale, const Float3 & rotOrigin, const Quaternion & unitQuat, const Float3 & translation)
{
	*this = (Float4x4)DirectX::XMMatrixAffineTransformation(scale, rotOrigin, unitQuat, translation);
	return *this;
}

inline Float4x4 & Float4x4::strTransformation(const Float3 & scale, const Float3 & translation, const Quaternion & unitQuat)
{
	Float4x4 scaleM = (Float4x4)DirectX::XMMatrixScalingFromVector(scale);
	Float4x4 transM = (Float4x4)DirectX::XMMatrixTranslationFromVector(translation);
	Float4x4 qtM = (Float4x4)DirectX::XMMatrixRotationQuaternion(unitQuat);
	*this = scaleM * transM*qtM;
	return *this;
}

inline bool Float4x4::equalInt(const Float4x4 & dest) const
{
	for (int i = 0; i < 4; ++i)
	{
		// 行単位で比較
		if (!DirectX::XMVector4EqualInt(GetMajor(i), dest.GetMajor(i)))
			return false;
	}
	return true;
}

inline Float4x4::operator DirectX::XMMATRIX() const
{
	DirectX::XMMATRIX mat = DirectX::XMLoadFloat4x4(&(DirectX::XMFLOAT4X4)*this);
	return mat;
}

#pragma endregion


inline float Clamp(float val, float min, float max)
{
	if (val < min) return min;
	if (val > max) return max;
	return val;
}
