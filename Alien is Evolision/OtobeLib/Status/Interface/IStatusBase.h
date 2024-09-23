#pragma once

//ステータスのインターフェースクラス

namespace OtobeLib
{
	class IStatusBase
	{
	public:
		//デストラクタ
		virtual ~IStatusBase() {}

		//-----------------------セッター----------------------------------------------------------------//

		//ステータスを設定する
		virtual void SetStatus(const int& hp, const int& mp, const int& atack, 
			const int& defence,const int& special, const int& level,const int& next) = 0;

		//ステータスの最大値を設定する
		virtual void SetMaxStatus(const int& maxhp, const int& maxmp, const int& maxatack,
			const int& maxdefence, const int& maxspecial, const int& maxlevel,const int& maxnext) = 0;

		//HPを設定する
		virtual void SetHP(const int& hp) = 0;

		//MPを設定する
		virtual void SetMP(const int& mp) = 0;

		//攻撃力を設定する
		virtual void SetAtack(const int& atack) = 0;

		//防御力を設定する
		virtual void SetDefence(const int& defence) = 0;

		//スペシャルポイントを設定する
		virtual void SetSpecial(const int& special) = 0;

		//レベルを設定する
		virtual void SetLevel(const int& level) = 0;

		//次のレベルまでの経験値を設定する
		virtual void SetNextLevelPoint(const int& point) = 0;

		//経験値を計算する際の補正値を設定する
		virtual void SetCompensation(const int& com) = 0;

		//HPの最大値を設定する
		virtual void SetMaxHP(const int& maxhp) = 0;

		//MPの最大値を設定する
		virtual void SetMaxMP(const int& maxmp) = 0;

		//攻撃力の最大値を設定する
		virtual void SetMaxAtack(const int& maxatack) = 0;

		//防御力の最大値を設定する
		virtual void SetMaxDefence(const int& maxdefence) = 0;
		
		//スペシャルポイントの最大値を設定する
		virtual void SetMaxSpecialPoint(const int& maxspecial) = 0;

		//成長限界を設定する
		virtual void SetMaxLevel(const int& maxlevel) = 0;

		//経験値の最大値を設定する
		virtual void SetMaxNextLevelPoint(const int& maxnext) = 0;


		//-----------------------セッター----------------------------------------------------------------//


		//-----------------------ゲッター----------------------------------------------------------------//

		//HPを取得する
		virtual const int& GetHP()const = 0;

		//MPを取得する
		virtual const int& GetMP()const = 0;

		//攻撃力を取得する
		virtual const int& GetAtack()const = 0;

		//防御力を取得する
		virtual const int& GetDefence()const = 0;

		//スペシャルポイントを取得する
		virtual const int& GetSpecial()const = 0;

		//レベルを設定する
		virtual const int& GetLevel()const = 0;

		//次のレベルまでの経験値を設定する
		virtual const int& GetNextLevelPoint()const = 0;

		//経験値を計算する際の補正値を設定する
		virtual const int& GetCompensation()const = 0;

		//HPの最大値を設定する
		virtual const int& GetMaxHP()const = 0;

		//MPの最大値を設定する
		virtual const int& GetMaxMP()const = 0;

		//攻撃力の最大値を設定する
		virtual const int& GetMaxAtack()const = 0;

		//防御力の最大値を設定する
		virtual const int& GetMaxDefence()const = 0;

		//スペシャルポイントの最大値を設定する
		virtual const int& GetMaxSpecialPoint()const = 0;

		//成長限界を設定する
		virtual const int& GetMaxLevel()const = 0;

		//経験値の最大値を設定する
		virtual const int& GetMaxNextLevelPoint()const = 0;

		//自身のインスタンスを取得する
		virtual IStatusBase* GetStatus() = 0;

		//-----------------------ゲッター----------------------------------------------------------------//
	};
}
