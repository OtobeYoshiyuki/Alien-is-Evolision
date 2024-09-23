#include "pch.h"
#include "DirecTingScene.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "OtobeLib/FrameWork/Graphics/Graphics.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/StackScene/SceneManager/SceneManager.h"
#include "OtobeLib/StackScene/Scene/Select/SelectScene.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"
#include "Game.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//ステートを切り替える時間
const float DirecTingScene::CHANGE_TIME_PLAYER = 2.0f;

//ステートを切り替える時間
const float DirecTingScene::CHANGE_TIME_EXPROSION = 1.0f;

//プレイヤーのアニメーションの待ち時間
const float DirecTingScene::WAIT_TIME_FACE = 1.0f;

//プレイヤーの背景のアニメーションの待ち時間
const float DirecTingScene::WAIT_TIME_BACKFADE = 3.0f;

DirecTingScene::DirecTingScene():
	m_TextureManager(nullptr),//画像の管理クラス
	m_backPlayer(nullptr),//プレイヤーの背面のテクスチャ
	m_backTexture(nullptr),//背景のテクスチャ
	m_playerAnimeTex(nullptr),//プレイヤーのアニメーション用のテクスチャ
	m_waveAnimeTex(nullptr),//オーラのアニメーション用のテクスチャ
	m_timer(0.0f)//経過時間
{
}

void DirecTingScene::Initialize()
{
	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//SEを再生する
	soundManager->Play(CRI_CUESHEET_0__41_オーラ);

	//生成の処理
	this->Create();
}

void DirecTingScene::Update()
{
	//TextureManagerの更新
	m_TextureManager->Update();

	//ステートの更新処理
	this->UpdateState();
}

void DirecTingScene::Render()
{
	//描画系の状態を取得する
	Graphics* Graph = Graphics::GetInstance();
	const auto& context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

	//描画の開始
	Graph->GetSpriteBatch()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred,
		Graph->GetCommonStates()->NonPremultiplied(), Graph->GetCommonStates()->PointClamp(),
		Graph->GetCommonStates()->DepthDefault(), nullptr,
		[&] {context->OMSetBlendState(Graph->GetCommonStates()->NonPremultiplied(), nullptr, 0xffffffff); });

	//画像の描画処理
	m_TextureManager->Render();

	//描画の終了
	Graph->GetSpriteBatch()->End();
}

void DirecTingScene::Finalize()
{
}

void DirecTingScene::Create()
{
	//生成処理

	//添え字を初期化する
	this->AddData2(NULL);

	//静的データを取得する
	StaticData* staticData = StaticData::GetInstance();

	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//TextureManagerを生成する
	m_TextureManager = std::make_unique<TextureManager>();

	//使用する画像を取得する
	ITextureBase* bg = resourceManager->GetTexture("ステージセレクト" + staticData->Get_StringData("ステージ"));
	ITextureBase* playerBack = resourceManager->GetTexture("必殺技プレイヤー背景");
	ITextureBase* playerTex = resourceManager->GetTexture("必殺技プレイヤー1");
	ITextureBase* wave = resourceManager->GetTexture("必殺技オーラ1");
	ITextureBase* expro = resourceManager->GetTexture("必殺技エフェクト1");

	//背景のテクスチャを生成する
	std::unique_ptr<ITextureBase> pbg = std::make_unique<TextureBase>(bg);
	//描画順を設定する
	pbg->SetDrawOrder((int)DRAWORDER::BACKGROUND);
	//描画を切る
	pbg->SetState(ITextureBase::STATE::NONE);
	//生ポインタを登録する
	m_backTexture = pbg.get();
	//TextureManagerに登録する
	m_TextureManager->Add(std::move(pbg));

	//プレイヤーのアニメーション用のテクスチャ
	std::unique_ptr<AnimeTexture> panimePlayer = std::make_unique<AnimeTexture>(playerTex);
	//初期化
	panimePlayer->InitAnimation("DeathBlowPlayer", false, false, "Face1", "Face4");
	//描画順を設定する
	panimePlayer->SetDrawOrder((int)DRAWORDER::PLAYER);
	//描画を切る
	panimePlayer->SetState(ITextureBase::STATE::NONE);
	//座標を設定する
	panimePlayer->SetPosition(Vector3(Game::MAX_WIDTH / 4, 0.0f, 0.0f));
	//生ポインタを登録する
	m_playerAnimeTex = panimePlayer.get();
	//TextureManagerに登録する
	m_TextureManager->Add(std::move(panimePlayer));

	//プレイヤーの背面のテクスチャ
	std::unique_ptr<ITextureBase> pbgPlayer = std::make_unique<TextureBase>(playerBack);
	//描画順を設定する
	pbgPlayer->SetDrawOrder((int)DRAWORDER::PLAYER_BACK);
	//描画を切る
	pbgPlayer->SetState(ITextureBase::STATE::NONE);
	//座標を設定する
	pbgPlayer->SetPosition(Vector3(Game::MAX_WIDTH / 4, 0.0f, 0.0f));
	//色を設定する
	pbgPlayer->SetColor(Colors::SkyBlue);
	//生ポインタを登録する
	m_backPlayer = pbgPlayer.get();
	//TextureManagerに登録する
	m_TextureManager->Add(std::move(pbgPlayer));

	//オーラのアニメーション用のテクスチャ
	std::unique_ptr<AnimeTexture> panimeWave = std::make_unique<AnimeTexture>(wave);
	//初期化
	panimeWave->InitAnimation("Wave", true, true, "Wave1", "Wave14");
	//描画順を設定する
	panimeWave->SetDrawOrder((int)DRAWORDER::WAVE);
	//描画を切る
	panimeWave->SetState(ITextureBase::STATE::NONE);
	//座標を設定する
	panimeWave->SetPosition(Vector3(200.0f,-300.0f, 0.0f));
	//拡大倍率を設定する
	panimeWave->SetScale(Vector3(1.5f, 1.5f, 1.0f));
	//生ポインタを登録する
	m_waveAnimeTex = panimeWave.get();
	//TextureManagerに登録する
	m_TextureManager->Add(std::move(panimeWave));

	//爆発のアニメーション用テクスチャ
	//3つ生成する
	for (int i = 0; i < EXPROSION_ARRAY_NUM; i++)
	{
		//爆発用のテクスチャ
		std::unique_ptr<AnimeTexture> pexpro = std::make_unique<AnimeTexture>(expro);
		//初期化
		pexpro->InitAnimation("Exprosion", false, false, "Expro1", "Expro9");
		//描画順を設定する
		pexpro->SetDrawOrder((int)DRAWORDER::EXPROSION);
		//描画を切る
		pexpro->SetState(ITextureBase::STATE::NONE);
		//生ポインタを登録する
		m_exproAnimeTex.push_back(pexpro.get());
		//TextureManagerに登録する
		m_TextureManager->Add(std::move(pexpro));
	}

	//爆発の座標を設定する
	m_exproAnimeTex[ARRAY_0]->SetPosition(Vector3(400.0f, 0.0f, 0.0f));
	m_exproAnimeTex[ARRAY_1]->SetPosition(Vector3(0.0f, 200.0f, 0.0f));
	m_exproAnimeTex[ARRAY_2]->SetPosition(Vector3(800.0f, 200.0f, 0.0f));
}

