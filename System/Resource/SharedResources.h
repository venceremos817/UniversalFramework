#pragma once
#include <memory>
#include <XMFLOAT_EX.h>
#include <GameObject.h>


struct Viewport
{
	float topLeftX;
	float topLeftY;
	float width;
	float height;
	float minDepth;
	float maxDepth;
};
/// <summary>
/// �r���[�|�[�g�ƃV�[�T�[��`�ݒ�
/// </summary>
void ResetViewport(const Viewport& vp);


/// <summary>
/// �J�����I�u�W�F�N�g
/// </summary>
class Camera
	: public GameObject
{
public:
	struct ViewProjection
	{
		Float4x4 view;
		Float4x4 proj;
	};
	struct Angle
	{
		float y, z;
	};

public:
	Camera(const std::shared_ptr<Stage>& pStage);
	virtual ~Camera();

	void OnCreate()override;
	void LateUpdate()override;

	virtual void Destroy()override;

	//=== �Z�b�^�[ ===
	void SetPos(const Float3& pos);
	void LookAt(const Float3& lookPos);
	void SetOffset(const Float3& offset);
	void SetViewport(const Viewport& viewport);


	//=== �Q�b�^�[ ===
	const Float4x4& GetViewMatrix()const;
	const Float4x4& GetProjectionMatrix()const;

	const Float3& GetPos()const;
	const Float3& GetLook()const;
	const Float3 GetLookPos()const;
	const Float3& GetOffset()const;
	const Float3 GetLookDirection()const;
	const Float3& GetUp()const;
	float	GetFOVY()const;
	float	GetAspect()const;
	float	GetNearClip()const;
	float	GetFarClip()const;


	/// <summary>
	/// �r���[�}�g���N�X�E�v���W�F�N�V�����}�g���N�X���v�Z
	/// </summary>
	void CalculateMatrix();

protected:
	struct Param;
	std::unique_ptr<Param> m_pParam;
};


/// <summary>
/// �r���[
/// </summary>
class ViewBase
{
protected:
	/// <summary>
	/// �v���e�N�g�R���X�g���N�^
	/// </summary>
	explicit ViewBase();
	/// <summary>
	/// �v���e�N�g�f�X�g���N�^
	/// </summary>
	virtual ~ViewBase();

public:
	/// <summary>
	/// �X�V�������L�����ǂ���
	/// </summary>
	/// <returns></returns>
	bool IsUpdateActive()const;
	/// <summary>
	/// �X�V�������L�����ǂ����ݒ�
	/// </summary>
	void SetUpdateActive(bool active);

	virtual void OnPreCreate() {}
	virtual void OnCreate() {}
	virtual void Update() {}

	virtual void SetCreated(bool b);

	/// <summary>
	/// �`��Ɏg�p����J�������擾
	/// </summary>
	/// <returns></returns>
	virtual const std::shared_ptr<Camera>& GetTargetCamera()const = 0;

	/// <summary>
	/// �`�悷��r���[�|�[�g���擾
	/// </summary>
	/// <returns></returns>
	virtual const Viewport& GetTargetViewport()const = 0;


private:
	struct Param;
	std::shared_ptr<Param> m_pParam;
};


/// <summary>
/// �V���O���r���[
/// </summary>
class SingleView
	: public ViewBase
{
public:
	explicit SingleView(const std::shared_ptr<Stage>& pStage);
	virtual ~SingleView();

	/// <summary>
	/// �X�V����
	/// </summary>
	virtual void Update()override;

	/// <summary>
	/// �r���[�|�[�g��ݒ�
	/// </summary>
	/// <param name="v"></param>
	void SetViewport(const Viewport& viewport);
	/// <summary>
	/// �r���[�|�[�g���擾
	/// </summary>
	/// <returns></returns>
	const Viewport& GetViewport()const;
	/// <summary>
	/// �J������ݒ�
	/// </summary>
	const void SetCamera(const std::shared_ptr<Camera>& camera);
	/// <summary>
	/// �J�������擾
	/// </summary>
	virtual const std::shared_ptr<Camera>& GetTargetCamera()const override;
	/// <summary>
	/// �r���[�|�[�g���擾
	/// </summary>
	virtual const Viewport& GetTargetViewport()const override;

private:
	struct Param;
	std::unique_ptr<Param> m_pParam;
};

/// <summary>
/// ���C�g�\����
/// </summary>
class Light
{
public:
	Float3 m_direction;	// ���C�g�̌���
	Float4 m_diffuse;	// �f�B�t���[�Y
	Float4 m_specular;	// �X�y�L�����[
	Float4 m_ambient;	// �A���r�G���g

	Light();
	Light(const Float3& dir, const Float4& dif, const Float4& spe,const Float4& amb);
	~Light() {}

	/// <summary>
	/// ���C�g�ʒu����̌�����ݒ�
	/// </summary>
	/// <param name="position">���C�g�̈ʒu</param>
	void SetPositionToDirectional(const Float3& position);
};

/// <summary>
/// ���C�g�I�u�W�F�N�g�x�[�X
/// </summary>
class LightBase
	: public GameObject
{
public:

protected:
	/// <summary>
	/// �v���e�N�g�R���X�g���N�^
	/// </summary>
	/// <param name="pStage"></param>
	explicit LightBase(const std::shared_ptr<Stage>& pStage);
	/// <summary>
	/// �v���e�N�g�f�X�g���N�^
	/// </summary>
	virtual ~LightBase();

public:
	/// <summary>
	/// �`��Ɏg�p���郉�C�g���擾
	/// </summary>
	/// <returns></returns>
	virtual const Light& GetTargetLight()const = 0;

private:
};


/// <summary>
/// �V���O�����C�g
/// </summary>
class SingleLight
	: public LightBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pStage"></param>
	explicit SingleLight(const std::shared_ptr<Stage>& pStage);
	virtual ~SingleLight();

	virtual void Destroy()override;

	/// <summary>
	/// ���C�g��ݒ�
	/// </summary>
	void SetLight(const Light& light);
	const Light& GetLight()const;
	Light& GetLight();
	/// <summary>
	/// �`��Ɏg�p���郉�C�g���擾
	/// </summary>
	const Light& GetTargetLight()const override;

private:
	struct Param;
	std::unique_ptr<Param> m_pParam;
};
