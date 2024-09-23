#include "pch.h"
#include "Ranking.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

Ranking::Ranking() :
	m_level(""),//レベル
	m_score(nullptr),//スコア
	m_timer(nullptr),//タイマー
	m_blink(nullptr),//点滅
	m_color(Colors::White)//色
{
	//スコアの生成
	m_score = std::make_unique<Score>();
	m_score->SetSoundCheck(true);
	
	//タイマーの生成
	m_timer = std::make_unique<TimerJudge>();

	//点滅の生成
	m_blink = std::make_unique<Blinking>();
	m_blink->SetTarget(0.5f);
	
	//色の設定
	m_color = Colors::Black;
}

void Ranking::Update()
{
	//スコアの更新
	m_score->Update();

	//タイマーの更新
	m_timer->Update();

	//点滅の更新
	m_blink->Update();
}
