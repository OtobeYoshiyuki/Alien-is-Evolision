#pragma once
#include <vector>
#include "Effect/SoapBabble/SoapBabble.h"
#include "TemplateState.h"
#include "OtobeLib/FrameWork/Owner/Owner.h"
#include "GameObject/MoveState/MoveState.h"
#include "GameObject/MoveState/JumpState/JumpState.h"
#include "GameObject/MoveState/AutoMoveState/AutoMoveState.h"

//前方宣言（相互インクルードするため）
class SuperEnemy;

//固定砲台クラス
class FixedTurret:public OtobeLib::Owner<SuperEnemy>
{
public:
	//子供の数
	static const int CHILD_NUM = 10;
	//生存時間
	static const float SURVIVAL_TIME;
private:
	//全ての泡のコンテナ
	std::vector<SoapBabble*>	m_allBabble;

	//親の泡
	SoapBabble*					m_parentBabble;

	//子供の泡
	std::vector<SoapBabble*>	m_childBabbles;

	//MoveStateのコンテナ
	std::vector<MoveState*>		m_StateList;

	//JumpState
	std::unique_ptr<JumpState>	m_JumpState;

	//AutoMoveState
	std::unique_ptr<AutoMoveState> m_AutoMoveState;

	//泡の生存時間
	float							m_SurvivalTime;

	//弾が発射されたフラグ
	bool							m_bulletFiring;
public:
	//コンストラクタ
	FixedTurret();

	//デストラクタ
	~FixedTurret() = default;

	//初期化
	void Initialize();

	//更新処理
	void Update();

public:
	//所有者のポインタを設定する
	void SetOwner(SuperEnemy* enemy)override { Owner::SetOwner(enemy); }

	//所有者のポインタを取得する
	SuperEnemy* GetOwner()override { return Owner::GetOwner(); }

public:
	//全ての泡を取得する
	std::vector<SoapBabble*>& GetBabbleList() { return m_allBabble; }

	//親の泡を取得する
	SoapBabble* GetParentBabble() { return m_parentBabble; }

	//子供の泡を取得する
	std::vector<SoapBabble*>& GetChildBabble() { return m_childBabbles; }

	//JumpStateを取得する
	JumpState* GetJumpState() { return m_JumpState.get(); }

	//AutoMoveStateを取得する
	AutoMoveState* GetAutoMoveState() { return m_AutoMoveState.get(); }

public:
	//乱数を振る
	void RandShake(SoapBabble* child);

	//発射済みのフラグを設定する
	void SetFring(const bool& flag) { m_bulletFiring = flag; }

	//情報を初期化する
	void InitTurret();

private:
	//ステートの更新処理
	void UpdateState();

	//泡の更新処理
	void UpdateBabble();
};

