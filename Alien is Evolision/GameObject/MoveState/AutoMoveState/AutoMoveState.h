#pragma once
#include "GameObject/MoveState/MoveState.h"
#include "Direct.h"
#include "TemplateState.h"
class AutoMoveState :public MoveState,State<DIRECT>
{
private:
	//1�t���[���ɉ��Z����X�s�[�h
	DirectX::SimpleMath::Vector3	m_Speed;
public:
	//�R���X�g���N�^
	AutoMoveState();

	//�f�X�g���N�^
	~AutoMoveState()override {}

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
	void SetNowState(const DIRECT& now)override { State::SetNowState(now); };

	//���݂̏�Ԃ��擾����
	const DIRECT& GetNowState()const override { return State::GetNowState(); };

	//�ЂƂO�̏�Ԃ��擾����
	const DIRECT& GetPreState()const override { return State::GetPreState(); };

public:
	//1�t���[���ɉ��Z����X�s�[�h��ݒ肷��
	void SetSpeed(const DirectX::SimpleMath::Vector3& speed) { m_Speed = speed; }

	//1�t���[���ɉ��Z����X�s�[�h���擾����
	const DirectX::SimpleMath::Vector3& GetSpeed()const { return m_Speed; }

	//�X�e�[�g�𔽓]������
	void ChangeState();
};

