#pragma once
#include <vector>
namespace OtobeLib
{
	struct StoryData
	{
		//�z����̓Y����
		int arrayNum;

		//�Ώۂ̕�����
		std::string text;
	};

	class Story:public Singleton<Story>
	{
	private:
		//�X�g�[���[�̃e�L�X�g�̏��
		std::vector<std::string>		m_storyText;

		//���ݓǂ�ł���e�L�X�g�̏ꏊ(���s�ڂ�)
		int								m_storyNumber;

	public:
		//�R���X�g���N�^
		Story();

		//�f�X�g���N�^
		~Story() {};

		//����������
		void Initialize(const wchar_t* fileName);

		//�X�V����
		void Update();

		//�w�肵���Ԓn�̕�������擾����
		const std::string& GetStoryText(const int& number)const { return m_storyText[number]; };

		//�w�肵��������̍\���̂��擾����
		const StoryData GetStoryData(const std::string& text)const;

		//�S�̂̕�����̔z����擾����
		const std::vector<std::string>& GetStoryArray()const { return m_storyText; };

		//������̕`��̐擪��ݒ肷��
		void SetStoryNumber(const int& number) { m_storyNumber = number; };

		//������̕`��̐擪���擾����
		const int& GetStoryNumber()const { return m_storyNumber; };

	};
}

