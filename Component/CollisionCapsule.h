#pragma once
#include <Collision.h>


class CollisionCapsule
	: public Collision
{
public:
	explicit CollisionCapsule(const std::shared_ptr<GameObject>& pGameObject);
	~CollisionCapsule();

	/// <summary>
	/// ダブルデスパッチ関数	衝突判定を相手に呼んでもらう
	/// </summary>
	/// <param name="pSrcCol"></param>
	void CollisionCall(const std::shared_ptr<Collision>& pSrcCol) override;

	/// <summary>
	/// 現在のCapsule境界ボリュームを取得
	/// </summary>
	/// <returns></returns>
	CapsuleBoundaryVolume GetCapsuleVolume()const;
	/// <summary>
	/// 1ターン前のCapsule境界ボリュームを取得
	/// </summary>
	/// <returns></returns>
	CapsuleBoundaryVolume GetBeforeCapsuleVolume()const;

	/// <summary>
	/// 包み込むSphere境界ボリュームを取得
	/// </summary>
	/// <returns></returns>
	SphereBoundaryVolume GetWrappedSphereVol()const override;
	/// <summary>
	/// 1つ前と現在の連結したSphere境界ボリュームを取得
	/// </summary>
	/// <returns></returns>
	SphereBoundaryVolume GetEnclosingSphereVol() const override;

	//========== 衝突テスト ==========
	/// <summary>
	/// Sphereとの衝突テスト
	/// </summary>
	/// <param name="pDestSphere"></param>
	void CollisionTest(const std::shared_ptr<CollisionSphere>& pDestSphere) override;
	/// <summary>
	/// OBBとの衝突テスト
	/// </summary>
	/// <param name="pDestOBB"></param>
	void CollisionTest(const std::shared_ptr<CollisionOBB>& pDestOBB) override;
	/// <summary>
	/// Capusuleとの衝突テスト
	/// </summary>
	/// <param name="pDestCapsule"></param>
	void CollisionTest(const std::shared_ptr<CollisionCapsule>& pDestCapsule) override;
	/// <summary>
	/// Rectとの衝突テスト
	/// </summary>
	/// <param name="pDestRect"></param>
	void CollisionTest(const std::shared_ptr<CollisionRect>& pDestRect) override;

	//========== エスケープ処理 ==========
	/// <summary>
	/// Sphereからエスケープ
	/// </summary>
	/// <param name="pDestCol"></param>
	void CollisionEscape(const std::shared_ptr<CollisionSphere>& pDestSphere) override;
	/// <summary>
	/// OBBからエスケープ (回転によるエスケープ未対応)
	/// </summary>
	/// <param name="pDestOBB"></param>
	void CollisionEscape(const std::shared_ptr<CollisionOBB>& pDestOBB) override;
	/// <summary>
	/// Capsuleからエスケープ
	/// </summary>
	/// <param name="pDestCapsule"></param>
	void CollisionEscape(const std::shared_ptr<CollisionCapsule>& pDestCapsule) override;
	/// <summary>
	/// Rectからエスケープ
	/// </summary>
	/// <param name="pDestRect"></param>
	void CollisionEscape(const std::shared_ptr<CollisionRect>& pDestRect) override;

	/// <summary>
	/// 衝突法線を取得
	/// </summary>
	/// <param name="pDestSphere"></param>
	/// <param name="retNormal"></param>
	void GetHitNormal(const std::shared_ptr<CollisionSphere>& pDestSphere, Float3 & retNormal) const override;
	void GetHitNormal(const std::shared_ptr<CollisionOBB>& pDestOBB, Float3 & retNormal) const override;
	void GetHitNormal(const std::shared_ptr<CollisionCapsule>& pDestCapsule, Float3 & retNormal) const override;
	void GetHitNormal(const std::shared_ptr<CollisionRect>& pDestRect, Float3 & retNormal) const override;

	/// <summary>
	/// 1つ前のターンからの指定時間の位置に戻る
	/// </summary>
	/// <param name="totalVelocity">トータル速度(RigidBodyとGravity..etc足したもの)</param>
	/// <param name="spanTime">時間</param>
	void BackToBefore(const Float3 & totalVelocity, float spanTime) override;


private:
	struct Param;
	std::unique_ptr<Param> m_pParam;
};