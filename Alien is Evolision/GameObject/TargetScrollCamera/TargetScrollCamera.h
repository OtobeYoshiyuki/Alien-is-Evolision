#pragma once
#include "OtobeLib/Camera/Base/CameraBase.h"
#include "OtobeLib/Texture/Interface/ITextureBase.h"
#include "TemplateState.h"
#include "TARGET_STATE.h"

class TargetScrollCamera:public OtobeLib::CameraBase,State<TARGET_STATE>
{
public:
	//�Ώە��Ɉړ�����ړ���
	static const float TARGET_TO_VECTOL;

	//�v���C���[�ƑΏە��Ƃ̋���
	static const float TARGET_TO_LENGTH;
private:
	//���݈ʒu����X�N���[��������ʒu
	DirectX::SimpleMath::Vector3 m_target;

	//�ǐՂ���i���o�[
	int							m_targetNumber;
public:
	//�R���X�g���N�^
	TargetScrollCamera();

	//�f�X�g���N�^
	~TargetScrollCamera()override {};

	//������
	void Initialize()override;

	//�X�V����
	void Update()override;

	//�I������
	void Finalize()override;

	//-------------------------------�Z�b�^�[---------------------------------//
		//���W��ݒ肷��
	virtual void SetPosition(const DirectX::SimpleMath::Vector3& pos) { CameraBase::SetPosition(pos); }

	//�s���ݒ肷��
	virtual void SetMatrix(const DirectX::SimpleMath::Matrix& matrix) { CameraBase::SetMatrix(matrix); }

	//�I�u�W�F�N�g�̈ړ��̗L����ݒ肷��
	virtual void SetObjectMove(const bool& move) { CameraBase::SetObjectMove(move); }
	//-------------------------------�Z�b�^�[---------------------------------//

	//-------------------------------�Q�b�^�[---------------------------------//
		//���W��ݒ肷��
	virtual const DirectX::SimpleMath::Vector3& GetPosition() { return CameraBase::GetPosition(); }

	//�s���ݒ肷��
	virtual const DirectX::SimpleMath::Matrix& GetMatrix() { return CameraBase::GetMatrix(); }

	//�I�u�W�F�N�g�̈ړ��̗L����ݒ肷��
	virtual const bool& GetObjectMove() { return CameraBase::GetObjectMove(); }
	//-------------------------------�Q�b�^�[---------------------------------//

public:
	//���݂̏�Ԃ�ݒ肷��
	void SetNowState(const TARGET_STATE& now)override { State::SetNowState(now); };

	//���݂̏�Ԃ��擾����
	const TARGET_STATE& GetNowState()const override { return State::GetNowState(); };

	//�ЂƂO�̏�Ԃ��擾����
	const TARGET_STATE& GetPreState()const override { return State::GetPreState(); };

public:
	//�X�N���[��������ʒu��ݒ肷��
	void SetTarget(const DirectX::SimpleMath::Vector3& target) { m_target = target; }

	//�ǐՂ���i���o�[��ݒ肷��
	void SetTargetNumber(const int& num) { m_targetNumber = num; }

	//�X�N���[��������ʒu���擾����
	const DirectX::SimpleMath::Vector3& GetTarget()const { return m_target; }

	//�ǐՂ���i���o�[���擾����
	const int& GetTargetNumber()const { return m_targetNumber; }

private:
	//�X�e�[�g�̍X�V���s��
	void UpdateState();

	//State_Tracking�̏���
	void State_Tracking();

	//State_Wait�̏���
	void State_Wait();

	//State_FadeOut�̏���
	void State_FadeOut();

	//State_FadeIn�̏���
	void State_FadeIn();

	//State_Change�̏���
	void State_Change();
};

