#include "pch.h"
#include "TimerJudge.h"
#include "OtobeLib/FrameWork/StaticData/StaticData.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

TimerJudge::TimerJudge():
	m_FrameCount(0.0f),//�t���[����
	m_LimitTimer(NULL),//��������
	m_TimerUp(false),//���Ԍo��
	m_Judge(false),//�I���t���O
	m_correction(1.0f)//1�b�P�ʂ̕␳�l
{
}

void TimerJudge::Initialize(const int& limit, const bool& up)
{
	//�������Ԃ�ݒ肷��
	m_LimitTimer = limit;

	//�^�C�}�[�̃J�E���g����t���O��ݒ肷��
	m_TimerUp = up;
}

void TimerJudge::Update()
{
	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//�^�C�}�[���~�܂��Ă���Ƃ��́A�������Ȃ�
	if (!m_TimerUp)return;

	//�������Ԃ��ݒ肳��Ă��Ȃ���΁A�������Ȃ�
	//if (m_LimitTimer <= 0.0f)return;

	//�t���[�������������Ԃ𒴂�����AJudge��true�ɂ���
	//�^�C�}�[�̍X�V���~�߂�
	if (m_LimitTimer < 0.0f)
	{
		m_Judge = true;
		m_TimerUp = false;
		return;
	}

	//�^�C�}�[���X�V����
	m_LimitTimer -= staticData->Get_FloatData("elapsedTime") * m_correction;
}

//�������Ԃ���A�b�������߂āA������ɕϊ�����
const std::string TimerJudge::AskTimeStringSecound()
{
	//���݂̎��Ԃ���A���P�ʂɒ���
	int secound = (int)m_LimitTimer % ONESECOND;

	//�������Ԃ���A�b���̕����𔲂��o��
	std::string str = std::to_string(secound);

	//�P�O�b�����̎��͐擪�ɂO��ǉ�����
	if (secound < 10)return "0" + str;

	return str;
}

//�������Ԃ���A���������߂āA������ɕϊ�����
const std::string TimerJudge::AskTimeStringMinute()
{
	//���݂̎��Ԃ���A���P�ʂɒ���
	int minute = (int)m_LimitTimer / ONESECOND;

	//�������Ԃ���A�b���̕����𔲂��o��
	std::string str = std::to_string(minute);

	//�P�O�������̎��͐擪�ɂO��ǉ�����
	if (minute < 10)return "0" + str;

	return str;
}