void DirecTingScene::UpdateState()
{
	//ステートの更新処理

	//Switch文で処理を分ける
	switch (this->GetNowData1())
	{
	case DIRECTING::NONE:
		this->State_None();
		break;
	case DIRECTING::WAVE_ANIME:
		this->State_WaveAnime();
		break;
	case DIRECTING::TIMER_UP:
		this->State_TimerUp();
		break;
	case DIRECTING::PLAYER_ANIME:
		this->State_PlayerAnime();
		break;
	case DIRECTING::BACK_FADE:
		this->State_BackFade();
		break;
	case DIRECTING::EXPROSION_ANIME:
		this->State_ExproAnime();
		break;
	case DIRECTING::END:
		this->State_End();
		break;
	}
}

void DirecTingScene::State_None()
{
	//State_Noneの処理

	//描画を起こす
	m_playerAnimeTex->SetState(ITextureBase::STATE::DRAW);
	m_backTexture->SetState(ITextureBase::STATE::DRAW);
	m_waveAnimeTex->SetState(ITextureBase::STATE::DRAW);

	//ステートを切り替える
	this->AddData1(DIRECTING::WAVE_ANIME);
}

void DirecTingScene::State_WaveAnime()
{
	//State_WaveAnimeの処理

	//オーラが全部出てきたら、ステートを切り替える
	if (m_waveAnimeTex->GetNowInfo() == "Wave9")
	{
		this->AddData1(DIRECTING::TIMER_UP);
	}
}

void DirecTingScene::State_TimerUp()
{
	//State_TimerUpの処理

	//静的データを取得する
	StaticData* staticData = StaticData::GetInstance();

	//2秒経過したら、ステートを切り替える
	if (m_timer > CHANGE_TIME_PLAYER)
	{
		//経過時間を初期化する
		m_timer = 0.0f;

		//プレイヤーのアニメーションを起動させる
		m_playerAnimeTex->SetAniFlag(true);

		//ステートを切り替える
		this->AddData1(DIRECTING::PLAYER_ANIME);

		//時間の更新は行わない
		return;
	}

	//経過時間を更新する
	m_timer += staticData->Get_FloatData("elapsedTime");
}

