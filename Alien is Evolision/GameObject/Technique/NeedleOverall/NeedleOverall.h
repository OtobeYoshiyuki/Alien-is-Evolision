#pragma once
#include "TemplateState.h"
#include "NeedleAction.h"
#include <vector>
#include "GameObject/Technique/Needle/Needle.h"

class NeedleOverall:public State<NEEDLEACTION>
{
public:
	//長距離用のNeedleの数
	static const int NEEDLERANGE_NUM = 13;

	//ゲージの回復量(コマンド技)
	static const int TECHNIQUE_HEAL_COMMAND = 45;

	//ゲージの回復量(必殺技)
	static const int TECHNIQUE_HEAL_SPECIAL = 60;

	//取得スコア
	static const int BASE_SCORE = 450;

	//コマンド技のゲージの消費量
	static const int GAGE_COST = 100;
private:
	//広範囲用の針のコンテナ
	std::vector<Needle*> m_NeedleList;

	//更新関数フラグ
	bool				m_Active;
public:
	//コンストラクタ
	NeedleOverall();

	//デストラクタ
	~NeedleOverall(){};

	//初期化
	void Initialize();

	//更新
	void Update();

private:

	//発射準備のステート
	void State_Mope();

	//とげキャノン発射のステート
	void State_Release();

	//とげキャノンのリロード
	void State_Reload();

public:

	//現在の状態を設定する
	void SetNowState(const NEEDLEACTION& now)override { State::SetNowState(now); };

	//現在の状態を取得する
	const NEEDLEACTION& GetNowState()const override { return State::GetNowState(); };

	//ひとつ前の状態を取得する
	const NEEDLEACTION& GetPreState()const override { return State::GetPreState(); };

public:

	//広範囲用の針のコンテナを取得する
	std::vector<Needle*>& GetNeedleList() { return m_NeedleList; }

	//更新関数フラグを設定する
	void SetActive(const bool& active) { m_Active = active; }

	//更新関数フラグを取得する
	const bool& GetActive()const { return m_Active; }
};

