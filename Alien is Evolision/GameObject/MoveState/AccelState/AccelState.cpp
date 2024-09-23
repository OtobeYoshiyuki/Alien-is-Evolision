#include "pch.h"
#include "AccelState.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "GameContext.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

AccelState::AccelState():mAccel(Vector3::Zero),mAccelCount(NULL)
{
}

const Vector3 AccelState::Update()
{
	//�x�N�g���̏�����
	Vector3 vel = Vector3::Zero;

	//���s�֐���false�̎��́A�������Ȃ�
	if (!this->GetExecute())return vel;

	//���������߂�
	float dir = 0.0f;

	//�x�N�g���̌���
	if (mAccel.x > NULL)dir = 1.0f;

	if (mAccel.x < NULL)dir = -1.0f;

	//�v�Z�񐔂�MAX_ACCEL���������Ƃ��A����ȏ�������Ȃ�
	if (mAccelCount > MAX_ACCEL)
	{
		return mAccel;
	}

	//�����x�̌v�Z
	mAccel += Vector3(dir, 0.0f, 0.0f);

	vel = mAccel;

	//�v�Z�񐔂̍X�V
	mAccelCount++;

	return vel;
}

void AccelState::InitState()
{
	//�X�e�[�g�̏�����������
	//�����x������������
	mAccel = Vector3::Zero;

	//�����񐔂�����������
	mAccelCount = NULL;

	//���s�֐�������������
	this->SetExecute(false);
}
