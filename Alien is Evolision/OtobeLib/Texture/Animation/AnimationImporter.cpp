#include "pch.h"
#include "AnimationImporter.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//�R���X�g���N�^
AnimationImporter::AnimationImporter(): m_GropName(""), m_Animation(""), m_Next(""), m_End(NULL), m_Refference("")
{
}

//�R���X�g���N�^(��������)
AnimationImporter::AnimationImporter(const AnimationImporter& importer)
{
	//�e�p�����[�^�[��ݒ肷��
	m_GropName = importer.GetGrop();
	m_Animation = importer.GetAnimation();
	m_Next = importer.GetNext();
	m_End = importer.GetEnd();
	m_Refference = importer.GetRefference();
}

//�f�[�^���A�j���[�V�����p�ɕϊ�����
const std::vector<AnimationImporter> AnimationImporter::Cast(const std::vector<std::vector<std::string>>& Input)
{
	//�f�[�^���i�[����R���e�i��錾����
	std::vector<AnimationImporter> ImportData;

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
			if (j == (int)ANIMATIONINFO::GROP)ImportData[i].SetGrop(str);
			//�A�j���[�V�����̏���ݒ肷��
			if (j == (int)ANIMATIONINFO::ANIMATION)ImportData[i].SetAnimation(str);
			//���̃A�j���[�V�����̏���ݒ肷��
			if (j == (int)ANIMATIONINFO::NEXT)ImportData[i].SetNext(str);
			//�A�j���[�V�����̏I�����Ԃ�ݒ肷��
			if (j == (int)ANIMATIONINFO::ENDTIME)ImportData[i].SetEnd(std::stoi(str));
			//�Q�ƃe�N�X�`����ݒ肷��
			if (j == (int)ANIMATIONINFO::TEXTURE)ImportData[i].SetRefference(str);
		}
	}

	//�ϊ������f�[�^��Ԃ�
	return ImportData;
}
