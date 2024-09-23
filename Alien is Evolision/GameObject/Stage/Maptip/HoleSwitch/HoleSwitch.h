#pragma once
#include "GameObject/Stage/Maptip/MapTip.h"
#include "TemplateState.h"
#include "GameObject/DamageUI/FadeState.h"

class HoleSwitch :public MapTip,State<FADE_STATE>
{
private:
	//起爆するカウント
	float m_DetonationCount;
	//カウントを更新するフラグ
	bool m_DetonationFlag;
	//シリアルナンバー
	int	m_SelealNumber;
	//背景のテクスチャ
	OtobeLib::ITextureBase* m_backTexture;
public:
	//コンストラクタ
	HoleSwitch(OtobeLib::ITextureBase* texture);

	//デストラクタ
	~HoleSwitch()override {};

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

public:
	//シリアルナンバーを設定する
	void SetSerealNumber(const int& number) { m_SelealNumber = number; }
	//シリアルナンバーを取得する
	const int& GetSerealNumber()const { return m_SelealNumber; }
};

