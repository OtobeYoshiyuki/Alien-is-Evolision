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
		//�R���X�g���N�^
		Array() = default;

	public:
		//�f�X�g���N�^
		virtual ~Array() = default;

	public:
		//�w�肵���^��1�����z��𐶐�(function�͏����ʒu������ۂɎg�p)
		template <typename T>
		const std::vector<T> CreateSingleArray(int size, std::function<T(int)> func);
	};

	//�w�肵���^��1�����z��𐶐�(function�͏����l������ۂɎg�p)
	template<typename T>
	inline const std::vector<T> Array::CreateSingleArray(int size, std::function<T(int)> func)
	{
		//�z���錾
		std::vector<T> tArray;

		//for���[�v�ŉ�
		for (int i = 0; i < size; i++)
		{
			tArray.push_back(func(i));
		}

		//�z���Ԃ�
		return tArray;
	}
}

