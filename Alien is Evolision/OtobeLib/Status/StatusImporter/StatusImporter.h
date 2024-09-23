#pragma once
#include <string>
#include <vector>

namespace OtobeLib
{
	//ステータスの情報を送信するクラス
	class StatusImporter
	{
	public:
		enum class STATUSINFO :int
		{
			GROP,//グループ名
			LEVEL,//レベル
			HP,//HP
			MP,//MP
			ATACK,//攻撃力
			DEFENCE,//防御力
			SPECIAL,//スペシャルポイント
			NEXT,//次のレベルまでの経験値
		};

	private:
		//ステータスグループの名前
		std::string			m_gropName;
		//レベル
		int					m_level;
		//HP
		int					m_hp;
		//MP
		int					m_mp;
		//攻撃力
		int					m_atack;
		//防御力
		int					m_defence;
		//スペシャルポイント
		int					m_special;
		//次のレベルまで
		int					m_next;

	public:
		//コンストラクタ
		StatusImporter();

		//コンストラクタ(引数アリ)
		StatusImporter(const StatusImporter& importer);

		//デストラクタ
		~StatusImporter() = default;

//----------------------------------------セッター-----------------------------------------------------//

	public:
		//グループ名を設定する
		void SetGrop(const std::string& grop) { m_gropName = grop; }

		//レベルを設定する
		void SetLevel(const int& level) { m_level = level; }

		//HPを設定する
		void SetHp(const int& hp) { m_hp = hp; }

		//MPを設定する
		void SetMp(const int& mp) { m_mp = mp; }

		//攻撃力を設定する
		void SetAtack(const int& atack) { m_atack = atack; }

		//防御力を設定する
		void SetDefence(const int& defence) { m_defence = defence; }

		//スペシャルポイントを設定する
		void SetSpecial(const int& special) { m_special = special; }

		//次のレベルまでの経験値を設定する
		void SetNextLevelPoint(const int& next) { m_next = next; }

//----------------------------------------セッター-----------------------------------------------------//


//----------------------------------------ゲッター-----------------------------------------------------//

	public:
		//グループ名を取得する
		const std::string& GetGrop()const { return m_gropName; }

		//レベルを取得する
		const int& GetLevel()const { return m_level; }

		//HPを取得する
		const int& GetHp()const { return m_hp; }

		//MPを取得する
		const int& GetMp()const { return m_mp; }

		//攻撃力を取得する
		const int& GetAtack()const { return m_atack; }

		//防御力を取得する
		const int& GetDefence()const { return m_defence; }

		//スペシャルポイントを取得する
		const int& GetSpecial()const { return m_special; }

		//次のレベルまでの経験値を取得する
		const int& GetNextLevelPoint()const { return m_next; }

//----------------------------------------ゲッター-----------------------------------------------------//

	public:
		//データをアニメーション用に変換する
		static const std::vector<StatusImporter> Cast(const std::vector<std::vector<std::string>>& Input);
	};
	
}
