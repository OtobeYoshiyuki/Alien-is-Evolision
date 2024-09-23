#pragma once
#include "GameObject/MoveState/MoveState.h"
#include "TemplateState.h"
#include "JUMP.h"
#include "OtobeLib/FrameWork/Owner/Owner.h"
#include "OtobeLib/Texture/Interface/ITextureBase.h"
#include <list>
class JumpState :public MoveState,State<JUMP>
{
private:
	//�X�e�[�g�̏��L�҂̃|�C���^
	std::unique_ptr<OtobeLib::Owner<OtobeLib::ITextureBase>> m_OM;
	//�X�V�����W�����v�̒l��ǉ����郊�X�g
	std::list<float>	m_JumpPowerList;
	//�W�����v�̌��E�l
	float				m_JumpLimit;
	//�W�����v���邽�тɁA�o�E���h������l
	float				m_BoundPower;
	//�W�����v�̃X�s�[�h��ݒ肷��
	float				m_JumpSpeed;
	//�W�����v�����
	int					m_JumpCount;
	//�W�����v����񐔂̌��E�l
	int					m_JumpCountLimit;
	//�W�����v�񐔂̃I�v�V�����J��
	bool				m_JumpCountOption;
	//�w�肵���񐔃W�����v�����t���O
	bool				m_JumpDesignation;
	//�㏸���牺���܂ŕ␳����x�N�g��
	float				m_Correction;
public:
	//�R���X�g���N�^
	JumpState();

	//�f�X�g���N�^
	~JumpState()override {}

	//�X�V����
	const DirectX::SimpleMath::Vector3 Update()override;

	//---------------------�Z�b�^�[------------------------------------------------------------------//
	//���s�֐���ݒ肷��
	void SetExecute(const bool& exe) { MoveState::SetExecute(exe); }

	//�폜�֐���ݒ肷��
	void SetIsVal(const bool& val) { MoveState::SetIsVal(val); }
	//---------------------�Z�b�^�[------------------------------------------------------------------//


	//---------------------�Q�b�^�[------------------------------------------------------------------//
	//���s�֐����擾����
	const bool& GetExecute()const { return MoveState::GetExecute(); }

	//�폜�֐����擾����
	const bool& GetIsVal()const { return MoveState::GetIsVal(); }
	//---------------------�Q�b�^�[------------------------------------------------------------------//

public:
	//���݂̏�Ԃ�ݒ肷��
	void SetNowState(const JUMP& now)override { State::SetNowState(now); };

	//���݂̏�Ԃ��擾����
	const JUMP& GetNowState()const override { return State::GetNowState(); };

	//�ЂƂO�̏�Ԃ��擾����
	const JUMP& GetPreState()const override { return State::GetPreState(); };

public:
	//�W�����v�̌��E�l��ݒ肷��
	void SetJumpLimit(const float& limit) { m_JumpLimit = limit; }

	//�W�����v�̃X�s�[�h��ݒ肷��
	void SetJumpSpeed(const float& speed) { m_JumpSpeed = speed; }

	//�W�����v����񐔂�ݒ肷��
	void SetJumpLimitCount(const int& limit) { m_JumpCountLimit = limit; }

	//�W�����v����񐔂̃I�v�V������ݒ肷��
	void SetJumpOption(const bool& option) { m_JumpCountOption = option; }

	//�㏸���牺���܂ł̕␳����x�N�g����ݒ肷��
	void SetCorrection(const float& cor) { m_Correction = cor; }

	//�o�E���h������l
	void SetBoundPower(const float& bound) { m_BoundPower = bound; }

	//�w�肵���񐔃W�����v�����t���O��ݒ肷��
	void SetJumpmDesignation(const bool& designation) { m_JumpDesignation = designation; }

	//���L�҂̃|�C���^��ݒ肷��
	void SetOM(OtobeLib::ITextureBase* texture) { m_OM->SetOwner(texture); }

	//�W�����v�̌��E�l���擾����
	const float& GetJumpLimit()const { return m_JumpLimit; }

	//�W�����v�̃X�s�[�h��ݒ肷��
	const float& GetJumpSpeed()const { return m_JumpSpeed; }

	//�W�����v����񐔂��擾����
	const int& GetJumpLimitCount()const { return m_JumpCountLimit; }

	//�W�����v����񐔂̃I�v�V������ݒ肷��
	const bool& GetJumpOption()const { return m_JumpCountOption; }

	//���݂̃W�����v�����擾����
	const int& GetJumpCount()const { return m_JumpCount; }

	//�w�肵���񐔃W�����v�����t���O���擾����
	const bool& GetJumpmDesignation()const { return m_JumpDesignation; }

	//�o�E���h������l���擾����
	const float& GetBoundPower()const { return m_BoundPower; }

	//�W�����v�x�N�g���̕␳�l���擾����
	const float& GetCorrection()const { return m_Correction; }

	//���L�҂��Ǘ����Ă���|�C���^���擾����
	OtobeLib::Owner<OtobeLib::ITextureBase>* GetOM() { return m_OM.get(); }


private:
	//�W�����v�J�n�̃X�e�[�g
	void State_Start(DirectX::SimpleMath::Vector3& vel);

	//�W�����v���̃X�e�[�g
	void State_Fly(DirectX::SimpleMath::Vector3& vel);

	//�W�����v���痎���Ɉڂ�X�e�[�g
	void State_Air(DirectX::SimpleMath::Vector3& vel);

	//�������̃X�e�[�g
	void State_Fall(DirectX::SimpleMath::Vector3& vel);

	//���n���̃X�e�[�g
	void State_Landing(DirectX::SimpleMath::Vector3& vel);

public:
	//�X�e�[�g�̏�����
	void InitState();

	//��������x�N�g������萔�ǉ�����
	void AddFall();

	//��������x�N�g�����J������
	void RemoveFall();
};

