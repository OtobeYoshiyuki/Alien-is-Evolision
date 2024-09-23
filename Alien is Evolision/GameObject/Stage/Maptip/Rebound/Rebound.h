#pragma once
#include "GameObject/Stage/Maptip/MapTip.h"
#include "GameObject/CollisionUI/CollisionUI.h"
#include "TemplateState.h"
#include "GameObject/DamageUI/FadeState.h"
#include <vector>
class Rebound:public MapTip,State<FADE_STATE>
{
private:
	//前面のテクスチャ
	OtobeLib::ITextureBase* m_frontTexture;
public:
	//コンストラクタ
	Rebound(OtobeLib::ITextureBase* texture);

	//デストラクタ
	~Rebound()override {};

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

public:
	//現在の状態を設定する
	void SetNowState(const FADE_STATE& now)override { State::SetNowState(now); };

	//現在の状態を取得する
	const FADE_STATE& GetNowState()const override { return State::GetNowState(); };

	//ひとつ前の状態を取得する
	const FADE_STATE& GetPreState()const override { return State::GetPreState(); };

private:
	//ステートの更新処理
	void UpdateState();
};

