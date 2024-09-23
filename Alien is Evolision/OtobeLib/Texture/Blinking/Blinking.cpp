#include "pch.h"
#include "Blinking.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

Blinking::Blinking():mState(true),mFrameCount(0.0f),mFrameFlag(false),mTarget(NULL),
mFinal(60),mFrameFinish(false),mTargetCount(0.0f)
{
}

Blinking::~Blinking()
{
}

void Blinking::Update()
{
	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//フレームカウントを更新する
	if (mFrameFlag)
	{
		mFrameCount += staticData->Get_FloatData("elapsedTime");
		mTargetCount += staticData->Get_FloatData("elapsedTime");
	}

	//フレームカウントが更新されていなかったら何もしない
	if (mFrameCount == NULL)return;

	//設定した間隔ごとに点滅させる
	if (mTargetCount > mTarget)
	{
		//フラグを切り替える
		mState = !mState;
		//ターゲットのカウントを初期化する
		mTargetCount = 0.0f;
		//終了時間になったら元に戻す
		if (mFrameFinish)
		{
			if (mFrameCount > mFinal)
			{
				mFrameCount = 0.0f;
				mState = true;
				mFrameFlag = false;
			}
		}
	}

}

bool Blinking::NowBlinking()
{
	//点滅していなかったらtrueを返す
	if (mState)return true;
	//点滅していたらfalseを返す
	return false;
}
