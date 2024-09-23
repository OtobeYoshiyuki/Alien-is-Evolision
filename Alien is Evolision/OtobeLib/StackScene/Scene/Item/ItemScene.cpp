#include "pch.h"
#include "ItemScene.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "OtobeLib/FrameWork/TextureFont/TextureFont.h"
#include "OtobeLib/FrameWork/Input/Input.h"
#include "OtobeLib/StackScene/SceneManager/SceneManager.h"
#include "GameObject/Item/Manager/HealManager.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "OtobeLib/FrameWork/Stream/Stream.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"
#include "OtobeLib/StackScene/Scene/Menu/MenuScene.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//背景の初期座標
const Vector3 ItemScene::INIT_POS_BACK = Vector3(400.0f,0.0f,0.0f);

//アイテムの初期座標
const Vector3 ItemScene::INIT_POS_ITEM = Vector3(500.0f, 100.0f, 0.0f);

//アイテムの裏面
const Vector3 ItemScene::INIT_POS_ITEM_BACK = Vector3(500.0f, 100.0f, 0.0f);

//説明欄の裏面
const Vector3 ItemScene::INIT_POS_EXPLANATION_COLUMN_BACK = Vector3(700.0f, 100.0f, 0.0f);

//背景の切り取り位置
const RECT ItemScene::INIT_RECT_BACK = RECT{0,0,880,720};

//アイテムの裏面の切り取り位置
const RECT ItemScene::INIT_RECT_ITEM_BACK = RECT{ 0,0,200,200 };

//説明欄の切り取り位置
const RECT ItemScene::INIT_RECT_EXPLANATION_COLUMN_BACK = RECT{ 0,0,580,200 };

//アイテム欄の初期座標
const Vector3 ItemScene::INIT_POS_ITEMTEXT = Vector3(550.0f,350.0f,0.0f);

//アイテムの間隔
const Vector3 ItemScene::ITEM_INTERVAL = Vector3(150.0f, 150.0f, 0.0f);

ItemScene::ItemScene():
	m_TextureManager(nullptr),//画像の管理クラス
	m_pageNum(NULL),//現在のページ数
	m_allPageNum(NULL),//ページ総数
	m_selectTexture(nullptr),//選択中の画像
	m_zoomItem(nullptr),//拡大して、表示されているアイテム
	m_rightArrowTexture(nullptr),//右矢印のテクスチャ
	m_leftArrowTexture(nullptr)//左矢印のテクスチャ
{
}

void ItemScene::Initialize()
{
	//静的データを取得する
	StaticData* staticData = StaticData::GetInstance();

	//生成処理
	this->Create();

	//現在のページ数を設定する
	m_pageNum = staticData->Get_IntData("ページ数");

	//ページ総数を設定する
	m_allPageNum = ((int)m_itemText.size() / (int)m_itemBackText.size()) + 1;

	//現在のステートを設定する
	this->SetNowState(NULL);

	//ファイルを取得する
	Stream* File = Stream::GetInstance();

	//ファイルが開けたら開く
	if (File->Open(L"FileData/ItemFont.csv", Stream::FILE::STREAM_IFSTREAM))
	{
		//データを読み込む
		std::vector<std::vector<std::string>> InputData = File->Load_Data();
		//データを変換する
		m_fontInfo = this->Cast(InputData);
		//ファイルを閉じる
		File->Close();
	}

	//選択肢の初期値を設定する
	staticData->Add("選択肢", (std::string)"デフォルト");

	//テキストデータのパスを設定する
	staticData->Add("ストーリーパス", (std::string)"FileData/ItemRequest.csv");
}

