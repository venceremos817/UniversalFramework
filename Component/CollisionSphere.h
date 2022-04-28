#pragma once
#include <Collision.h>



class CollisionSphere
	: public Collision
{
public:
	explicit CollisionSphere(const std::shared_ptr<GameObject>& pGameObject);
	virtual ~CollisionSphere();



	/// <summary>
	/// ダブルデスパッチ関数	衝突判定を相手に呼んでもらう
	/// </summary>
	/// <param name="pSrcCol"></param>
	 void CollisionCall(const std::shared_ptr<Collision>& pSrcCol) override;

	//========== ゲッター・セッター ==========
	/// <summary>
	/// 作成時の半径を取得
	/// </summary>
	/// <returns></returns>
	float GetMakedRadius()const;
	/// <summary>
	/// 作成時の半径を設定
	/// </summary>
	/// <param name="radius"></param>
	void SetMakedRadius(float radius);
	/// <summary>
	/// 作成時の直径を取得
	/// </summary>
	/// <returns></returns>
	float GetMakedDiameter()const;
	/// <summary>
	/// 作成時の直径を設定
	/// </summary>
	/// <param name="diameter"></param>
	/// <returns></returns>
	void SetMakeDiameter(float diameter);
	/// <summary>
	/// 計算時XYZのどのスケールを使用するかを取得
	/// </summary>
	/// <returns></returns>
	CalcScaling GetCalcScaling()const;
	/// <summary>
	/// 計算時XYZのどのスケールを使用するかを設定
	/// </summary>
	/// <param name="s"></param>
	void SetCalcScaling(CalcScaling s);
	/// <summary>
	/// 現在のSphere境界ボリュームを取得
	/// </summary>
	/// <returns></returns>
	SphereBoundaryVolume GetSphereVolume()const;
	/// <summary>
	/// 1ターン前のSphere境界ボリュームを取得
	/// </summary>
	/// <returns></returns>
	SphereBoundaryVolume GetBeforeSphereVolume()const;

	/// <summary>
	/// Sphereボリュームを取得
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
	void CollisionTest(const std::shared_ptr<CollisionSphere>& pDestSphere)override;
	/// <summary>
	/// OBBとの衝突テスト (回転によるエスケープ未対応)
	/// </summary>
	/// <param name="pDestOBB"></param>
	void CollisionTest(const std::shared_ptr<CollisionOBB>& pDestOBB)override;
	/// <summary>
	/// Capsuleとの衝突テスト
	/// </summary>
	/// <param name="pDestCapsule"></param>
	void CollisionTest(const std::shared_ptr<CollisionCapsule>& pDestCapsule)override;
	/// <summary>
	/// Rectとの衝突テスト
	/// </summary>
	/// <param name="pDestRect"></param>
	void CollisionTest(const std::shared_ptr<CollisionRect>& pDestRect)override;

	//========== エスケープ処理 ==========
	/// <summary>
	/// Sphereからエスケープ
	/// </summary>
	/// <param name="pDestSphere"></param>
	void CollisionEscape(const std::shared_ptr<CollisionSphere>& pDestSphere)override;
	/// <summary>
	/// OBBからエスケープ
	/// </summary>
	/// <param name="pDestOBB"></param>
	void CollisionEscape(const std::shared_ptr<CollisionOBB>& pDestOBB)override;
	/// <summary>
	/// Capsuleからエスケープ
	/// </summary>
	/// <param name="pDestCapsule"></param>
	void CollisionEscape(const std::shared_ptr<CollisionCapsule>& pDestCapsule)override;
	/// <summary>
	/// Rectからエスケープ
	/// </summary>
	/// <param name="pDestRect"></param>
	void CollisionEscape(const std::shared_ptr<CollisionRect>& pDestRect)override;


	/// <summary>
	/// 衝突法線を取得
	/// </summary>
	/// <param name="retNormal">衝突法線を格納して戻す</param>
	void GetHitNormal(const std::shared_ptr<CollisionSphere>& pDestSphere, Float3 & retNormal) const override;
	void GetHitNormal(const std::shared_ptr<CollisionOBB>& pDestOBB, Float3 & retNormal) const override;
	void GetHitNormal(const std::shared_ptr<CollisionCapsule>& pDestCapsule, Float3 & retNormal) const override;
	void GetHitNormal(const std::shared_ptr<CollisionRect>& pDestRect, Float3 & retNormal) const override;

	/// <summary>
	/// 1つ前のターンからの指定時間の位置に戻る
	/// </summary>
	/// <param name="totalVelocity">トータル速度(RigidBodyとGravity..etc足したもの)</param>
	/// <param name="spanTime">時間</param>
	void BackToBefore(const Float3& totalVelocity, float spanTime)override;


private:
	struct Param;
	std::unique_ptr<Param> m_pParam;


	// Collision を介して継承されました



};