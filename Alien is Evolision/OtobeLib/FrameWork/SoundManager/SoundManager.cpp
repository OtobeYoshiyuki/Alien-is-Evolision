#include "pch.h"
#include "SoundManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

void SoundManager::Play(const int& id)
{
	//ADX2を取得する
	ADX2* adx2 = ADX2::GetInstance();

	int no = adx2->Play(id);

	//再生する
	m_soundId[id] = no;
}

void SoundManager::Stop(const int& id)
{
	//ADX2を取得する
	ADX2* adx2 = ADX2::GetInstance();

	//停止する
	adx2->Stop(m_soundId[id]);	
}
