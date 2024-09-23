#include "pch.h"
#include "Score.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

Score::Score():
	m_score(NULL),//スコア
	m_saving(NULL),//貯蓄
	m_add(1),//加算量(デフォルトでは1)
	m_soundCheck(false)//音の停止を無視するフラグ
{
}

void Score::Initialize()
{
}

void Score::Update()
{
	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//貯蓄からスコアを加算していく
	if (m_saving > 0.0f)
	{
		//スコアをインクリメント
		m_score += m_frameSave * m_add;

		//貯蓄を使う
		m_saving -= m_frameSave;

		if (m_saving < 0.0f)
		{
			float sav = fabsf(m_saving);
			m_score -= sav;
			m_score = (int)m_score;
			m_saving = 0.0f;
			m_frameSave = 0.0f;
		}
	}
	else
	{
		if (!m_soundCheck)
		{
			//音を止める
			soundManager->Stop(CRI_CUESHEET_0__19_スコア加算);
		}
	}

	//スコアを補正する
	if (m_score < NULL)
	{
		m_score = NULL;
	}
}
