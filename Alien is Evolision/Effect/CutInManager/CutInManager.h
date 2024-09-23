#pragma once
#include "OtobeLib/Texture/TextureBase/TextureBase.h"

class CutInManager
{
private:
	//�J�b�g�C���̃e�N�X�`��
	OtobeLib::TextureBase* m_cutinTexture;

	//�w�i�̃e�N�X�`��
	OtobeLib::TextureBase* m_backTexture;

	//�^�C�}�[
	float					m_timer;

	//�o�ߎ���
	float					m_totaltimer;

public:
	//�R���X�g���N�^
	CutInManager();

	//�f�X�g���N�^
	~CutInManager() = default;

	//������
	void Initialize();

	//�X�V
	void Update();

};

