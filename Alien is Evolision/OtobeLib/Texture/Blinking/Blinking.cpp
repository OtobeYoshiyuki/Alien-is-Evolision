#include "pch.h"
#include "Blinking.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

Blinking::Blinking():mState(true),mFrameCount(0.0f),mFrameFlag(false),mTarget(NULL),
mFinal(60),mFrameFinish(false),mTargetCount(0.0f)
{
}

Blinking::~Blinking()
{
}

void Blinking::Update()
{
	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//�t���[���J�E���g���X�V����
	if (mFrameFlag)
	{
		mFrameCount += staticData->Get_FloatData("elapsedTime");
		mTargetCount += staticData->Get_FloatData("elapsedTime");
	}

	//�t���[���J�E���g���X�V����Ă��Ȃ������牽�����Ȃ�
	if (mFrameCount == NULL)return;

	//�ݒ肵���Ԋu���Ƃɓ_�ł�����
	if (mTargetCount > mTarget)
	{
		//�t���O��؂�ւ���
		mState = !mState;
		//�^�[�Q�b�g�̃J�E���g������������
		mTargetCount = 0.0f;
		//�I�����ԂɂȂ����猳�ɖ߂�
		if (mFrameFinish)
		{
			if (mFrameCount > mFinal)
			{
				mFrameCount = 0.0f;
				mState = true;
				mFrameFlag = false;
			}
		}
	}

}

bool Blinking::NowBlinking()
{
	//�_�ł��Ă��Ȃ�������true��Ԃ�
	if (mState)return true;
	//�_�ł��Ă�����false��Ԃ�
	return false;
}
