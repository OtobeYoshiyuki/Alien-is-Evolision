#include "pch.h"
#include "Story.h"
#include "OtobeLib/FrameWork/Stream/Stream.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

Story::Story():
	m_storyNumber(NULL)//配列の添え字
{
}

void Story::Initialize(const wchar_t* fileName)
{
	//ファイルを取得する
	Stream* file = Stream::GetInstance();

	//ファイルが見つかったら開く
	if (file->Open(fileName, Stream::FILE::STREAM_IFSTREAM))
	{
		//データを読み込む
		std::vector<std::vector<std::string>> vec = Stream::GetInstance()->Load_Data();

		for (int i = 0; i < (int)vec.size(); i++)
		{
			for (int j = 0; j < (int)vec[i].size(); j++)
			{
				//2次元配列の情報を、1次元配列に変換する
				m_storyText.push_back(vec[i][j]);
			}
		}

		//ファイルを閉じる
		file->Close();
	}
}

void Story::Update()
{
}

const StoryData Story::GetStoryData(const std::string& text) const
{
	//構造体を宣言する
	StoryData story;

	for (int i = 0; i < (int)m_storyText.size(); i++)
	{
		//対象の文字が見つかった
		if (m_storyText[i] == text)
		{
			//添え字を代入
			story.arrayNum = i;

			//文字列を代入
			story.text = m_storyText[i];

			//ループを抜ける
			break;
		}
	}

	//検索した文字と添え字を返す
	return story;
}
