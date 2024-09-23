#include "pch.h"
#include "SelectScene.h"
#include "OtobeLib/StackScene/SceneManager/SceneManager.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/FrameWork/Input/Input.h"
#include "OtobeLib/FrameWork/TextureFont/TextureFont.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//必殺技の名前(爆発)
const std::string SelectScene::DEATHBLOW_NAME_EXPROSION_TOP = "ダ－ク";
const std::string SelectScene::DEATHBLOW_NAME_EXPROSION_BOTOM = "エクスプロ－ジョン";
const std::string SelectScene::DEATHBLOW_NAME_EXPROSION = "ダ－クエクスプロ－ジョン";

//必殺技の名前(アビリティ)
const std::string SelectScene::DEATHBLOW_NAME_ABILITY_TOP = "リバ－ス";
const std::string SelectScene::DEATHBLOW_NAME_ABILITY_BOTOM = "アビリティ";
const std::string SelectScene::DEATHBLOW_NAME_ABILITY = "リバ－スアビリティ";

//点滅の間隔の最小値
const float SelectScene::BLINK_INTERVAL_MIN = 0.1f;

SelectScene::SelectScene():
	m_backTexture(nullptr),//背景の画像
	m_TextureManager(nullptr),//画像の管理クラス
	m_frameTexture(nullptr)//周りの枠の画像
{
	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//使用するテクスチャを取得する
	ITextureBase* back = resourceManager->GetTexture("選択背景");
	ITextureBase* whiteBack = resourceManager->GetTexture("WhiteBack");
	ITextureBase* frame = resourceManager->GetTexture("周りの枠");

	//CoroKunを取得
	CoroKun* player = GameContext::Get<CoroKun>();

	//TextureManagerを生成する
	m_TextureManager = std::make_unique<TextureManager>();

	//背景の画像を生成する
	std::unique_ptr<ITextureBase> pback = std::make_unique<TextureBase>(back);
	//描画順を設定する
	pback->SetDrawOrder((int)DRAWORDER::BACKGROUND);
	//生ポインタを登録する
	m_backTexture = pback.get();
	//TextureManagerに登録する
	m_TextureManager->Add(std::move(pback));

	//半透明の黒を設定する
	Color blackHalf = Color{ 0.0f,0.0f,0.0f,0.5f };

	for (int i = 0; i < ARRAY_NUM; i++)
	{
		//四角の画像を生成する
		std::unique_ptr<TextureBase> psquare = std::make_unique<TextureBase>(whiteBack);
		//描画順を設定する
		psquare->SetDrawOrder((int)DRAWORDER::WHITEBACK);
		//切り取り位置を設定する
		psquare->SetRect(RECT{ 0,0,600,200 });
		//色を設定する
		psquare->SetColor(Colors::White);
		//点滅の設定
		psquare->SetBlink(false);
		//点滅の秒数
		psquare->SetFinal(5);
		//点滅オプションの解放
		psquare->SetFinalOption(true);
		//点滅の間隔の設定
		psquare->SetTarget(1.0f);
		//生ポインタを登録する
		m_squareTexture.push_back(psquare.get());
		//TextureManagerに登録する
		m_TextureManager->Add(std::move(psquare));
	}

	//四角の画像の座標の設定を行う
	m_squareTexture[ARRAY_0]->SetPosition(Vector3(300.0f, 50.0f, 0.0f));
	m_squareTexture[ARRAY_1]->SetPosition(Vector3(300.0f, 400.0f, 0.0f));


	for (int i = 0; i < ARRAY_NUM; i++)
	{
		//全面の半透明の画像を生成する
		std::unique_ptr<ITextureBase> phalfTex = std::make_unique<TextureBase>(whiteBack);

		//描画順を設定する
		phalfTex->SetDrawOrder((int)DRAWORDER::FRONT_SELECT);

		//切り取り位置を設定する
		phalfTex->SetRect(RECT{ 0,0,1280,330 });

		//生ポインタを登録する
		m_frontTexture.push_back(phalfTex.get());

		//TextureManagerに登録する
		m_TextureManager->Add(std::move(phalfTex));
	}

	//全面の半透明の画像の座標の設定を行う
	m_frontTexture[ARRAY_0]->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	m_frontTexture[ARRAY_1]->SetPosition(Vector3(0.0f, 330.0f, 0.0f));

	//全面の半透明の画像の色の設定を行う
	if (player->GetSpecial() < GAGE_COST_EXPROSION)
	{
		m_frontTexture[ARRAY_0]->SetColor(Vector4(0.0f,0.0f,0.0f,0.75f));
	}
	else m_frontTexture[ARRAY_0]->SetColor(Vector4::Zero);

	m_frontTexture[ARRAY_1]->SetColor(blackHalf);

	//画面下部のUIを生成する
	std::unique_ptr<ITextureBase> pBotomUI = std::make_unique<TextureBase>(whiteBack);
	pBotomUI->SetPosition(Vector3(0.0f, 660.0f, 0.0f));
	pBotomUI->SetRect(RECT{ 0,0,1280,60 });
	m_TextureManager->Add(std::move(pBotomUI));

	//周りの枠のUIを生成する
	std::unique_ptr<ITextureBase> pframe = std::make_unique<TextureBase>(frame);
	pframe->SetDrawOrder((int)DRAWORDER::SELECT_FRAME);
	m_frameTexture = pframe.get();
	m_TextureManager->Add(std::move(pframe));

	//選択中の必殺技を設定する
	this->AddData2(ARRAY_0);

	//現在のステートを設定する
	this->AddData1(SELECT::WAITKEY);
}

