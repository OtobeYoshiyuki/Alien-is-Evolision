#include "pch.h"
#include "AnimationImporter.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//コンストラクタ
AnimationImporter::AnimationImporter(): m_GropName(""), m_Animation(""), m_Next(""), m_End(NULL), m_Refference("")
{
}

//コンストラクタ(引数あり)
AnimationImporter::AnimationImporter(const AnimationImporter& importer)
{
	//各パラメーターを設定する
	m_GropName = importer.GetGrop();
	m_Animation = importer.GetAnimation();
	m_Next = importer.GetNext();
	m_End = importer.GetEnd();
	m_Refference = importer.GetRefference();
}

//データをアニメーション用に変換する
const std::vector<AnimationImporter> AnimationImporter::Cast(const std::vector<std::vector<std::string>>& Input)
{
	//データを格納するコンテナを宣言する
	std::vector<AnimationImporter> ImportData;

	//使用する数だけ、あらかじめ容量を確保する
	ImportData.resize(Input.size() - 1);

	//指定したグループの情報を取得する
	for (size_t i = 0; i < Input.size() - 1; i++)
	{
		for (size_t j = 0; j < Input[i].size(); j++)
		{
			//空のデータを入れておく
			std::string str = "";
			//1行ずつデータを格納する
			str = Input[i + 1][j];
			//グループ名を設定する
			if (j == (int)ANIMATIONINFO::GROP)ImportData[i].SetGrop(str);
			//アニメーションの情報を設定する
			if (j == (int)ANIMATIONINFO::ANIMATION)ImportData[i].SetAnimation(str);
			//次のアニメーションの情報を設定する
			if (j == (int)ANIMATIONINFO::NEXT)ImportData[i].SetNext(str);
			//アニメーションの終了時間を設定する
			if (j == (int)ANIMATIONINFO::ENDTIME)ImportData[i].SetEnd(std::stoi(str));
			//参照テクスチャを設定する
			if (j == (int)ANIMATIONINFO::TEXTURE)ImportData[i].SetRefference(str);
		}
	}

	//変換したデータを返す
	return ImportData;
}