void DirecTingScene::State_PlayerAnime()
{
	//State_PlayerAnimeの処理

	//SceneManagerを取得する
	SceneManager* manager = SceneManager::GetInstance();

	//静的データを取得する
	StaticData* staticData = StaticData::GetInstance();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//プレイヤーのアニメーションが終わったとき
	if (!m_playerAnimeTex->GetAniFlag())
	{
		//必殺技がダークエクスプロージョンの時
		if (staticData->Get_StringData("必殺技") == SelectScene::DEATHBLOW_NAME_EXPROSION)
		{
			//目を開き終わってから、時間が経過したとき
			if (m_timer > WAIT_TIME_FACE)
			{
				//経過時間を初期化する
				m_timer = 0.0f;

				//もう一度カットインを出す
				manager->PushScene("CutIn");

				//ステートを切り替える
				this->AddData1(DIRECTING::EXPROSION_ANIME);

				//SEを停止させる
				soundManager->Stop(CRI_CUESHEET_0__41_オーラ);

				//時間の更新は行わない
				return;
			}
		}

		//必殺技がリバースアビリティの時
		if (staticData->Get_StringData("必殺技") == SelectScene::DEATHBLOW_NAME_ABILITY)
		{
			//目を開き終わってから、時間が経過したとき
			if (m_timer > WAIT_TIME_FACE)
			{
				//経過時間を初期化する
				m_timer = 0.0f;

				//もう一度カットインを出す
				manager->PushScene("CutIn");

				//ステートを切り替える
				this->AddData1(DIRECTING::BACK_FADE);
				this->AddData3(FADE_STATE::NONE);

				//SEを停止させる
				soundManager->Stop(CRI_CUESHEET_0__41_オーラ);

				//時間の更新は行わない
				return;
			}
		}


		//経過時間を更新する
		m_timer += staticData->Get_FloatData("elapsedTime");
	}
}

void DirecTingScene::State_BackFade()
{
	//State_BackFadeの処理

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//SceneManagerを取得する
	SceneManager* manager = SceneManager::GetInstance();

	//静的データを取得する
	StaticData* staticData = StaticData::GetInstance();

	//プレイヤーの背景を描画する
	m_backPlayer->SetState(ITextureBase::STATE::DRAW);

	//オーラの描画を切る
	m_waveAnimeTex->SetState(ITextureBase::STATE::NONE);

	//背景の色情報を取得する
	Color color = m_backPlayer->GetColor();
	float alfa = color.A();

	//ステートによって、処理を切り替える
	switch (this->GetNowData3())
	{
	case FADE_STATE::NONE:
		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__10_強化);
		this->AddData3(FADE_STATE::DOWN);
		break;
	case FADE_STATE::DOWN:
		if (color.A() <= 0.5f)this->AddData3(FADE_STATE::FUP);
		else alfa -= 0.01f;
		break;
	case FADE_STATE::FUP:
		if (color.A() >= 1.0f)this->AddData3(FADE_STATE::DOWN);
		else alfa += 0.01f;
		break;
	}

	//色情報を再設定する
	color.A(alfa);
	m_backPlayer->SetColor(color);

	//時間が経過したら、再びSceneを追加生成する
	if (m_timer > WAIT_TIME_BACKFADE)
	{
		//もう一度カットインを出す
		manager->PushScene("CutIn");

		//ステートを切り替える
		this->AddData1(DIRECTING::END);

		//経過時間を初期化する
		m_timer = 0.0f;

		//SEを停止させる
		soundManager->Stop(CRI_CUESHEET_0__10_強化);

		//時間の更新を行わない
		return;
	}

	//経過時間を更新する
	m_timer += staticData->Get_FloatData("elapsedTime");
}

void DirecTingScene::State_ExproAnime()
{
	//State_ExproAnimeの処理

	//静的データを取得する
	StaticData* staticData = StaticData::GetInstance();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//描画を消す
	m_playerAnimeTex->SetState(ITextureBase::STATE::NONE);
	m_backTexture->SetState(ITextureBase::STATE::NONE);
	m_waveAnimeTex->SetState(ITextureBase::STATE::NONE);

	//1秒おきに実行
	if (m_timer > CHANGE_TIME_EXPROSION)
	{
		//現在の添え字を取得する
		int suffix = this->GetNowData2();

		//経過時間を初期化する
		m_timer = 0.0f;

		//アニメーションを起動させる
		m_exproAnimeTex[this->GetNowData2()]->SetAniFlag(true);

		//描画を起こす
		m_exproAnimeTex[this->GetNowData2()]->SetState(ITextureBase::STATE::DRAW);

		//添え字を更新する
		suffix++;

		//添え字を設定する
		this->AddData2(suffix);

		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__09_爆発音);
	}

	//最後のアニメーションが起動
	if (this->GetNowData2() >= EXPROSION_ARRAY_NUM)
	{
		//ステートを切り替える
		this->AddData1(DIRECTING::END);

		//経過時間の更新はしない
		return;
	}

	//経過時間を更新する
	m_timer += staticData->Get_FloatData("elapsedTime");
}


void DirecTingScene::State_End()
{
	//State_Endの処理

	//SceneManagerを取得する
	SceneManager* manager = SceneManager::GetInstance();

	for (AnimeTexture* animeTex : m_exproAnimeTex)
	{
		//アニメーションが終了
		if (!animeTex->GetAniFlag())animeTex->SetState(ITextureBase::STATE::NONE);
	}

	//アニメーションが終了
	if (!m_exproAnimeTex[ARRAY_2]->GetAniFlag() ||
		this->GetPreData1() == DIRECTING::BACK_FADE)
	{
		//現在のSceneを消す
		manager->PopScene();
	}

}
