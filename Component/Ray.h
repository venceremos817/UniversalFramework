#pragma once

#include <Collision.h>

class Ray
{
	Ray() = delete;
public:
	Ray(const Float3& startPos, const Float3& direction);
	~Ray() {}


	/// <summary>
	/// 球とレイの衝突判定
	/// 距離と衝突地点を格納して返す
	/// </summary>
	/// <param name="rayStartPos">レイの開始点</param>
	/// <param name="rayDirection">レイの方向</param>
	/// <param name="sp">球ボリューム</param>
	/// <param name="refDistance">距離を格納する</param>
	/// <param name="refHitPos">衝突地点を格納する</param>
	/// <returns></returns>
	static bool IsHitSphere(const Float3& rayStartPos, const Float3& rayDirection, const SphereBoundaryVolume& sp, float& refDistance, Float3& refHitPos);
	/// <summary>
	/// 球との衝突判定
	/// </summary>
	/// <param name="sp">球界ボリューム</param>
	/// <param name="refDistance">衝突距離を返す</param>
	/// <param name="refHitPos">衝突座標を返す</param>
	/// <returns></returns>
	bool IsHitSphere(const SphereBoundaryVolume& sp, float& refDistance, Float3& refHitPos);

#ifdef _DEBUG
	void Draw();
#endif // DEBUG


private:
	Float3 m_startPos;
	Float3 m_direction;
};
