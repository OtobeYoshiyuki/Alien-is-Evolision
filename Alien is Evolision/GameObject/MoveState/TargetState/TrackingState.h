#pragma once
#include "GameObject/MoveState/MoveState.h"

class TrackingState:public MoveState
{
private:
	//�ǐՑΏ�
	const DirectX::SimpleMath::Vector3* m_target;

	//���g
	const DirectX::SimpleMath::Vector3* m_this;

	//�x�N�g�����v�Z����␳�l
	DirectX::SimpleMath::Vector3 m_correction;
public:
	//�R���X�g���N�^
	TrackingState();

	//�f�X�g���N�^
	~TrackingState()override {}

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
	//�ǐՑΏۂ�ݒ肷��
	void SetTarget(const DirectX::SimpleMath::Vector3* target) { m_target = target; }

	//���g�̍��W��ݒ肷��
	void SetThis(const DirectX::SimpleMath::Vector3* myself) { m_this = myself; }

	//�x�N�g���̕␳�l��ݒ肷��
	void SetCorrection(const DirectX::SimpleMath::Vector3& correction) { m_correction = correction; }

	//�ǐՑΏۂ��擾����
	const DirectX::SimpleMath::Vector3& GetTarget()const { return *m_target; }

	//�x�N�g���̕␳�l���擾����
	const DirectX::SimpleMath::Vector3& GetCorrection()const { return m_correction; }
};

