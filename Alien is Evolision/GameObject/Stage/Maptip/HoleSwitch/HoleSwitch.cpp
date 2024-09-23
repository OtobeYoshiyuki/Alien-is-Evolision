#include "pch.h"
#include "HoleSwitch.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "GameContext.h"
#include "GameObject/Player/CoroKun.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/FrameWork/Collision/CollisionManager/CollisionManager.h"
#include "GameObject/Player/NormalTechnique/NormalTechnique.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "GameObject/Stage/Maptip/HoleFloor/HoleFloor.h"
#include "Utils.h"
#include "GameObject/TargetScrollCamera/TargetScrollCamera.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

HoleSwitch::HoleSwitch(ITextureBase* texture):
	MapTip(texture),//基底クラスに情報を送信する
	m_DetonationCount(0.0f),//起爆カウント
	m_DetonationFlag(false),//起爆フラグ
	m_SelealNumber(NULL),//シリアルナンバー
	m_backTexture(nullptr)//背景のテクスチャ
{
	//ResourceManagerを取得する
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//GameManagerを取得する
	TextureManager* gameManager = GameContext::Get<PlayScene>()->GetGameManager().get();

	//使うテクスチャを取得する
	ITextureBase* mapTip = resourceManager->GetTexture("マップチップ");

	//背景のテクスチャを生成する
	std::unique_ptr<ITextureBase>pback = std::make_unique<TextureBase>(mapTip);
	pback->SetRect(RECT{ 40,160,80,200 });
	pback->SetDrawOrder(4);
	m_backTexture = pback.get();

	//gameManagerに追加する
	gameManager->Add(std::move(pback));
}

void HoleSwitch::Initialize()
{
	//背景の座標を設定する
	m_backTexture->SetPosition(this->GetPosition());

	//ステートを初期化する
	this->SetNowState(FADE_STATE::DOWN);
}

void HoleSwitch::Update()
{
	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//Playerを取得する
	CoroKun* player = GameContext::Get<CoroKun>();

	//マップチップの更新処理
	MapTip::Update();

	//背景の色情報を取得する
	Color color = m_backTexture->GetColor();
	float alfa = color.A();

	//ステートによって、処理を切り替える
	switch (this->GetNowState())
	{
	case FADE_STATE::DOWN:
		if (color.A() <= 0.5f)this->SetNowState(FADE_STATE::FUP);
		else alfa -= 0.01f;
		break;
	case FADE_STATE::FUP:
		if (color.A() >= 1.0f)this->SetNowState(FADE_STATE::DOWN);
		else alfa += 0.01f;
		break;
	}

	//色情報を再設定する
	color.A(alfa);
	m_backTexture->SetColor(color);

	//CameraManagerを取得する
	CameraManager* cameraManager = GameContext::Get<CameraManager>();

	//現在稼働しているカメラを取得する
	CameraBase* nowCamera = cameraManager->GetNowCamera();

	//スクロールカメラにキャストする
	TargetScrollCamera* scrollCamera = dynamic_cast<TargetScrollCamera*>(nowCamera);

	//カメラがスクロールカメラの時は、更新を止める
	if (!nowCamera->GetObjectMove() && scrollCamera && 
		scrollCamera->GetNowState() < TARGET_STATE::WAIT)return;

	//描画が切られていたら、起爆スラグを立てる
	if (this->GetState() == STATE::NONE)
	{
		m_DetonationFlag = true;
	}

	//起爆フラグがtrueのとき、起爆カウントを更新する
	if (m_DetonationFlag)
	{
		//起爆カウントを更新する
		m_DetonationCount += staticData->Get_FloatData("elapsedTime") * 10;
		//起爆カウントをStaticDataに登録する
		staticData->Add(this->GetName() + std::to_string(this->GetSerealNumber()) + "Count", m_DetonationCount);
		//自身の連番を登録する
		staticData->Add(this->GetName() + std::to_string(this->GetSerealNumber()),this->GetSerealNumber());
	}	

	//ビームを取得する
	VirusBeam* beam = GameContext::Get<VirusBeam>();

	//画像の横の幅を取得する
	float length = (beam->GetRect().right - beam->GetRect().left) / 2;

	//当たり判定を追加する
	GameContext::Get<CollisionManager>()->AddUseOther(this, player, 200.0f);
	GameContext::Get<CollisionManager>()->AddUseOther(this, beam, length + 100.0f);

	//ニードルの長距離範囲攻撃を取得する
	std::vector<Needle*> needleListOver = GameContext::Get<NeedleOverall>()->GetNeedleList();

	for (Needle* needle : needleListOver)
	{
		//当たり判定を追加する
		GameContext::Get<CollisionManager>()->AddUseOther(this, needle, 100.0f);
	}

	//ニードルの短距離範囲攻撃を取得する
	std::vector<Needle*> needleListWide = GameContext::Get<Resurrection>()->GetNeedleList();

	for (Needle* needle : needleListWide)
	{
		//当たり判定を追加する
		GameContext::Get<CollisionManager>()->AddUseOther(this, needle, 500.0f);
	}

	//通常弾を取得する
	std::vector<NormalBullet*>bulletList = GameContext::Get<NormalTechnique>()->GetBulletsList();

	for (NormalBullet* bullet : bulletList)
	{
		//当たり判定を追加する
		GameContext::Get<CollisionManager>()->AddUseOther(this, bullet, 300.0f);
	}
}

void HoleSwitch::Render()
{
	//マップチップの描画処理
	MapTip::Render();
}

void HoleSwitch::Finalize()
{
}

void HoleSwitch::OnCollisionEnter(ITextureBase* obj)
{
	MapTip::OnCollisionEnter(obj);

	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//Playerを取得する
	CoroKun* player = GameContext::Get<CoroKun>();

	//CameraManagerを取得する
	CameraManager* cameraManager = GameContext::Get<CameraManager>();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//描画されているときのみ、実行
	if (this->GetState() == STATE::DRAW)
	{
		//考えられる最大の値を設定する
		float min = FLT_MAX;

		//入れ替え用の座標
		Vector3 tempPos = Vector3::Zero;

		//入れ替え用の変数
		int tempSerial = NULL;

		//SEを再生する
		soundManager->Play(CRI_CUESHEET_0__22_獲得);

		//対応する落とし穴のリストを取得する
		for (HoleFloor* hole : HoleFloor::GetHoleGrop(this->GetSerealNumber()))
		{
			//距離を求める
			Vector3 len = Length(player->GetPosition(), hole->GetPosition());

			//入れ替えの処理を行う(判定はX座標で行う)
			//比較対象の方が小さいときに入れ替える
			if (min > len.x)
			{
				min = len.x;
				tempPos = hole->GetPosition();
				tempSerial = hole->GetSerealNumber();
			}
		}

		//カメラの限界地よりスクロール値が大きいときは、補正する
		if(tempPos.x > 3460.0f)tempPos.x = 3460.0f;
		
		//一番近い落とし穴が、TARGET_TO_LENGTHより大きいとき、カメラを切り替える
		if (min > TargetScrollCamera::TARGET_TO_LENGTH)
		{
			//カメラをターゲットスクロールカメラに切り替える
			cameraManager->PushCamera("スクロールカメラ");

			//静的データに一時格納する
			staticData->Add("ターゲット", tempPos);
			staticData->Add("ターゲット", tempSerial);
		}
	}
	
	//描画を切る
	this->SetState(STATE::NONE);
	m_backTexture->SetState(STATE::NONE);
}

void HoleSwitch::NonCollisionEnter(ITextureBase* obj)
{
	MapTip::NonCollisionEnter(obj);
}
