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
	//�����_���f�o�C�X
	std::random_device rand_device;

	//�����Z���k�E�c�C�X�^
	std::mt19937 mt(rand_device());

	// [min, max] �͈͂̈�l����
	std::uniform_int_distribution<> randint(min, max);  

	//������Ԃ�
	return randint(mt);
}

const double Rand::RandomMachine(const double& min, const double& max) const
{
	//�����_���f�o�C�X
	std::random_device rand_device;

	//�����Z���k�E�c�C�X�^
	std::mt19937 mt(rand_device());

	// [min, max] �͈͂̈�l����
	std::uniform_real_distribution<> randdouble(min, max);

	return randdouble(mt);
}
