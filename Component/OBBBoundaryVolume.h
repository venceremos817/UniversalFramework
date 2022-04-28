#pragma once
#include <XMFLOAT_EX.h>
#include <SphereBoundaryVolume.h>


class OBBBoundaryVolume
{
public:
	/// <summary>
	/// デフォルトコンストラクタ
	/// 1辺1.0のOBB
	/// </summary>
	OBBBoundaryVolume();
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="size">サイズ</param>
	/// <param name="rot">回転ベクトル</param>
	/// <param name="center">中心</param>
	OBBBoundaryVolume(const Float3& size, const Float3& rot, Float3& center);
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="size">サイズ</param>
	/// <param name="quat">回転クォータニオン</param>
	/// <param name="center">中心</param>
	OBBBoundaryVolume(const Float3& size, const Quaternion& quat, const Float3& center);
	/// <summary>
	/// コンストラクタ
	/// 作成されたサイズと変換行列からOBBを作成
	/// </summary>
	/// <param name="size">サイズ</param>
	/// <param name="matrix">変換行列</param>
	OBBBoundaryVolume(const Float3& size, const Float4x4 matrix);

	/// <summary>
	/// OBBボリュームを包み込むSphereVolumeを取得
	/// </summary>
	/// <returns></returns>
	SphereBoundaryVolume GetWrappedSphereVolume();

	/// <summary>
	/// サイズと変換行列からOBBを作る
	/// </summary>
	/// <param name="size">サイズ</param>
	/// <param name="matrix">変換行列</param>
	void Create(const Float3& size, const Float4x4& matrix);

	/// <summary>
	/// 回転行列を得る
	/// </summary>
	/// <returns></returns>
	Float4x4 GetRotMatrix()const;

	/// <summary>
	/// ワールド行列を得る
	/// </summary>
	/// <returns></returns>
	Float4x4 GetWorldMatrix()const;

	/// <summary>
	/// 一番大きい辺の長さを得る
	/// </summary>
	/// <returns></returns>
	float GetMaxSize()const;
	/// <summary>
	/// 一番大きい辺の半分の長さを得る
	/// </summary>
	/// <returns></returns>
	float GetHalfMaxSize()const;
	/// <summary>
	/// 一番小さい辺の長さを得る
	/// </summary>
	/// <returns></returns>
	float GetMinSize()const;
	/// <summary>
	/// 一番小さい辺の半分の長さを得る
	/// </summary>
	/// <returns></returns>
	float GetHalfMinSize()const;


public:
	Float3 m_center;	// 中心
	Float3 m_rot[3];	// 各座標軸の傾きを表す方向ベクトル
	Float3 m_size;	// 各座標軸に沿った長さの半分(中心点から面までの長さ
};