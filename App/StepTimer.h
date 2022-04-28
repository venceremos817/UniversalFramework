#pragma once

#include <Windows.h>


class StepTimer
{
	typedef long long int64;
	typedef unsigned long long uint64;
	typedef unsigned int uint32;
public:
	StepTimer();
	~StepTimer() {};

	/// <summary>
	/// �O��Update�Ăяo������o�߂����J�E���^���擾
	/// </summary>
	/// <returns></returns>
	uint64	GetElapsedTicks()const { return m_elapsedTicks; }

	/// <summary>
	/// �O��Update�Ăяo������o�߂����b�����擾
	/// </summary>
	/// <returns></returns>
	double	GetElapsedSeconds()const { return TicksToSeconds(m_elapsedTicks); }

	/// <summary>
	/// �v���O�������J�n���Ă���o�߂����J�E���^���擾
	/// </summary>
	/// <returns></returns>
	uint64	GetTotalTicks()const { return m_totalTicks; }

	/// <summary>
	/// �v���O�������J�n���Ă���o�߂����b�����擾
	/// </summary>
	/// <returns></returns>
	double	GetTotalSeconds()const { return TicksToSeconds(m_totalTicks); }

	/// <summary>
	/// �v���O�����J�n����̍��v�X�V�񐔂��擾
	/// </summary>
	/// <returns></returns>
	uint32	GetFrameCount()const { return m_frameCount; }

	/// <summary>
	/// ���݂̃t���[�����[�g���擾
	/// </summary>
	/// <returns></returns>
	uint32	GetFPS()const { return m_framesPerSecond; }

	/// <summary>
	/// �^�C���X�e�b�v�̃��[�h���Œ�/�ςɂ���
	/// </summary>
	/// <param name="isFixedTimeStep"></param>
	void SetFixedTimeStep(bool isFixedTimeStep) { m_isFixedTimeStep = isFixedTimeStep; }

	/// <summary>
	/// �Œ�^�C���X�e�b�v���[�h��Update�̌Ăяo���p�x��ݒ�
	/// </summary>
	/// <param name="targetElapsed">�Ăяo���p�x(�J�E���^)</param>
	void SetTargetElapsedTicks(uint64 targetElapsed) { m_targetElapsedTicks = targetElapsed; }

	/// <summary>
	/// �Œ�^�C���X�e�b�v���[�h��Update�̌Ăяo���p�x��ݒ�
	/// </summary>
	/// <param name="targetElapsed">�Ăяo���p�x(����)</param>
	void SetTargetElapsedSeconds(double targetElapsed) { m_targetElapsedTicks = SecondsToTicks(targetElapsed); }

	/// <summary>
	/// �Ăяo���̍X�V
	/// �Ӑ}�I�ȃ^�C�~���O�̕s�A�����̂���(�u���b�NIO����Ȃ�)������Ăяo���ƁA
	/// �Œ�^�C���X�e�b�v���W�b�N�ɂ���Ĉ�A�̃L���b�`�A�b�v���{�s�����̂�����ł���
	/// </summary>
	void ResetElapsedTime();

	/// <summary>
	/// �^�C�}�[��Ԃ��X�V���A�w���Update�֐���K�؂ȉ񐔂����Ăяo��
	/// </summary>
	/// <param name="update">�Ăяo���ׂ�Update�֐�</param>
	template<typename TUpdate>
	void Tick(const TUpdate& update);


public:
	/// <summary>
	/// �e�B�b�N�����Ԃɕϊ�
	/// </summary>
	/// <param name="ticks"></param>
	/// <returns></returns>
	static double	TicksToSeconds(uint64 ticks) { return static_cast<double>(ticks) / TicksPerSecond; }

	/// <summary>
	/// ���Ԃ��e�B�b�N�ɕϊ�
	/// </summary>
	/// <param name="seconds"></param>
	/// <returns></returns>
	static uint64	SecondsToTicks(double seconds) { return static_cast<uint64>(seconds*TicksPerSecond); }


public:
	static const uint64 TicksPerSecond = 10000000;	// 1�b������10000000�e�B�b�N


private:
	LARGE_INTEGER		m_qpcFrequency;
	LARGE_INTEGER		m_qpcLastTime;
	uint64	m_qpcMaxDelta;

