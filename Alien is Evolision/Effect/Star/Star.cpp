#include "pch.h"
#include "Star.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

Star::Star(ITextureBase* texture):TextureBase(texture),m_AngleSpeed(0.0f),m_AutoMoveState(nullptr),m_JumpState(nullptr)
{
	//AutoMoveStateを生成する
	m_AutoMoveState = std::make_unique<AutoMoveState>();
	m_AutoMoveState->SetExecute(false);

	//JumpStateを生成する
	m_JumpState = std::make_unique<JumpState>();
	m_JumpState->SetExecute(false);

	//リストに追加する
	m_MoveStateList.push_back(m_AutoMoveState.get());
	m_MoveStateList.push_back(m_JumpState.get());
}

void Star::Initialize()
{
}

void Star::Update()
{
	//ステートの更新を行う
	this->UpdateState();

	//画像の更新処理
	TextureBase::Update();

	//角度の更新を行う
	this->UpdateAngle();
}

void Star::Render()
{
	//画像の描画処理
	TextureBase::Render();
}

void Star::Finalize()
{
}

void Star::UpdateState()
{
	//ベクトルを初期化する
	Vector3 vel = Vector3::Zero;

	//ステートの更新を行う
	for (MoveState*& move : m_MoveStateList)
	{
		vel += move->Update();
	}

	//ベクトルを設定する
	this->SetVelocity(vel);
}

void Star::UpdateAngle()
{
	//角度の更新を行う

	float angle = this->GetAngle();

	angle += m_AngleSpeed;

	this->SetAngle(angle);
}