void ItemScene::Update()
{
	//キーボードの状態
	Input* input = Input::GetInstance();
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();
	const auto& Mouse_Trakker = Input::GetInstance()->GetMouse_Trakker();

	//SceneManagerを取得する
	SceneManager* manager = SceneManager::GetInstance();

	//CollisionManagerを取得する
	CollisionManager* collisionManager = GameContext::Get<CollisionManager>();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//MenuSceneを取得する
	MenuScene* menu = GameContext::Get<MenuScene>();

	//Playerを取得する
	CoroKun* player = GameContext::Get<CoroKun>();

	//アイテムが選択されていれば
	if (keyTrakker.IsKeyPressed(Keyboard::Space))
	{
		//選択中のアイテムがある時
		if (this->OnSelectItem())
		{
			//HPが最大の時
			if (player->GetHP() >= player->GetMaxHP())
			{
				//SEを再生する
				soundManager->Play(CRI_CUESHEET_0__06_キャンセル音);
			}
			//HPが最大より小さいとき
			else
			{
				//選択肢のSceneを追加生成する
				manager->PushScene("Choices");

				//SEを再生する
				soundManager->Play(CRI_CUESHEET_0__05_決定音);

				//以降の処理は、行わない
				return;
			}
		}
		//アイテムがないとき
		else
		{
			//SEを再生する
			soundManager->Play(CRI_CUESHEET_0__06_キャンセル音);
		}
	}

	//選択肢ではいが押されたら
	if (staticData->Get_StringData("選択肢") == "はい")
	{
		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__38_回復);

		//アイテムを使用する
		this->UseItem();

		//選択肢を初期化する
		staticData->Add("選択肢", (std::string)"デフォルト");
	}

	//Xキーが押されたら、メニューに戻る
	if (keyTrakker.IsKeyPressed(Keyboard::X))
	{
		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__06_キャンセル音);

		manager->PopScene();
		return;
	}

	//ESCが押されたら、Playに戻る
	if (keyTrakker.IsKeyPressed(Keyboard::Escape))
	{
		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__06_キャンセル音);

		manager->PopScene(2);
		collisionManager->ResetUseHero();
		return;
	}

	//TextureManagerの更新
	m_TextureManager->Update();

	//アイテム欄の更新
	this->UpdateItemText();

	//選択画像の更新
	this->UpdateSelectTexture();

	//MenuのTextureManagerの更新をかける(UIを動かすため)
	menu->GetTextureManager()->Update();
}

void ItemScene::Render()
{
	const auto& context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

	Graphics* Graph = Graphics::GetInstance();

	//フォントを取得する
	TextureFont* font = TextureFont::GetInstance();

	Graph->GetSpriteBatch()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred,
		Graph->GetCommonStates()->NonPremultiplied(), Graph->GetCommonStates()->PointClamp(),
		Graph->GetCommonStates()->DepthDefault(), nullptr,
		[&] {context->OMSetBlendState(Graph->GetCommonStates()->NonPremultiplied(), nullptr, 0xffffffff); });

	//TextureManagerの描画
	m_TextureManager->Render();

	//for文の先頭
	int first = m_pageNum * (int)m_itemBackText.size() - (int)m_itemBackText.size();

	//アイテムがあれば、表示
	if (this->OnSelectItem())
	{
		//選択中のアイテムを取得
		ITextureBase* item = this->SelectNowItem();

		for (const std::pair<std::string, Vector2>& exColu : m_fontInfo[item->GetName()])
		{
			//フォントを描画する
			font->DrawFormatString(exColu.first, exColu.second, 1.3f, Colors::Black);
		}
	}
	//アイテムがない時
	else
	{
		for (const std::pair<std::string, Vector2>& exColu : m_fontInfo["アイテム欄5"])
		{
			//フォントを描画する
			font->DrawFormatString(exColu.first, exColu.second, 2.0f, Colors::Black);
		}
	}

	//Xキーのフォントの描画
	font->DrawFormatString("Xキ－：メニュ－へ", Vector2(50.0f, 60.0f), 1.0f, Colors::Black);

	//SPACEキーのフォントの描画
	font->DrawFormatString("SPACEキ－：決定", Vector2(525.0f, 30.0f), 1.0f, Colors::Black);

	//ESCキーのフォントの描画
	font->DrawFormatString("ESCキ－：プレイへ", Vector2(925.0f, 30.0f), 1.0f, Colors::Black);

	//ページ数を描画
	font->DrawFormatString(std::to_string(m_pageNum) + "/" + std::to_string(m_allPageNum),
		Vector2(750.0f, 640.0f), 2.0f, Colors::Black);


	Graph->GetSpriteBatch()->End();
}

