#pragma once
#include "GameObject/Stage/Maptip/MapTip.h"
#include "TemplateState.h"
#include "GameObject/DamageUI/FadeState.h"

class HoleSwitch :public MapTip,State<FADE_STATE>
{
private:
	//�N������J�E���g
	float m_DetonationCount;
	//�J�E���g���X�V����t���O
	bool m_DetonationFlag;
	//�V���A���i���o�[
	int	m_SelealNumber;
	//�w�i�̃e�N�X�`��
	OtobeLib::ITextureBase* m_backTexture;
public:
	//�R���X�g���N�^
	HoleSwitch(OtobeLib::ITextureBase* texture);

	//�f�X�g���N�^
	~HoleSwitch()override {};

	//������
	void Initialize()override;

	//�X�V
	void Update()override;

	//�`��
	void Render()override;

	//�I��
	void Finalize()override;

	//���̃I�u�W�F�N�g�Ɠ����������ɌĂ΂��֐�
	void OnCollisionEnter(ITextureBase* obj)override;

	//���̃I�u�W�F�N�g�Ɠ������Ă��Ȃ��Ƃ��ɌĂ΂��֐�
	void NonCollisionEnter(ITextureBase* obj)override;

public:
	//���݂̏�Ԃ�ݒ肷��
	void SetNowState(const FADE_STATE& now)override { State::SetNowState(now); };

	//���݂̏�Ԃ��擾����
	const FADE_STATE& GetNowState()const override { return State::GetNowState(); };

	//�ЂƂO�̏�Ԃ��擾����
	const FADE_STATE& GetPreState()const override { return State::GetPreState(); };

public:
	//�V���A���i���o�[��ݒ肷��
	void SetSerealNumber(const int& number) { m_SelealNumber = number; }
	//�V���A���i���o�[���擾����
	const int& GetSerealNumber()const { return m_SelealNumber; }
};

