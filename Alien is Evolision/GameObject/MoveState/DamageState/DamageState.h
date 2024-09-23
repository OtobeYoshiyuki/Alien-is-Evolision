#pragma once
#include "GameObject/MoveState/MoveState.h"
class DamageState:public MoveState
{
private:
	//�^����_���[�W�̒l
	DirectX::SimpleMath::Vector3	mDamage;

	//�_���[�W��^�����
	int								mDamageCount;

	//����̃_���[�W
	DirectX::SimpleMath::Vector3	mFirstDamage;
public:
	//�_���[�W��^����ő�l
	static const int				MAX_DAMAGE = 7;
public:
	//�R���X�g���N�^
	DamageState();

	//�f�X�g���N�^
	~DamageState()override {}

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

	//�_���[�W�̒l��ݒ肷��
	void SetDamage(const DirectX::SimpleMath::Vector3& damage) { mDamage = damage; }

	//����_���[�W�̒l��ݒ肷��
	void SetFirstDamage(const DirectX::SimpleMath::Vector3& first) { mFirstDamage = first; }

	//�_���[�W�̒l���擾����
	const DirectX::SimpleMath::Vector3& GetDamage()const { return mDamage; }

	//����_���[�W�̒l��ݒ肷��
	const DirectX::SimpleMath::Vector3& GetFirstDamage()const { return mFirstDamage; }

	//�_���[�W�X�e�[�g�̏�����
	void InitDamageState();

	//�_���[�W�J�E���g���擾
	const int& GetDamageCount()const { return mDamageCount; }
};

