#pragma once
#include <string>
#include <map>
#include <vector>

//�����̂ŏȗ�
using ItemGropInfo = std::map<std::string, std::map<int, int>>;

//�A�C�e�����h���b�v����m��
class ItemDrop 
{
public:
	enum class ITEMINFO :int
	{
		GROP,//�O���[�v��
		LEVEL,//���x��
		PROBABILITY,//�m��
	};
private:
	//�O���[�v�̖��O
	std::string m_gropName;

	//�G�̃��x��
	int m_level;

	//�h���b�v����m��
	int m_probability;

	//�A�C�e���̑S�̂̏��
	static ItemGropInfo s_itemInfo;
public:
	//�R���X�g���N�^(�t�@�C������̃f�[�^�����Ƃɐ�������)
	ItemDrop();

	//�f�X�g���N�^
	~ItemDrop() = default;

	//-----------------�Z�b�^�[------------------------------//

	//�O���[�v����ݒ肷��
	void SetGropName(const std::string& name) { m_gropName = name; }

	//�G�̃��x����ݒ肷��
	void SetLevel(const int& level) { m_level = level; }

	//�h���b�v����m����ݒ肷��
	void SetProbability(const int& pro) { m_probability = pro; }

	//-----------------�Z�b�^�[------------------------------//

	//-----------------�Q�b�^�[------------------------------//

	//�O���[�v�����擾����
	const std::string& GetGropName()const { return m_gropName; }

	//�G�̃��x�����擾����
	const int& GetLevel()const { return m_level; }

	//�h���b�v����m�����擾����
	const int& GetProbability()const { return m_probability; }

	//-----------------�Q�b�^�[------------------------------//

public:
	//�f�[�^��ϊ�����
	static const std::vector<ItemDrop> Cast(const std::vector<std::vector<std::string>>& data);

	//�f�[�^��ǉ�����
	static void AddItem(const std::vector<ItemDrop>& itemGrop);

	//�h���b�v����m�����擾����
	static const int& GetProbability(const std::string& grop,const int& level) { return s_itemInfo[grop][level]; }

};