void ItemScene::Finalize()
{
}

void ItemScene::Create()
{
	//静的データを取得する
	StaticData* staticData = StaticData::GetInstance();

	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//TextureManagerを生成する
	m_TextureManager = std::make_unique<TextureManager>();

	//使用するテクスチャを取得する
	ITextureBase* back = resourceManager->GetTexture("WhiteBack");
	ITextureBase* item = resourceManager->GetTexture("回復");
	ITextureBase* arrow = resourceManager->GetTexture("矢印");

	//背景の画像を生成する
	std::unique_ptr<ITextureBase> pback = std::make_unique<TextureBase>(back);

	//座標を設定する
	pback->SetPosition(INIT_POS_BACK);

	//切り取り位置を設定する
	pback->SetRect(INIT_RECT_BACK);

	//色を設定する
	pback->SetColor(Colors::Gray);

	//アイテムの裏面を生成する
	std::unique_ptr<ITextureBase> pitemBack = std::make_unique<TextureBase>(back);

	//座標を設定する
	pitemBack->SetPosition(INIT_POS_ITEM_BACK);

	//切り取り位置を設定する
	pitemBack->SetRect(INIT_RECT_ITEM_BACK);

	//色を設定する
	pitemBack->SetColor(Colors::Aqua);

	//アイテムを生成する
	std::unique_ptr<ITextureBase> pitem = std::make_unique<TextureBase>(item);

	//座標を設定する
	pitem->SetPosition(INIT_POS_ITEM);

	//拡大倍率を設定する
	pitem->SetScale(Vector3(0.5f));

	//生ポインタを登録する
	m_zoomItem = pitem.get();

	//説明欄の裏面を生成する
	std::unique_ptr<ITextureBase> pexCol = std::make_unique<TextureBase>(back);

	//座標を設定する
	pexCol->SetPosition(INIT_POS_EXPLANATION_COLUMN_BACK);

	//切り取り位置を設定する
	pexCol->SetRect(INIT_RECT_EXPLANATION_COLUMN_BACK);

	//TextureManagerに追加する
	m_TextureManager->Add(std::move(pback));
	m_TextureManager->Add(std::move(pitemBack));
	m_TextureManager->Add(std::move(pitem));
	m_TextureManager->Add(std::move(pexCol));

	for (int i = 0; i < COLITEM; i++)
	{
		for (int j = 0; j < ROWITEM; j++)
		{
			//アイテム欄の背面を生成する
			std::unique_ptr<ITextureBase> pitemTextBack = std::make_unique<TextureBase>(back);

			//座標を設定する
			pitemTextBack->SetPosition(INIT_POS_ITEMTEXT + Vector3(ITEM_INTERVAL.x * j, ITEM_INTERVAL.y * i, 0.0f));

			//切り取り位置を設定する
			pitemTextBack->SetRect(RECT{ 0,0,120,120 });

			//色を設定する
			pitemTextBack->SetColor(Colors::Aquamarine);
			
			//生ポインタを登録する
			m_itemBackText.push_back(pitemTextBack.get());

			//TextureManagerに追加する
			m_TextureManager->Add(std::move(pitemTextBack));
		}
	}

	//ItemManagerを取得する
	HealManager* itemManager = GameContext::Get<PlayScene>()->GetHealManager();

	for (int i = 0;i < (int)itemManager->GetItem().size(); i++)
	{
		//アイテムの情報を取得
		std::pair<std::string, float>itemInfo = this->ChangeItemTag(itemManager->GetItem()[i]->GetName());

		//使用するテクスチャを取得する
		ITextureBase* useTex = resourceManager->GetTexture(itemInfo.first);

		//添え字を合わせる
		int t = i % (int)m_itemBackText.size();

		//画像を生成する
		std::unique_ptr<Heal> pItemTex = std::make_unique<Heal>(useTex);

		//回復量を設定する
		pItemTex->SetHealPower(itemInfo.second);

		//描画は全て消す
		pItemTex->SetState(ITextureBase::STATE::NONE);

		//座標は、それぞれ合わせる
		pItemTex->SetPosition(m_itemBackText[t]->GetPosition());

		//生ポインタを登録する
		m_itemText.push_back(pItemTex.get());

		//TextureManagerに追加する
		m_TextureManager->Add(std::move(pItemTex));
	}

	//選択中の画像を生成する
	std::unique_ptr<ITextureBase> pselect = std::make_unique<TextureBase>(back);

	//座標を設定する
	pselect->SetPosition(INIT_POS_ITEMTEXT);

	//色を設定する
	pselect->SetColor(Color{ 0.0f,0.0f,1.0f,0.5f });

	//切り取り位置を設定する
	pselect->SetRect(RECT{ 0,0,120,120 });

	//生ポインタを登録する
	m_selectTexture = pselect.get();

	//TextureManagerに追加する
	m_TextureManager->Add(std::move(pselect));

	//Xキーの裏面のテクスチャを生成する
	std::unique_ptr<ITextureBase> pbackXkey = std::make_unique<TextureBase>(back);
	pbackXkey->SetPosition(Vector3(30.0f, 50.0f, 0.0f));
	pbackXkey->SetRect(RECT{ 0,0,330,50 });

	//ESCキーの裏面のテクスチャを生成する
	std::unique_ptr<ITextureBase> pbackEsckey = std::make_unique<TextureBase>(back);
	pbackEsckey->SetPosition(Vector3(900.0f, 25.0f, 0.0f));
	pbackEsckey->SetRect(RECT{ 0,0,370,50 });

	//SPACEキーの裏面のテクスチャを生成する
	std::unique_ptr<ITextureBase> pbackSpace = std::make_unique<TextureBase>(back);
	pbackSpace->SetPosition(Vector3(500.0f, 25.0f, 0.0f));
	pbackSpace->SetRect(RECT{ 0,0,370,50 });

	//左の矢印を生成する
	std::unique_ptr<Arrow> pArrowLeft = std::make_unique<Arrow>(arrow);
	pArrowLeft->InitArrow(Vector3(0.4f), Vector3(0.7f), Vector3(0.4f), 120.0f, ARROWSCALE::UPSCALE);
	pArrowLeft->SetPosition(Vector3(475.0f, 475.0f, 0.0f));
	pArrowLeft->SetOrigin(pArrowLeft->AskOrigin());
	pArrowLeft->SetAngle(180.0f);
	pArrowLeft->SetColor(Colors::Red);
	pArrowLeft->SetState(ITextureBase::STATE::NONE);
	m_leftArrowTexture = pArrowLeft.get();

	//右の矢印を生成する
	std::unique_ptr<Arrow> pArrowRight = std::make_unique<Arrow>(arrow);
	pArrowRight->InitArrow(Vector3(0.4f), Vector3(0.7f), Vector3(0.4f), 120.0f, ARROWSCALE::UPSCALE);
	pArrowRight->SetPosition(Vector3(1200.0f, 475.0f, 0.0f));
	pArrowRight->SetOrigin(pArrowRight->AskOrigin());
	pArrowRight->SetColor(Colors::Red);
	pArrowRight->SetState(ITextureBase::STATE::NONE);
	m_rightArrowTexture = pArrowRight.get();

	//TextureManagerに追加する
	m_TextureManager->Add(std::move(pbackXkey));
	m_TextureManager->Add(std::move(pbackEsckey));
	m_TextureManager->Add(std::move(pbackSpace));
	m_TextureManager->Add(std::move(pArrowLeft));
	m_TextureManager->Add(std::move(pArrowRight));
}

