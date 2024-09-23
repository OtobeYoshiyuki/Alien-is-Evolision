#pragma once
class MoveState
{
private:
	//���s����t���O
	bool						mExecute;
	//�폜����t���O
	bool						mIsVal;
public:
	//�R���X�g���N�^
	MoveState();

	//�f�X�g���N�^
	virtual ~MoveState(){}

	//�X�V����
	virtual const DirectX::SimpleMath::Vector3 Update() { return DirectX::SimpleMath::Vector3::Zero; }

	//---------------------�Z�b�^�[------------------------------------------------------------------//
	//���s�֐���ݒ肷��
	void SetExecute(const bool& exe) { mExecute = exe; }

	//�폜�֐���ݒ肷��
	void SetIsVal(const bool& val) { mIsVal = val; }
	//---------------------�Z�b�^�[------------------------------------------------------------------//


	//---------------------�Q�b�^�[------------------------------------------------------------------//
	//���s�֐����擾����
	const bool& GetExecute()const { return mExecute; }

	//�폜�֐����擾����
	const bool& GetIsVal()const { return mIsVal; }
	//---------------------�Q�b�^�[------------------------------------------------------------------//
};

