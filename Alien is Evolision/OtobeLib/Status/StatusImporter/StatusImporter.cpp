#include "pch.h"
#include "StatusImporter.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

StatusImporter::StatusImporter():
	m_gropName(""),//�O���[�v��
	m_level(NULL),//���x��
	m_hp(NULL),//HP
	m_mp(NULL),//MP
	m_atack(NULL),//�U����
	m_defence(NULL),//�h���
	m_special(NULL),//�X�y�V�����|�C���g
	m_next(NULL)//���̃��x���܂�
{
}

StatusImporter::StatusImporter(const StatusImporter& importer)
{
	//�e�p�����[�^�[��ݒ肷��
	m_gropName = importer.GetGrop();
	m_level = importer.GetLevel();
	m_hp = importer.GetHp();
	m_mp = importer.GetMp();
	m_atack = importer.GetAtack();
	m_defence = importer.GetDefence();
	m_special = importer.GetSpecial();
	m_next = importer.GetNextLevelPoint();
}

const std::vector<StatusImporter> StatusImporter::Cast(const std::vector<std::vector<std::string>>& Input)
{
	//�f�[�^���i�[����R���e�i��錾����
	std::vector<StatusImporter> ImportData;

	//�g�p���鐔�����A���炩���ߗe�ʂ��m�ۂ���
	ImportData.resize(Input.size() - 1);

	//�w�肵���O���[�v�̏����擾����
	for (size_t i = 0; i < Input.size() - 1; i++)
	{
		for (size_t j = 0; j < Input[i].size(); j++)
		{
			//��̃f�[�^�����Ă���
			std::string str = "";
			//1�s���f�[�^���i�[����
			str = Input[i + 1][j];
			//�O���[�v����ݒ肷��
			if (j == (int)STATUSINFO::GROP)ImportData[i].SetGrop(str);
			//���x����ݒ肷��
			if (j == (int)STATUSINFO::LEVEL)ImportData[i].SetLevel(std::stoi(str));
			//HP��ݒ肷��
			if (j == (int)STATUSINFO::HP)ImportData[i].SetHp(std::stoi(str));
			//MP��ݒ肷��
			if (j == (int)STATUSINFO::MP)ImportData[i].SetMp(std::stoi(str));
			//�U���͂�ݒ肷��
			if (j == (int)STATUSINFO::ATACK)ImportData[i].SetAtack(std::stoi(str));
			//�h��͂�ݒ肷��
			if (j == (int)STATUSINFO::DEFENCE)ImportData[i].SetDefence(std::stoi(str));
			//�X�y�V�����|�C���g��ݒ肷��
			if (j == (int)STATUSINFO::SPECIAL)ImportData[i].SetSpecial(std::stoi(str));
			//���̃��x���܂ł̌o���l��ݒ肷��
			if (j == (int)STATUSINFO::NEXT)ImportData[i].SetNextLevelPoint(std::stoi(str));
		}
	}

	//�ϊ������f�[�^��Ԃ�
	return ImportData;

}
