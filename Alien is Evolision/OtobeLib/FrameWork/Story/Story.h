#pragma once
#include <vector>
namespace OtobeLib
{
	struct StoryData
	{
		//配列内の添え字
		int arrayNum;

		//対象の文字列
		std::string text;
	};

	class Story:public Singleton<Story>
	{
	private:
		//ストーリーのテキストの情報
		std::vector<std::string>		m_storyText;

		//現在読んでいるテキストの場所(何行目か)
		int								m_storyNumber;

	public:
		//コンストラクタ
		Story();

		//デストラクタ
		~Story() {};

		//初期化処理
		void Initialize(const wchar_t* fileName);

		//更新処理
		void Update();

		//指定した番地の文字列を取得する
		const std::string& GetStoryText(const int& number)const { return m_storyText[number]; };

		//指定した文字列の構造体を取得する
		const StoryData GetStoryData(const std::string& text)const;

		//全体の文字列の配列を取得する
		const std::vector<std::string>& GetStoryArray()const { return m_storyText; };

		//文字列の描画の先頭を設定する
		void SetStoryNumber(const int& number) { m_storyNumber = number; };

		//文字列の描画の先頭を取得する
		const int& GetStoryNumber()const { return m_storyNumber; };

	};
}

