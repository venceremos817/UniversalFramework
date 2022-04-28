#pragma once

#include <Collision.h>

class Ray
{
	Ray() = delete;
public:
	Ray(const Float3& startPos, const Float3& direction);
	~Ray() {}


	/// <summary>
	/// ���ƃ��C�̏Փ˔���
	/// �����ƏՓ˒n�_���i�[���ĕԂ�
	/// </summary>
	/// <param name="rayStartPos">���C�̊J�n�_</param>
	/// <param name="rayDirection">���C�̕���</param>
	/// <param name="sp">���{�����[��</param>
	/// <param name="refDistance">�������i�[����</param>
	/// <param name="refHitPos">�Փ˒n�_���i�[����</param>
	/// <returns></returns>
	static bool IsHitSphere(const Float3& rayStartPos, const Float3& rayDirection, const SphereBoundaryVolume& sp, float& refDistance, Float3& refHitPos);
	/// <summary>
	/// ���Ƃ̏Փ˔���
	/// </summary>
	/// <param name="sp">���E�{�����[��</param>
	/// <param name="refDistance">�Փˋ�����Ԃ�</param>
	/// <param name="refHitPos">�Փˍ��W��Ԃ�</param>
	/// <returns></returns>
	bool IsHitSphere(const SphereBoundaryVolume& sp, float& refDistance, Float3& refHitPos);

#ifdef _DEBUG
	void Draw();
#endif // DEBUG


private:
	Float3 m_startPos;
	Float3 m_direction;
};
