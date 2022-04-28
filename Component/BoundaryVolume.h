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
	/// コライダーの形
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
	/// 2つのSphere境界ボリュームをつなげたSphere境界ボリュームを取得
	/// </summary>
	/// <param name="sp1"></param>
	/// <param name="sp2"></param>
	/// <returns></returns>
	static SphereBoundaryVolume SphereEnclosingSphere(const SphereBoundaryVolume& sp0, const SphereBoundaryVolume& sp1);


	/// <summary>
	/// 球と球の衝突判定
	/// </summary>
	/// <param name="sp1"></param>
	/// <param name="sp2"></param>
	/// <returns></returns>
	static bool Sphere_Sphere(const SphereBoundaryVolume& sp1, const SphereBoundaryVolume& sp2);
	/// <summary>
	/// Sphereと動かないSphereの衝突判定
	/// </summary>
	/// <param name="srcSphere">動く球</param>
	/// <param name="srcVelocity">動く球の速度</param>
	/// <param name="destSphere">動かない球</param>
	/// <param name="startTime">開始時間</param>
	/// <param name="endTime">終了時間</param>
	/// <param name="retHitTime">ヒット時間を格納して返す</param>
	/// <returns></returns>
	static bool CollisionTestSphereSphere(const SphereBoundaryVolume& srcSphere, const Float3& srcVelocity,
		const SphereBoundaryVolume& destSphere, float startTime, float endTime, float& retHhitTime);

	/// <summary>
	/// 球とOBBの衝突判定
	/// </summary>
	/// <param name="sp"></param>
	/// <param name="obb"></param>
	/// <param name="retClosestPos">最接近点を格納する参照</param>
	/// <returns></returns>
	static bool Sphere_OBB(const SphereBoundaryVolume& sp, const OBBBoundaryVolume& obb, Float3& retClosestPos);
	/// <summary>
	/// Sphereと動かないOBBの衝突判定
	/// </summary>
	/// <param name="srcSphere">動く球</param>
	/// <param name="srcVelocity">動く球の速度</param>
	/// <param name="destOBB">動かないOBB</param>
	/// <param name="startTime">開始時間</param>
	/// <param name="endTime">終了時間</param>
	/// <param name="retHitTime">ヒット時間を格納して返す</param>
	/// <returns></returns>
	static bool CollisionTestSphereOBB(const SphereBoundaryVolume& srcSphere, const Float3& srcVelocity,
		const OBBBoundaryVolume& destOBB, float startTime, float endTime, float& retHitTime);

	/// <summary>
	/// 球とカプセルの衝突判定
	/// </summary>
	/// <param name="sphere"></param>
	/// <param name="capsule"></param>
	/// <param name="retClosestPos">最接近点を格納して戻す</param>
	/// <returns></returns>
	static bool SPHERE_CAPSULE(const SphereBoundaryVolume& sphere, const CapsuleBoundaryVolume& capsule, Float3& retClosestPos);
	/// <summary>
	/// Sphereと動かないカプセルの衝突判定
	/// </summary>
	/// <param name="srcSphere">動く球</param>
	/// <param name="srcVelocity">動く球の速度</param>
	/// <param name="destCapsule">動かないカプセル</param>
	/// <param name="startTime">開始時間</param>
	/// <param name="endTime">終了時間</param>
	/// <param name="retHitTime">ヒット時間を格納して返す</param>
	/// <returns></returns>
	static bool CollisionTestSphereCapsule(const SphereBoundaryVolume& srcSphere, const Float3& srcVelocity,
		const CapsuleBoundaryVolume& destCapsule, float startTime, float endTime, float& retHitTime);

	/// <summary>
	/// カプセルとOBBの衝突判定
	/// </summary>
	/// <param name="capsule">カプセル</param>
	/// <param name="obb">OBB</param>
	/// <param name="retClosestPos">最接近点を格納して戻す</param>
	/// <returns></returns>
	static bool CAPSULE_OBB(const CapsuleBoundaryVolume& capsule, const OBBBoundaryVolume& obb, Float3& retClosestPos);
	/// <summary>
	/// カプセルと動かないOBBの衝突判定
	/// </summary>
	/// <param name="srcCapVol">動くカプセル</param>
	/// <param name="srcVelocity">動くカプセルの速度</param>
	/// <param name="destOBBVol">動かないOBB</param>
	/// <param name="startTime">開始時間</param>
	/// <param name="endTime">終了時間</param>
	/// <param name="retHitTime">ヒットした時間を格納して返す</param>
	/// <returns></returns>
	static bool CollisionTestCapsuleOBB(const CapsuleBoundaryVolume& srcCapVol, const Float3& srcVelocity,
		const OBBBoundaryVolume& destOBBVol, float startTime, float endTime, float& retHitTime);

	/// <summary>
	/// カプセルとカプセルの衝突判定
	/// </summary>
	/// <param name="capA">カプセルA</param>
	/// <param name="capB">カプセルB</param>
	/// <param name="retClosestPosA">カプセルAの最接近点を格納して返す</param>
	/// <param name="retClosestPosB">カプセルBの最接近点を格納して返す</param>
	/// <returns></returns>
	static bool CAPSULE_CAPSULE(const CapsuleBoundaryVolume& capA, const CapsuleBoundaryVolume& capB, Float3& retClosestPosA, Float3& retClosestPosB);

	/// <summary>
	/// カプセルと動かないカプセルの衝突判定
	/// </summary>
	/// <param name="srcCapVol">動くカプセル</param>
	/// <param name="srcVelocity">動くカプセルの速度</param>
	/// <param name="destCapVol">動かないカプセル</param>
	/// <param name="startTime">開始時間</param>
	/// <param name="endTime">終了時間</param>
	/// <param name="retHitTime">ヒットした時間を格納して返す</param>
	/// <returns></returns>
	static bool CollisionTestCapsuleCapsule(const CapsuleBoundaryVolume& srcCapVol, const Float3& srcVelocity,
		const CapsuleBoundaryVolume& destCapVol, float startTime, float endTime, float& hitTime);

	/// <summary>
	/// 線分ABと平面の衝突判定
	/// </summary>
	/// <param name="a">線分開始点</param>
	/// <param name="b">線分終点</param>
	/// <param name="plane">平面境界ボリューム</param>
	/// <param name="outT">最接近点の線分上の位置の割合を格納して返す</param>
	/// <param name="outClosestPos">衝突している場合、最接近点を格納して返す</param>
	/// <returns></returns>
	static bool SEGMENT_PLANE(const Float3& a, const Float3& b, const PlaneBoundaryVolume& plane, float& outT, Float3& outClosestPos);

	/// <summary>
	/// カプセルと矩形の衝突判定
	/// </summary>
	/// <param name="capsuleVol">カプセル境界ボリューム</param>
	/// <param name="rectVol">矩形境界ボリューム</param>
	/// <param name="retClosestPos">最接近点を格納して返す</param>
	/// <returns></returns>
	static bool CAPSULE_RECT(const CapsuleBoundaryVolume& capsuleVol, const RectBoundaryVolume& rectVol, Float3& retClosestPos);
	/// <summary>
	/// カプセルと動かない矩形の衝突判定
	/// </summary>
	/// <param name="srcCapVol">動くカプセル</param>
	/// <param name="srcVelocity">動くカプセルの速度</param>
	/// <param name="destRectVol">動かない矩形</param>
	/// <param name="startTime">開始時間</param>
	/// <param name="endTime">終了時間</param>
	/// <param name="retHitTime">ヒットした時間を格納して返す</param>
	/// <returns></returns>
	static bool CollisionTestCapsuleRect(const CapsuleBoundaryVolume& srcCapVol, const Float3& srcVelocity,
		const RectBoundaryVolume& destRectVol, float startTime, float endTime, float& retHitTime);

	/// <summary>
	/// OBBとOBBの衝突判定
	/// </summary>
	/// <param name="obb1"></param>
	/// <param name="obb2"></param>
	/// <returns></returns>
	static bool OBB_OBB(const OBBBoundaryVolume& obb1, const OBBBoundaryVolume& obb2);
	/// <summary>
	/// OBBと動かないOBBの衝突判定
	/// (速度を重視し、当たらないと早めに判断できる
	/// </summary>
	/// <param name="srcOBB">動くOBB</param>
	/// <param name="srcVelocity">動くOBBの速度</param>
	/// <param name="destOBB">動かないOBB</param>
	/// <param name="startTime">開始時間</param>
	/// <param name="endTime">終了時間</param>
	/// <param name="retHitTime">ヒット時間を格納して返す</param>
	/// <returns></returns>
	static bool CollisionTestOBBOBB(const OBBBoundaryVolume& srcOBB, const Float3& srcVelocity,
		const OBBBoundaryVolume& destOBB, float startTime, float endTime, float& retHitTime);
	/// <summary>
	/// OBBと動かないOBBの衝突判定 (精密
	/// </summary>
	/// <param name="srcOBB">動くOBB</param>
	/// <param name="srcVelocity">動くOBBの速度</param>
	/// <param name="destOBB">動かないOBB</param>
	/// <param name="startTime">開始時間</param>
	/// <param name="endTime">終了時間</param>
	/// <param name="retHitTime">ヒット時間を格納して返す</param>
	/// <returns></returns>
	static bool CollisionTestOBBOBBsub(const OBBBoundaryVolume& srcOBB, const Float3& srcVelocity,
		const OBBBoundaryVolume& destOBB, float startTime, float endTime, float& retHitTime);

	/// <summary>
	/// SphereとRectの衝突判定
	/// </summary>
	/// <param name="sphere"></param>
	/// <param name="rect"></param>
	/// <param name="retClosestPos">最接近点を格納して返す</param>
	/// <returns></returns>
	static bool SPHERE_RECT(const SphereBoundaryVolume& sphere, const RectBoundaryVolume& rect, Float3& retClosestPos);
	/// <summary>
	/// Sphereと動かないRectの衝突判定
	/// </summary>
	/// <param name="srcSphere">動く球</param>
	/// <param name="srcVelocity">動く球の速度</param>
	/// <param name="destRect">動かない矩形</param>
	/// <param name="startTime">開始時間</param>
	/// <param name="endTime">終了時間</param>
	/// <param name="retHitTime">ヒット時間を格納して返す</param>
	/// <returns></returns>
	static bool CollisionTestSphereRect(const SphereBoundaryVolume& srcSphere, const Float3& srcVelocity,
		const RectBoundaryVolume& destRect, float startTime, float endTime, float& retHitTime);

	/// <summary>
	/// OBBとPLANEの衝突判定
	/// </summary>
	/// <param name="obb"></param>
	/// <param name="plane"></param>
	/// <returns></returns>
	static bool OBB_PLANE(const OBBBoundaryVolume& obb, const PlaneBoundaryVolume& plane);

	/// <summary>
	/// OBBと矩形の衝突判定
	/// </summary>
	/// <param name="obb"></param>
	/// <param name="rect"></param>
	/// <returns></returns>
	static bool OBB_RECT(const OBBBoundaryVolume& obb, const RectBoundaryVolume& rect);
	/// <summary>
	/// OBBと動かないRectの衝突判定
	/// </summary>
	/// <param name="srcOBB">動くOBB</param>
	/// <param name="srcVelocity">動くOBBの速度</param>
	/// <param name="destRect">動かない矩形</param>
	/// <param name="startTime">開始時間</param>
	/// <param name="endTime">終了時間</param>
	/// <param name="hitTime">ヒット時間を格納して返す</param>
	/// <returns></returns>
	static bool CollisionTestOBBRect(const OBBBoundaryVolume& srcOBB, const Float3& srcVelocity,
		const RectBoundaryVolume& destRect, float startTime, float endTime, float& hitTime);

	/// <summary>
	/// pointから見たOBBの最接近点を求める
	/// </summary>
	/// <param name="point"></param>
	/// <param name="obb"></param>
	/// <param name="retClosestPos">最接近点を格納する参照</param>
	static void ClosestPtPointOBB(const Float3& point, const OBBBoundaryVolume& obb, Float3& retClosestPos);

	/// <summary>
	/// 点と線分の最接近点を求める
	/// </summary>
	/// <param name="c">点</param>
	/// <param name="a">線分開始点</param>
	/// <param name="b">線分終了点</param>
	/// <param name="retT">最接近点の線分上の位置の割合を格納して返す</param>
	/// <param name="retClosestPos">最接近点を格納して返す</param>
	static void ClosestPtPointSegment(const Float3& c, const Float3& a, const Float3& b, float& retT, Float3& retClosestPos);

	/// <summary>
	/// カプセルとOBBの最接近点を取得
	/// </summary>
	/// <param name="capsule">カプセル</param>
	/// <param name="obb">OBB</param>
	/// <param name="retFlg">2つの球との位置関係</param>
	/// <returns></returns>
	static Float3 ClosestPtCapsuleOBB(const CapsuleBoundaryVolume& capsule, const OBBBoundaryVolume& obb, int& retFlg);

	/// <summary>
	/// 点と矩形の最接近点を求める
	/// </summary>
	/// <param name="point">点</param>
	/// <param name="rect">矩形</param>
	/// <param name="retClosestPos">最接近点を格納して返す</param>
	static void ClosestPtPointRect(const Float3& point, const RectBoundaryVolume& rect, Float3& retClosestPos);

	/// <summary>
	/// 線分同士の最接近点を求める
	/// </summary>
	/// <param name="sA">線分Aの始点</param>
	/// <param name="eA">線分Aの終点</param>
	/// <param name="sB">線分Bの始点</param>
	/// <param name="eB">線分Bの終点</param>
	/// <param name="retS">線分Aの最接近点のパラメータ</param>
	/// <param name="retT">線分Bの最接近点のパラメータ</param>
	/// <param name="retClosestPosA">最接近点A</param>
	/// <param name="retClosestPosB">最接近点B</param>
	/// <returns>最接近点PosA,PosBを計算し、そのパラメータs,tを計算し、線分同士の距離の平方を返す</returns>
	static float ClosestPtSegmentSegment(const Float3& sA, const Float3& eA, const Float3& sB, const Float3& eB, 
		float& retS, float& retT, Float3& retClosestPosA, Float3& retClosestPosB);

	/// <summary>
	/// 点と線分の距離の平方を取得
	/// </summary>
	/// <param name="a">線分開始点</param>
	/// <param name="b">線分終了点</param>
	/// <param name="c">点</param>
	/// <returns></returns>
	static float SqrtDistPointSegment(const Float3& a, const Float3& b, const Float3& c);


public:
	ColliderKind m_kind;
};


