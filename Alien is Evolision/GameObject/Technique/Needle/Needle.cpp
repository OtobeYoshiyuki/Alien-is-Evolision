#include "pch.h"
#include "Needle.h"
#include "GameObject/Technique/NeedleOverall/NeedleOverall.h"
#include "GameContext.h"
#include "OtobeLib/FrameWork/Collision/CollisionManager/CollisionManager.h"
#include "OtobeLib/Camera/Manager/CameraManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

Needle::Needle(ITextureBase* texture):TextureBase(texture)
{
	//当たり判定の半径を設定する
	this->SetRadius(Vector3(this->AskOrigin()*2));
}

void Needle::Initialize()
{
}

void Needle::Update()
{
	//CameraManagerを取得する
	CameraManager* cameraManager = GameContext::Get<CameraManager>();

	//現在稼働しているカメラを取得する
	CameraBase* nowCamera = cameraManager->GetNowCamera();

	//カメラがスクロールカメラの時は、更新を止める
	if (!nowCamera->GetObjectMove())return;

	//画像の更新処理
	TextureBase::Update();

	//当たり判定の座標を設定する
	this->SetCenter(this->GetPosition());

	//当たり判定の追加
	GameContext::Get<CollisionManager>()->AddUseHero(this);
}

void Needle::Render()
{
	//画像の描画処理
	TextureBase::Render();
}

void Needle::Finalize()
{
}

void Needle::OnCollisionEnter(ITextureBase* obj)
{
	CollisionBase::OnCollisionEnter(obj);
}

void Needle::NonCollisionEnter(ITextureBase* obj)
{
	CollisionBase::NonCollisionEnter(obj);
}
