#include "pch.h"
#include "Animation.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

AnimationMap Animation::s_AniGrop = AnimationMap();

Animation::Animation():m_AniCount(NULL),m_AniFlag(false),m_LoopFlag(false),m_NowInfo(""),
m_LoopInfo(""),m_NowRect(RECT())
{
}

void Animation::Initialize()
{
}

void Animation::Update()
{
	//現在のアニメーションからアニメーションの終了時間を取得する
	int endtime = this->GetEndTime(this->GetNowInfo());

	//アニメーションの終了時間になったら、次のアニメーションに切り替える
	if (this->GetAniCount() > endtime)
	{
		//アニメーションをループ再生する
		if (this->GetLoopFlag())
		{
			//現在の情報を更新する
			this->SetNowInfo(this->GetNextAnimation(this->GetNowInfo()));
			//カウントを初期化する
			m_AniCount = NULL;
		}
		//アニメーションをループ再生しない
		else
		{
			//アニメーションが終わりじゃないとき
			if (this->GetLoopInfo() != this->GetNowInfo())
			{
				//現在の情報を更新する
				this->SetNowInfo(this->GetNextAnimation(this->GetNowInfo()));
				//カウントを初期化する
				m_AniCount = NULL;
			}
			//アニメーションが終わりのとき
			else
			{
				//現在の情報を更新する
				this->SetNowInfo(this->GetNextAnimation(this->GetNowInfo()));
				//カウントを初期化する
				m_AniCount = NULL;
				//カウントの更新を止める
				this->SetAniFlag(false);
			}
		}
	}

	//現在のアニメーションを更新する
	this->SetNowAnimation(this->GetAnimation(this->GetNowInfo()));

	//アニメーションの更新を行う
	if (this->GetAniFlag())m_AniCount++;
}

void Animation::Finalize()
{
}

void Animation::SetNewAnimation(Animation* ani)
{
	//引数の情報をもとに、新しいアニメーションの情報を設定する

	//アニメーションのコンテナを再設定する
	m_Animation = ani->GetAnimationMap();

	//次のアニメーションのコンテナを再設定する
	m_NextAnimation = ani->GetNextAnimationMap();

	//アニメーションの終了時間のコンテナを再設定する
	m_EndTime = ani->GetEndTimeMap();

	//現在のアニメーションを設定する
	m_NowRect = ani->GetNowAnimation();

	//現在のアニメーションの情報を設定する
	m_NowInfo = ani->GetNowInfo();

	//アニメーションのループ情報を設定する
	m_LoopInfo = ani->GetLoopInfo();

	//アニメーションのカウントを設定する
	m_AniCount = ani->GetAniCount();

	//アニメーションの更新フラグを設定する
	m_AniFlag = ani->GetAniFlag();

	//アニメーションのループフラグを取得する
	m_LoopFlag = ani->GetLoopFlag();
}

void Animation::InitMiniMapAnimation()
{
	//アニメーション情報を上書きする
	
	////アニメーションのコンテナを初期化する
	//m_Animation.clear();

	////次回のアニメーションを初期化する
	//m_NextAnimation.clear();

	////アニメーションの終了時間を初期化する
	//m_EndTime.clear();

	////現在のアニメーション情報を初期化する
	//m_NowInfo.clear();

	////アニメーションのループ情報を初期化する
	//m_LoopInfo.clear();

	//アニメーションの更新フラグを初期化する
	m_AniFlag = false;

	//アニメーションのループフラグを初期化する
	m_LoopFlag = false;

	//現在のアニメーションを初期化する
	//m_NowRect = RECT{};

	//アニメーションカウントを初期化する
	m_AniCount = NULL;
}

