#pragma once
#include "Effect/Star/Star.h"
#include <list>
class StarManager
{
private:
	//���S�̐�
	Star* m_MainStar;

	//�͂����鐯�̃��X�g
	std::list<Star*>	m_PopStars;

	//�͂����鐯�i�j
	Star* m_PopStarBlue;

	//�͂����鐯�i�ԁj
	Star* m_PopStarRed;
public:
	//�R���X�g���N�^
	StarManager();

	//�f�X�g���N�^
	~StarManager(){}

	//����������
	void Initialize(const DirectX::SimpleMath::Vector3& pos);

	//�X�V����
	void Update();

public:

	//���S�̐����擾����
	Star* GetMainStar() { return m_MainStar; }

	//�͂����鐯�̃��X�g���擾����
	std::list<Star*>& GetPopStarList() { return m_PopStars; }

	//�͂�����Ԃ������擾����
	Star* GetPopStarRed() { return m_PopStarRed; }

	//�͂�����������擾����
	Star* GetPopStarBlue() { return m_PopStarBlue; }

private:
	//���S�̐��̍X�V����
	void UpdateMainStar();

	//�͂�����Ԃ����̍X�V����
	void UpdatePopStarRed();

	//�͂���������̍X�V����
	void UpdatePopStarBlue();

};