	uint64	m_elapsedTicks;
	uint64	m_totalTicks;
	uint64	m_leftOverTicks;

	uint32	m_frameCount;
	uint32	m_framesPerSecond;
	uint32	m_framesThisSecond;
	uint64	m_qpcSecondCounter;

	bool	m_isFixedTimeStep;
	uint64	m_targetElapsedTicks;

	StepTimer(const StepTimer&) = delete;
	StepTimer& operator=(const StepTimer&) = delete;
	StepTimer(const StepTimer&&) = delete;
	StepTimer& operator=(const StepTimer&&) = delete;
};

template<typename TUpdate>
inline void StepTimer::Tick(const TUpdate & update)
{
	// ���݂̎������N�G������
	LARGE_INTEGER currentTime;

	if (!QueryPerformanceCounter(&currentTime))	// �Ăяo�����u�Ԃ̃J�E���g����64bit�l�Ŏ擾
	{
		// TODO : �G���[����
	}

	uint64 timeDelta = currentTime.QuadPart - m_qpcLastTime.QuadPart;

	m_qpcLastTime = currentTime;
	m_qpcSecondCounter += timeDelta;

	// �ɒ[�ɑ傫�Ȏ��ԍ����N�����v
	if (timeDelta > m_qpcMaxDelta)
	{
		timeDelta = m_qpcMaxDelta;
	}

	// QPC�P�ʂ�W���̖ڐ���`���ɕϊ�����B
	timeDelta *= TicksPerSecond;
	timeDelta /= m_qpcFrequency.QuadPart;

	uint32 lastFrameCount = m_frameCount;

	if (m_isFixedTimeStep)
	{
		/*	�Œ�^�C���X�e�b�v�X�V
		�N�����v�����^�[�Q�b�g�o�ߎ���(1/4ms�ȓ�)�ɐڋ߂��āA�A�v���P�[�V���������s����Ă���ꍇ
		�^�[�Q�b�g�l�ɐ��m�Ɉ�v������N���b�N�B����ɂ��A���������֌W�ȃG���[�̔�����h���B
		���̃N�����v�Ȃ���60fps��v�������Q�[���͌Œ肳�ꂽ�X�V(59.94NTSC�f�B�X�v���C��ŗL����vsync�Ŏ��s)��
		�ŏI�I�ɂ̓t���[�����h���b�v����̂ɏ\���ȏ������G���[��ݐς���B
		�킸���ȋ͍����[���Ɏ����Ă����X���[�Y�ɐi�s����悤�ɂ���B
		*/

		if (abs(static_cast<int64>(timeDelta - m_targetElapsedTicks)) < TicksPerSecond / 4000)
		{
			timeDelta = m_targetElapsedTicks;
		}

		m_leftOverTicks += timeDelta;

		while (m_leftOverTicks >= m_targetElapsedTicks)
		{
			m_elapsedTicks = m_targetElapsedTicks;
			m_totalTicks += m_targetElapsedTicks;
			m_leftOverTicks -= m_targetElapsedTicks;
			++m_frameCount;

			update();
		}
	}
	else
	{
		// �σ^�C���X�e�b�v�X�V���W�b�N
		m_elapsedTicks = timeDelta;
		m_totalTicks += timeDelta;
		m_leftOverTicks = 0;
		++m_frameCount;

		update();
	}

	// ���݂̃t���[�����[�g��ǐ�
	if (m_frameCount != lastFrameCount)
	{
		++m_framesThisSecond;
	}

	if (m_qpcSecondCounter >= static_cast<uint64>(m_qpcFrequency.QuadPart))
	{
		m_framesPerSecond = m_framesThisSecond;
		m_framesThisSecond = 0;
		m_qpcSecondCounter %= m_qpcFrequency.QuadPart;
	}
}
