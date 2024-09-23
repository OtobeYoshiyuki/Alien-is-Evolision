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
	//�x�N�g��������������
	Vector3 vel = Vector3::Zero;

	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//���s�֐���false�̂Ƃ��A�������Ȃ�
	if (!this->GetExecute())return vel;

	//�X�e�[�g�ɂ���āA�x�N�g����ύX����
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

	//�x�N�g����Ԃ�
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
