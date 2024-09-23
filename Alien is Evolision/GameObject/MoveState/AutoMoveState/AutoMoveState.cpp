#include "pch.h"
#include "AutoMoveState.h"
#include "OtobeLib/FrameWork/Input/Input.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

AutoMoveState::AutoMoveState():m_Speed(Vector3::Zero)
{}

const Vector3 AutoMoveState::Update()
{
	//ベクトルを初期化する
	Vector3 vel = Vector3::Zero;

	//静的データの取得
	StaticData* staticData = StaticData::GetInstance();

	//実行関数がfalseのとき、何もしない
	if (!this->GetExecute())return vel;

	//ステートによって、ベクトルを変更する
	switch (this->GetNowState())
	{
	case DIRECT::LEFT:
		vel.x = -m_Speed.x;
		break;
	case DIRECT::RIGHT:
		vel.x = m_Speed.x;
		break;
	case DIRECT::UP:
		vel.y = -m_Speed.y;
		break;
	case DIRECT::DDOWN:
		vel.y = m_Speed.y;
		break;
	}

	//ベクトルを返す
	return vel;
}

void AutoMoveState::ChangeState()
{
	if (this->GetNowState() == DIRECT::LEFT)
	{
		this->SetNowState(DIRECT::RIGHT);
		return;
	}

	if (this->GetNowState() == DIRECT::RIGHT)
	{
		this->SetNowState(DIRECT::LEFT);
	}

	if (this->GetNowState() == DIRECT::UP)
	{
		this->SetNowState(DIRECT::DDOWN);
		return;
	}

	if (this->GetNowState() == DIRECT::DDOWN)
	{
		this->SetNowState(DIRECT::UP);
	}
}
