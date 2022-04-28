#pragma once


#include <Component.h>
#include <XMFLOAT_EX.h>

class GameObject;

/// <summary>
/// ビヘイビアベースクラス
/// </summary>
class Behavior
	:public Component
{
public:
	explicit Behavior(const std::shared_ptr<GameObject>& pGameObject) : Component(pGameObject) {}
	virtual ~Behavior() {}

protected:
};




/// <summary>
/// 重力をかけるビヘイビア	(Collisionでも利用するので、重力をかけたい場合は基本的にこのビヘイビアを使う
/// </summary>
class GravityBehavior
	:public Behavior
{
public:
	explicit GravityBehavior(const std::shared_ptr<GameObject>& pGameObject);
	virtual ~GravityBehavior();


	/// <summary>
	/// 現在の重力を取得
	/// </summary>
	/// <returns></returns>
	const Float3& GetGravity()const;

	/// <summary>
	/// 現在の重力を設定する
	/// </summary>
	/// <param name="gravity"></param>
	void SetGravity(const Float3& gravity);
	void SetGravity(float x, float y, float z);

	/// <summary>
	/// 指定した時間分の重力をかける
	/// </summary>
	/// <param name="time"></param>
	void UpdateFromTime(float time);

	/// <summary>
	/// ジャンプの開始
	/// </summary>
	/// <param name="startVelocity">初速度</param>
	/// <param name="escapeSpan">めり込み補正</param>
	void Jump(const Float3& startVelocity, float escapeSpan = 0.01f);

	/// <summary>
	/// 行動を実行
	/// </summary>
	void Execute();


private:
	struct Param;
	std::unique_ptr<Param> m_param;
};


/// <summary>
/// 共通ビヘイビア
/// </summary>
class CommonBehavior
	: public Behavior
{
public:
	explicit CommonBehavior(const std::shared_ptr<GameObject>& pGameObject) : Behavior(pGameObject) {}
	virtual ~CommonBehavior() {}

	/// <summary>
	/// 指定した方向(ベクトル)に補間によって向く
	/// </summary>
	/// <param name="vector"></param>
	/// <param name="lerpFactor">補間係数</param>
	void RotToVecotr(const Float3& vector, float lerpFactor);
};