void SelectScene::Initialize()
{
	//SceneManagerからSceneStateを取得する
	SceneManager* Manager = SceneManager::GetInstance();
	SceneState* state = Manager->GetState();

	state->SetFadeState(SceneState::WAITKEY);
	state->SetAlfa(0.0f);
}

void SelectScene::Update()
{
	//SceneManagerからSceneStateを取得する
	SceneManager* Manager = SceneManager::GetInstance();
	SceneState* state = Manager->GetState();

	//ステートの更新処理
	this->UpdateState();

	//TextureManagerの更新
	m_TextureManager->Update();
}

void SelectScene::Render()
{
	//描画系の状態を取得する
	Graphics* Graph = Graphics::GetInstance();
	const auto& context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

	//フォントの状態を取得する
	TextureFont* Font = TextureFont::GetInstance();

	//描画の開始
	Graph->GetSpriteBatch()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred,
		Graph->GetCommonStates()->NonPremultiplied(), Graph->GetCommonStates()->PointClamp(),
		Graph->GetCommonStates()->DepthDefault(), nullptr,
		[&] {context->OMSetBlendState(Graph->GetCommonStates()->NonPremultiplied(), nullptr, 0xffffffff); });

	//画像の描画処理
	m_TextureManager->Render();

	//技名の描画
	if (m_squareTexture[ARRAY_0]->NowBlinking())
	{
		Font->DrawFormatString(DEATHBLOW_NAME_EXPROSION_TOP, Vector2(550.0f, 70.0f), 1.5f, Colors::Black);
		Font->DrawFormatString(DEATHBLOW_NAME_EXPROSION_BOTOM, Vector2(400.0f, 150.0f), 1.5f, Colors::Black);
	}

	if (m_squareTexture[ARRAY_1]->NowBlinking())
	{
		Font->DrawFormatString(DEATHBLOW_NAME_ABILITY_TOP, Vector2(500.0f, 450.0f), 1.5f, Colors::Black);
		Font->DrawFormatString(DEATHBLOW_NAME_ABILITY_BOTOM, Vector2(550.0f, 520.0f), 1.5f, Colors::Black);
	}

	//フォントの描画
	//上下キーのフォントの描画
	Font->DrawFormatString("上下キ－：移動", Vector2(10.0f, 670.0f), 1.0f, Colors::Black);

	//SPACEキーのフォントの描画
	Font->DrawFormatString("SPACEキ－：決定", Vector2(500.0f, 670.0f), 1.0f, Colors::Black);

	//ESCキーのフォントの描画
	Font->DrawFormatString("ESCキ－：戻る", Vector2(1000.0f, 670.0f), 1.0f, Colors::Black);

	//描画の終了
	Graph->GetSpriteBatch()->End();
}

void SelectScene::Finalize()
{
}

void SelectScene::Create()
{
}

void SelectScene::UpdateState()
{
	//ステートの更新処理

	//Switch文で処理を分ける
	switch (this->GetNowData1())
	{
	case SELECT::WAITKEY:
		this->State_WaitKey();
		break;
	case SELECT::CALCING:
		this->State_Calcing();
		break; 
	case SELECT::CHANGE_CUTIN:
		this->State_Change_CutIn();
		break;
	case SELECT::CHANGE_AND_DELETE:
		this->State_Change_Delete();
		break;
	}
}

