#pragma once
#include <map>
#include "ADX2/Adx2.h"
#include "Audio/CueSheet_0.h"
#include "OtobeLib/FrameWork/Singleton/Singleton.h"

//ADX2�̃w���p�[�N���X
namespace OtobeLib
{
	class SoundManager:public Singleton<SoundManager>
	{
		friend Singleton<SoundManager>;
	private:
		//�A�z�z��ŁA�L���[��ID���Ǘ�����
		std::map<int, int>	m_soundId;

	private:
		//�R���X�g���N�^
		SoundManager() = default;

	public:
		//�f�X�g���N�^
		~SoundManager() = default;

		//�Đ�
		void Play(const int& id);

		//��~
		void Stop(const int& id);
	};
}

