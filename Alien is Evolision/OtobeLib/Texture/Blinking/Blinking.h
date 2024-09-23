#pragma once
#include "OtobeLib/FrameWork/Graphics/Graphics.h"
namespace OtobeLib
{
	//点滅制御クラス
	class Blinking
	{
	private:
		//描画を管理するステート
		bool mState;			

		//フレームカウント
		float mFrameCount;		

		//フレーム更新フラグ
		bool mFrameFlag;		

		//点滅する感覚
		float mTarget;

		//点滅感覚のカウント
		float mTargetCount;

		//点滅の終了時間
		int mFinal;				

		//点滅の終了時間を適応するか	
		bool mFrameFinish;		
	public:
		//コンストラクタ
		Blinking();

		//デストラクタ
		~Blinking();

		//更新処理
		void Update();

		//点滅するかどうか返す
		virtual bool NowBlinking();

		//セッター//
		void SetState(const bool& state) { mState = state; }
		virtual void SetBlink(const bool& flag) { mFrameFlag = flag; }
		virtual void SetTarget(const float& target) { mTarget = target; }
		virtual void SetFinal(const int& Final) { mFinal = Final; }
		virtual void SetFinalOption(const bool& finish) { mFrameFinish = finish; }
		virtual void SetTargetCount(const int& target) { mTargetCount = target; }

		//ゲッター//
		virtual const bool& GetBlink()const { return mFrameFlag; }
		virtual const float& GetFrameCount()const { return mFrameCount; }
		virtual const int& GetFinal()const { return mFinal; }
		virtual const float& GetTarget()const { return mTarget; }
		virtual const float& GetTargetCount()const { return mTargetCount; }
	};
}