const std::pair<std::string, float> ItemScene::ChangeItemTag(const std::string& name)
{
	//変数を宣言する
	std::pair<std::string, float> itemInfo;

	//対応するものに変換する
	if (name == "回復")itemInfo = { "アイテム欄1",0.5f };
	if (name == "回復1")itemInfo = { "アイテム欄2",0.1f };
	if (name == "回復2")itemInfo = { "アイテム欄3",0.2f };
	if (name == "回復3")itemInfo = { "アイテム欄4",0.3f };

	//変換したものを返す
	return itemInfo;
}

//選択しているアイテムから、元のタグと、座標を取得する
const std::pair<std::string, Vector3> ItemScene::ReverseItem(const std::string& name)
{
	//変数を宣言する
	std::pair<std::string, Vector3> itemInfo;

	//対応するものに変換する
	if (name == "アイテム欄1")itemInfo = { "回復",INIT_POS_ITEM };
	if (name == "アイテム欄2")itemInfo = { "回復1",INIT_POS_ITEM + Vector3(75.0f,0.0f,0.0f) };
	if (name == "アイテム欄3")itemInfo = { "回復2",INIT_POS_ITEM + Vector3(40.0f,0.0f,0.0f) };
	if (name == "アイテム欄4")itemInfo = { "回復3",INIT_POS_ITEM + Vector3(40.0f,0.0f,0.0f) };
	if (name == "アイテム欄5")itemInfo = { "空っぽ",INIT_POS_ITEM };

	//変換したものを返す
	return itemInfo;
}

