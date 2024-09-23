#include "pch.h"
#include "StatusImporter.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

StatusImporter::StatusImporter():
	m_gropName(""),//グループ名
	m_level(NULL),//レベル
	m_hp(NULL),//HP
	m_mp(NULL),//MP
	m_atack(NULL),//攻撃力
	m_defence(NULL),//防御力
	m_special(NULL),//スペシャルポイント
	m_next(NULL)//次のレベルまで
{
}

StatusImporter::StatusImporter(const StatusImporter& importer)
{
	//各パラメーターを設定する
	m_gropName = importer.GetGrop();
	m_level = importer.GetLevel();
	m_hp = importer.GetHp();
	m_mp = importer.GetMp();
	m_atack = importer.GetAtack();
	m_defence = importer.GetDefence();
	m_special = importer.GetSpecial();
	m_next = importer.GetNextLevelPoint();
}

const std::vector<StatusImporter> StatusImporter::Cast(const std::vector<std::vector<std::string>>& Input)
{
	//データを格納するコンテナを宣言する
	std::vector<StatusImporter> ImportData;

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
			if (j == (int)STATUSINFO::GROP)ImportData[i].SetGrop(str);
			//レベルを設定する
			if (j == (int)STATUSINFO::LEVEL)ImportData[i].SetLevel(std::stoi(str));
			//HPを設定する
			if (j == (int)STATUSINFO::HP)ImportData[i].SetHp(std::stoi(str));
			//MPを設定する
			if (j == (int)STATUSINFO::MP)ImportData[i].SetMp(std::stoi(str));
			//攻撃力を設定する
			if (j == (int)STATUSINFO::ATACK)ImportData[i].SetAtack(std::stoi(str));
			//防御力を設定する
			if (j == (int)STATUSINFO::DEFENCE)ImportData[i].SetDefence(std::stoi(str));
			//スペシャルポイントを設定する
			if (j == (int)STATUSINFO::SPECIAL)ImportData[i].SetSpecial(std::stoi(str));
			//次のレベルまでの経験値を設定する
			if (j == (int)STATUSINFO::NEXT)ImportData[i].SetNextLevelPoint(std::stoi(str));
		}
	}

	//変換したデータを返す
	return ImportData;

}
