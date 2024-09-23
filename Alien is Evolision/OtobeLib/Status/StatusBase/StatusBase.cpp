#include "pch.h"
#include "StatusBase.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

StatusMap StatusBase::s_statusGrop = StatusMap();

//�R���X�g���N�^
StatusBase::StatusBase():
	m_Hp(NULL),//HP
	m_Mp(NULL),//�Z�Q�[�W
	m_Atack(NULL),//�U����
	m_Defence(NULL),//�h���
	m_Special(NULL),//�K�E�Z�Q�[�W
	m_Level(NULL),//���x��
	m_LevelPoint(NULL),//�o���l
	m_compensation(NULL),//�o���l���v�Z����ۂ̕␳�l
	m_MaxHP(NULL),//�ő�HP
	m_MaxMP(NULL),//�ő�MP
	m_MaxAtack(NULL),//�ő�U����
	m_MaxDefence(NULL),//�ő�h���
	m_MaxSpecial(NULL),//�ő�K�E�Z�Q�[�W
	m_MaxLevel(NULL),//�������E
	m_MaxNextLevelPoint(NULL)//���̃��x���܂ł̌o���l�̍ő�l
{
}

//���݂̃X�e�[�^�X��ݒ肷��
void StatusBase::SetStatus(const int& hp, const int& mp, const int& atack, const int& defence,
	const int& special,const int& level, const int& point)
{
	//HP��ݒ肷��
	this->SetHP(hp);

	//MP��ݒ肷��
	this->SetMP(mp);

	//�U���͂�ݒ肷��
	this->SetAtack(atack);

	//�h��͂�ݒ肷��
	this->SetDefence(defence);

	//�X�y�V�����|�C���g��ݒ肷��
	this->SetSpecial(special);

	//���x����ݒ肷��
	this->SetLevel(level);

	//�o���l��ݒ肷��
	this->SetNextLevelPoint(point);
}

//�X�e�[�^�X�̍ő�l��ݒ肷��
void StatusBase::SetMaxStatus(const int& maxhp, const int& maxmp, const int& maxatack,
	const int& maxdefence, const int& maxspecial, const int& maxlevel,const int& maxnext)
{
	//HP�̍ő�l��ݒ肷��
	this->SetMaxHP(maxhp);

	//MP�̍ő�l��ݒ肷��
	this->SetMaxMP(maxmp);

	//�U���͂̍ő�l��ݒ肷��
	this->SetMaxAtack(maxatack);

	//�h��͂̍ő�l��ݒ肷��
	this->SetMaxDefence(maxdefence);

	//�X�y�V�����|�C���g�̍ő�l��ݒ肷��
	this->SetMaxSpecialPoint(maxspecial);

	//�������E��ݒ肷��
	this->SetMaxLevel(maxlevel);

	//�o���l�̍ő�l��ݒ肷��
	this->SetMaxNextLevelPoint(maxnext);
}
