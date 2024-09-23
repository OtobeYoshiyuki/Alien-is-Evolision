#pragma once
#include "OtobeLib/FrameWork/Singleton/Singleton.h"
namespace OtobeLib
{
	class Rand:public Singleton<Rand>
	{
		friend class Singleton<Rand>;//基本的にメンバにアクセスしない
	public:
		//コンストラクタ
		Rand();

		//デストラクタ
		~Rand()override{}

		//最大値と最小値をもらって、その乱数を返す（int）
		const int RandomMachine(const int& min, const int& max)const;

		//最大値と最小値をもらって、その乱数を返す（Double）
		const double RandomMachine(const double& min, const double& max)const;
	};
}