void ItemScene::UpdateItemText()
{
	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//ページ総数を設定する
	m_allPageNum = ((int)m_itemText.size() / (int)m_itemBackText.size()) + 1;

	//アイテムが8個の時は、ページ総数を1に変更する
	if ((int)m_itemText.size() == (int)m_itemBackText.size())m_allPageNum = 1;

	//ページ総数が1の時
	if (m_allPageNum <= 1)
	{
		//ページ数を1にする
		m_pageNum = 1;

		//右矢印の表示を消す
		m_rightArrowTexture->SetState(ITextureBase::STATE::NONE);

		//左矢印の表示を消す
		m_leftArrowTexture->SetState(ITextureBase::STATE::NONE);
	}
	//現在のページ数が2ページ目から総ページ数未満の場合
	else if (m_pageNum > 1 && m_pageNum < m_allPageNum)
	{
		//右矢印を表示
		m_rightArrowTexture->SetState(ITextureBase::STATE::DRAW);

		//左矢印を表示
		m_leftArrowTexture->SetState(ITextureBase::STATE::DRAW);
	}
	//ページ数が１ページ目の時
	else if (m_pageNum == 1)
	{
		//右矢印を表示
		m_rightArrowTexture->SetState(ITextureBase::STATE::DRAW);

		//左矢印の表示を消す
		m_leftArrowTexture->SetState(ITextureBase::STATE::NONE);
	}
	//ページ数が最終ページの時
	else if (m_pageNum == m_allPageNum)
	{
		//右矢印の表示を消す
		m_rightArrowTexture->SetState(ITextureBase::STATE::NONE);

		//左矢印を表示
		m_leftArrowTexture->SetState(ITextureBase::STATE::DRAW);
	}
		
	//for文の先頭
	int first = m_pageNum * (int)m_itemBackText.size() - (int)m_itemBackText.size();

	//for文の終わり
	int finish = first + (int)m_itemBackText.size();

	//アイテムの獲得数が8個未満の場合は
	if (finish > (int)m_itemText.size())
	{
		//終わりを獲得したアイテムの数にする
		finish = (int)m_itemText.size();
	}

	for (ITextureBase* item : m_itemText)
	{
		//全てのアイテムの描画を切る
		item->SetState(ITextureBase::STATE::NONE);
	}

	//現在、開かれているページに対して、更新をかける
	for (int i = first; i < finish; i++)
	{
		//添え字を合わせる
		int t = i % (int)m_itemBackText.size();

		//選択しているページのアイテムのみ、表示する
		m_itemText[i]->SetState(ITextureBase::STATE::DRAW);

		//座標は、それぞれ合わせる
		m_itemText[i]->SetPosition(m_itemBackText[t]->GetPosition());
	}

	//アイテムがなければ、何もしない
	if (!this->OnSelectItem())
	{
		//現在、選択しているアイテムを大きく表示する
		std::pair<std::string, Vector3> zoomInfo = this->ReverseItem("アイテム欄5");

		//現在使用しているテクスチャを取得する
		ITextureBase* usingTexture = resourceManager->GetTexture(zoomInfo.first);

		//テクスチャを入れ替える
		m_zoomItem->SetTexture(usingTexture);

		//拡大倍率を設定する
		m_zoomItem->SetScale(Vector3(0.5f));

		//座標を更新する
		m_zoomItem->SetPosition(zoomInfo.second);
	}
	else
	{
		//現在、選択しているアイテムを大きく表示する
		std::pair<std::string, Vector3> zoomInfo = this->ReverseItem(this->SelectNowItem()->GetName());

		//現在使用しているテクスチャを取得する
		ITextureBase* usingTexture = resourceManager->GetTexture(zoomInfo.first);

		//テクスチャを入れ替える
		m_zoomItem->SetTexture(usingTexture);

		//拡大倍率を設定する
		m_zoomItem->SetScale(Vector3(0.5f));

		//座標を更新する
		m_zoomItem->SetPosition(zoomInfo.second);
	}
}

