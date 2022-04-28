#pragma once

#include <Component.h>
#include <XMFLOAT_EX.h>



class RigidBody
	: public Component
{
public:
	explicit RigidBody(const std::shared_ptr<GameObject>& pGameObject);
	virtual ~RigidBody();



	/// <summary>
	/// 更新処理
	/// </summary>
	void Update()override;




	/// <summary>
	/// 速度を取得
	/// </summary>
	/// <returns></returns>
	const Float3& GetVelocity()const;
	/// <summary>
	/// 速度を設定
	/// </summary>
	/// <param name="velocity"></param>
	const Float3& SetVelocity(const Float3& velocity);
	const Float3& SetVelocity(float x, float y, float z);
	/// <summary>
	/// 速度を0に設定
	/// </summary>
	void SetVelocityZero();
	
	/// <summary>
	/// 重力速度を取得
	/// </summary>
	/// <returns></returns>
	const Float3& GetGravityVelocity()const;
	/// <summary>
	/// 重力速度を設定
	/// </summary>
	/// <param name="velocity"></param>
	void SetGravityVelocity(const Float3& velocity);
	void SetGravityVelocity(float x, float y, float z);
	/// <summary>
	/// 重力速度を0に設定
	/// </summary>
	void SetGravityVelocityZero();

	/// <summary>
	/// 最高速度を取得
	/// </summary>
	/// <returns></returns>
	float GetMaxSpeed()const;
	/// <summary>
	/// 最高速度を設定
	/// </summary>
	/// <param name="speed"></param>
	void SetMaxSpeed(float speed);
	/// <summary>
	/// 最低速度を取得
	/// </summary>
	const Float3& GetMinVelocity()const;
	/// <summary>
	/// 最低速度を設定
	/// </summary>
	/// <param name="velocity"></param>
	void SetMinVelocity(const Float3& velocity);
	void SetMinVelocity(float x, float y, float z);

	/// <summary>
	/// 回転速度を取得
	/// </summary>
	/// <returns></returns>
	const Float3& GetAngularVelocity()const;
	/// <summary>
	/// 回転速度を設定
	/// </summary>
	/// <param name="angularVelocity"></param>
	void SetAngularVelocity(const Float3& angularVelocity);
	void SetAngularVelocity(float x, float y, float z);

	/// <summary>
	/// 質量を取得
	/// </summary>
	/// <returns></returns>
	float GetMass()const;
	/// <summary>
	/// 質量を設定
	/// </summary>
	/// <param name="mass"></param>
	void SetMass(float mass);

	/// <summary>
	/// 反発係数を得る
	/// </summary>
	/// <returns></returns>
	float GetReflection()const;
	/// <summary>
	/// 反発係数を設定
	/// </summary>
	/// <param name="reflection"></param>
	void SetReflection(float reflection);

	/// <summary>
	/// フォースを取得
	/// </summary>
	/// <returns></returns>
	const Float3& GetForce()const;
	/// <summary>
	/// フォースを設定
	/// </summary>
	/// <param name="force"></param>
	void SetForce(const Float3& force);
	void SetForce(float x, float y, float z);

	/// <summary>
	/// 減速フォースを取得
	/// </summary>
	/// <returns></returns>
	float GetFrictionForce()const;
	/// <summary>
	/// 減速フォースを設定
	/// </summary>
	/// <param name="frictionF"></param>
	void SetFrictionForce(float frictionF);

	/// <summary>
	/// 最大フォースを取得
	/// </summary>
	/// <returns></returns>
	float GetMaxForce()const;
	/// <summary>
	/// 最大フォースを設定
	/// </summary>
	/// <param name="f"></param>
	void SetMaxForce(float f);

	/// <summary>
	/// 指定した時間分速度にしたがって移動する
	/// </summary>
	/// <param name="calcTime"></param>
	void UpdateFromTime(float calcTime);



public:
	struct Param;
	std::unique_ptr<Param> m_pParam;
};


