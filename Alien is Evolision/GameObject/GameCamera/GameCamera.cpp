#include "pch.h"
#include "GameCamera.h"
#include "GameContext.h"
#include "GameObject/BackGround/BackGround.h"
#include "Game.h"
#include "OtobeLib/FrameWork/Input/Input.h"
#include "GameObject/Player/CoroKun.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

GameCamera::GameCamera()
{
}

void GameCamera::Initialize()
{
	//ゲームカメラをRegisterに登録する
	GameContext::Register<GameCamera>(this);
}

void GameCamera::Update()
{
	//キーの状態を取得する
	Input* input = Input::GetInstance();

	//キーボードの状態を取得する
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//プレイヤーを取得
	CoroKun* player = GameContext::Get<CoroKun>();

	//座標を取得する
	Vector3 pos = this->GetPosition();

	//行列を取得する
	Matrix matrix = this->GetMatrix();

	if (player->GetPosition().x > 500.0f && player->GetPosition().x < 3460)
	{
		pos = Vector3(-player->GetPosition().x + 500, 0.0f, 0.0f);
	}

	matrix = Matrix::CreateTranslation(pos);

	//座標を設定する
	this->SetPosition(pos);

	//行列を設定する
	this->SetMatrix(matrix);
}

void GameCamera::Finalize()
{
}
