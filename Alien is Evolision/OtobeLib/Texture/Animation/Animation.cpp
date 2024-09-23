#include "pch.h"
#include "Animation.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

AnimationMap Animation::s_AniGrop = AnimationMap();

Animation::Animation():m_AniCount(NULL),m_AniFlag(false),m_LoopFlag(false),m_NowInfo(""),
m_LoopInfo(""),m_NowRect(RECT())
{
}

void Animation::Initialize()
{
}

void Animation::Update()
{
	//���݂̃A�j���[�V��������A�j���[�V�����̏I�����Ԃ��擾����
	int endtime = this->GetEndTime(this->GetNowInfo());

	//�A�j���[�V�����̏I�����ԂɂȂ�����A���̃A�j���[�V�����ɐ؂�ւ���
	if (this->GetAniCount() > endtime)
	{
		//�A�j���[�V���������[�v�Đ�����
		if (this->GetLoopFlag())
		{
			//���݂̏����X�V����
			this->SetNowInfo(this->GetNextAnimation(this->GetNowInfo()));
			//�J�E���g������������
			m_AniCount = NULL;
		}
		//�A�j���[�V���������[�v�Đ����Ȃ�
		else
		{
			//�A�j���[�V�������I��肶��Ȃ��Ƃ�
			if (this->GetLoopInfo() != this->GetNowInfo())
			{
				//���݂̏����X�V����
				this->SetNowInfo(this->GetNextAnimation(this->GetNowInfo()));
				//�J�E���g������������
				m_AniCount = NULL;
			}
			//�A�j���[�V�������I���̂Ƃ�
			else
			{
				//���݂̏����X�V����
				this->SetNowInfo(this->GetNextAnimation(this->GetNowInfo()));
				//�J�E���g������������
				m_AniCount = NULL;
				//�J�E���g�̍X�V���~�߂�
				this->SetAniFlag(false);
			}
		}
	}

	//���݂̃A�j���[�V�������X�V����
	this->SetNowAnimation(this->GetAnimation(this->GetNowInfo()));

	//�A�j���[�V�����̍X�V���s��
	if (this->GetAniFlag())m_AniCount++;
}

void Animation::Finalize()
{
}

void Animation::SetNewAnimation(Animation* ani)
{
	//�����̏������ƂɁA�V�����A�j���[�V�����̏���ݒ肷��

	//�A�j���[�V�����̃R���e�i���Đݒ肷��
	m_Animation = ani->GetAnimationMap();

	//���̃A�j���[�V�����̃R���e�i���Đݒ肷��
	m_NextAnimation = ani->GetNextAnimationMap();

	//�A�j���[�V�����̏I�����Ԃ̃R���e�i���Đݒ肷��
	m_EndTime = ani->GetEndTimeMap();

	//���݂̃A�j���[�V������ݒ肷��
	m_NowRect = ani->GetNowAnimation();

	//���݂̃A�j���[�V�����̏���ݒ肷��
	m_NowInfo = ani->GetNowInfo();

	//�A�j���[�V�����̃��[�v����ݒ肷��
	m_LoopInfo = ani->GetLoopInfo();

	//�A�j���[�V�����̃J�E���g��ݒ肷��
	m_AniCount = ani->GetAniCount();

	//�A�j���[�V�����̍X�V�t���O��ݒ肷��
	m_AniFlag = ani->GetAniFlag();

	//�A�j���[�V�����̃��[�v�t���O���擾����
	m_LoopFlag = ani->GetLoopFlag();
}

void Animation::InitMiniMapAnimation()
{
	//�A�j���[�V���������㏑������
	
	////�A�j���[�V�����̃R���e�i������������
	//m_Animation.clear();

	////����̃A�j���[�V����������������
	//m_NextAnimation.clear();

	////�A�j���[�V�����̏I�����Ԃ�����������
	//m_EndTime.clear();

	////���݂̃A�j���[�V������������������
	//m_NowInfo.clear();

	////�A�j���[�V�����̃��[�v��������������
	//m_LoopInfo.clear();

	//�A�j���[�V�����̍X�V�t���O������������
	m_AniFlag = false;

	//�A�j���[�V�����̃��[�v�t���O������������
	m_LoopFlag = false;

	//���݂̃A�j���[�V����������������
	//m_NowRect = RECT{};

	//�A�j���[�V�����J�E���g������������
	m_AniCount = NULL;
}

