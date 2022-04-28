#pragma once

#include <GameObject.h>
#include <System/Resource/SharedResources.h>

class SceneInterface
{
public:
	/// <summary>
	/// ����������
	/// �R���|�[�l���g�ǉ���
	/// </summary>
	virtual void OnPreCreate() {}
	/// <summary>
	/// ����������
	/// �R���|�[�l���g�ݒ蓙
	/// </summary>
	virtual void OnCreate() {}
	/// <summary>
	/// �X�V����
	/// </summary>
	virtual void Update() {}
	/// <summary>
	/// �x��A�b�v�f�[�g
	/// </summary>
	virtual void LateUpDate() {}
	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw();
	/// <summary>
	/// �I������
	/// </summary>
	virtual void OnDestroy() {}

	/// <summary>
	/// �쐬�ς݃t���O�ݒ�
	/// </summary>
	/// <param name="created"></param>
	void SetCreated(bool created);
	/// <summary>
	/// �쐬�ς݂��ǂ������擾
	/// </summary>
	/// <returns></returns>
	bool IsCreated();

	/// <summary>
	/// �X�e�[�W��ݒ�
	/// </summary>
	/// <param name="stage"></param>
	void SetStage(const std::shared_ptr<Stage>& stage);
	/// <summary>
	/// �X�e�[�W���擾
	/// </summary>
	/// <returns></returns>
	std::shared_ptr<Stage> GetStage()const;


protected:
	SceneInterface();
	virtual ~SceneInterface();

private:
	struct Param;
	std::unique_ptr<Param> m_param;
};
