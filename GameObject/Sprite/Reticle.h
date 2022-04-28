#pragma once
#include <GameObject.h>

class Reticle
	: public GameObject
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pStage"></param>
	explicit Reticle(std::shared_ptr<Stage>& pStage);
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Reticle();

	/// <summary>
	/// ����������
	/// </summary>
	void OnCreate()override;
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update()override;

	virtual void Destroy()override;

	/// <summary>
	/// �J������ݒ�
	/// </summary>
	/// <param name="camera"></param>
	void SetCamera(std::shared_ptr<Camera>& camera);

private:
	struct Param;
	std::unique_ptr<Param> m_param;
};