#pragma once
#include "GameObject/MoveState/MoveState.h"
class HomingState:public MoveState
{
private:
	//�x�N�g���̋���(Z�������Ƃ���)
	DirectX::SimpleMath::Vector3	m_length;

	//�x�N�g���̃X�J���l
	DirectX::SimpleMath::Vector3	m_scalar;

	//�x�N�g���̍ő�l
	DirectX::SimpleMath::Vector3	m_maxVel;

	//�x�N�g���̍ŏ��l
	DirectX::SimpleMath::Vector3	m_minVel;

	//�x�N�g���̌���
	DirectX::SimpleMath::Vector3	m_dirVel;

	//��]�i�f�O���[�j
	float							m_angle;

	//�J�n�t���O
	bool							m_start;

	//�I���t���O
	bool							m_finish;

public:
	//�R���X�g���N�^
	HomingState();

	//�f�X�g���N�^
	~HomingState()override {}

	//�X�V����
	const DirectX::SimpleMath::Vector3 Update()override;

	//---------------------�Z�b�^�[------------------------------------------------------------------//
	 
	//���s�֐���ݒ肷��
	void SetExecute(const bool& exe) { MoveState::SetExecute(exe); }

	//�폜�֐���ݒ肷��
	void SetIsVal(const bool& val) { MoveState::SetIsVal(val); }

	//������ݒ肷��
	void SetLength(const DirectX::SimpleMath::Vector3& length) { m_length = length; }

	//�p�x��ݒ肷��
	void SetAngle(const float& angle) { m_angle = angle; }

	//�X�J���l��ݒ肷��
	void SetScalar(const DirectX::SimpleMath::Vector3& scalar) { m_scalar = scalar; }

	//�x�N�g���̍ő�l��ݒ肷��
	void SetMaxVel(const DirectX::SimpleMath::Vector3& max) { m_maxVel = max; }

	//�x�N�g���̍ŏ��l��ݒ肷��
	void SetMinVel(const DirectX::SimpleMath::Vector3& min) { m_minVel = min; }

	//�x�N�g���̌�����ݒ肷��
	void SetDirVel(const DirectX::SimpleMath::Vector3& dir) { m_dirVel = dir; }

	//�J�n�t���O��ݒ肷��
	void SetStartFlag(const bool& start) { m_start = start; }

	//�I���t���O��ݒ肷��
	void SetFinishFlag(const bool& finish) { m_finish = finish; }

	//---------------------�Z�b�^�[------------------------------------------------------------------//


	//---------------------�Q�b�^�[------------------------------------------------------------------//
	 
	//���s�֐����擾����
	const bool& GetExecute()const { return MoveState::GetExecute(); }

	//�폜�֐����擾����
	const bool& GetIsVal()const { return MoveState::GetIsVal(); }

	//�������擾����
	const DirectX::SimpleMath::Vector3& GetLength()const { return m_length; }

	//�p�x���擾����
	const float& GetAngle()const { return m_angle; }

	//�X�J���l���擾����
	const DirectX::SimpleMath::Vector3& GetScalar()const { return m_scalar; }

	//�x�N�g���̍ő�l���擾����
	const DirectX::SimpleMath::Vector3& GetMaxVel()const { return m_maxVel; }

	//�x�N�g���̍ŏ��l���擾����
	const DirectX::SimpleMath::Vector3& GetMinVel()const { return m_minVel; }

	//�x�N�g���̌������擾����
	const DirectX::SimpleMath::Vector3& GetDirVel()const { return m_dirVel; }

	//�J�n�t���O���擾����
	const bool& GetStartFlag()const { return m_start; }

	//�I���t���O���擾����
	const bool& GetFinishFlag()const { return m_finish; }

	//---------------------�Q�b�^�[------------------------------------------------------------------//

public:
	//�X�e�[�g������������
	void InitState(const DirectX::SimpleMath::Vector3& dir);

};

