#pragma once
#include "OtobeLib/FrameWork/Singleton/Singleton.h"
namespace OtobeLib
{
	class Rand:public Singleton<Rand>
	{
		friend class Singleton<Rand>;//��{�I�Ƀ����o�ɃA�N�Z�X���Ȃ�
	public:
		//�R���X�g���N�^
		Rand();

		//�f�X�g���N�^
		~Rand()override{}

		//�ő�l�ƍŏ��l��������āA���̗�����Ԃ��iint�j
		const int RandomMachine(const int& min, const int& max)const;

		//�ő�l�ƍŏ��l��������āA���̗�����Ԃ��iDouble�j
		const double RandomMachine(const double& min, const double& max)const;
	};
}
