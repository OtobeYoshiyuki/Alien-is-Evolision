#include "pch.h"
#include "DamageState.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"
#include "OtobeLib/FrameWork/Input/Input.h"
#include "GameObject/Player/CoroKun.h"
#include "GameObject/MoveState/WalkState/WalkState.h"
#include "GameContext.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

DamageState::DamageState():mDamage(Vector3::Zero),mDamageCount(NULL),mFirstDamage(Vector3::Zero)
{
}

const Vector3 DamageState::Update()
{
	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//�x�N�g���̏�����
	Vector3 vel = Vector3::Zero;

	//�v���C���[�����擾
	CoroKun* player = GameContext::Get<CoroKun>();

	//���s�֐���false�̂Ƃ��͉������Ȃ�
	if (!this->GetExecute())return vel;

	//7��ȏ�_���[�W��^�����牽�����Ȃ�
	if (mDamageCount > MAX_DAMAGE)
	{
		//�X�e�[�g�̏�����
		this->InitDamageState();
		return vel;
	}

	//�_���[�W����x�N�g�����Z�o
	vel = mDamage * abs(mDamageCount - MAX_DAMAGE) + mDamage;

	//����_���[�W���L������
	if (mDamageCount == NULL)mFirstDamage = vel / 8;

	//�_���[�W�����X�V����
	mDamageCount++;

	//�x�N�g����Ԃ�
	return vel;
}

void DamageState::InitDamageState()
{
	//�X�e�[�g�̏��������s��
	mDamage = Vector3::Zero;
	mDamageCount = NULL;
	mFirstDamage = Vector3::Zero;
	this->SetExecute(false);
}
