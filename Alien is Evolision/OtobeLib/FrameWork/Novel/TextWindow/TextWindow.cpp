#include "pch.h"
#include "TextWindow.h"
#include "OtobeLib/FrameWork/Novel/Novel.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"
#include "OtobeLib/FrameWork/Stream/Stream.h"
#include "OtobeLib/FrameWork/Input/Input.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//改行＆更新停止
const std::string TextWindow::NEWLINE_AND_STOP = "＃";

//改行
const std::string TextWindow::NEWLINE = "//";

//テキストのテクスチャの初期位置
const Vector3 TextWindow::INIT_POS = Vector3(64.0f, 455.0f, 0.0f);

//次に表示するテクスチャのインターバル
const float TextWindow::MAX_INTERBAL = 64.0f;

TextWindow::TextWindow(ITextureBase* texture):
	TextureBase(texture),//基底クラスに情報を送信する
	m_drawCheck(true),//描画するフラグ
	m_drawCount(NULL),//描画するカウント
	m_keyCheck(true),//キー入力のフラグ
	m_setupID(NULL),//描画の先頭
	m_frameCount(NULL),//フレーム数
	m_drawInterval(5)//カウントを更新する感覚
{
}

void TextWindow::Initialize()
{
	//ファイルを取得する
	Stream* File = Stream::GetInstance();

	//ファイルを開くのに成功したら開く
	if (File->Open(L"FileData/NoDrawList.csv", Stream::FILE::STREAM_IFSTREAM))
	{
		//vectorに登録する名前の情報を読み込む
		std::vector<std::vector<std::string>>NoDraw = File->Load_Data();
		m_noDrawText = NoDraw[0];
		//ファイルを閉じる
		File->Close();
	}

	//イベント関数を登録する
	m_ivent["＃"] = std::bind(&TextWindow::Ivent_＃,this);
	m_ivent["%"] = std::bind(&TextWindow::Ivent_％, this);
	m_ivent["FS"] = std::bind(&TextWindow::Ivent_FS, this);
	m_ivent["EOF"] = std::bind(&TextWindow::Ivent_EOF, this);
	m_ivent["END"] = std::bind(&TextWindow::Ivent_END, this);

	//実行関数を起こす
	this->SetActive(true);
}

void TextWindow::Update()
{
	//フラグがfalseのとき、何もしない
	if (!this->GetActive())return;

	Story* story = this->GetOwner()->GetStory();

	//画像の更新処理
	TextureBase::Update();

	//キー入力＆マウスの処理
	bool check = this->InPut();

	//イベント関連の処理(Keyの処理が実行されなかったときのみ)
	if(!check)this->Ivent_Start();
}

void TextWindow::Render()
{
	//フラグがfalseのとき、メッセージ関連は何も描画しない
	if (!m_drawCheck) return; 

	//テキストウィンドウの描画
	TextureBase::Render();

	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//Storyを取得する
	Story* story = this->GetOwner()->GetStory();

	//ストーリーのテキストを取得する
	const std::vector<std::string> TextData = story->GetStoryArray();

	//DrawCountが範囲外を参照しようとしたら、何もしない
	if (m_drawCount >= (int)TextData.size())return;

	//座標の初期化
	Vector3 pos = INIT_POS;

	//テキストの描画
	for (int i = m_setupID;i < (int)TextData.size();i++)
	{
		//テキストを取得
		const std::string& text = TextData[i];

		//テキストが使えないテクスチャだったら飛ばす
		if (!this->FindText(text) && !this->FindLine(text))continue;

		//改行するとき
		if (this->FindLine(text)) { pos.x = INIT_POS.x; pos.y += MAX_INTERBAL; continue; }

		//必要なテクスチャを取得する
		ITextureBase* UseTexture = resourceManager->GetTexture(text);

		//描画は1フレームにm_DrawCount枚まで
		if (m_drawCount > i)
		{
			//使うテクスチャを生成する
			std::unique_ptr<ITextureBase> texture = std::make_unique<TextureBase>(UseTexture);
			//座標を設定する
			texture->SetPosition(pos);
			//色を設定する
			texture->SetColor(Colors::Black);
			//改行しないとき
			pos.x += MAX_INTERBAL;
			//テクスチャの描画
			texture->Render();
		}
	}
}

void TextWindow::Finalize()
{
}

//キー入力＆マウスの処理
const bool TextWindow::InPut()
{
	//テキストの更新がされたかチェックするフラグ
	bool update = false;

	//テキストの更新処理
	update = this->TextUpdate();

	//テキストの停止処理（Keyのトリガーが変更されても、処理は次のフレームから）
	//そうしないと、キー入力が同じフレームで適応されてしまうから
	bool check = false;

	if(!update)check = this->TextStop();

	//処理が実行されたらtrue、実行されなかったらfalseを返す
	return check;
}

