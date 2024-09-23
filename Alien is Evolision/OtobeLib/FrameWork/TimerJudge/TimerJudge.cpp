#include "pch.h"
#include "TimerJudge.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

TimerJudge::TimerJudge():
	m_FrameCount(0.0f),//フレーム数
	m_LimitTimer(NULL),//制限時間
	m_TimerUp(false),//時間経過
	m_Judge(false),//終了フラグ
	m_correction(1.0f)//1秒単位の補正値
{
}

void TimerJudge::Initialize(const int& limit, const bool& up)
{
	//制限時間を設定する
	m_LimitTimer = limit;

	//タイマーのカウントするフラグを設定する
	m_TimerUp = up;
}

void TimerJudge::Update()
{
	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//タイマーが止まっているときは、何もしない
	if (!m_TimerUp)return;

	//制限時間が設定されていなければ、何もしない
	//if (m_LimitTimer <= 0.0f)return;

	//フレーム数が制限時間を超えたら、Judgeをtrueにする
	//タイマーの更新を止める
	if (m_LimitTimer < 0.0f)
	{
		m_Judge = true;
		m_TimerUp = false;
		return;
	}

	//タイマーを更新する
	m_LimitTimer -= staticData->Get_FloatData("elapsedTime") * m_correction;
}

//制限時間から、秒数を求めて、文字列に変換する
const std::string TimerJudge::AskTimeStringSecound()
{
	//現在の時間から、分単位に直す
	int secound = (int)m_LimitTimer % ONESECOND;

	//制限時間から、秒数の部分を抜き出す
	std::string str = std::to_string(secound);

	//１０秒未満の時は先頭に０を追加する
	if (secound < 10)return "0" + str;

	return str;
}

//制限時間から、分数を求めて、文字列に変換する
const std::string TimerJudge::AskTimeStringMinute()
{
	//現在の時間から、分単位に直す
	int minute = (int)m_LimitTimer / ONESECOND;

	//制限時間から、秒数の部分を抜き出す
	std::string str = std::to_string(minute);

	//１０分未満の時は先頭に０を追加する
	if (minute < 10)return "0" + str;

	return str;
}
