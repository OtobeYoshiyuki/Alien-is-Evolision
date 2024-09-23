#pragma once
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "OtobeLib/FrameWork/Collision/CollisionTemplate/CollisionBase.h"
#include <list>
#include "GameObject/Player/NormalTechnique/NormalBullet/NormalBullet.h"

class NormalGage;

class NormalTechnique
{
public:
	//�ʏ�e�̒e��
	static const int BULLET_NUM = 10;

	//�Q�[�W�̉񕜗�(�R�}���h�Z)
	static const int TECHNIQUE_HEAL_COMMAND = 10;

	//�Q�[�W�̉񕜗�(�K�E�Z)
	static const int TECHNIQUE_HEAL_SPECIAL = 20;

private:
	//�Q�[�W�̗���
	OtobeLib::ITextureBase* m_BackGage;

	//�Q�[�W�̕\��
	NormalGage*		m_NormalGage;

	//�ʏ�e
	std::vector<NormalBullet*>	m_NormalBulletsList;

	//���˂����
	NormalBullet*	m_FrontBullet;

	//���s�֐��t���O
	bool			m_Active;

	//�����������ǂ����̔���t���O
	bool			m_checkPush;

	//�����������ǂ����̔���t���O
	bool			m_checkRelease;

	//�`���[�W���ɓ��������ۂ̍Ĕ��˂̎���
	float			m_time;

public:
	//�R���X�g���N�^
	NormalTechnique();

	//�f�X�g���N�^
	~NormalTechnique() {};

	//������
	void Initialize();

	//�X�V
	void Update();

public:
	//�擪�̒e��ݒ肷��
	void SetFrontBullet(NormalBullet* bullet) { m_FrontBullet = bullet; }

	//NomalGage���擾����
	NormalGage* GetNormalGage() { return m_NormalGage; }

	//�擪�̒e���擾����
	NormalBullet* GetFrontBullet() { return m_FrontBullet; }

	//�擪�̔��ˑҋ@���Ă���e���擾����
	NormalBullet* GetAllBulletFront();

	//�e�̃��X�g���擾����
	std::vector<NormalBullet*>& GetBulletsList() { return m_NormalBulletsList; }

	//���s�֐��t���O��ݒ肷��
	void SetActive(const bool& active) { m_Active = active; }

	//���s�֐��t���O���擾����
	const bool& GetActive()const { return m_Active; }

	//�_���[�W����p�̃t���O��ݒ肷��
	void SetCheckPush(const bool& push) {m_checkPush = push;}

	//�_���[�W����p�̃t���O��ݒ肷��
	void SetCheckRelease(const bool& release) { m_checkRelease = release; }

	//���Ԃ�ݒ肷��
	void SetTime(const float& time) { m_time = time; }

	//�_���[�W����p�̃t���O���擾����
	const bool& GetCheckPush()const { return m_checkPush; }

	//�_���|�W����p�̃t���O���擾����
	const bool& GetCheckRelease()const { return m_checkRelease; }

	//���Ԃ��擾����
	const float& GetTime()const { return m_time; }

	//�Q�[�W�̗��ʂ��擾����
	OtobeLib::ITextureBase* GetBackGage() { return m_BackGage; }
};

