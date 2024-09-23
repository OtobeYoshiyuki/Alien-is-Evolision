#pragma once
#include "TemplateState.h"
#include "ReNeedle.h"
#include <list>
#include "GameObject/Technique/Needle/Needle.h"

class Resurrection :public State<RENEEDLE>
{
public:
	//長距離用のNeedleの数
	static const int NEEDLESHORT_NUM = 13;

	//待機から攻撃に移るまでの時間
	static const float MOVETIME;

	//ゲージの回復量(コマンド技)
	static const int TECHNIQUE_HEAL_COMMAND = 40;

	//ゲージの回復量(必殺技)
	static const int TECHNIQUE_HEAL_SPECIAL = 50;

	//取得スコア
	static const int BASE_SCORE = 500;

	//コマンド技のゲージの消費量
	static const int GAGE_COST = 90;
private:
	//広範囲用の針のコンテナ
	std::vector<Needle*> m_NeedleList;

	//更新関数のフラグ
	bool				m_Active;

	//タイマー
	float				m_Timer;
public:
	//コンストラクタ
	Resurrection();

	//デストラクタ
	~Resurrection()override {};

	//初期化
	void Initialize();

	//更新
	void Update();

public:
	//現在の状態を設定する
	void SetNowState(const RENEEDLE& now)override { State::SetNowState(now); };

	//現在の状態を取得する
	const RENEEDLE& GetNowState()const override { return State::GetNowState(); };

	//ひとつ前の状態を取得する
	const RENEEDLE& GetPreState()const override { return State::GetPreState(); };

public:

	//更新関数のフラグを設定する
	void SetActive(const bool& active) { m_Active = active; }

	//更新関数のフラグを取得する
	const bool& GetActive()const { return m_Active; }

	//ニードルのリストを取得する
	std::vector<Needle*>& GetNeedleList() { return m_NeedleList; }

private:

	//待機中のステートの処理
	void State_Little();

	//攻撃中のステートの処理
	void State_Atack();

	//終了のステートの処理
	void State_End();
};

