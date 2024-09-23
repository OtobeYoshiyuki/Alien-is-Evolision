#pragma once

namespace OtobeLib
{
	class Score
	{
	private:
		//スコア
		float		m_score;

		//スコアの貯蓄
		float		m_saving;

		//スコアの1秒当たりの加算量
		float	m_frameSave;

		//スコアの加算量
		int		m_add;

		//音の停止を無視するフラグ
		bool	m_soundCheck;

	public:
		//コンストラクタ
		Score();

		//デストラクタ
		virtual ~Score() = default;

		//初期化
		void Initialize();

		//更新
		void Update();

		//スコアを設定する
		void SetScore(const float& score) { m_score = score; }

		//スコアの貯蓄を設定する
		void SetSaving(const float& save) { m_saving = save; }

		//スコアを加算していく
		void AddScore(const float& score) { m_saving += score; }

		//秒単位の加速度を設定する
		void SetFrameSave(const float& score, const float& frame) { m_frameSave = score / frame; }

		//スコアの加算量を設定する
		void SetAdd(const int& add) { m_add = add; }

		//音の停止を無視するフラグを設定する
		void SetSoundCheck(const bool& flag) { m_soundCheck = flag; }

		//スコアを取得する
		const float& GetScore()const { return m_score; }

		//スコアの貯蓄を取得する
		const float& GetSaving()const { return m_saving; }
	};
}

