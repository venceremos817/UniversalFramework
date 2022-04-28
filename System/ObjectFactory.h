#pragma once

#include <memory>

class ObjectFactory
{
public:
	/// <summary>
	/// �I�u�W�F�N�g�쐬
	/// </summary>
	/// <param name="...params">�ϒ��p�����[�^</param>
	/// <returns>�쐬�����I�u�W�F�N�g(shared_ptr)</returns>
	template<typename T, typename... Ts>
	static std::shared_ptr<T> Create(Ts&&... params);
};

template<typename T, typename... Ts>
inline std::shared_ptr<T> ObjectFactory::Create(Ts&&... params)
{
	std::shared_ptr<T> obj = std::shared_ptr<T>(new T(params...));
	// ���z�֐��Ăяo��
	obj->OnPreCreate();
	obj->OnCreate();
	obj->SetCreated(true);
	return obj;
}
