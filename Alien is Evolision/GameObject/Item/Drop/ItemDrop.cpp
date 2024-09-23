#include "pch.h"
#include "ItemDrop.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

ItemGropInfo ItemDrop::s_itemInfo = ItemGropInfo();

ItemDrop::ItemDrop():
	m_gropName(""),//グループ名
	m_level(NULL),//敵のレベル
	m_probability(NULL)//ドロップする確率
{
}

const std::vector<ItemDrop> ItemDrop::Cast(const std::vector<std::vector<std::string>>& data)
{
	//データを格納するコンテナを宣言する
	std::vector<ItemDrop> itemDrop;

	//使用する数だけ、あらかじめ容量を確保する
	itemDrop.resize(data.size() - 1);

	//指定したグループの情報を取得する
	for (size_t i = 0; i < data.size() - 1; i++)
	{
		for (size_t j = 0; j < data[i].size(); j++)
		{
			//空のデータを入れておく
			std::string str = "";
			//1行ずつデータを格納する
			str = data[i + 1][j];
			//グループ名を設定する
			if (j == (int)ITEMINFO::GROP)itemDrop[i].SetGropName(str);
			//アニメーションの情報を設定する
			if (j == (int)ITEMINFO::LEVEL)itemDrop[i].SetLevel(std::stoi(str));
			//次のアニメーションの情報を設定する
			if (j == (int)ITEMINFO::PROBABILITY)itemDrop[i].SetProbability(std::stoi(str));
		}
	}

	//変換したデータを返す
	return itemDrop;
}

void ItemDrop::AddItem(const std::vector<ItemDrop>& itemGrop)
{
	for (const ItemDrop& importer : itemGrop)
	{
		//グループ名を取得する
		std::string grop = importer.GetGropName();
		
		//レベルを取得する
		int level = importer.GetLevel();

		//確率を取得する
		int pro = importer.GetProbability();

		//情報を設定する
		s_itemInfo[grop][level] = pro;
	}
}
