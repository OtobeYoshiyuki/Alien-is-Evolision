#include "pch.h"
#include "JumpState.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

JumpState::JumpState():m_JumpLimit(0.0f),m_JumpSpeed(0.0f),m_JumpCount(NULL),
m_JumpCountOption(false),m_JumpCountLimit(NULL),m_JumpDesignation(false),
m_Correction(1.0f),m_OM(nullptr),m_BoundPower(0.0f)
{
	m_OM = std::make_unique<Owner<ITextureBase>>();
}

const Vector3 JumpState::Update()
{
	//�x�N�g��������������
	Vector3 vel = Vector3::Zero;

	//���s�֐���false�̂Ƃ��́A�������Ȃ�
	if (!this->GetExecute())return vel;

	//�W�����v�̌��E�l���ݒ肳��Ă��Ȃ���΁A�������Ȃ�
	if (m_JumpLimit == 0.0f)return vel;

	//�X�e�[�g�ɂ���āA�������s��
	switch (this->GetNowState())
	{
	case JUMP::START:
		this->State_Start(vel);
		break;
	case JUMP::FLY:
		this->State_Fly(vel);
		break;
	case JUMP::AIR:
		this->State_Air(vel);
		break;
	case JUMP::FALL:
		this->State_Fall(vel);
		break;
	case JUMP::LANDING:
		this->State_Landing(vel);
		break;
	}

	//�x�N�g����Ԃ�
	return vel;
}

void JumpState::State_Start(Vector3& vel)
{
	//�W�����v�J�n�̏���
	if (m_JumpCountOption)
	{
		//�ݒ肵���W�����v�񐔂܂œ͂��Ă��Ȃ�
		if (m_JumpCount <= m_JumpCountLimit)
		{
			//�W�����v�񐔂��X�V����
			m_JumpCount++;
		}
	}

	//�X�e�[�g��Fly�Ɉڍs����
	this->SetNowState(JUMP::FLY);
}

void JumpState::State_Fly(Vector3& vel)
{
	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//�㏸���̏���

	//�ϐ��̏�����
	float jumpSum = 0.0f;

	//���X�g�̒��g�̃W�����v�l�̍��v�����߂�
	for (float& jump : m_JumpPowerList)
	{
		jumpSum += jump;
	}

	//�W�����v�l�����E�l�ȏ�ɂȂ�����A�X�e�[�g��؂�ւ�
	//�������΂�
	if (jumpSum >= m_JumpLimit)
	{
		//�X�e�[�g���㏸���痎���ɕύX
		this->SetNowState(JUMP::AIR);
		return;
	}

	//���X�g�ɃW�����v�p���[��ǉ�����
	m_JumpPowerList.push_back(m_JumpSpeed);

	//���X�g�̈�Ԍ��̑��x���x�N�g���ɐݒ肷��
	//������̃x�N�g���Ȃ̂ŁA�l�̓}�C�i�X�Ƃ���
	vel.y = -m_JumpPowerList.back();
}

void JumpState::State_Air(Vector3& vel)
{
	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//�㏸���痎���Ɉڂ�X�e�[�g

	//�X�s�[�h�����X��0�ɋ߂Â���
	//�X�s�[�h�̓��X�g�̈�Ԍ����Q�Ƃ���
	float speed = m_JumpPowerList.back() - m_Correction;

	//�X�s�[�h��0��菬�����Ȃ�����AFALL�ɐ؂�ւ���
	if (speed < 0.0f)
	{
		this->SetNowState(JUMP::FALL);
		return;
	}

	//���X�g�ɃX�s�[�h��ǉ�����
	m_JumpPowerList.push_back(speed);

	//�x�N�g����ݒ肷��
	vel.y = -m_JumpPowerList.back();
}

void JumpState::State_Fall(Vector3& vel)
{
	//�������̏���
	
	//���X�g�ɒǉ�����Ă���JumpSpeed���x�N�g���Ƃ���
	vel.y = m_JumpPowerList.back();

	//�W�����v�I�v�V�������������Ă���΁A���X�g����납�����
	if (m_JumpCountOption)
	{
		//�x�N�g������Ԍ�납��������Ă���
		m_JumpPowerList.pop_back();
	}
	//�I�v�V�������������Ă��Ȃ���΁A
	else
	{
		if (m_JumpSpeed != vel.y)
		{
			//�x�N�g������Ԍ�납��������Ă���
			m_JumpPowerList.pop_back();
		}
	}

	//���X�g�̒��g���S�ĂȂ��Ȃ�����A�X�e�[�g��؂�ւ���
	if (m_JumpPowerList.empty())
	{
		this->SetNowState(JUMP::LANDING);

		//�o�E���h������
		m_JumpLimit -= m_BoundPower;
	}
}

void JumpState::State_Landing(Vector3& vel)
{
	//���n���̏���

	//�I�v�V�������������Ă���Ƃ��́A
	//�ĂсA�W�����v������
	if (m_JumpCountOption)
	{
		//�ݒ肵���W�����v�񐔂܂œ͂���
		if (m_JumpCount >= m_JumpCountLimit)
		{
			//�X�e�[�g������������
			this->InitState();
			//�w�肵���񐔃W�����v������Aflag��true�ɂ���
			m_JumpDesignation = true;
		}
		else
		{
			//�ĂсA�W�����v
			this->SetNowState(JUMP::START);
		}
	}
	else
	{
		//�X�e�[�g������������
		this->InitState();
	}
}

void JumpState::InitState()
{
	//�X�e�[�g�̏��������s��

	//�X�e�[�g�����ɖ߂�
	this->SetNowState(JUMP::JNONE);

	//�W�����v�񐔂�����������
	m_JumpCount = NULL;

	//���X�g�̒��g���J������
	m_JumpPowerList.clear();

	////���E�l������������
	//m_JumpLimit = 0.0f;

	////�W�����v�X�s�[�h������������
	//m_JumpSpeed = 0.0f;


	////�W�����v�I�v�V����������������
	//m_JumpCountOption = false;

	////�W�����v�񐔂̌��E�l������������
	//m_JumpCountLimit = NULL;

}

void JumpState::AddFall()
{
	m_JumpPowerList.push_back(m_JumpSpeed);
}

void JumpState::RemoveFall()
{
	m_JumpPowerList.clear();
}
