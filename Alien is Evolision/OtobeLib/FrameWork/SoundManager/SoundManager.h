#pragma once
#include <map>
#include "ADX2/Adx2.h"
#include "Audio/CueSheet_0.h"
#include "OtobeLib/FrameWork/Singleton/Singleton.h"

//ADX2のヘルパークラス
namespace OtobeLib
{
	class SoundManager:public Singleton<SoundManager>
	{
		friend Singleton<SoundManager>;
	private:
		//連想配列で、キューのIDを管理する
		std::map<int, int>	m_soundId;

	private:
		//コンストラクタ
		SoundManager() = default;

	public:
		//デストラクタ
		~SoundManager() = default;

		//再生
		void Play(const int& id);

		//停止
		void Stop(const int& id);
	};
}

