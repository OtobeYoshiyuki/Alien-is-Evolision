#include "pch.h"
#include "StageSelectScene.h"
#include "ExternalLib/Easing/Easing.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/FrameWork/Input/Input.h"
#include "OtobeLib/StackScene/SceneManager/SceneManager.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "GameObject/Player/CoroKun.h"
#include "OtobeLib/FrameWork/TextureFont/TextureFont.h"
#include "OtobeLib/FrameWork/Stream/Stream.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;
using namespace ExternalLib;

//スクロール前の座標
const Vector3 StageSelectScene::BEFORE_POS = Vector3(1550.0f, 150.0f, 0.0f);

//スクロール後の座標
const Vector3 StageSelectScene::AFTER_POS = Vector3(-1300.0f, 150.0f, 0.0f);

StageSelectScene::StageSelectScene():
	m_TextureManager(nullptr),//画像の管理クラス
	m_time(0.0f),//イージング関数の経過時間
	m_direct(NULL),//ベクトルの向き
	m_ifState(nullptr),//関数オブジェクト
	m_rightArrow(nullptr),//右矢印の画像
	m_leftArrow(nullptr),//左矢印の画像
	m_spaceBackTexture(nullptr)//スペースキーの背面のテクスチャ
{
}

void StageSelectScene::Initialize()
{
	//ファイルを取得する
	Stream* file = Stream::GetInstance();

	//生成処理
	this->Create();

	//SceneManagerを取得する
	SceneManager* sceneManager = SceneManager::GetInstance();

	//SceneStateを取得する
	SceneState* sceneState = sceneManager->GetState();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//フェードインを行う
	sceneState->SetFadeState(SceneState::FADEIN);

	//添え字を設定する
	this->AddData1(NULL);

	//ステージのクリア状況を読み込む
	//ファイルが開けたら、開く
	if (file->Open(L"FileData/StageClear.csv", Stream::FILE::STREAM_IFSTREAM))
	{
		//データを取得する
		m_clearInfo = file->Load_Data();

		//ファイルを閉じる
		file->Close();
	}

	//Adx2を取得する
	ADX2* adx2 = ADX2::GetInstance();

	//BGMを再生する
	soundManager->Play(CRI_CUESHEET_0__02_ステージセレクト用BGM_);
}

void StageSelectScene::Update()
{
	//SceneManagerを取得する
	SceneManager* sceneManager = SceneManager::GetInstance();

	//SceneStateを取得する
	SceneState* sceneState = sceneManager->GetState();

	//キーボードの状態を取得
	Input* input = Input::GetInstance();
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//フェードアウトが完了したら、タイトルに移行する
	if (sceneState->GetFade() == SceneState::FADERESULT)
	{
		sceneManager->ChangeScene("Play");
	}

	//キー入力待ち以外の時は、処理を飛ばす
	if (sceneState->GetFade() != SceneState::WAITKEY)return;

	//TextureManagerの更新
	m_TextureManager->Update();

	//ステージのスクロールの処理
	this->UpdateEasing();
}

void StageSelectScene::Render()
{
	//描画系の情報を取得
	Graphics* Graph = Graphics::GetInstance();
	const auto& context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

	//フォントを取得
	TextureFont* font = TextureFont::GetInstance();
	
	Graph->GetSpriteBatch()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred,
		Graph->GetCommonStates()->NonPremultiplied(), Graph->GetCommonStates()->PointClamp(),
		Graph->GetCommonStates()->DepthDefault(), nullptr,
		[&] {context->OMSetBlendState(Graph->GetCommonStates()->NonPremultiplied(), nullptr, 0xffffffff); });

	//TextureManagerの描画
	m_TextureManager->Render();

	for (int i = 0;i < (int)m_stageTexture.size();i++)
	{
		//ステージ画像の座標を取得する
		Vector3 pos = m_stageTexture[i]->GetPosition();

		//フォントを描画する
		font->DrawFormatString(m_clearInfo[(int)STAGEINFO::STAGENAME][i], Vector2(pos.x + 250.0f, 50.0f), 3.0f, Colors::Black);
	}

	if (m_spaceBackTexture->NowBlinking())
	{
		//スペースキーの描画
		font->DrawFormatString("PUSH SPACE", Vector2(350.0f, 600.0f), 2.0f, Colors::Black);
	}

	Graph->GetSpriteBatch()->End();
}

void StageSelectScene::Finalize()
{
	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//BGMを止める
	soundManager->Stop(CRI_CUESHEET_0__02_ステージセレクト用BGM_);
}

