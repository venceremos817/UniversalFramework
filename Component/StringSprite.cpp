#include "StringSprite.h"
#include <App.h>
#include <ThrowException.h>
#include <sstream>

struct StringSprite::Param
{
	std::wstring text;
	float fontSize;
	Float2 textBlockSize;	// �t�H���g��
	std::wstring fontName;
	Float4 fontColor;
	Float4 backColor;
	Float2 startPosition;
	StringSprite::TextAlignment textAlignment;

	Float2 backTextMargin;	// �w�i�F��h��Ԃ��}�[�W��

	DWRITE_TEXT_METRICS textMetrics;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush;
	Microsoft::WRL::ComPtr<ID2D1DrawingStateBlock> stateBlock;
	Microsoft::WRL::ComPtr<IDWriteTextLayout> textLayout;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> textFormat;

	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> backBrush;

	Param()
		: text()
		, fontSize(16.0f)
		, textBlockSize(128.0f, 32.0f)
		, fontName(L"MS�S�V�b�N")
		, fontColor(1.0f, 1.0f, 1.0f, 1.0f)
		, backColor(0.0f, 0.0f, 0.0f, 0.0f)
		, startPosition(fontSize, fontSize)
		, textAlignment(TextAlignment::Left)
		, backTextMargin(4.0f, 4.0f)
	{}
	~Param()
	{}
};

StringSprite::StringSprite(const std::shared_ptr<GameObject>& pGameObject)
	: Component(pGameObject)
	, m_param(new Param())
{
	ZeroMemory(&m_param->textMetrics, sizeof(DWRITE_TEXT_METRICS));

	auto dev = APP->GetDeviceResources();
	auto D2DFactory = dev->GetD2DFactory();
	auto DWriteFactory = dev->GetDWriteFactory();
	auto D2DDeviceContext = dev->GetD2DDeviceContext();

	// �t�H���g�쐬
	HRESULT hr = DWriteFactory->CreateTextFormat(
		m_param->fontName.c_str(),
		nullptr,
		DWRITE_FONT_WEIGHT_LIGHT,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		m_param->fontSize,
		L"ja",
		&m_param->textFormat
	);

	// �t�H���g�A���C�����g�ݒ�
	hr = m_param->textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	// �t�H���g�X�e�[�g�u���b�N�ݒ�
	hr = D2DFactory->CreateDrawingStateBlock(&m_param->stateBlock);

	// �t�H���g�u���V�쐬
	auto color = D2D1::ColorF(m_param->fontColor.x, m_param->fontColor.y, m_param->fontColor.z, m_param->fontColor.w);
	hr = D2DDeviceContext->CreateSolidColorBrush(
		color, &m_param->brush
	);

	// �o�b�N�O���E���h�u���V�쐬
	color = D2D1::ColorF(m_param->backColor.x, m_param->backColor.y, m_param->backColor.z, m_param->backColor.w);
	hr = D2DDeviceContext->CreateSolidColorBrush(
		color, &m_param->backBrush
	);

}

StringSprite::~StringSprite()
{
}

void StringSprite::OnCreate()
{
	GetGameObject()->SetDrawActive(true);
	GetGameObject()->SetIsAlpha(true);
}

void StringSprite::Update()
{
	auto dev = APP->GetDeviceResources();
	auto D2DDeviceContext = dev->GetD2DDeviceContext();
	auto DWriteFactory = dev->GetDWriteFactory();

	// �t�H���g���C�A�E�g�쐬
	HRESULT hr = DWriteFactory->CreateTextLayout(
		m_param->text.c_str(),
		(UINT32)m_param->text.length(),
		m_param->textFormat.Get(),
		m_param->textBlockSize.x,	// ���̓e�L�X�g�ő啝
		m_param->textBlockSize.y,	// ���̓e�L�X�g�ő卂��
		&m_param->textLayout
	);

	// �e�L�X�g���g���N�X�擾
	hr = m_param->textLayout->GetMetrics(&m_param->textMetrics);
}

