#pragma once

#include <vector>
#include <Component.h>
//#include <GameObject.h>
#include <BoundaryVolume.h>

class CollisionSphere;
class CollisionOBB;
class CollisionCapsule;
class CollisionRect;


/// <summary>
/// 衝突判定コンポーネントの抽象親クラス
/// </summary>
class Collision
	: public Component
{
public:
	/// <summary>
	/// 衝突したときの動作
	/// </summary>
	enum class HitAction
	{
		None,	// 何もしない
		Slide,	// スライド
		Auto,	// オート計算
		Stop,	// ストップ
	};

	/// <summary>
	/// スケールが1つだった場合の計算に使用するスケール
	/// </summary>
	enum class CalcScaling
	{
		XScale,
		YScale,
		ZScale
	};


public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pGameObj"></param>
	explicit Collision(const std::shared_ptr<GameObject>& pGameObject);
	~Collision();


	//========== パラメータゲッター・セッター ==========
	/// <summary>
	/// すり抜けるかどうかを取得
	/// </summary>
	/// <returns></returns>
	bool IsTrigger()const;
	/// <summary>
	/// すり抜けるかどうかを設定
	/// </summary>
	void SetIsTrigger(bool isTrigger);
	/// <summary>
	/// 固定衝突オブジェクトかどうかを取得
	/// </summary>
	/// <returns></returns>
	bool IsFixed()const;
	/// <summary>
	/// 固定衝突オブジェクトかどうかを設定
	/// </summary>
	/// <param name="isFixed"></param>
	void SetFixed(bool isFixed);

	/// <summary>
	/// エスケープ処理の調整値を取得
	/// </summary>
	/// <returns></returns>
	float GetEscapeAdjustedValue()const;
	/// <summary>
	/// エスケープ処理の調整値を設定
	/// </summary>
	/// <param name="f"></param>
	void SetEscapeAdjustedValue(float adjustedVal);

	/// <summary>
	/// エスケープSlide処理の調整値を取得
	/// </summary>
	/// <returns></returns>
	float GetEscapeSlideValue()const;
	/// <summary>
	/// エスケープSlide処理の調整値を設定
	/// </summary>
	/// <param name="f"></param>
	void SetEscapeSlideValue(float slideVal);

	/// <summary>
	/// 衝突したときの動作を取得
	/// </summary>
	/// <returns></returns>
	HitAction GetHitAction()const;
	/// <summary>
	/// 衝突したときの動作を設定
	/// </summary>
	/// <param name="hitAction"></param>
	void SetHitAction(HitAction hitAction);

	//========== ヒットオブジェクト配列操作 ==========
	/// <summary>
	/// ヒットオブジェクトを追加
	/// </summary>
	/// <param name="pDestObject"></param>
	void AddHitObject(const std::shared_ptr<GameObject>& pDestObject);
	/// <summary>
	/// 1つ前のヒットオブジェクトにコピー
	/// </summary>
	void SetToBeforeHitObject();
	/// <summary>
	/// ヒットオブジェクトをクリア
	/// </summary>
	void ClearHitObject();
	/// <summary>
	/// ヒットオブジェクト配列を取得
	/// </summary>
	/// <returns></returns>
	std::vector<std::shared_ptr<GameObject>>& GetHitCollisionObjectVec();
	std::vector<std::shared_ptr<GameObject>>& GetHitTriggerObjectVec();
	/// <summary>
	/// ヒットオブジェクト配列に入っているかどうか
	/// </summary>
	/// <param name="obj"></param>
	/// <returns></returns>
	bool IsHitCollisionObject(const std::shared_ptr<GameObject>& obj)const;
	bool IsHitTriggerObject(const std::shared_ptr<GameObject>& obj)const;
	/// <summary>
	/// 1つ前のヒットオブジェクト配列を取得
	/// </summary>
	/// <returns></returns>
	std::vector<std::shared_ptr<GameObject>>& GetBeforeHitCollisionObjectVec();
	std::vector<std::shared_ptr<GameObject>>& GetBeforeHitTriggerObjectVec();
	/// <summary>
	/// 新しくヒットしたオブジェクト配列を取得
	/// </summary>
	/// <returns></returns>
	std::vector<std::shared_ptr<GameObject>>& GetCollisionEnterObjectVec();
	std::vector<std::shared_ptr<GameObject>>& GetTriggerEnterObjectVec();
	/// <summary>
	/// 衝突し続けているヒットオブジェクト配列を取得
	/// </summary>
	/// <returns></returns>
	std::vector<std::shared_ptr<GameObject>>& GetCollisionStayObjectVec();
	std::vector<std::shared_ptr<GameObject>>& GetTriggerStayObjectVec();
	/// <summary>
	/// 衝突から離れたヒットオブジェクト配列を取得
	/// </summary>
	/// <returns></returns>
	std::vector<std::shared_ptr<GameObject>>& GetCollisionExitObjectVec();
	std::vector<std::shared_ptr<GameObject>>& GetTriggerExitObjectVec();


	/// <summary>
	/// ダブルデスパッチ関数	衝突判定を相手に呼んでもらう
	/// </summary>
	/// <param name="srcCol"></param>
	virtual void CollisionCall(const std::shared_ptr<Collision>& pSrcCol) = 0;


	//========== 衝突テスト ==========
	/// <summary>
	/// Sphereとの衝突テスト
	/// </summary>
	/// <param name="pDestSphere"></param>
	virtual void CollisionTest(const std::shared_ptr<CollisionSphere>& pDestSphere) = 0;
	/// <summary>
	/// OBBとの衝突テスト
	/// </summary>
	/// <param name="pDestOBB"></param>
	virtual void CollisionTest(const std::shared_ptr<CollisionOBB>& pDestOBB) = 0;
	/// <summary>
	/// Capusuleとの衝突テスト
	/// </summary>
	/// <param name="pDestCapsule"></param>
	virtual void CollisionTest(const std::shared_ptr<CollisionCapsule>& pDestCapsule) = 0;
	/// <summary>
	/// Rectとの衝突テスト
	/// </summary>
	/// <param name="pDestRect"></param>
	virtual void CollisionTest(const std::shared_ptr<CollisionRect>& pDestRect) = 0;

	//========== エスケープ処理 ==========
	/// <summary>
	/// Sphereからエスケープ
	/// </summary>
	/// <param name="pDestCol"></param>
	virtual void CollisionEscape(const std::shared_ptr<CollisionSphere>& pDestSphere) = 0;
	/// <summary>
	/// OBBからエスケープ (回転によるエスケープ未対応)
	/// </summary>
	/// <param name="pDestOBB"></param>
	virtual void CollisionEscape(const std::shared_ptr<CollisionOBB>& pDestOBB) = 0;
	/// <summary>
	/// Capsuleからエスケープ
	/// </summary>
	/// <param name="pDestCapsule"></param>
	virtual void CollisionEscape(const std::shared_ptr<CollisionCapsule>& pDestCapsule) = 0;
	/// <summary>
	/// Rectからエスケープ
	/// </summary>
	/// <param name="pDestRect"></param>
	virtual void CollisionEscape(const std::shared_ptr<CollisionRect>& pDestRect) = 0;

	/// <summary>
	/// 衝突法線を取得
	/// </summary>
	/// <param name="rNormal">衝突法線を格納して戻す</param>
	virtual void GetHitNormal(const std::shared_ptr<CollisionSphere>& pDestSphere, Float3& retNormal)const = 0;
	virtual void GetHitNormal(const std::shared_ptr<CollisionOBB>& pDestOBB, Float3& retNormal)const = 0;
	virtual void GetHitNormal(const std::shared_ptr<CollisionCapsule>& pDestCapsule, Float3& retNormal)const = 0;
	virtual void GetHitNormal(const std::shared_ptr<CollisionRect>& pDestRect, Float3& retNormal)const = 0;


	/// <summary>
	/// 1つ前のターンからの指定時間の位置に戻る
	/// </summary>
	/// <param name="totalVelocity">トータル速度(RigidBodyとGravity..etc足したもの)</param>
	/// <param name="spanTime">時間</param>
	virtual void BackToBefore(const Float3& totalVelocity, float spanTime) = 0;

	/// <summary>
	/// 衝突判定後の処理
	/// </summary>
	/// <param name="destCol">相手のCollision</param>
	/// <param name="srcVelocity">自分の速度</param>
	/// <param name="destVelocity">相手の速度</param>
	/// <param name="hitNormal">衝突法線</param>
	/// <param name="afterHitTime">衝突後のターン時間</param>
	void AfterCollision(const std::shared_ptr<Collision>& pDestCol, const Float3& srcVelocity,
		const Float3& destVelocity, const Float3& hitNormal, float afterHitTime);

	/// <summary>
	/// 衝突判定後処理呼び出し用テンプレート1
	/// </summary>
	/// <param name="srcVelocity">srcの速度</param>
	/// <param name="destVelocity">destの速度</param>
	/// <param name="elapsedTime">ターン経過時間</param>
	/// <param name="hitTime">衝突時間</param>
	/// <param name="srceColl">srcコリジョン</param>
	/// <param name="destColl">destコリジョン</param>
	template<class SrcColType, class DestColType>
	void AfterCollisionTemplate_1(const Float3& srcVelocity, const Float3& destVelocity,
		float elapsedTime, float hitTime, const std::shared_ptr<SrcColType>& pSrcColl, const std::shared_ptr<DestColType>& pDestColl)
	{
		// 衝突相手を登録
		AddHitObject(pDestColl->GetGameObject());

		// 自分または相手がトリガーなら衝突後のエスケープ等は必要ないのでリターン
		if (IsTrigger() || pDestColl->IsTrigger())
		{
			// 相手がfixedでなければ相手の衝突相手に登録
			if (!pDestColl->IsFixed())
				pDestColl->AddHitObject(GetGameObject());
			return;
		}

		// 時間調整
		if (hitTime <= 0.0f)
			hitTime = 0.0f;
		if (hitTime >= elapsedTime)
			hitTime = elapsedTime;

		float afterHitTime = elapsedTime - hitTime;
		bool useSrcAfter = false;
		bool useDestAfter = false;


		if (GetHitAction() != HitAction::None)
		{
			// 事後処理が有効なら衝突前まで戻る
			BackToBefore(srcVelocity, hitTime);
			useSrcAfter = true;
		}

		if (!pDestColl->IsFixed())
		{
			// 相手がfixedでなければ相手の衝突相手に登録
			pDestColl->AddHitObject(GetGameObject());
			if (pDestColl->GetHitAction() != HitAction::None)
			{
				// 事後処理が有効なら衝突前まで戻る
				pDestColl->BackToBefore(destVelocity, hitTime);
				useDestAfter = true;
			}
		}
		else
		{
			// 相手がfixedなら自己退避だけではまだ衝突している可能性があるので判定をしてエスケープ処理
			if (GetHitAction() != HitAction::None)
				CollisionEscape(pDestColl);
		}

		if (useSrcAfter)
		{
			Float3 hitNormal;
			GetHitNormal(pDestColl, hitNormal);
			AfterCollision(pDestColl, srcVelocity, destVelocity, hitNormal, afterHitTime);
		}
		if (useDestAfter)
		{
			Float3 hitNormal;
			GetHitNormal(pSrcColl, hitNormal);
			pDestColl->GetHitNormal(pSrcColl, hitNormal);
			pDestColl->AfterCollision(pSrcColl, destVelocity, srcVelocity, hitNormal, afterHitTime);
		}
	}
	/// <summary>
	/// 衝突判定後処理呼び出し用テンプレート2
	/// </summary>
	/// <param name="srcVelocity">srcの速度</param>
	/// <param name="destVelocity">destの速度</param>
	/// <param name="elapsedTime">ターン経過時間</param>
	/// <param name="hitTime">衝突時間</param>
	/// <param name="srceColl">srcコリジョン</param>
	/// <param name="destColl">destコリジョン</param>
	template<class SrcColType, class DestColType>
	void AfterCollisionTemplate_2(const Float3& srcVelocity, const Float3& destVelocity,
		float elapsedTime, float hitTime, const std::shared_ptr<SrcColType>& pSrcColl, const std::shared_ptr<DestColType>& pDestColl)
	{
		// 衝突相手を登録
		AddHitObject(pDestColl->GetGameObject());

		// 自分または相手がトリガーなら衝突後のエスケープ等は必要ないのでリターン
		if (IsTrigger() || pDestColl->IsTrigger())
		{
			pDestColl->AddHitObject(GetGameObject());
			return;
		}

		// 自分または相手がトリガーなら衝突後のエスケープ等は必要ないのでリターン
		if (IsTrigger() || pDestColl->IsTrigger())
			return;

		// 時間調整
		if (hitTime <= 0.0f)
			hitTime = 0.0f;
		if (hitTime >= elapsedTime)
			hitTime = elapsedTime;

		float afterHitTime = elapsedTime - hitTime;
		bool useSrcAfter = false;

		if (GetHitAction() != HitAction::None)
		{
			// 事後処理が有効なら衝突前まで戻る
			BackToBefore(srcVelocity, hitTime);
			useSrcAfter = true;
		}
		if (GetHitAction() != HitAction::None)
		{
			CollisionEscape(pDestColl);
		}
		if (useSrcAfter)
		{
			Float3 hitNormal;
			GetHitNormal(pDestColl, hitNormal);
			AfterCollision(pDestColl, srcVelocity, destVelocity, hitNormal, afterHitTime);
		}
	}



	/// <summary>
	/// 包み込んだSphere境界ボリュームを取得
	/// </summary>
	/// <returns></returns>
	virtual SphereBoundaryVolume GetWrappedSphereVol()const = 0;

	/// <summary>
	/// 1つ前と現在の連結したSphere境界ボリュームを取得
	/// </summary>
	/// <returns></returns>
	virtual SphereBoundaryVolume GetEnclosingSphereVol()const = 0;

	/// <summary>
	/// 自分と相手の連結したSphere境界ボリュームの衝突判定
	/// </summary>
	/// <param name="pDestCol"></param>
	/// <returns></returns>
	template<class T>
	bool CollisionWrappedSphere(const std::shared_ptr<T>& pDestCol)
	{
		SphereBoundaryVolume src = GetEnclosingSphereVol();
		SphereBoundaryVolume dest = pDestCol->GetEnclosingSphereVol();
		return (BoundaryVolume::Sphere_Sphere(src, dest));
	}




private:
	/// <summary>
	/// 衝突配列を検証してテンポラリ配列にセットする
	/// </summary>
	/// <param name="target">検証するオブジェクト</param>
	/// <param name="setVec">セットするテンポラリ配列</param>
	/// <param name="scanVec">スキャンする配列</param>
	/// <param name="isFind">見つかったらセットするかどうか (falseの場合は見つからなかったらセットする)</param>
	void FindIfCollisionVector(std::shared_ptr<GameObject> target, std::vector<std::shared_ptr<GameObject>>& setVec, const std::vector<std::shared_ptr<GameObject>>& scanVec, bool isFind);


private:
	struct Param;
	std::unique_ptr<Param> m_pParam;
};