void StageSelectScene::Create()
{
	//ResourceManagerを取得
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//TextureManagerを生成
	m_TextureManager = std::make_unique<TextureManager>();

	//使用する画像を取得
	ITextureBase* whiteBack = resourceManager->GetTexture("WhiteBack");
	ITextureBase* stageBack = resourceManager->GetTexture("ステージセレクト背景");
	ITextureBase* arrow = resourceManager->GetTexture("矢印");

	//背景の画像を生成
	std::unique_ptr<ITextureBase> pback = std::make_unique<TextureBase>(stageBack);

	//TextureManagerに追加
	m_TextureManager->Add(std::move(pback));

	for (int i = 1; i <= 4; i++)
	{
		//使用する画像を取得
		ITextureBase* stageTex = resourceManager->GetTexture("ステージセレクト" + std::to_string(i));

		//ステージ1から4の画像を生成
		std::unique_ptr<ITextureBase> pstage = std::make_unique<TextureBase>(stageTex);
		pstage->SetPosition(Vector3(1500.0f, 150.0f, 0.0f));
		pstage->SetScale(Vector3(0.75f));
		m_stageTexture.push_back(pstage.get());

		//TextureManagerに追加
		m_TextureManager->Add(std::move(pstage));
	}


	//他の4つのステージは、未開放にする
	//未開放の画像
	std::unique_ptr<ITextureBase> punopened = std::make_unique<TextureBase>(whiteBack);
	//座標を設定する
	punopened->SetPosition(Vector3(1500.0f, 150.0f, 0.0f));
	//RECTを設定する
	punopened->SetRect(RECT{ 0,0,LONG(m_stageTexture[0]->GetRect().right * 0.75f),
		LONG(m_stageTexture[0]->GetRect().bottom * 0.75f) });
	//色を設定する
	punopened->SetColor(Colors::Black);
	//生ポインタを登録する
	m_stageTexture.push_back(punopened.get());
	//TextureManagerに追加
	m_TextureManager->Add(std::move(punopened));

	//ステージ1のみ初期化する
	m_stageTexture[0]->SetPosition(Vector3(150.0f, 150.0f, 0.0f));

	//左の矢印を生成する
	std::unique_ptr<Arrow> pArrowLeft = std::make_unique<Arrow>(arrow);
	pArrowLeft->InitArrow(Vector3(0.5f), Vector3(1.2f), Vector3(0.5f), 120.0f, ARROWSCALE::UPSCALE);
	pArrowLeft->SetPosition(Vector3(100.0f, 360.0f, 0.0f));
	pArrowLeft->SetOrigin(pArrowLeft->AskOrigin());
	pArrowLeft->SetAngle(180.0f);
	pArrowLeft->SetColor(Colors::Red);
	m_leftArrow = pArrowLeft.get();

	//右の矢印を生成する
	std::unique_ptr<Arrow> pArrowRight = std::make_unique<Arrow>(arrow);
	pArrowRight->InitArrow(Vector3(0.5f), Vector3(1.2f), Vector3(0.5f), 120.0f, ARROWSCALE::UPSCALE);
	pArrowRight->SetPosition(Vector3(1150.0f, 360.0f, 0.0f));
	pArrowRight->SetOrigin(pArrowRight->AskOrigin());
	pArrowRight->SetColor(Colors::Red);
	m_rightArrow = pArrowRight.get();

	//スペースキーの背面のテクスチャを生成する
	std::unique_ptr<TextureBase> pspaceBack = std::make_unique<TextureBase>(whiteBack);
	pspaceBack->SetPosition(Vector3(300.0f, 550.0f, 0.0f));
	pspaceBack->SetRect(RECT{ 0,0,750,150 });
	pspaceBack->SetTarget(0.5f);
	pspaceBack->SetBlink(true);
	m_spaceBackTexture = pspaceBack.get();

	//TextureManagerに追加
	m_TextureManager->Add(std::move(pArrowLeft));
	m_TextureManager->Add(std::move(pArrowRight));
	m_TextureManager->Add(std::move(pspaceBack));
}

void StageSelectScene::UpdateEasing()
{
	//ステートの処理

	//switch文で処理を分ける
	switch (this->GetNowData2())
	{
	case BACKOUT::NONE:
		this->State_None();
		break;
	case BACKOUT::ADMISSON:
		this->State_Admisson();
		break;
	case BACKOUT::EXIT:
		this->State_Exit();
		break;
	case BACKOUT::RESULT:
		this->State_Result();
		break;
	}
}

