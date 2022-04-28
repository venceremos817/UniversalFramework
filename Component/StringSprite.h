#pragma once
#include <Component.h>
#include <dwrite.h>
#include <wrl/client.h>
#include <string>
#include <XMFLOAT_EX.h>

#pragma comment(lib,"Dwrite.lib")

/// <summary>
/// ������`��R���|�[�l���g
/// </summary>
class StringSprite
	:public Component
{
public:
	enum class TextAlignment
	{
		Left,		// �����킹
		Center,		// �������킹
		Right,		// �E���킹
	};

public:
	explicit StringSprite(const std::shared_ptr<GameObject>& pGameObject);
	virtual ~StringSprite();

	void OnCreate()override;
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update()override;
	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw()override;


	/// <summary>
	/// �t�H���g��ݒ�
	/// </summary>
	/// <param name="fontName">�t�H���g��</param>
	/// <param name="fontSize"></param>
	void SetFont(const std::wstring& fontName, float fontSize);
	/// <summary>
	/// �t�H���g�����擾
	/// </summary>
	/// <returns></returns>
	const std::wstring& GetFontName()const;
	/// <summary>
	/// �t�H���g�T�C�Y���擾
	/// </summary>
	/// <returns></returns>
	float GetFontSize()const;

	/// <summary>
	/// �e�L�X�g�A���C�����g��ݒ�
	/// </summary>
	/// <param name="alignment"></param>
	void SetTextAlignment(TextAlignment alignment);
	/// <summary>
	/// �e�L�X�g�̃A���C�����g���擾
	/// </summary>
	/// <returns></returns>
	TextAlignment GetTextAlignment()const;

	/// <summary>
	/// ��������擾
	/// </summary>
	/// <returns></returns>
	const std::wstring& GetText()const;
	/// <summary>
	/// �������ݒ�
	/// </summary>
	/// <param name="str"></param>
	void SetText(const std::wstring& str);
	/// <summary>
	/// �������ǉ�
	/// </summary>
	/// <param name="str"></param>
	void AddText(const std::wstring& str);

	/// <summary>
	/// �t�H���g�F��ݒ�
	/// </summary>
	/// <param name="color"></param>
	void SetFontColor(const Float4& color);
	/// <summary>
	/// �t�H���g�F���擾
	/// </summary>
	/// <returns></returns>
	const Float4& GetFontColor()const;
	/// <summary>
	/// �w�i�F��ݒ�
	/// </summary>
	void SetBackColor(const Float4& color);
	/// <summary>
	/// �w�i�F���擾
	/// </summary>
	/// <returns></returns>
	const Float4& GetBackColor()const;

	/// <summary>
	/// �w�i�̃e�L�X�g�}�[�W����ݒ�
	/// </summary>
	/// <param name="margine"></param>
	void SetBackTextMargine(const Float2& margine);
	/// <summary>
	/// �w�i�̃e�L�X�g�}�[�W�����擾
	/// </summary>
	/// <returns></returns>
	Float2 GetBackTextMargine()const;

	/// <summary>
	/// �e�L�X�g�u���b�N�̕���ݒ�
	/// </summary>
	/// <param name="widh"></param>
	void SetTextBlockWidth(float widh);
	/// <summary>
	/// �e�L�X�g�u���b�N�̍�����ݒ�
	/// </summary>
	/// <param name="height"></param>
	void SetTextBlockHeight(float height);

	/// <summary>
	/// �e�L�X�g���C�A�E�g���擾
	/// </summary>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<IDWriteTextLayout>& GetTextLayout()const;
	/// <summary>
	/// �e�L�X�g�t�H�[�}�b�g���擾
	/// </summary>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<IDWriteTextFormat>& GetTextFormat()const;

	/// <summary>
	/// �e�L�X�g���g���N�X���擾
	/// </summary>
	/// <returns></returns>
	const DWRITE_TEXT_METRICS& GetDriteTextMetrics()const;

	/// <summary>
	/// �X�^�[�g�ʒu��ݒ�
	/// </summary>
	/// <param name="pos"></param>
	void SetStartPosition(const Float2& pos);

	/// <summary>
	/// �e�L�X�g�̋�`��ݒ�
	/// </summary>
	/// <param name="rect"></param>
	void SetTextRect(float left, float top, float right, float bottom);


public:
	/// <summary>
	/// float�𕶎���ɕϊ�����`��
	/// </summary>
	enum class FloatModify
	{
		Default,	// ���������_
		Fixed,		// �������o��
		Scientific,	// e+09�Ȃǂ̌`��
	};
	/// <summary>
	/// ���������𕶎���(wstring)�ɕϊ�����
	/// </summary>
	/// <param name="val"></param>
	/// <param name="precision">���x</param>
	/// <param name="modify">�`��</param>
	/// <returns></returns>
	static std::wstring FloatToWstr(float val, std::streamsize precision = 0, FloatModify modify = FloatModify::Default);

	/// <summary>
	/// �����𕶎���ɕϊ�����`��
	/// </summary>
	enum class NumModify
	{
		Oct,	// 8�i��
		Dec,	// 10�i��
		Hex,	// 16�i��
	};
	/// <summary>
	/// UINT��wstring�ɕϊ�
	/// </summary>
	/// <param name="val"></param>
	/// <param name="modify">�i��</param>
	/// <returns></returns>
	static std::wstring UintToWstr(UINT val, NumModify modify = NumModify::Dec);
	/// <summary>
	/// 
	/// </summary>
	/// <param name="val"></param>
	/// <param name="modify"></param>
	/// <returns></returns>
	static std::wstring IntToWstr(int val, NumModify modify = NumModify::Dec);

private:
	struct Param;
	std::unique_ptr<Param> m_param;
};
