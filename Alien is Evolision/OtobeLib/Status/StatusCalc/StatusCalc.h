#pragma once
namespace OtobeLib
{
	//ダメージ計算をするクラス
	class StatusCalc
	{
	public:
		//ダメージ値を計算する
		static void CalcDamage(int& nowhp, const int& atack, const int& defence,
			const int& texLength, const int& maxhp, LONG& element,float correction = 1.0f);

		//経験値を求める
		static void CalcExp(const int& level, const float& strength,
			const int& rand_max,const int& texLength,const int& maxep,int& exp,int& texexp,float& ratio);

		//回復値を計算する
		static void CalcHeal(int& status,int& heal, const int& texLength, 
			const int& maxLength, LONG& element, float corrrection = 1.0f);
	};
}

