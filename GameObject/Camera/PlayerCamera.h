#pragma once
#include <System/Resource/SharedResources.h>
#include <InputDevice.h>


class PlayerCamera
	: public Camera
{
public:
	PlayerCamera(const std::shared_ptr<Stage>& pStage);
	virtual ~PlayerCamera();

	/// <summary>
	/// 更新処理	
	/// カメラは他オブジェクトの移動後に更新したいためLateUpdate
	/// </summary>
	virtual void LateUpdate()override;

	virtual void Destroy()override;

	/// <summary>
	/// 追従するオブジェクトを設定
	/// </summary>
	void SetPrimaryTarget(const std::shared_ptr<Transform>& pPrimary);
	void SetPrimaryTarget(const std::shared_ptr<GameObject>& pPrimary);
	void SetPrimaryTarget(const std::shared_ptr<Component>& pPrimary);

	/// <summary>
	/// 追従遅れ係数を設定
	/// </summary>
	/// <param name="t">遅れ補完係数</param>
	void SetLateLerp(float t);

	/// <summary>
	/// カメラ感度X設定
	/// </summary>
	/// <param name="sensiX"></param>
	void SetSensiX(float sensiX);
	/// <summary>
	/// カメラ感度Y設定
	/// </summary>
	/// <param name="sensiY"></param>
	void SetSensiY(float sensiY);

	/// <summary>
	/// アームの長さを設定
	/// </summary>
	/// <param name="length"></param>
	void SetArmLength(float length);
	/// <summary>
	/// アームの最大長を設定
	/// </summary>
	/// <param name="length"></param>
	void SetArmMaxLength(float length);
	/// <summary>
	/// アームの最小長を設定
	/// </summary>
	/// <param name="length"></param>
	void SetArmMinLength(float length);

	Float3	GetMoveForward();
	Float3	GetMoveBack();
	Float3	GetMoveLeft();
	Float3	GetMoveRight();

private:
	/// <summary>
	/// ゲームパッドによる操作
	/// </summary>
	void OperationByGamePad(const GamePadState& gamePad);
	/// <summary>
	/// マウスによる操作
	/// </summary>
	void OperationByMouse();

private:
	struct Param;
	std::unique_ptr<Param> m_param;
};