void StringSprite::Draw()
{
	auto dev = APP->GetDeviceResources();
	auto D2DDeviceContext = dev->GetD2DDeviceContext();
	auto DWriteFactory = dev->GetDWriteFactory();
	auto elapssedTime = APP->GetElapsedTime();

	D2D_RECT_F backRect = D2D1::RectF(
		m_param->startPosition.x, m_param->startPosition.y,
		m_param->startPosition.x + m_param->textBlockSize.x,
		m_param->startPosition.y + m_param->textBlockSize.y
	);

	backRect.left -= m_param->startPosition.x;
	backRect.top -= m_param->startPosition.y;
	backRect.right += m_param->startPosition.x;
	backRect.bottom += m_param->startPosition.y;

	D2DDeviceContext->SaveDrawingState(m_param->stateBlock.Get());

	D2DDeviceContext->BeginDraw();

	// �o�b�N�O���E���h�`��
	D2DDeviceContext->FillRectangle(&backRect, m_param->backBrush.Get());

	D2D1::Matrix3x2F screenTranslation = D2D1::Matrix3x2F::Translation(
		m_param->startPosition.x, m_param->startPosition.y
	);
	D2DDeviceContext->SetTransform(screenTranslation);

	DWRITE_TEXT_ALIGNMENT alignment = DWRITE_TEXT_ALIGNMENT_LEADING;

	switch (m_param->textAlignment)
	{
	case TextAlignment::Left:
		alignment = DWRITE_TEXT_ALIGNMENT_LEADING;
		break;
	case TextAlignment::Center:
		alignment = DWRITE_TEXT_ALIGNMENT_CENTER;
		break;
	case TextAlignment::Right:
		alignment = DWRITE_TEXT_ALIGNMENT_TRAILING;
		break;
	}

	// �e�L�X�g�A���C�����g�ݒ�
	HRESULT hr = m_param->textFormat->SetTextAlignment(alignment);

	D2DDeviceContext->DrawTextLayout(
		D2D1::Point2F(0.0f, 0.0f),
		m_param->textLayout.Get(),
		m_param->brush.Get()
	);

	hr = D2DDeviceContext->EndDraw();

	D2DDeviceContext->RestoreDrawingState(m_param->stateBlock.Get());
}

void StringSprite::SetFont(const std::wstring & fontName, float fontSize)
{
	ZeroMemory(&m_param->textMetrics, sizeof(DWRITE_TEXT_METRICS));

	auto dev = APP->GetDeviceResources();
	auto D2DFactory = dev->GetD2DFactory();
	auto DWriteFactory = dev->GetDWriteFactory();
	auto D2DDeviceContext = dev->GetD2DDeviceContext();

	// �t�H���g�쐬
	DWriteFactory->CreateTextFormat(
		fontName.c_str(),
		nullptr,
		DWRITE_FONT_WEIGHT_LIGHT,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		fontSize,
		L"ja",
		&m_param->textFormat
	);

	m_param->fontName = fontName;
	m_param->fontSize = fontSize;

	// �t�H���g�A���C�����g�ݒ�
	m_param->textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	// �t�H���g�X�e�[�g�u���b�N�ݒ�
	D2DFactory->CreateDrawingStateBlock(&m_param->stateBlock);
}

void StringSprite::SetText(const std::wstring & str)
{
	m_param->text = str;
}

void StringSprite::AddText(const std::wstring & str)
{
	m_param->text += str;
}

void StringSprite::SetBackColor(const Float4 & color)
{
	auto dev = APP->GetDeviceResources();
	auto D2DDeviceContext = dev->GetD2DDeviceContext();

	m_param->backColor = color;
	auto colorBrush = D2D1::ColorF(color.x, color.y, color.z, color.w);

	// �o�b�N�u���V�쐬
	D2DDeviceContext->CreateSolidColorBrush(
		colorBrush, &m_param->backBrush
	);
}

void StringSprite::SetStartPosition(const Float2 & pos)
{
	m_param->startPosition = pos;
}

void StringSprite::SetTextRect(float left, float top, float right, float bottom)
{
	SetStartPosition(Float2(left, top));
	SetTextBlockWidth(abs(left - right));
	SetTextBlockHeight(abs(top - bottom));
}

std::wstring StringSprite::FloatToWstr(float val, std::streamsize precision, FloatModify modify)
{
	std::wstring str;
	std::wostringstream stream;

	// ���������_�̐��x
	if (precision > 0)
		stream.precision(precision);
	switch (modify)
	{
	case StringSprite::FloatModify::Default:
		stream << val;
		break;
	case StringSprite::FloatModify::Fixed:
		stream << std::fixed << val;
		break;
	case StringSprite::FloatModify::Scientific:
		stream << std::scientific << val;
		break;
	default:
		stream << val;
		break;
	}
	str = stream.str();
	return str;
}

std::wstring StringSprite::UintToWstr(UINT val, NumModify modify)
{
	std::wstring str;
	std::wostringstream stream;

	switch (modify)
	{
	case StringSprite::NumModify::Oct:
		stream << std::oct << val;
		break;
	case StringSprite::NumModify::Dec:
		stream << std::dec << val;
		break;
	case StringSprite::NumModify::Hex:
		stream << std::hex << val;
		break;
	default:
		stream << std::dec << val;
		break;
	}
	str = stream.str();
	return str;
}

std::wstring StringSprite::IntToWstr(int val, NumModify modify)
{
	std::wstring str;
	std::wostringstream stream;

	switch (modify)
	{
	case StringSprite::NumModify::Oct:
		stream << std::oct << val;
		break;
	case StringSprite::NumModify::Dec:
		stream << std::dec << val;
		break;
	case StringSprite::NumModify::Hex:
		stream << std::hex << val;
		break;
	default:
		stream << std::dec << val;
		break;
	}
	str = stream.str();
	return str;
}

void StringSprite::SetTextBlockWidth(float widh)
{
	m_param->textBlockSize.x = widh;
}

void StringSprite::SetTextBlockHeight(float height)
{
	m_param->textBlockSize.y = height;
}
