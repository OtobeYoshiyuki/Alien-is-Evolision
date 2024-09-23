#include "pch.h"
#include "Rebound.h"
#include "GameContext.h"
#include "GameObject/Player/CoroKun.h"
#include "OtobeLib/FrameWork/Collision/CollisionManager/CollisionManager.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

Rebound::Rebound(OtobeLib::ITextureBase* texture):
	MapTip(texture),//基底クラスに情報を送信する
	m_frontTexture(nullptr)//前面のテクスチャ
{
}

void Rebound::Initialize()
{
	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//GameManagerを取得する
	TextureManager* gameManager = GameContext::Get<PlayScene>()->GetGameManager().get();

	//使うテクスチャを取得する
	ITextureBase* whiteBack = resourceManager->GetTexture("WhiteBack");

	//色情報を設定する
	Color color = Colors::Red;
	color.A(0.5f);

	//テクスチャを生成する
	std::unique_ptr<ITextureBase>pfront = std::make_unique<TextureBase>(whiteBack);
	pfront->SetColor(color);
	pfront->SetRect(RECT{ 0,0,40,40 });
	pfront->SetDrawOrder(0);
	pfront->SetPosition(this->GetPosition());
	m_frontTexture = pfront.get();

	//GameManagerに追加する
	gameManager->Add(std::move(pfront));

	//ステートを初期化する
	this->SetNowState(FADE_STATE::DOWN);
}

void Rebound::Update()
{
	MapTip::Update();

	//ステートの更新処理
	this->UpdateState();

	//プレイヤーを取得する
	CoroKun* player = GameContext::Get<CoroKun>();

	//プレイヤーが生存しているとき
	if (player->GetHP() > NULL)
	{
		//当たり判定を追加する
		GameContext::Get<CollisionManager>()->AddUseOther(this, player, 200.0f);
	}

	//ニードルの長距離範囲攻撃を取得する
	std::vector<Needle*> needleList = GameContext::Get<NeedleOverall>()->GetNeedleList();

	for (Needle* needle : needleList)
	{
		//当たり判定を追加する
		GameContext::Get<CollisionManager>()->AddUseOther(this, needle, 100.0f);
	}
}

void Rebound::Render()
{
	MapTip::Render();
}

void Rebound::Finalize()
{
}

void Rebound::OnCollisionEnter(ITextureBase* obj)
{
	MapTip::OnCollisionEnter(obj);
}

void Rebound::NonCollisionEnter(ITextureBase* obj)
{
	MapTip::NonCollisionEnter(obj);
}

void Rebound::UpdateState()
{
	//背景の色情報を取得する
	Color color = m_frontTexture->GetColor();
	float alfa = color.A();

	//Switch文によって、処理を分ける
	switch (this->GetNowState())
	{
	case FADE_STATE::DOWN:
		if (color.A() <= 0.0f)this->SetNowState(FADE_STATE::FUP);
		else alfa -= 0.01f;
		break;
	case FADE_STATE::FUP:
		if (color.A() >= 0.5f)this->SetNowState(FADE_STATE::DOWN);
		else alfa += 0.01f;
		break;
	}

	//色情報を再設定する
	color.A(alfa);
	m_frontTexture->SetColor(color);
}
