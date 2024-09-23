#include "pch.h"
#include "AnimeTexture.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

AnimeTexture::AnimeTexture(ITextureBase* texture):
	TextureBase(texture)//基底クラスに情報を送信する
{
}

void AnimeTexture::Initialize()
{
}

void AnimeTexture::InitAnimation(
	const std::string& grop,//アニメーショングループの名前
	const bool& animeflag,//アニメーションカウント更新用のフラグ
	const bool& loopfrag,//アニメーションのループ再生用のフラグ
	const std::string& nowinfo,//現在のアニメーションの情報
	const std::string& loopinfo//アニメーションのループ情報
)
{
	//ResourcesManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//アニメーションの設定
	for (const std::unique_ptr<AnimationImporter>& importer : Animation::GetAnimeList(grop))
	{
		//アニメーションの使用するテクスチャを取得する
		ITextureBase* useTexture = resourceManager->GetTexture(importer->GetRefference());

		//アニメーション情報を設定する
		this->SetAnimation(importer->GetAnimation(), useTexture->GetRect());

		//次のアニメーションの情報を設定する
		this->SetNextAnimation(importer->GetAnimation(), importer->GetNext());

		//アニメーションの終了時間を設定する
		this->SetEndTime(importer->GetAnimation(), importer->GetEnd());
	}

	//アニメーションのフラグを設定する
	this->SetAniFlag(animeflag);
	this->SetLoopFlag(loopfrag);

	//現在のアニメーション情報を設定する
	this->SetNowInfo(nowinfo);
	this->SetLoopInfo(loopinfo);
	this->SetNowAnimation(this->GetNowAnimation());

	//実行関数を起こす
	this->SetActive(true);
}

void AnimeTexture::Update()
{
	//UIの表示が終わるまで何もしない
	TimeLag* go = GameContext::Get<PlayScene>()->GetLagGo().get();
	if (!go->GetFinishFlag())return;

	//実行関数が落ちているときは、何もしない
	if (!this->GetActive())return;

	//アニメーションの更新処理
	Animation::Update();

	//画像の更新処理
	TextureBase::Update();

	//画像の切り出し位置を更新する
	this->SetRect(this->GetNowAnimation());
}

void AnimeTexture::Render()
{
	//画像の描画処理を行う
	TextureBase::Render();
}

void AnimeTexture::Finalize()
{
}
