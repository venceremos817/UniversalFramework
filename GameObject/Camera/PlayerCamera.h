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
	/// �X�V����	
	/// �J�����͑��I�u�W�F�N�g�̈ړ���ɍX�V����������LateUpdate
	/// </summary>
	virtual void LateUpdate()override;

	virtual void Destroy()override;

	/// <summary>
	/// �Ǐ]����I�u�W�F�N�g��ݒ�
	/// </summary>
	void SetPrimaryTarget(const std::shared_ptr<Transform>& pPrimary);
	void SetPrimaryTarget(const std::shared_ptr<GameObject>& pPrimary);
	void SetPrimaryTarget(const std::shared_ptr<Component>& pPrimary);

	/// <summary>
	/// �Ǐ]�x��W����ݒ�
	/// </summary>
	/// <param name="t">�x��⊮�W��</param>
	void SetLateLerp(float t);

	/// <summary>
	/// �J�������xX�ݒ�
	/// </summary>
	/// <param name="sensiX"></param>
	void SetSensiX(float sensiX);
	/// <summary>
	/// �J�������xY�ݒ�
	/// </summary>
	/// <param name="sensiY"></param>
	void SetSensiY(float sensiY);

	/// <summary>
	/// �A�[���̒�����ݒ�
	/// </summary>
	/// <param name="length"></param>
	void SetArmLength(float length);
	/// <summary>
	/// �A�[���̍ő咷��ݒ�
	/// </summary>
	/// <param name="length"></param>
	void SetArmMaxLength(float length);
	/// <summary>
	/// �A�[���̍ŏ�����ݒ�
	/// </summary>
	/// <param name="length"></param>
	void SetArmMinLength(float length);

	Float3	GetMoveForward();
	Float3	GetMoveBack();
	Float3	GetMoveLeft();
	Float3	GetMoveRight();

private:
	/// <summary>
	/// �Q�[���p�b�h�ɂ�鑀��
	/// </summary>
	void OperationByGamePad(const GamePadState& gamePad);
	/// <summary>
	/// �}�E�X�ɂ�鑀��
	/// </summary>
	void OperationByMouse();

private:
	struct Param;
	std::unique_ptr<Param> m_param;
};
