#pragma once
#include "GameObject/Stage/Maptip/MapTip.h"
#include "OtobeLib/Texture/Animation/Animation.h"
class Wall:public MapTip, OtobeLib::Animation
{
public:
	//コンストラクタ
	Wall(OtobeLib::ITextureBase* texture);

	//デストラクタ
	~Wall()override {};

	//初期化
	void Initialize()override;

	//更新
	void Update()override;

	//描画
	void Render()override;

	//終了
	void Finalize()override;

	//他のオブジェクトと当たった時に呼ばれる関数
	void OnCollisionEnter(ITextureBase* obj)override;

	//他のオブジェクトと当たっていないときに呼ばれる関数
	void NonCollisionEnter(ITextureBase* obj)override;

	//アニメーションの情報を設定する
	void SetAnimation(const std::string& name, const RECT& rect)override { Animation::SetAnimation(name, rect); }

	//次に再生するアニメーションの情報を設定する
	void SetNextAnimation(const std::string& now, const std::string& next)override { Animation::SetNextAnimation(now, next); }

	//アニメーションの終了時間を設定する
	void SetEndTime(const std::string& name, const int& time)override { Animation::SetEndTime(name, time); }

	//現在のアニメーションを設定する
	void SetNowAnimation(const RECT& rect)override { Animation::SetNowAnimation(rect); }

	//現在のアニメーションの情報を設定する
	void SetNowInfo(const std::string& info)override { Animation::SetNowInfo(info); }

	//アニメーションのループ情報を設定する
	void SetLoopInfo(const std::string& loop)override { Animation::SetLoopInfo(loop); }

	//アニメーションの更新フラグを設定する
	void SetAniFlag(const bool& flag)override { Animation::SetAniFlag(flag); }

	//アニメーションのループフラグを設定する
	void SetLoopFlag(const bool& flag)override { Animation::SetLoopFlag(flag); }

	//アニメーションのパラメーターを引き継ぐ
	void SetNewAnimation(Animation* ani)override { Animation::SetNewAnimation(ani); }

	//アニメーションの情報を取得する
	const RECT& GetAnimation(const std::string& name)override { return Animation::GetAnimation(name); }

	//次に再生するアニメーションの情報を取得する
	const std::string& GetNextAnimation(const std::string& now)override { return Animation::GetNextAnimation(now); }

	//アニメーションの終了時間を設定する
	const int& GetEndTime(const std::string& name)override { return Animation::GetEndTime(name); }

	//現在のアニメーションを取得する
	const RECT& GetNowAnimation()const override { return Animation::GetNowAnimation(); }

	//現在のアニメーションの情報を設定する
	const std::string& GetNowInfo()const override { return Animation::GetNowInfo(); }

	//アニメーションのループ情報を設定する
	const std::string& GetLoopInfo()const override { return Animation::GetLoopInfo(); }

	//アニメーションの更新フラグを取得する
	const bool& GetAniFlag()const override { return Animation::GetAniFlag(); }

	//アニメーションのループフラグを取得する
	const bool& GetLoopFlag()const override { return Animation::GetLoopFlag(); }

	//アニメーションカウントを取得する
	const int& GetAniCount()const override { return Animation::GetAniCount(); }
};

