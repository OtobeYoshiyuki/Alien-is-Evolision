#pragma once
#include "GameObject/Stage/Maptip/MapTip.h"
#include "GameObject/CollisionUI/CollisionUI.h"
#include "TemplateState.h"
#include "GameObject/DamageUI/FadeState.h"
#include <vector>
class Rebound:public MapTip,State<FADE_STATE>
{
private:
	//�O�ʂ̃e�N�X�`��
	OtobeLib::ITextureBase* m_frontTexture;
public:
	//�R���X�g���N�^
	Rebound(OtobeLib::ITextureBase* texture);

	//�f�X�g���N�^
	~Rebound()override {};

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

private:
	//�X�e�[�g�̍X�V����
	void UpdateState();
};

