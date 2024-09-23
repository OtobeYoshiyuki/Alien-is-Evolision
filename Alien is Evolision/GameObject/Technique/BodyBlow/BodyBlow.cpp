#include "pch.h"
#include "BodyBlow.h"
#include "OtobeLib/Camera/Manager/CameraManager.h" 
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

BodyBlow::BodyBlow(ITextureBase* texture):TextureBase(texture)
{
	//描画を切る
	this->SetState(STATE::NONE);
	//画像の中心を左上から中央にする
	this->SetOrigin(TextureBase::AskOrigin());
}

void BodyBlow::Initialize()
{
}

void BodyBlow::Update()
{	
	//CameraManagerを取得する
	CameraManager* cameraManager = GameContext::Get<CameraManager>();

	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//現在稼働しているカメラを取得する
	CameraBase* nowCamera = cameraManager->GetNowCamera();

	//カメラがスクロールカメラの時は、更新を止める
	if (!nowCamera->GetObjectMove())return;

	//アニメーションの情報を設定する
	this->SetRect(this->GetNowAnimation());

	if(!this->GetActive())soundManager->Stop(CRI_CUESHEET_0__26_体当たり);

	//アニメーションの更新
	Animation::Update();

	//画像の更新処理
	TextureBase::Update();
}

void BodyBlow::Render()
{
	//画像の描画処理
	TextureBase::Render();
}

void BodyBlow::Finalize()
{
}

void BodyBlow::WithPlayer(ITextureBase* texture)
{
	//画像を押し出す
	TextureBase::Extrude(texture);
}