void StageSelectScene::State_None()
{
	//State_Noneの処理

	//静的データを取得する
	StaticData* staticData = StaticData::GetInstance();

	//SceneManagerを取得する
	SceneManager* sceneManager = SceneManager::GetInstance();

	//SceneStateを取得する
	SceneState* sceneState = sceneManager->GetState();

	//キーボードの状態を取得
	Input* input = Input::GetInstance();
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//右キーが押されたら
	if (keyState.Right)
	{
		//ベクトルの向きを設定する
		m_direct = 1;

		//経過時間を設定する
		m_time = CoroKun::EASING_MAX;

		//ステートを切り替える
		this->AddData2(BACKOUT::ADMISSON);

		//関数オブジェクトを設定する
		m_ifState = [&](int& nextState)
		{
			//領域外を参照しようとした
			if (nextState >= (int)m_stageTexture.size())
			{
				nextState = NULL;
			}
		};

		//スクロール対象以外を初期位置に戻す
		this->OtherInitPos(this->GetNowData1(),BEFORE_POS);

		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__08_スライド);
	}

	//左キーが押されたら
	if (keyState.Left)
	{
		//ベクトルの向きを設定する
		m_direct = -1;

		//経過時間を設定する
		m_time = CoroKun::EASING_MAX;

		//ステートを切り替える
		this->AddData2(BACKOUT::ADMISSON);

		//関数オブジェクトを設定する
		m_ifState = [&](int& nextState)
		{
			//領域外を参照しようとした
			if (nextState < NULL)
			{
				nextState = (int)m_stageTexture.size() - 1;
			}
		};

		//スクロール対象以外を初期位置に戻す
		this->OtherInitPos(this->GetNowData1(), AFTER_POS);

		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__08_スライド);
	}

	//スペースキーで遷移する
	if (keyTrakker.IsKeyPressed(Keyboard::Space))
	{
		//選択したステージが解放されている
		if (m_clearInfo[(int)STAGEINFO::RELEASEINFO][this->GetNowData1()] == "解放済み")
		{
			//読み込むステージを設定する
			staticData->Add("ステージ", std::to_string(this->GetNowData1() + 1));
			staticData->Add("ステージ",this->GetNowData1() + 1);

			//Playに遷移する
			sceneState->SetFadeState(SceneState::FADEOUT);

			//SEを再生する
			soundManager->Play(CRI_CUESHEET_0__24_ステージフェードアウト);
		}
		//ボスステージ
		else
		{
			//SEを再生する
			soundManager->Play(CRI_CUESHEET_0__06_キャンセル音);
		}
	}
}

void StageSelectScene::State_Admisson()
{
	//State_Admissonの処理

	//ベクトルを初期化する
	Vector3 vel = Vector3::Zero;

	//静的データを取得する
	StaticData* staticData = StaticData::GetInstance();

	//前回のフレームからの経過時間を取得する
	float elapsedTime = staticData->Get_FloatData("elapsedTime");

	//現在の添え字を取得する
	int nowState = this->GetNowData1();

	//次の添え字を取得する
	int nextState = nowState + m_direct;

	//設定されたif文の処理を行う
	m_ifState(nextState);

	//ステージの画像を取得する
	ITextureBase* nowStageTexture = m_stageTexture[nowState];

	//次のステージの画像を取得する
	ITextureBase* nextStageTexture = m_stageTexture[nextState];

	//ベクトルを設定する
	vel.x = Easing::InExp(m_time, CoroKun::EASING_TOTALTIME,CoroKun::EASING_MAX) * 250.0f * (m_direct * -1);
	nowStageTexture->SetVelocity(vel);
	nextStageTexture->SetVelocity(vel);

	//経過時間を更新する
	m_time -= elapsedTime * 2.0f;

	//時間が経過したら、ステートを切り替える
	if (m_time <= CoroKun::EASING_MIN)
	{
		m_time = CoroKun::EASING_MIN;
		nowStageTexture->SetVelocity(Vector3::Zero);
		nextStageTexture->SetVelocity(Vector3::Zero);
		this->AddData2(BACKOUT::RESULT);
	}
	
}

void StageSelectScene::State_Exit()
{
	//State_Exitの処理
}

void StageSelectScene::State_Result()
{
	//State_Resultの処理

	//現在の添え字を取得する
	int nowState = this->GetNowData1();

	//次の添え字を取得する
	int nextState = nowState + m_direct;

	//if文の処理
	m_ifState(nextState);

	//添え字を再設定する
	this->AddData1(nextState);

	//ステートを切り替える
	this->AddData2(BACKOUT::NONE);
}

void StageSelectScene::OtherInitPos(const int& target, const Vector3& pos)
{
	for (int i = 0; i < (int)m_stageTexture.size(); i++)
	{
		//ターゲット以外のものであるとき
		if (i != target)
		{
			//ステージ画像を取得
			ITextureBase* stage = m_stageTexture[i];

			//座標を設定する
			stage->SetPosition(pos);
		}
	}
}
