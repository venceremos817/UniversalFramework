#pragma once
#include <float.h>
#include <cmath>


// float��r���덷�p
_NODISCARD static inline bool AdjEqual(const float num1, const float num2)
{
	return fabsf(num1 - num2) <= DBL_EPSILON * fmaxf(1, fmaxf(fabsf(num1), fabsf(num2)));
}

/// <summary>
/// 0.0f�`1.0f�̗������擾
/// </summary>
/// <returns></returns>
_NODISCARD static inline float RandZeroToOne()
{
	double prob = (double)rand() / (double)RAND_MAX;
	if (prob <= 0.0)
	{
		prob = 0.0;
	}
	return (float)prob;
}

enum class LerpMode
{
	Linear,		// 1���֐�(���`)
	EaseIn,		// 2���֐� �������㏸
	EaseOut,	// 2���֐� �Ōオ�������
	Cube,		// 3���֐�
	Cos,		// �R�T�C��
};

/// <summary>
/// �⊮�v�Z
/// </summary>
/// <param name="start">�J�n�l</param>
/// <param name="end">�I���l</param>
/// <param name="startTime">�J�n����</param>
/// <param name="endTime">�I������</param>
/// <param name="nowTime">���ݎ���</param>
/// <param name="r">�v�Z���@</param>
/// <returns>�⊮��������</returns>
template<class T>
static inline T CalculateLerp(const T & start, const T & end, float startTime, float endTime, float nowTime, LerpMode r = LerpMode::Linear)
{
	float t = (nowTime - startTime) / (endTime - startTime);
	if (t < 0.0f)
		t = 0.0f;
	else if (t > 1.0f)
		t = 1.0f;

	//-- ��Ԓl��ݒ�
	float cal_rate;	// 1���֐��⊮�l�ɕϊ�(�f�t�H���g
	switch (r)
	{
	case LerpMode::Linear:
		cal_rate = t;
		break;
	case LerpMode::EaseIn:
		cal_rate = t * t;
		break;
	case LerpMode::EaseOut:
		cal_rate = t * (2.0f - t);
		break;
	case LerpMode::Cube:
		cal_rate = t * t * (3.0f - 2.0f * t);
		break;
	case LerpMode::Cos:
		cal_rate = (1.0f - cos(t*DirectX::XM_PI)) * 0.5f;
		break;
	}

	return start * (1.0f - cal_rate) + end * cal_rate;
}
