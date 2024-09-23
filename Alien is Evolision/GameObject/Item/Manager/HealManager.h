#pragma once
#include <vector>
#include <string>
#include "GameObject/Item/Heal/Heal.h"
#include "TemplateState.h"
#include <functional>

class HealManager:public State<int>
{
public:
	//�񕜃A�C�e���̐�
	static const int HEALITEM_NUM = 5;

	//�A�C�e���������Ԋu
	static const float FIRING_INTERVAL;
private:
	//�񕜃A�C�e���̃��X�g(���Ԍo�߂ŗ���)
	std::vector<Heal*> m_healItem;

	//�擾�ς݃A�C�e���̃��X�g
	std::vector<std::unique_ptr<Item>> m_getItem;

	//�S�ẴA�C�e��
	std::vector<Item*> m_allItem;

	//�o�ߎ���
	float				m_timer;
public:
	//�R���X�g���N�^
	HealManager();

	//�f�X�g���N�^
	~HealManager() = default;

	//������
	void Initialize();

	//�X�V����
	void Update();

public:
	//���݂̏�Ԃ�ݒ肷��
	void SetNowState(const int& now)override { State::SetNowState(now); };

	//���݂̏�Ԃ��擾����
	const int& GetNowState()const override { return State::GetNowState(); };

	//�ЂƂO�̏�Ԃ��擾����
	const int& GetPreState()const override { return State::GetPreState(); };

public:
	//�񕜃A�C�e���𐶐�����
	void CreateItem(OtobeLib::ITextureBase* refTex,std::function<void(Item*)> func);

	//�񕜃A�C�e�����擾����
	std::vector<Heal*> GetHealList() { return m_healItem; }

	//�S�ẴA�C�e�����擾����
	std::vector<Item*> GetAllItem() { return m_allItem; }

	//�擾�ς݃A�C�e�����擾����
	std::vector<std::unique_ptr<Item>>& GetItem() { return m_getItem; }

	//�擾�����A�C�e����ǉ�����
	void AddItem(Item* item) { m_getItem.push_back(std::make_unique<Item>(item)); }
	void AddItem(std::unique_ptr<Item>&& item) { m_getItem.push_back(std::move(item)); }

	//�g�p�����A�C�e��������
	void EraseItem(const int& num) { m_getItem.erase(m_getItem.begin() + num); }
};