void ItemScene::UpdateSelectTexture()
{
	//キーボードの状態
	Input* input = Input::GetInstance();
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();
	const auto& Mouse_Trakker = Input::GetInstance()->GetMouse_Trakker();

	//右キーが押されたら、右へ選択画像を移動
	if (keyTrakker.IsKeyPressed(Keyboard::Right))
	{
		//選択中の処理
		this->InputSelect(1, [&](int& state, Vector3& pos)
			{
				//上限を超えたら、ページを更新
				if (state % ROWITEM == NULL)
				{
					//ページ総数に達していなければ
					if (m_pageNum < m_allPageNum)
					{
						//ページ数を更新
						m_pageNum++;
					}
					//選択対象を初期位置に戻す
					pos.x = INIT_POS_ITEMTEXT.x;

					//ステートを元に戻す
					state -= ROWITEM;
				}
				//上限を超えていなければ、入れ替える
				else
				{
					//隣の座標を取得する
					Vector3 nextPos = m_itemBackText[state]->GetPosition();

					//入れ替えを行う
					std::swap(pos, nextPos);
				}
				;}
		);
	}

	//左キーが押されたら、左へ選択画像を移動
	if (keyTrakker.IsKeyPressed(Keyboard::Left))
	{
		//選択中の処理
		this->InputSelect(-1, [&](int& state, Vector3& pos)
			{
				//上限を超えたら、ページを更新
				if ((state + 1) % ROWITEM == NULL)
				{
					//1ページ目以降の場合
					if (m_pageNum > 1)
					{
						//ページ数を更新
						m_pageNum--;
					}
					//ステートを元に戻す
					state += ROWITEM;

					//選択対象を初期位置に戻す
					pos.x = m_itemBackText[state]->GetPosition().x;
				}
				//上限を超えていなければ、入れ替える
				else
				{
					//隣の座標を取得する
					Vector3 nextPos = m_itemBackText[state]->GetPosition();

					//入れ替えを行う
					std::swap(pos, nextPos);
				}
				; }
		);
	}

	//上キーが押されたら、上へ選択画像を移動
	if (keyTrakker.IsKeyPressed(Keyboard::Up))
	{
		//選択中の処理
		this->InputSelect(-4, [&](int& state, Vector3& pos)
			{
				//上限を超えたら、ページを更新
				if (state < NULL)
				{
					//ステートを元に戻す
					state += ROWITEM * 2;

					//選択対象を初期位置に戻す
					pos.y = m_itemBackText[state]->GetPosition().y;
				}
				//上限を超えていなければ、入れ替える
				else
				{
					//隣の座標を取得する
					Vector3 nextPos = m_itemBackText[state]->GetPosition();

					//入れ替えを行う
					std::swap(pos, nextPos);
				}
			});
	}

	//下キーが押されたら、下へ選択画像を移動
	if (keyTrakker.IsKeyPressed(Keyboard::Down))
	{
		//選択中の処理
		this->InputSelect(4, [&](int& state, Vector3& pos)
			{
				//上限を超えたら、ページを更新
				if (state >= (int)m_itemBackText.size())
				{
					//ステートを元に戻す
					state -= ROWITEM * 2;

					//選択対象を初期位置に戻す
					pos.y = m_itemBackText[state]->GetPosition().y;
				}
				//上限を超えていなければ、入れ替える
				else
				{
					//隣の座標を取得する
					Vector3 nextPos = m_itemBackText[state]->GetPosition();

					//入れ替えを行う
					std::swap(pos, nextPos);
				}
			});
	}
}

