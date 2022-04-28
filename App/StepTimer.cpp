#include "StepTimer.h"

StepTimer::StepTimer()
	: m_elapsedTicks((uint64)0.0)
	, m_totalTicks((uint64)0.0)
	, m_leftOverTicks((uint64)0.0)
	, m_frameCount((uint32)0.0)
	, m_framesPerSecond((uint32)0.0)
	, m_framesThisSecond((uint32)0.0)
	, m_qpcSecondCounter((uint64)0.0)
	, m_isFixedTimeStep(false)
	, m_targetElapsedTicks((uint64)(TicksPerSecond / 60.0))
{
	if (!QueryPerformanceFrequency(&m_qpcFrequency))	// �V�X�e����1�b�Ԃ̃J�E���g��(���g��)��64bi�l�Ŏ擾
	{
		// TODO : �G���[����
	}

	if (!QueryPerformanceCounter(&m_qpcLastTime))	// �Ăяo�����u�Ԃ̃J�E���g����64bit�l�Ŏ擾
	{
		// TODO : �G���[����
	}

	// �ő�f���^��1�b��1/10�ɏ���������
	m_qpcMaxDelta = m_qpcFrequency.QuadPart / 10;
}

void StepTimer::ResetElapsedTime()
{
	if (QueryPerformanceCounter(&m_qpcLastTime))	// �Ăяo�����u�Ԃ̃J�E���g����64bit�l�Ŏ擾
	{
		// TODO : �G���[����
	}

	m_leftOverTicks = 0;
	m_framesPerSecond = 0;
	m_framesThisSecond = 0;
	m_qpcSecondCounter = 0;
}
