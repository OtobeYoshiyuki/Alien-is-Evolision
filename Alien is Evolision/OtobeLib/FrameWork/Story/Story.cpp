#include "pch.h"
#include "Story.h"
#include "OtobeLib/FrameWork/Stream/Stream.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

Story::Story():
	m_storyNumber(NULL)//�z��̓Y����
{
}

void Story::Initialize(const wchar_t* fileName)
{
	//�t�@�C�����擾����
	Stream* file = Stream::GetInstance();

	//�t�@�C��������������J��
	if (file->Open(fileName, Stream::FILE::STREAM_IFSTREAM))
	{
		//�f�[�^��ǂݍ���
		std::vector<std::vector<std::string>> vec = Stream::GetInstance()->Load_Data();

		for (int i = 0; i < (int)vec.size(); i++)
		{
			for (int j = 0; j < (int)vec[i].size(); j++)
			{
				//2�����z��̏����A1�����z��ɕϊ�����
				m_storyText.push_back(vec[i][j]);
			}
		}

		//�t�@�C�������
		file->Close();
	}
}

void Story::Update()
{
}

const StoryData Story::GetStoryData(const std::string& text) const
{
	//�\���̂�錾����
	StoryData story;

	for (int i = 0; i < (int)m_storyText.size(); i++)
	{
		//�Ώۂ̕�������������
		if (m_storyText[i] == text)
		{
			//�Y��������
			story.arrayNum = i;

			//���������
			story.text = m_storyText[i];

			//���[�v�𔲂���
			break;
		}
	}

	//�������������ƓY������Ԃ�
	return story;
}
