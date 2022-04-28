#pragma once

#include <Component.h>
#include <Utility/XMFLOAT_EX.h>

class Transform
	: public Component
{
public:
	explicit Transform(const std::shared_ptr<GameObject>& pGameObject);
	~Transform();


	/// <summary>
	/// 更新処理
	/// </summary>
	void Update()override;


	/// <summary>
	/// 前のスケールを取得
	/// </summary>
	/// <returns></returns>
	Float3 GetPreScale()const;
	/// <summary>
	/// スケールを取得
	/// </summary>
	Float3 GetScale()const;
	/// <summary>
	/// スケールを設定
	/// </summary>
	void SetScale(const Float3& scale);
	void SetScale(float x, float y, float z);

	/// <summary>
	/// 前の重心を取得
	/// </summary>
	/// <returns></returns>
	Float3 GetPrePivod()const;
	/// <summary>
	/// 重心を取得
	/// </summary>
	Float3 GetPivod()const;
	/// <summary>
	/// 重心を設定
	/// </summary>
	void SetPivod(const Float3& pivot);
	void SetPivot(float x, float y, float z);

	/// <summary>
	/// 前の回転を取得(クォータニオン)
	/// </summary>
	/// <returns></returns>
	Quaternion GetPreQuat()const;
	/// <summary>
	/// 回転を取得(クォータニオン)
	/// </summary>
	Quaternion GetQuaternion()const;
	/// <summary>
	/// 回転を設定
	/// </summary>
	void SetQuaternion(const Quaternion& quat);
	/// <summary>
	/// 前の回転を取得(ベクトル)
	/// </summary>
	/// <returns></returns>
	Float3 GetPreRotation()const;
	/// <summary>
	/// 回転を取得(ベクトル)
	/// </summary>
	Float3 GetRotation()const;
	/// <summary>
	/// 回転を設定する(ベクトル)
	/// </summary>
	void SetRotation(const Float3& rot);
	void SetRotation(float x, float y, float z);

	/// <summary>
	/// 前の座標を取得
	/// </summary>
	/// <returns></returns>
	Float3 GetPrePos()const;
	/// <summary>
	/// 座標を取得
	/// </summary>
	Float3 GetPos()const;
	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="pos"></param>
	void SetPos(const Float3& pos);
	void SetPos(float x, float y, float z);
	/// <summary>
	/// 座標と前の座標を設定
	/// </summary>
	void ResetPos(const Float3& pos);

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	Float3 GetWorldPos();
	/// <summary>
	/// ワールド座標を設定
	/// </summary>
	void SetWorldPos(const Float3& pos);
	void SetWorldPos(float x, float y, float z);
	/// <summary>
	/// ワールド座標と前のワールド座標を設定
	/// </summary>
	void ResetWorldPos(const Float3& pos);

	/// <summary>
	/// 前のワールド行列を取得
	/// </summary>
	/// <returns></returns>
	Float4x4 GetPreWolrdMatrix();
	/// <summary>
	/// ワールドマトリクスを取得
	/// </summary>
	const Float4x4& GetWorldMatrix();

	const Float4x4& Get2DWorldMatrix();


	/// <summary>
	/// Transform上で計算した速度を取得
	/// </summary>
	/// <returns></returns>
	Float3 GetVelocity()const;

	/// <summary>
	/// 前のターンの情報を更新
	/// </summary>
	void SetToBefore();

	/// <summary>
	/// 指定した方向を向く
	/// </summary>
	/// <param name="lookPos"></param>
	void LookAt(const Float3& lookPos);

	/// <summary>
	/// 前方方向を取得
	/// </summary>
	/// <returns></returns>
	Float3 GetForward();
	/// <summary>
	/// 上方向を取得
	/// </summary>
	/// <returns></returns>
	Float3 GetUp();
	/// <summary>
	/// 右方向を取得
	/// </summary>
	/// <returns></returns>
	Float3 GetRight();

	/// <summary>
	/// 現在のワールドマトリクスと同値か
	/// </summary>
	/// <param name="mat"></param>
	/// <returns></returns>
	bool IsSameWorldMatrix(const Float4x4& mat);
	/// <summary>
	/// 1つ前のワールド行列と同値か
	/// </summary>
	/// <param name="mat"></param>
	/// <returns></returns>
	bool IsSameBeforeWorldMatrix(const Float4x4& mat);




public:
	struct Param;
	std::unique_ptr<Param> m_param;
};