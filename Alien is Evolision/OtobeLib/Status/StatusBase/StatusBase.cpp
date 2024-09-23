#include "pch.h"
#include "StatusBase.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

StatusMap StatusBase::s_statusGrop = StatusMap();

//コンストラクタ
StatusBase::StatusBase():
	m_Hp(NULL),//HP
	m_Mp(NULL),//技ゲージ
	m_Atack(NULL),//攻撃力
	m_Defence(NULL),//防御力
	m_Special(NULL),//必殺技ゲージ
	m_Level(NULL),//レベル
	m_LevelPoint(NULL),//経験値
	m_compensation(NULL),//経験値を計算する際の補正値
	m_MaxHP(NULL),//最大HP
	m_MaxMP(NULL),//最大MP
	m_MaxAtack(NULL),//最大攻撃力
	m_MaxDefence(NULL),//最大防御力
	m_MaxSpecial(NULL),//最大必殺技ゲージ
	m_MaxLevel(NULL),//成長限界
	m_MaxNextLevelPoint(NULL)//次のレベルまでの経験値の最大値
{
}

//現在のステータスを設定する
void StatusBase::SetStatus(const int& hp, const int& mp, const int& atack, const int& defence,
	const int& special,const int& level, const int& point)
{
	//HPを設定する
	this->SetHP(hp);

	//MPを設定する
	this->SetMP(mp);

	//攻撃力を設定する
	this->SetAtack(atack);

	//防御力を設定する
	this->SetDefence(defence);

	//スペシャルポイントを設定する
	this->SetSpecial(special);

	//レベルを設定する
	this->SetLevel(level);

	//経験値を設定する
	this->SetNextLevelPoint(point);
}

//ステータスの最大値を設定する
void StatusBase::SetMaxStatus(const int& maxhp, const int& maxmp, const int& maxatack,
	const int& maxdefence, const int& maxspecial, const int& maxlevel,const int& maxnext)
{
	//HPの最大値を設定する
	this->SetMaxHP(maxhp);

	//MPの最大値を設定する
	this->SetMaxMP(maxmp);

	//攻撃力の最大値を設定する
	this->SetMaxAtack(maxatack);

	//防御力の最大値を設定する
	this->SetMaxDefence(maxdefence);

	//スペシャルポイントの最大値を設定する
	this->SetMaxSpecialPoint(maxspecial);

	//成長限界を設定する
	this->SetMaxLevel(maxlevel);

	//経験値の最大値を設定する
	this->SetMaxNextLevelPoint(maxnext);
}
