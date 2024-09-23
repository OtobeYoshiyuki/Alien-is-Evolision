#include "pch.h"
#include "Rand.h"
#include <random>
#include <algorithm>

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

Rand::Rand()
{
}

const int Rand::RandomMachine(const int& min, const int& max) const
{
	//ランダムデバイス
	std::random_device rand_device;

	//メルセンヌ・ツイスタ
	std::mt19937 mt(rand_device());

	// [min, max] 範囲の一様乱数
	std::uniform_int_distribution<> randint(min, max);  

	//乱数を返す
	return randint(mt);
}

const double Rand::RandomMachine(const double& min, const double& max) const
{
	//ランダムデバイス
	std::random_device rand_device;

	//メルセンヌ・ツイスタ
	std::mt19937 mt(rand_device());

	// [min, max] 範囲の一様乱数
	std::uniform_real_distribution<> randdouble(min, max);

	return randdouble(mt);
}
