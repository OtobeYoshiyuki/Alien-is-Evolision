#include "pch.h"
#include "StatusCalc.h"
#include "OtobeLib/FrameWork/Rand/Rand.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//ダメージを計算する関数
void StatusCalc::CalcDamage(
	int& nowhp,//現在のHP
	const int& atack,//攻撃力
	const int& defence,//防御力
	const int& texLength,//画像の幅(最大値)
	const int& maxhp,//最大HP
	LONG& element,//画像の幅
	float correction//ダメージの補正値
)
{
	//ダメージ値を計算する
	float damage = (float)atack - (float)defence;

	//自身の防御力の方が高ければ、ダメージ値を修正する
	if (damage <= NULL)damage = 1.0f;

	//計算したダメージ値に補正値を乗算する
	damage *= correction;

	//HPにダメージを与える
	nowhp -= (int)damage;

	//HPが0より小さければ、修正する
	if (nowhp < NULL)nowhp = NULL;

	//HPバーの画像と実際のHPの値の比率を求める
	float ratio = (float)texLength / (float)maxhp;

	//ダメージの値を補正する
	damage *= ratio;

	//HPゲージにダメージを与える
	element -= damage;

	//ゲージが0より小さくなったら、0にする
	if (element < 0)element = 0;
}

void StatusCalc::CalcExp(
	const int& level,//レベル
	const float& strength,//強さ
	const int& rand_max,//乱数の最大値
	const int& texLength,//テクスチャの幅の最大値
	const int& mapep,//経験値の最大値
	int& Exp,//実際の経験値
	int& texexp,//画像上の経験値
	float& ratio//経験値の比率
)
{
	//乱数クラスを取得する
	Rand* random = Rand::GetInstance();

	//経験値の乱数を振る
	Exp = random->RandomMachine(0, rand_max);

	//HPバーの画像と実際のHPの値の比率を求める
	ratio = (float)texLength / (float)mapep;

	//経験値は、乱数に敵の強さとレベルを掛けた値に、振った乱数の値を足したものとする
	Exp += level * (int)strength;

	//経験値を画像の比率に合わせる
	texexp = Exp * ratio;
}

void StatusCalc::CalcHeal
(
	int& status,//対象のステータス
	int& heal,//回復値
	const int& texLength,//画像の横幅
	const int& maxLength,//対象のステータスの最大値
	LONG& element,//対象のRECT
	float corrrection//補正値
)
{
	//回復値がステータスの最大値より大きければ、補正する
	if (heal > maxLength - status)heal = maxLength - status;

	//計算した回復値に補正値を乗算する
	heal *= corrrection;

	//ステータスを回復させる
	status += heal;

	//ステータスが最大値を超えていたら、修正する
	if (status > maxLength)status = maxLength;

	//HPバーの画像と実際のHPの値の比率を求める
	float ratio = (float)texLength / (float)maxLength;

	//回復値を補正する
	float grapHeal = (float)heal * ratio;

	//HPゲージを回復させる
	element += grapHeal;

	//ゲージが最大値より大きければ、最大値に修正する
	if (element > texLength)element = texLength;
}
