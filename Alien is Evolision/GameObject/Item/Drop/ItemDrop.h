#pragma once
#include <string>
#include <map>
#include <vector>

//長いので省略
using ItemGropInfo = std::map<std::string, std::map<int, int>>;

//アイテムがドロップする確率
class ItemDrop 
{
public:
	enum class ITEMINFO :int
	{
		GROP,//グループ名
		LEVEL,//レベル
		PROBABILITY,//確率
	};
private:
	//グループの名前
	std::string m_gropName;

	//敵のレベル
	int m_level;

	//ドロップする確率
	int m_probability;

	//アイテムの全体の情報
	static ItemGropInfo s_itemInfo;
public:
	//コンストラクタ(ファイルからのデータをもとに生成する)
	ItemDrop();

	//デストラクタ
	~ItemDrop() = default;

	//-----------------セッター------------------------------//

	//グループ名を設定する
	void SetGropName(const std::string& name) { m_gropName = name; }

	//敵のレベルを設定する
	void SetLevel(const int& level) { m_level = level; }

	//ドロップする確率を設定する
	void SetProbability(const int& pro) { m_probability = pro; }

	//-----------------セッター------------------------------//

	//-----------------ゲッター------------------------------//

	//グループ名を取得する
	const std::string& GetGropName()const { return m_gropName; }

	//敵のレベルを取得する
	const int& GetLevel()const { return m_level; }

	//ドロップする確率を取得する
	const int& GetProbability()const { return m_probability; }

	//-----------------ゲッター------------------------------//

public:
	//データを変換する
	static const std::vector<ItemDrop> Cast(const std::vector<std::vector<std::string>>& data);

	//データを追加する
	static void AddItem(const std::vector<ItemDrop>& itemGrop);

	//ドロップする確率を取得する
	static const int& GetProbability(const std::string& grop,const int& level) { return s_itemInfo[grop][level]; }

};