const bool TextWindow::TextUpdate()
{
	//キー関連の情報を取得
	Input* input = Input::GetInstance();

	//Key関連の情報を取得
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//Mouse関連の情報を取得
	const auto& mouseState = input->GetMouse_State();

	//Key入力がtrue 
	if (m_keyCheck)
	{
		//KeyかMouseが押されたら、文末までスキップする
		if (keyTrakker.IsKeyPressed(Keyboard::Space)/* || 
			mouseState.leftButton == Mouse::ButtonStateTracker::ButtonState::PRESSED*/)
		{
			//キー入力を停止させる
			m_keyCheck = false;

			//%イベントの1つ前の＃イベントのところまで、戻る（テキストを停止させるため）
			m_drawCount = this->Convert("＃");

			//処理が実行されたことを明示する
			return true;
		}
		else
		{
			//一定間隔ごと
			if (m_frameCount >= m_drawInterval)
			{
				//DrawCountを更新する
				m_drawCount++;

				//フレーム数を初期化
				m_frameCount = NULL;
			}

			//フレーム数を更新する
			m_frameCount++;
		}
	}
	return false;
}

const bool TextWindow::TextStop()
{
	//キー関連の情報を取得
	Input* input = Input::GetInstance();

	//Key関連の情報を取得
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//Mouse関連の情報を取得
	const auto& mouseState = input->GetMouse_State();

	//Key入力がfalse DrawCountを更新する
	if (!m_keyCheck)
	{
		//KeyかMouseが押されたら
		if (keyTrakker.IsKeyPressed(Keyboard::Space)/* ||
			mouseState.leftButton == Mouse::ButtonStateTracker::ButtonState::PRESSED*/)
		{
			//キー入力を再開する
			m_keyCheck = true;

			//フレーム数を補正する
			m_frameCount = m_drawInterval;
			return true;
		}
	}

	return false;
}

const int TextWindow::Convert(const std::string& text)
{
	//Storyを取得する
	Story* story = this->GetOwner()->GetStory();

	//ストーリーのテキストを取得する
	const std::vector<std::string> TextData = story->GetStoryArray();

	//範囲外を参照しようとしたら、何もしない
	if (m_drawCount >= (int)TextData.size())return -1;

	//目的物を探す
	for (size_t i = m_drawCount; i < TextData.size(); i++)
	{
		const std::string& search = TextData[i];
		if (text == search)return i;
	}

	//見つからなかったときは、例外として-1を返す
	return -1;
}

const bool TextWindow::FindText(const std::string& text)
{
	//テクスチャとして使えるかどうか調べる
	for (const std::string& notext : m_noDrawText)
	{
		//使えないものを発見した時点でfalseを返す
		if (text == notext)return false;
	}

	//使えないものが発見されなかったらtrueを返す
	return true;
}

const bool TextWindow::FindLine(const std::string& text)
{
	//改行＃のとき
	if (text == NEWLINE_AND_STOP)return true;

	//改行//のとき
	if (text == NEWLINE)return true;

	return false;
}

//＃の時の処理を書く
void TextWindow::Ivent_＃()
{	
	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//文字の更新を止める
	m_keyCheck = false;

	//SEを停止する
	soundManager->Stop(CRI_CUESHEET_0__47_メッセージ表示);
}

//%の時の処理を書く
void TextWindow::Ivent_％()
{	
	//描画の開始位置を変更する
	//（次のテキストの先頭に持ってくる）
	m_setupID = this->Convert("%") + 1;
}

//FSの時の処理を書く
void TextWindow::Ivent_FS()
{
	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//SEを再生する
	soundManager->Play(CRI_CUESHEET_0__47_メッセージ表示);

	//表示間隔を補正する
	m_frameCount = 5;
}

//EOFの時の処理を書く
void TextWindow::Ivent_EOF()
{	
	//文字の更新を止める
	m_keyCheck = false;

	//テキストウィンドウの更新＆描画を止める
	m_drawCheck = false;

	//Updateの更新を止める
	this->SetActive(false);
}

//ENDの時の処理を書く
void TextWindow::Ivent_END()
{	
	//文字の更新を止める
	m_keyCheck = false;

	//テキストウィンドウの更新＆描画を止める
	m_drawCheck = false;

	//Updateの更新を止める
	this->SetActive(false);
}

void TextWindow::Ivent_Start()
{
	//Storyを取得する
	Story* story = this->GetOwner()->GetStory();

	//ストーリーのテキストを取得する
	const std::vector<std::string> TextData = story->GetStoryArray();

	//範囲外のところを参照しようとしたら、何もしない
	if (m_drawCount >= (int)TextData.size())return;

	//現在読まれているテキストを取得する
	const std::string& nowText = TextData[m_drawCount];

	//現在読まれているテキストがイベントと無関係だったら、何もしない
	if (!this->Find_Ivent(nowText))return;

	//イベントに結びついている関数を実行する
	std::function<void()> Execute = m_ivent[nowText];
	Execute();
}

const bool TextWindow::Find_Ivent(const std::string& ivent)
{
	//＃イベントのとき
	if (ivent == "＃")return true;

	//%イベントのとき
	if (ivent == "%")return true;

	//FSイベントのとき
	if (ivent == "FS")return true;

	//EOFイベントのとき
	if (ivent == "EOF")return true;

	//ENDイベントのとき
	if (ivent == "END")return true;

	return false;
}
