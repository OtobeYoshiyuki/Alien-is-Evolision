#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "OtobeLib/FrameWork/Singleton/Singleton.h"

namespace OtobeLib
{
	class Stream:public Singleton<Stream>
	{
		friend class Singleton<Stream>;//基本的にメンバにアクセスしない
	public:
		enum FILE :int
		{
			STREAM_NONE,//開かない
			STREAM_IFSTREAM,//入力モード
			STREAM_OFSTREAM,//出力モード
		};
	private:
		//入力用ストリーム
		std::ifstream m_ifs;

		//出力用ストリーム
		std::ofstream m_ofs;

		//ファイルモード
		FILE m_stream;
	public:
		//コンストラクタ
		Stream();

		//デストラクタ
		~Stream();

		//ファイルを開く
		bool Open(const wchar_t* file_name, FILE mode);

		//ファイルを閉じる
		void Close();

		//読み込んだ文字を引数ごとに分ける
		std::vector<std::vector<std::string >> split(std::vector<std::string>& input, char delimiter);

		//ファイルの中身をすべて読み込む
		std::vector<std::vector<std::string >> Load_Data();

		//配列の中身を全て、ファイルに保存する（すでに、指定したファイルがある場合は、上書きする）
		void Data_Input(std::vector<std::vector<std::string>>& input);
	};
}
