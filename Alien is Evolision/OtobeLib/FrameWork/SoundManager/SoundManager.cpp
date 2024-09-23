#include "pch.h"
#include "SoundManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

void SoundManager::Play(const int& id)
{
	//ADX2‚ðŽæ“¾‚·‚é
	ADX2* adx2 = ADX2::GetInstance();

	int no = adx2->Play(id);

	//Ä¶‚·‚é
	m_soundId[id] = no;
}

void SoundManager::Stop(const int& id)
{
	//ADX2‚ðŽæ“¾‚·‚é
	ADX2* adx2 = ADX2::GetInstance();

	//’âŽ~‚·‚é
	adx2->Stop(m_soundId[id]);	
}
