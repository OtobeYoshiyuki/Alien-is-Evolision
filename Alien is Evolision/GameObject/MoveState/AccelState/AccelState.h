#pragma once
#include "GameObject/MoveState/MoveState.h"
class AccelState:public MoveState
{
public:
	//����������E
	static const int MAX_ACCEL = 15;
private:
	//�����x
	DirectX::SimpleMath::Vector3 mAccel;
	//������
	int							 mAccelCount;
public:
	//�R���X�g���N�^
	AccelState();

	//�f�X�g���N�^
	~AccelState()override {}

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
	//�����x��ݒ肷��
	void SetAccel(const DirectX::SimpleMath::Vector3& accel) { mAccel = accel; }

	//�����x���擾����
	const DirectX::SimpleMath::Vector3& GetAccel()const { return mAccel; }

	//AccelState�̏�����������
	void InitState();
};