void ItemScene::InputSelect(const int& move, std::function<void(int&, Vector3&)> func)
{
	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//SEを再生
	soundManager->Play(CRI_CUESHEET_0__07_上下音);

	//現在の選択対象を取得する
	int state = this->GetNowState();

	//選択中の座標を取得する
	Vector3 nowPos = m_selectTexture->GetPosition();

	//ステートを指定した数だけずらす
	state += move;

	//if文の処理
	func(state, nowPos);

	//選択対象の画像の更新を行う
	m_selectTexture->SetPosition(nowPos);

	//ステートの更新を行う
	this->SetNowState(state);
}

const bool ItemScene::OnSelectItem()
{
	//for文の先頭
	int first = m_pageNum * (int)m_itemBackText.size() - (int)m_itemBackText.size();

	//アイテムがあれば、表示
	if (first + this->GetNowState() < (int)m_itemText.size())return true;

	return false;
}

Item* ItemScene::SelectNowItem()
{
	//for文の先頭
	int first = m_pageNum * (int)m_itemBackText.size() - (int)m_itemBackText.size();

	//現在のアイテムを返す
	return m_itemText[first + this->GetNowState()];
}

void ItemScene::UseItem()
{
	//Playerを取得する
	CoroKun* player = GameContext::Get<CoroKun>();

	//ItemManagerを取得する
	HealManager* itemManager = GameContext::Get<PlayScene>()->GetHealManager();

	//アイテムを取得する
	Item* item = this->SelectNowItem();

	//キャストする
	Heal* healItem = dynamic_cast<Heal*>(item);

	//回復量を設定する
	int heal = player->GetMaxHP() * healItem->GetHealPower();

	//アイテムに応じたHPを回復する
	player->OnHealHp(heal);

	//使用したアイテムを消す
	item->SetIsVal(true);
	m_itemText.erase(m_itemText.begin() + this->OnSelectPlace());
	itemManager->EraseItem(this->OnSelectPlace());
}

const int ItemScene::OnSelectPlace()
{
	//for文の先頭
	int first = m_pageNum * (int)m_itemBackText.size() - (int)m_itemBackText.size();

	//添え字を返す
	return first + this->GetNowState();
}

const FontInfo ItemScene::Cast(const std::vector<std::vector<std::string>>& input)
{
	//データを格納するコンテナを宣言する
	FontInfo itemFont;

	//データを格納するコンテナを宣言する
	std::vector<std::vector<std::string>> strContena;

	//データの数を宣言する
	strContena.resize(input.size() - 1);

	//指定したグループの情報を取得する
	for (size_t i = 0; i < input.size() - 1; i++)
	{
		for (size_t j = 0; j < input[i].size(); j++)
		{
			//1行ずつデータを格納する
			strContena[i].push_back(input[i + 1][j]);
		}
	}

	//データの型を変換する
	for (int i = 0; i < (int)strContena.size(); i++)
	{
		itemFont[strContena[i][0]].push_back({ strContena[i][1],Vector2(std::stof(strContena[i][2]),std::stof(strContena[i][3])) });
	}

	//変換したデータを返す
	return itemFont;
}
