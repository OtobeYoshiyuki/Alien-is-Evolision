#pragma once
#include "OtobeLib/Status/Interface/IStatusBase.h"
#include "OtobeLib/Status/StatusImporter/StatusImporter.h"

//ステータスの基底クラス
namespace OtobeLib
{
	//型名が長いので省略
	using StatusList = std::map<int,std::unique_ptr<StatusImporter>>;
	using StatusMap = std::map<std::string, StatusList>;

	class StatusBase :public IStatusBase
	{
	private:
		//HP
		int m_Hp;

		//MP
		int m_Mp;

		//攻撃力
		int m_Atack;

		//防御力
		int m_Defence;

		//スペシャルポイント
		int m_Special;

		//現在のレベル
		int m_Level;

		//経験値
		int m_LevelPoint;

		//経験値を計算する際の補正値
		int m_compensation;

		//最大HP
		int m_MaxHP;

		//最大MP
		int m_MaxMP;

		//最大攻撃力
		int m_MaxAtack;

		//最大防御力
		int m_MaxDefence;

		//最大スペシャルポイント		
		int m_MaxSpecial;
		
		//成長限界
		int m_MaxLevel;

		//経験値の最大
		int m_MaxNextLevelPoint;

		//アニメーションのグループを設定する
		static StatusMap s_statusGrop;
	public:
		//コンストラクタ
		StatusBase();

		//デストラクタ(デフォルト)
		~StatusBase() = default;

		//-----------------------セッター----------------------------------------------------------------//

		//ステータスを設定する
		virtual void SetStatus(const int& hp, const int& mp, const int& atack, 
			const int& defence,const int& special,const int& level, const int& next)override;

		//ステータスの最大値を設定する
		virtual void SetMaxStatus(const int& maxhp, const int& maxmp, const int& maxatack,
			const int& maxdefence, const int& maxspecial, const int& maxlevel,const int& next)override;

		//HPを設定する
		virtual void SetHP(const int& hp)override { m_Hp = hp; }

		//MPを設定する
		virtual void SetMP(const int& mp)override { m_Mp = mp; }

		//攻撃力を設定する
		virtual void SetAtack(const int& atack)override { m_Atack = atack; }

		//防御力を設定する
		virtual void SetDefence(const int& defence)override { m_Defence = defence; }

		//スペシャルポイントを設定する
		virtual void SetSpecial(const int& special)override { m_Special = special; }

		//レベルを設定する
		virtual void SetLevel(const int& level) override { m_Level = level; };

		//次のレベルまでの経験値を設定する
		virtual void SetNextLevelPoint(const int& point)override { m_LevelPoint = point; };

		//経験値を計算する際の補正値を設定する
		virtual void SetCompensation(const int& com)override { m_compensation = com; };

		//HPの最大値を設定する
		virtual void SetMaxHP(const int& maxhp)override { m_MaxHP = maxhp; };

		//MPの最大値を設定する
		virtual void SetMaxMP(const int& maxmp)override { m_MaxMP = maxmp; };

		//攻撃力の最大値を設定する
		virtual void SetMaxAtack(const int& maxatack)override { m_MaxAtack = maxatack; };

		//防御力の最大値を設定する
		virtual void SetMaxDefence(const int& maxdefence)override { m_MaxDefence = maxdefence; };

		//スペシャルポイントの最大値を設定する
		virtual void SetMaxSpecialPoint(const int& maxspecial)override { m_MaxSpecial = maxspecial; };

		//成長限界を設定する
		virtual void SetMaxLevel(const int& maxlevel)override { m_MaxLevel = maxlevel; };

		//経験値の最大値を設定する
		virtual void SetMaxNextLevelPoint(const int& maxnext)override { m_MaxNextLevelPoint = maxnext; };

		//-----------------------セッター----------------------------------------------------------------//


		//-----------------------ゲッター----------------------------------------------------------------//

		//HPを取得する
		virtual const int& GetHP()const override { return m_Hp; }

		//MPを取得する
		virtual const int& GetMP()const override { return m_Mp; }

		//攻撃力を取得する
		virtual const int& GetAtack()const override { return m_Atack; }

		//防御力を取得する
		virtual const int& GetDefence()const override { return m_Defence; }

		//スペシャルポイントを取得する
		virtual const int& GetSpecial()const override { return m_Special; }

		//レベルを設定する
		virtual const int& GetLevel()const override { return m_Level; };

		//次のレベルまでの経験値を設定する
		virtual const int& GetNextLevelPoint()const override { return m_LevelPoint; };

		//経験値を計算する際の補正値を設定する
		virtual const int& GetCompensation()const override { return m_compensation; };

		//HPの最大値を設定する
		virtual const int& GetMaxHP()const override { return m_MaxHP; };

		//MPの最大値を設定する
		virtual const int& GetMaxMP()const override { return m_MaxMP; };

		//攻撃力の最大値を設定する
		virtual const int& GetMaxAtack()const override { return m_MaxAtack; };

		//防御力の最大値を設定する
		virtual const int& GetMaxDefence()const override { return m_MaxDefence; };

		//スペシャルポイントの最大値を設定する
		virtual const int& GetMaxSpecialPoint()const override { return m_MaxSpecial; };

		//成長限界を設定する
		virtual const int& GetMaxLevel()const override { return m_MaxLevel; };

		//経験値の最大値を設定する
		virtual const int& GetMaxNextLevelPoint()const override { return m_MaxNextLevelPoint; };

		//自身のインスタンスを取得する
		virtual IStatusBase* GetStatus()override { return this; };

		//-----------------------ゲッター----------------------------------------------------------------//

		public:
			//ステータスの情報を追加する
			static void Add(const std::vector<StatusImporter>& Importer)
			{
				for (const StatusImporter& importer : Importer)
				{
					//グループ名を取得する
					std::string grop = importer.GetGrop();

					//キャラのレベルを取得する
					int level = importer.GetLevel();

					//インスタンスを生成する
					s_statusGrop[grop][level] = std::make_unique<StatusImporter>(importer);
				}
			}

			//アニメーションコンテナを取得する
			static const StatusMap& GetStatusMap() { return s_statusGrop; }

			//コンテナの中のリストを取得する
			static const StatusList& GetStatusList(const std::string& grop)
			{
				return s_statusGrop[grop];
			}

			//指定したレベルのステータスを取得する
			static StatusImporter* FindStatus(const std::string& grop, const int& level)
			{
				return s_statusGrop[grop][level].get();
			}
	};
}