void SelectScene::State_WaitKey()
{
	//WAITKEYの処理

	//CoroKunを取得
	CoroKun* player = GameContext::Get<CoroKun>();

	//SceneManagerからSceneStateを取得する
	SceneManager* Manager = SceneManager::GetInstance();
	SceneState* state = Manager->GetState();

	//静的データを取得する
	StaticData* staticData = StaticData::GetInstance();

	//キーの状態の取得
	Input* input = Input::GetInstance();
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//Escキーが押されたら、PlaySceneに戻る
	if (keyTrakker.IsKeyPressed(Keyboard::Escape))
	{
		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__06_キャンセル音);

		Manager->PopScene();
	}

	//上下のキーが押されたら、選択を変更する
	if (keyTrakker.IsKeyPressed(Keyboard::Up) ||
		keyTrakker.IsKeyPressed(Keyboard::Down))
	{
		//選択対象を取得する
		int target = this->GetNowData2();

		//選択対象をデクリメントする
		target--;

		//選択対象を絶対値に直す
		target = abs(target);

		//色を取得する
		Color preColor = m_frontTexture[ARRAY_0]->GetColor();
		Color nowColor = m_frontTexture[ARRAY_1]->GetColor();

		//入れ替える
		std::swap(preColor, nowColor);

		//色を再設定する
		m_frontTexture[ARRAY_0]->SetColor(preColor);
		m_frontTexture[ARRAY_1]->SetColor(nowColor);

		//選択対象を設定する
		this->AddData2(target);

		//ダークエクスプロージョンの時
		if (target == ARRAY_0)
		{
			//SPが300以上あるとき
			if (player->GetSpecial() >= SelectScene::GAGE_COST_EXPROSION)
			{
				m_frontTexture[target]->SetColor(Color(0.0f, 0.0f, 0.0f, 0.0f));
			}
			//SPが300未満の時
			else m_frontTexture[target]->SetColor(Color(0.0f, 0.0f, 0.0f, 0.75f));
		}
		//ターゲットじゃないとき
		else m_frontTexture[ARRAY_0]->SetColor(Color(0.0f, 0.0f, 0.0f, 0.5f));

		//リバースアビリティの時
		if (target == ARRAY_1)
		{
			//SPが100以上あるとき
			if (player->GetSpecial() >= SelectScene::GAGE_COST_REVASEAVIRITY)
			{
				m_frontTexture[target]->SetColor(Color(0.0f, 0.0f, 0.0f, 0.0f));
			}
			//SPが300未満の時
			else m_frontTexture[target]->SetColor(Color(0.0f, 0.0f, 0.0f, 0.75f));
		}
		//ターゲットじゃないとき
		else m_frontTexture[ARRAY_1]->SetColor(Color(0.0f, 0.0f, 0.0f, 0.5f));

		//選択中の画像の座標を設定する
		m_frameTexture->SetPosition(m_frontTexture[target]->GetPosition());

		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__07_上下音);
	}

	//スペースキーが押されたら、選択対象を点滅させる
	if (keyTrakker.IsKeyPressed(Keyboard::Space))
	{
		//ダークエクスプロージョンを選択
		if (this->GetNowData2() == ARRAY_0)
		{
			//ゲージが300以上の時
			if (player->GetSpecial() >= GAGE_COST_EXPROSION)
			{
				//静的データに情報を追加する
				staticData->Add("必殺技", DEATHBLOW_NAME_EXPROSION);

				//必殺技を使用する
				player->OnSpesial(GAGE_COST_EXPROSION);

				//ステートを切り替える
				this->AddData1(SELECT::CALCING);

				//点滅させる
				m_squareTexture[this->GetNowData2()]->SetBlink(true);

				//SEを再生する
				soundManager->Play(CRI_CUESHEET_0__05_決定音);
			}
			else
			{
				//SEを再生する
				soundManager->Play(CRI_CUESHEET_0__06_キャンセル音);
			}
		}

		//リバースアビリティを選択
		if (this->GetNowData2() == ARRAY_1)
		{
			//ゲージが100以上の時
			if (player->GetSpecial() >= GAGE_COST_REVASEAVIRITY)
			{
				//静的データに情報を追加する
				staticData->Add("必殺技", DEATHBLOW_NAME_ABILITY);

				//必殺技を使用する
				player->OnSpesial(GAGE_COST_REVASEAVIRITY);

				//ステートを切り替える
				this->AddData1(SELECT::CALCING);

				//点滅させる
				m_squareTexture[this->GetNowData2()]->SetBlink(true);

				//SEを再生する
				soundManager->Play(CRI_CUESHEET_0__05_決定音);
			}
			else
			{
				//SEを再生する
				soundManager->Play(CRI_CUESHEET_0__06_キャンセル音);
			}
		}
	}
}

void SelectScene::State_Calcing()
{
	//CALCINGの処理

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//選択対象を取得する
	TextureBase* target = m_squareTexture[this->GetNowData2()];

	//点滅間隔を取得する
	float interval = target->GetTarget();

	//点滅の間隔を0.1fまで減らす
	if (interval > BLINK_INTERVAL_MIN)
	{
		interval -= 0.005f;
	}

	//点滅間隔を設定する
	target->SetTarget(interval);

	//現在表示されていれば、SEを再生する
	if (target->GetTargetCount() <= 0.0f)
	{
		soundManager->Play(CRI_CUESHEET_0__40_点滅);
	}

	//点滅が終了
	if (!target->GetBlink())
	{
		//ステートを切り替える
		this->AddData1(SELECT::CHANGE_CUTIN);
	}
}

void SelectScene::State_Change_CutIn()
{
	//CHANGE_CUTINの処理

	//SceneManagerからSceneStateを取得する
	SceneManager* Manager = SceneManager::GetInstance();

	//CutInSceneを追加生成する
	Manager->PushScene("CutIn");

	//ステートを切り替える
	this->AddData1(SELECT::CHANGE_AND_DELETE);
}

void SelectScene::State_Change_Delete()
{
	//CHANGE_AND_DELETEの処理

	//SceneManagerからSceneStateを取得する
	SceneManager* Manager = SceneManager::GetInstance();

	//現在のSceneを消す
	Manager->PopScene();

	//DirecTingSceneを追加生成する
	Manager->PushScene("DirecTing");
}

