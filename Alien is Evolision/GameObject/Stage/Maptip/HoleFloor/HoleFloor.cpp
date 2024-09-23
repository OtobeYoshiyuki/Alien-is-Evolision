#include "pch.h"
#include "HoleFloor.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "GameContext.h"
#include "GameObject/Player/CoroKun.h"
#include "OtobeLib/FrameWork/Collision/CollisionManager/CollisionManager.h"
#include "OtobeLib/Camera/Manager/CameraManager.h"
#include "GameObject/TargetScrollCamera/TargetScrollCamera.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//落とし穴のグループを初期化する
HoleFloor::HoleMap HoleFloor::s_holeGrop = HoleMap();

HoleFloor::HoleFloor(ITextureBase* texture):MapTip(texture),m_ExprosionCount(0.0f),m_ExprosionFlag(false),
m_SelealNumber(NULL)
{
}

void HoleFloor::Initialize()
{
	//グループを設定する
	s_holeGrop[m_SelealNumber].push_back(this);
}

void HoleFloor::Update()
{
	//CameraManagerを取得する
	CameraManager* cameraManager = GameContext::Get<CameraManager>();

	//現在稼働しているカメラを取得する
	CameraBase* nowCamera = cameraManager->GetNowCamera();	

	//スクロールカメラにキャストする
	TargetScrollCamera* scrollCamera = dynamic_cast<TargetScrollCamera*>(nowCamera);

	//カメラがスクロールカメラの時は、更新を止める
	if (!nowCamera->GetObjectMove() && scrollCamera &&
		scrollCamera->GetNowState() < TARGET_STATE::WAIT)return;

	//マップチップの更新処理
	MapTip::Update();

	//爆発するタイミングを更新する
	this->UpdateExprosion();

	//プレイヤーを取得する
	CoroKun* player = GameContext::Get<CoroKun>();

	//当たり判定を追加する
	GameContext::Get<CollisionManager>()->AddUseOther(this, player, 200.0f);

	//ニードルの長距離範囲攻撃を取得する
	std::vector<Needle*> needleList = GameContext::Get<NeedleOverall>()->GetNeedleList();

	for (Needle* needle : needleList)
	{
		//当たり判定を追加する
		GameContext::Get<CollisionManager>()->AddUseOther(this, needle, 100.0f);
	}
}

void HoleFloor::Render()
{
	//マップチップの描画処理
	MapTip::Render();
}

void HoleFloor::Finalize()
{
}

void HoleFloor::OnCollisionEnter(ITextureBase* obj)
{
	MapTip::OnCollisionEnter(obj);
}

void HoleFloor::NonCollisionEnter(ITextureBase* obj)
{
	MapTip::NonCollisionEnter(obj);
}

const std::vector<float> HoleFloor::FileDataConvert(const std::vector<std::vector<std::string>>& Input)
{
	//データを格納する配列を宣言する
	std::vector<float>ConvertData;

	for (size_t i = 0; i < Input.size(); i++)
	{
		for (size_t j = 0; j < Input[i].size(); j++)
		{
			//何も入っていなければ、何もしない
			if (Input[i][j] == "")continue;

			//データをStringからintに変換する
			ConvertData.push_back(std::stoi(Input[i][j]));
		}
	}

	//変換したデータを返す
	return ConvertData;
}

void HoleFloor::UpdateExprosion()
{
	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//爆発するタイミングを更新する
	//爆発するタイミングが設定されていなかったら、何もしない
	if (m_ExprosionCount <= 0.0f)return;

	//自身の連番と同じ起爆スイッチが登録されているか確認する
	if (!staticData->Request_Data("switch" + std::to_string(this->GetSerealNumber())))return;

	//自身の連番と同じ起爆スイッチの連番を取得する
	float anotherCount = staticData->Get_FloatData("switch" + std::to_string(this->GetSerealNumber()) + "Count");

	//爆発するタイミングになったら、描画を切る
	if (anotherCount > m_ExprosionCount)
	{
		if (this->GetState() == STATE::DRAW)
		{
			//SEを再生する
			soundManager->Play(CRI_CUESHEET_0__36_ブロック崩壊);
		}
		this->SetState(STATE::NONE);
	}
	
}
