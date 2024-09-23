#pragma once
#include <map>
#include <string>
#include "GameObject/MoveState/AutoMoveState/AutoMoveState.h"
#include "GameObject/MoveState/JumpState/JumpState.h"
class TimeLag
{
private:
	//�ŏ��̕�����ݒ肷��
	std::string					m_Target;
	//���݂̕�����ݒ肷��
	std::string					m_NowString;
	//�`��̊J�n����
	std::map<std::string, int>	m_InTime;
	//�`��̏I������
	int							m_OutTime;
	//���ɕ\�����镶����
	std::map<std::string, std::string> m_NextString;
	//�^�[�Q�b�g�̍��W
	DirectX::SimpleMath::Vector3 m_Pos;
	//�^�[�Q�b�g�̃x�N�g��
	DirectX::SimpleMath::Vector3 m_Vel;
	//�t���[�����̃J�E���g
	int							m_FrameCount;
	//�t���[�������J�E���g����t���O
	bool						m_FrameFlag;
	//�S�Ă̕������\�����ꂽ�t���O
	bool						m_AllStringFlag;
	//���o���I��������
	bool						m_Finish;
	//�J�X�^���t���O
	bool						m_customFlag;
	//AutoMoveState
	std::unique_ptr<AutoMoveState>	m_AutoMoveState;
	//JumpState
	std::unique_ptr<JumpState>		m_JumpState;
	//MoveState�̃��X�g
	std::list<MoveState*>			m_MoveList;
public:
	//�R���X�g���N�^
	TimeLag();

	//�f�X�g���N�^
	~TimeLag() {};

	//������
	void Initialize(const bool& flag, const std::string& target,
		const DirectX::SimpleMath::Vector3& pos,const DirectX::SimpleMath::Vector3& vel);

	//�X�V
	void Update();

public:
	//�Ώۂ̕�����ݒ肷��
	void SetTarget(const std::string& target) { m_Target = target; }

	//�w�肵���L�[�̕`��̊J�n���Ԃ�ݒ肷��
	void SetInTime(const std::string& name, const int& time) { m_InTime[name] = time; }

	//�w�肵���L�[�̕`��̊J�n���Ԃ�ݒ肷��
	void SetOutTime(const int& time) { m_OutTime = time; }

	//�w�肵���L�[�̎��̕\�����镶�����ݒ肷��
	void SetNextString(const std::string& now, const std::string& next) { m_NextString[now] = next; }

	//�X�V�t���O��ݒ肷��
	void SetFrameFlag(const bool& flag) { m_FrameFlag = flag; }

	//�x�N�g����ݒ肷��
	void SetVelocity(const DirectX::SimpleMath::Vector3& vel) { m_Vel = vel; }

	//���o�̃t���O��ݒ肷��
	void SetFinishFlag(const bool& flag) { m_Finish = flag; }

	//�J�X�^���t���O��ݒ肷��
	void SetCustomFlag(const bool& flag) { m_customFlag = flag; }

public:
	//���݂̕`�悷�镶������擾����
	const std::string GetNowString()const { return m_NowString; };

	//���o�̃t���O���擾����
	const bool& GetFinishFlag()const { return m_Finish; }

	//�S�̂̕����̔���p�̃t���O���擾����
	const bool& GetAllStringFlag()const { return m_AllStringFlag; }

	//���̕\�����镶���񂪂��邩�ǂ����A�m�F����
	const bool NextStringRequest(const std::string& now)const
	{
		//�C�e���[�^�[��錾����
		std::map<std::string, std::string>::const_iterator iter;

		//�ړI������������
		iter = m_NextString.find(now);

		//�ړI���������o������Atrue��Ԃ�
		if (iter != m_NextString.end())return true;

		//�ړI���������ł��Ȃ�����
		return false;
	}

	//���W���擾����
	const DirectX::SimpleMath::Vector3& GetPosition()const { return m_Pos; }

	//AutoMoveState���擾����
	std::unique_ptr<AutoMoveState>& GetAutoMoveState() { return m_AutoMoveState; }

	//JumpState���擾����
	std::unique_ptr<JumpState>& GetJumpState() { return m_JumpState; }

	//FrameFrag���擾����
	const bool& GetFrameFrag()const { return m_FrameFlag; }

	//�J�X�^���t���O���擾����
	const bool& GetCustomFlag()const { return m_customFlag; }

	//�t���[���J�E���g���擾����
	const int& GetFrameCount()const { return m_FrameCount; }

private:
	//���W�̍X�V
	void UpdatePosition();

	//�����̍X�V
	void UpdateString();
};
