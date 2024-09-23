#pragma once
#include "GameObject/Stage/Maptip/MapTip.h"
class Goal:public MapTip
{
public:
	//�R���X�g���N�^
	Goal(OtobeLib::ITextureBase* texture);

	//�f�X�g���N�^
	~Goal()override {};

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
};

