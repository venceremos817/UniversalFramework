#pragma once
#include <XMFLOAT_EX.h>
#include <SphereBoundaryVolume.h>

class CapsuleBoundaryVolume
{
public:
	/// <summary>
	/// コンストラクタ
	/// 半径:0.5 bottom:(0,-0.5,0) top(0,0.5,0)
	/// </summary>
	CapsuleBoundaryVolume();
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="radius">半径</param>
	/// <param name="bottomPos">中間部線分開始点</param>
	/// <param name="topPos">中間部線分終了点</param>
	CapsuleBoundaryVolume(float radius, Float3& bottomPos, Float3& topPos);
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="makedRadius">作成時の半径</param>
	/// <param name="makedBottomPos">作成時の中間部線分開始点</param>
	/// <param name="makedTopPos">作成時の中間部線分終了点</param>
	/// <param name="matrix">変換行列</param>
	CapsuleBoundaryVolume(float makedRadius, const Float3& makedBottomPos, const Float3& makedTopPos, const Float4x4& matrix);

	/// <summary>
	/// 包み込むSphere境界ボリュームを取得
	/// </summary>
	/// <returns></returns>
	SphereBoundaryVolume GetWrappedSphereBoundaryVolume()const;
	

	/// <summary>
	/// 中心点を取得
	/// </summary>
	/// <returns></returns>
	Float3 GetCenter()const;
	/// <summary>
	/// 中心点を設定
	/// </summary>
	/// <param name="center"></param>
	void SetCenter(const Float3& center);

public:
	float m_radius;			// 半径
	Float3 m_bottomPos;		// 中間部分線分開始点
	Float3 m_topPos;		// 中間部分線分終了点
};