#pragma once
#include "GameObject/MoveState/MoveState.h"
class WalkState:public MoveState
{
private:
	//1�t���[���ɐi�ރx�N�g��
	DirectX::SimpleMath::Vector3 mWalk;
public:
	//�R���X�g���N�^
	WalkState();

	//�f�X�g���N�^
	~WalkState()override{}

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
	//�x�N�g����ݒ肷��
	void SetWalk(const DirectX::SimpleMath::Vector3& walk) { mWalk = walk; }

	//�x�N�g�����擾����
	const DirectX::SimpleMath::Vector3& GetWalk()const { return mWalk; }
};

