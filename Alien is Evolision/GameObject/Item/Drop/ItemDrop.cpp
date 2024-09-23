#include "pch.h"
#include "ItemDrop.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

ItemGropInfo ItemDrop::s_itemInfo = ItemGropInfo();

ItemDrop::ItemDrop():
	m_gropName(""),//�O���[�v��
	m_level(NULL),//�G�̃��x��
	m_probability(NULL)//�h���b�v����m��
{
}

const std::vector<ItemDrop> ItemDrop::Cast(const std::vector<std::vector<std::string>>& data)
{
	//�f�[�^���i�[����R���e�i��錾����
	std::vector<ItemDrop> itemDrop;

	//�g�p���鐔�����A���炩���ߗe�ʂ��m�ۂ���
	itemDrop.resize(data.size() - 1);

	//�w�肵���O���[�v�̏����擾����
	for (size_t i = 0; i < data.size() - 1; i++)
	{
		for (size_t j = 0; j < data[i].size(); j++)
		{
			//��̃f�[�^�����Ă���
			std::string str = "";
			//1�s���f�[�^���i�[����
			str = data[i + 1][j];
			//�O���[�v����ݒ肷��
			if (j == (int)ITEMINFO::GROP)itemDrop[i].SetGropName(str);
			//�A�j���[�V�����̏���ݒ肷��
			if (j == (int)ITEMINFO::LEVEL)itemDrop[i].SetLevel(std::stoi(str));
			//���̃A�j���[�V�����̏���ݒ肷��
			if (j == (int)ITEMINFO::PROBABILITY)itemDrop[i].SetProbability(std::stoi(str));
		}
	}

	//�ϊ������f�[�^��Ԃ�
	return itemDrop;
}

void ItemDrop::AddItem(const std::vector<ItemDrop>& itemGrop)
{
	for (const ItemDrop& importer : itemGrop)
	{
		//�O���[�v�����擾����
		std::string grop = importer.GetGropName();
		
		//���x�����擾����
		int level = importer.GetLevel();

		//�m�����擾����
		int pro = importer.GetProbability();

		//����ݒ肷��
		s_itemInfo[grop][level] = pro;
	}
}
