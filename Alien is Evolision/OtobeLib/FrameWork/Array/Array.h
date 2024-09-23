#pragma once
#include "OtobeLib/FrameWork/Singleton/Singleton.h"
#include <vector>
#include <functional>
namespace OtobeLib
{
	class Array:public Singleton<Array>
	{
		friend Singleton<Array>;
	private:
		//コンストラクタ
		Array() = default;

	public:
		//デストラクタ
		virtual ~Array() = default;

	public:
		//指定した型の1次元配列を生成(functionは初期位置を入れる際に使用)
		template <typename T>
		const std::vector<T> CreateSingleArray(int size, std::function<T(int)> func);
	};

	//指定した型の1次元配列を生成(functionは初期値を入れる際に使用)
	template<typename T>
	inline const std::vector<T> Array::CreateSingleArray(int size, std::function<T(int)> func)
	{
		//配列を宣言
		std::vector<T> tArray;

		//forループで回す
		for (int i = 0; i < size; i++)
		{
			tArray.push_back(func(i));
		}

		//配列を返す
		return tArray;
	}
}

