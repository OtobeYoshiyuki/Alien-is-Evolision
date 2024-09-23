#include "pch.h"
#include "DamageUI.h"
#include "GameContext.h"
#include "GameObject/MoveState/DamageState/DamageState.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "TemplateState.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

DamageUI::DamageUI(ITextureBase* texture) :TextureBase(texture)
{
	//現在のステートを設定する
	State::SetNowState(FADE_STATE::NONE);

	//描画順を変更する
	this->SetDrawOrder(-1);

	//描画を切る
	this->SetState(STATE::NONE);
}

void DamageUI::Initialize()
{
}

void DamageUI::Update()
{
	//テクスチャの更新処理
	TextureBase::Update();

	//フェードの更新処理
	this->UpdateFade();
}

void DamageUI::Render()
{
	//テクスチャの描画処理
	TextureBase::Render();
}

void DamageUI::Finalize()
{
}

void DamageUI::UpdateFade()
{
	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//ダメージステートを取得する
	DamageState* damageState = GameContext::Get<DamageState>();

	//フェードの色を取得する
	Color color = this->GetColor();

	//データが入ってるかどうか確認する
	//データが入ってなかったら、何もしない
	if (!staticData->Request_Data("プレイヤーダメージ"))return;

	//泡に当たっておらず、反転台に当たっていたら、何もしない
	if (staticData->Get_StringData("プレイヤーダメージ") == "rebound")return;

	//データが空だったら、何もしない
	if (staticData->Get_StringData("プレイヤーダメージ") == "")return;
	
	//フェードの更新処理
	//実行関数が実行中だったら透明度を下げる
	if (damageState->GetExecute())
	{
		this->SetNowState(FADE_STATE::DOWN);
	}

	//透明度が減算の時
	if (this->GetNowState() == FADE_STATE::DOWN)
	{		
		//描画をオンにする
		this->SetState(STATE::DRAW);
		//毎フレーム透明度を減算する
		color = Color(color.R(), color.G(), color.B(), color.A() - 0.01f);
		//色を設定する
		this->SetColor(color);
	}

	//透明度が0になったらステートを元に戻す
	if (this->GetColor().A() <= 0.0f)
	{
		//ステートをデフォルトに戻す
		this->SetNowState(FADE_STATE::NONE);
		//描画を切る
		this->SetState(STATE::NONE);
		//色を設定する
		this->SetColor(Colors::Red);
	}
}
