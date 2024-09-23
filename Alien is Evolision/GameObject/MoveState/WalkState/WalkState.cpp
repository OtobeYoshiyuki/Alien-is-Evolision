#include "pch.h"
#include "WalkState.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "OtobeLib/FrameWork/Input/Input.h"
#include "GameObject/Player/CoroKun.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

WalkState::WalkState():mWalk(Vector3::Zero)
{
}

const Vector3 WalkState::Update()
{
	//�L�[�̏�Ԃ��擾����
	Input* input = Input::GetInstance();

	//�L�[�{�[�h�̏�Ԃ��擾����
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//�x�N�g��������������
	Vector3 vel = Vector3::Zero;

	//���s�֐���false
	if (!this->GetExecute())return vel;

	//�x�N�g����ݒ肷��
	if (keyState.Left)vel.x = -CoroKun::SPEED;

	if (keyState.Right)vel.x = CoroKun::SPEED;

	if (keyState.Up)vel.y = -CoroKun::SPEED;

	if (keyState.Down)vel.y = CoroKun::SPEED;

	//�x�N�g����Ԃ�
	return vel;
}
