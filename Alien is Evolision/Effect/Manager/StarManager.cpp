#include "pch.h"
#include "StarManager.h"
#include "GameContext.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "Game.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

StarManager::StarManager():m_MainStar(nullptr)
{
	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();
	
	//星のテクスチャを取得する
	ITextureBase* star = resourceManager->GetTexture("星");

	//TextureManagerを取得
	TextureManager* texMana = GameContext::Get<PlayScene>()->GetSubInfoManager().get();

	//MainStarを生成する
	std::unique_ptr<Star> pManiStar = std::make_unique<Star>(star);
	pManiStar->SetPosition(Vector3(Game::MAX_WIDTH / 2, Game::MAX_HEIGHT / 2, 0.0f));
	pManiStar->SetOrigin(pManiStar->AskOrigin());
	pManiStar->SetColor(Colors::Yellow);
	pManiStar->SetState(ITextureBase::STATE::NONE);
	pManiStar->SetAngleSpeed(1.0f);
	pManiStar->SetDrawOrder(0);
	m_MainStar = pManiStar.get();

	//AutoMoveStateの設定をする
	m_MainStar->GetAutoMoveState()->SetSpeed(Vector3(5.0f, 5.0f, 0.0f));

	//PopStarRedを生成する
	std::unique_ptr<Star> pPopRedStar = std::make_unique<Star>(star);
	pPopRedStar->SetOrigin(pPopRedStar->AskOrigin());
	pPopRedStar->SetColor(Colors::Red);
	pPopRedStar->SetState(ITextureBase::STATE::NONE);
	pPopRedStar->SetScale(Vector3(0.5f));
	pPopRedStar->SetAngleSpeed(1.0f);
	m_PopStarRed = pPopRedStar.get();

	//AutoMoveStateの設定をする
	m_PopStarRed->GetAutoMoveState()->SetSpeed(Vector3(5.0f, 5.0f, 0.0f));
	//JumpStateの設定をする
	m_PopStarRed->GetJumpState()->SetJumpLimit(100.0f);
	m_PopStarRed->GetJumpState()->SetJumpSpeed(10.0f);
	m_PopStarRed->GetJumpState()->SetJumpOption(true);
	m_PopStarRed->GetJumpState()->SetJumpLimitCount(1);

	//PopStarBlueを生成する
	std::unique_ptr<Star> pPopBlueStar = std::make_unique<Star>(star);
	pPopBlueStar->SetOrigin(pPopBlueStar->AskOrigin());
	pPopBlueStar->SetColor(Colors::Blue);
	pPopBlueStar->SetState(ITextureBase::STATE::NONE);
	pPopBlueStar->SetScale(Vector3(0.5f));
	pPopBlueStar->SetAngleSpeed(1.0f);
	m_PopStarBlue = pPopBlueStar.get();

	//AutoMoveStateの設定をする
	m_PopStarBlue->GetAutoMoveState()->SetSpeed(Vector3(5.0f, 5.0f, 0.0f));
	//JumpStateの設定をする
	m_PopStarBlue->GetJumpState()->SetJumpLimit(100.0f);
	m_PopStarBlue->GetJumpState()->SetJumpSpeed(10.0f);
	m_PopStarBlue->GetJumpState()->SetJumpOption(true);
	m_PopStarBlue->GetJumpState()->SetJumpLimitCount(1);

	//リストに追加する
	m_PopStars.push_back(m_PopStarRed);
	m_PopStars.push_back(m_PopStarBlue);

	//TextureManagerに登録する
	texMana->Add(std::move(pManiStar));
	texMana->Add(std::move(pPopRedStar));
	texMana->Add(std::move(pPopBlueStar));
}

void StarManager::Initialize(const Vector3& pos)
{
	//座標を設定する

	//中心の星の座標を設定する
	m_MainStar->SetPosition(pos + Vector3(m_MainStar->AskOrigin().x,0.0f,0.0f));

	//描画をオンにする
	m_MainStar->SetState(ITextureBase::STATE::DRAW);

	//下に移動させる
	m_MainStar->GetAutoMoveState()->SetExecute(true);
	m_MainStar->GetAutoMoveState()->SetNowState(DIRECT::DDOWN);
}

void StarManager::Update()
{
	//Readyエフェクトを取得する
	TimeLag* Ready = GameContext::Get<PlayScene>()->GetLagStart().get();

	//中心の星の更新処理
	this->UpdateMainStar();

	//はじける赤い星の更新処理
	this->UpdatePopStarRed();

	//はじける青い星の更新処理
	this->UpdatePopStarBlue();
}

void StarManager::UpdateMainStar()
{
	//中心の星の更新処理

	//画像の下の部分で判定を取る
	float StarBotom = m_MainStar->GetPosition().y + m_MainStar->AskOrigin().y;

	//画像が画面の一番下まで来たら、移動を停止して、描画を止める
	if (StarBotom >= Game::MAX_HEIGHT + 80)
	{
		//描画がオンの時に、描画を切る
		if (m_MainStar->GetState() == ITextureBase::STATE::DRAW)
		{
			m_MainStar->GetAutoMoveState()->SetNowState(DIRECT::NNONE);
			m_MainStar->SetState(ITextureBase::STATE::NONE);

			//中心の星の座標をもとに、赤い星と青い星の座標を設定する
			m_PopStarRed->SetPosition(m_MainStar->GetPosition());
			m_PopStarBlue->SetPosition(m_MainStar->GetPosition());

			//赤い星のステートの設定を行う
			m_PopStarRed->GetAutoMoveState()->SetExecute(true);
			m_PopStarRed->GetJumpState()->SetExecute(true);
			m_PopStarRed->GetAutoMoveState()->SetNowState(DIRECT::RIGHT);
			m_PopStarRed->GetJumpState()->SetNowState(JUMP::START);
			m_PopStarRed->SetState(ITextureBase::STATE::DRAW);

			//青い星のステートの設定を行う
			m_PopStarBlue->GetAutoMoveState()->SetExecute(true);
			m_PopStarBlue->GetJumpState()->SetExecute(true);
			m_PopStarBlue->GetAutoMoveState()->SetNowState(DIRECT::LEFT);
			m_PopStarBlue->GetJumpState()->SetNowState(JUMP::START);
			m_PopStarBlue->SetState(ITextureBase::STATE::DRAW);
		}

	}
}

void StarManager::UpdatePopStarRed()
{
	//はじける赤い星の更新処理

	//ジャンプして、着地したタイミングで、描画を切る
	if (m_PopStarRed->GetJumpState()->GetJumpmDesignation())
	{
		m_PopStarRed->SetState(ITextureBase::STATE::NONE);
	}
}

void StarManager::UpdatePopStarBlue()
{
	//はじける青い星の更新処理

	//ジャンプして、着地したタイミングで、描画を切る
	if (m_PopStarBlue->GetJumpState()->GetJumpmDesignation())
	{
		m_PopStarBlue->SetState(ITextureBase::STATE::NONE);
	}
}

