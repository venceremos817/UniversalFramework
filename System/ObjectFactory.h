#pragma once

#include <memory>

class ObjectFactory
{
public:
	/// <summary>
	/// オブジェクト作成
	/// </summary>
	/// <param name="...params">可変長パラメータ</param>
	/// <returns>作成したオブジェクト(shared_ptr)</returns>
	template<typename T, typename... Ts>
	static std::shared_ptr<T> Create(Ts&&... params);
};

template<typename T, typename... Ts>
inline std::shared_ptr<T> ObjectFactory::Create(Ts&&... params)
{
	std::shared_ptr<T> obj = std::shared_ptr<T>(new T(params...));
	// 仮想関数呼び出し
	obj->OnPreCreate();
	obj->OnCreate();
	obj->SetCreated(true);
	return obj;
}
