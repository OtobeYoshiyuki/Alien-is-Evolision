#include "pch.h"
#include "TimeLag.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

TimeLag::TimeLag():m_Pos(Vector3::Zero),m_Vel(Vector3::Zero),m_Finish(false),
m_FrameCount(NULL),m_FrameFlag(false),m_Target(""),m_AutoMoveState(nullptr),m_NowString(""),
m_JumpState(nullptr),m_AllStringFlag(false),m_customFlag(false)
{
	//AutoMoveState����
	m_AutoMoveState = std::make_unique<AutoMoveState>();
	m_AutoMoveState->SetExecute(false);

	//JumpState����
	m_JumpState = std::make_unique<JumpState>();
	m_JumpState->SetExecute(false);

	//MoveState�̃��X�g�ɒǉ�����
	m_MoveList.push_back(m_AutoMoveState.get());
	m_MoveList.push_back(m_JumpState.get());
}

void TimeLag::Initialize(const bool& flag, const std::string& target,
	const Vector3& pos, const Vector3& vel)
{
	//�t���O��ݒ肷��
	m_FrameFlag = flag;

	//�S�̂̕������ݒ肷��
	m_Target = target;

	//���W��ݒ肷��
	m_Pos = pos;

	//�x�N�g����ݒ肷��
	m_Vel = vel;
}

void TimeLag::Update()
{
	//���W�̍X�V�������Ȃ�
	this->UpdatePosition();

	//�����̍X�V���s��
	this->UpdateString();
}

void TimeLag::UpdatePosition()
{
	//�x�N�g��������������
	Vector3 vel = m_Vel;

	//���s���̃X�e�[�g����x�N�g�����Z�o
	for (MoveState*& move : m_MoveList)
	{
		vel += move->Update();
	}

	//���������^�����s��
	m_Pos += vel;
}

void TimeLag::UpdateString()
{
	//�����̍X�V���s��

	//�`��̊J�n���Ԃ��擾����
	int intime = m_InTime[m_Target];

	//�J�n�����ɂȂ�����A������ݒ肷��
	//���̕������ݒ肳��Ă��Ȃ���΁A���̂܂�
	if (m_FrameCount > intime)
	{
		if (this->NextStringRequest(m_Target))
		{
			//���݂̕�����ݒ肷��
			m_Target = m_NextString[m_Target];
			m_NowString = m_Target;
		}
		else
		{
			//�S�Ă̕������\�����ꂽ
			m_AllStringFlag = true;
		}
	}

	//�����S�̂̏I�����ԂɂȂ�����
	if (m_FrameCount > m_OutTime)
	{
		//�J�E���^�[������������
		m_FrameCount = NULL;
		//�J�E���^�[�̍X�V���~�߂�
		m_FrameFlag = false;
		//���݂̕�����ݒ肷��
		m_NowString = "";
		//�I���t���O��true�ɂ���
		m_Finish = true;
	}

	//�t���[���J�E���g���X�V����
	if (m_FrameFlag)m_FrameCount++;

}


