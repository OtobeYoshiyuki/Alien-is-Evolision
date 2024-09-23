#pragma once
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "OtobeLib/FrameWork/Collision/CollisionTemplate/CollisionBase.h"
#include "OtobeLib/Texture/Animation/Animation.h"
#include "GameObject/MoveState/AutoMoveState/AutoMoveState.h"
#include "GameObject/MoveState/MoveState.h"
#include "TemplateState.h"
#include "GameObject/Enemy/ENEMYSTATE.h"
#include "OtobeLib/Status/StatusBase/StatusBase.h"
#include "GameObject/Gage/HPGage/HPGage.h"
#include "GameObject/FixdTurret/FixedTurret.h"
#include <list>
#include <vector>
#include "Effect/Experience/Experience.h"
#include "Effect/MasicEffectManager/MasicEffectManager.h"
#include "GameObject/Item/Heal/Heal.h"
#include "OtobeLib/Texture/TextureBase/ColorChange.h"

class SuperEnemy :public OtobeLib::TextureBase, OtobeLib::CollisionBase<OtobeLib::ITextureBase>,
	OtobeLib::Animation,StateArray<ENEMYSTATE,int,COLOR_CHANGE>, OtobeLib::StatusBase
{
public:
	//固定砲台の数
	static const int TURRET_NUM = 2;

	//思考時間
	static const float THINK_TIME;

	//成長限界
	static const int MAX_LEVEL = 20;

	//敵の強さ(経験値に影響する)
	static const float STRENGTH;

private:
	//HPGage
	HPGage* m_HpGage;

	//HPGageの裏面
	OtobeLib::ITextureBase* m_HpBack;

	//固定砲台のリスト
	std::vector<std::unique_ptr<FixedTurret>> m_TurrentList;

	//思考時間
	float					  m_ThinkTime;

	//経験値(追尾)
	Experience* m_experience;

	//MasicPower用のエフェクト
	std::unique_ptr<MasicEffectManager>	m_masicEffectManager;

	//回復アイテム
	Heal* m_healItem;

	//フレーム数のカウント
	int m_frameCount;

	//ダメージのチェック用のフラグ
	bool							m_damageCheck;

public:
	//コンストラクタ
	SuperEnemy(OtobeLib::ITextureBase* texture);

	//デストラクタ
	~SuperEnemy()override {};

	//初期化
	void Initialize()override;

	//更新
	void Update()override;

	//描画
	void Render()override;

	//終了
	void Finalize()override;

	//----------------------セッター------------------------------------------------------------//

	//テクスチャを設定する
	void SetTexture(ITextureBase* texture)override { TextureBase::SetTexture(texture); };

	//ハンドルを設定する
	void SetHandle(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& handle)override { TextureBase::SetHandle(handle); };

	//座標を設定する
	void SetPosition(const DirectX::SimpleMath::Vector3& pos)override { TextureBase::SetPosition(pos); };

	//ベクトルを設定する
	void SetVelocity(const DirectX::SimpleMath::Vector3& vel)override { TextureBase::SetVelocity(vel); };

	//切り出す画像の情報を設定する
	void SetRect(const RECT& rect)override { TextureBase::SetRect(rect); };

	//画像の色情報を設定する
	void SetColor(const DirectX::SimpleMath::Color& color)override { TextureBase::SetColor(color); };
	void SetColor(const DirectX::XMVECTORF32& color)override { TextureBase::SetColor(color); }

	//画像の回転角度を設定する
	void SetAngle(const float& angle)override { TextureBase::SetAngle(angle); };

	//画像の拡大倍率を設定する
	void SetScale(const DirectX::SimpleMath::Vector3& scale)override { TextureBase::SetScale(scale); };

	//画像の状態を設定する
	void SetState(const ITextureBase::STATE& state)override { TextureBase::SetState(state); };

	//画像の名前を設定する
	void SetName(const std::string& name)override { TextureBase::SetName(name); };

	//画像の描画順を設定する
	void SetDrawOrder(const int& order)override { TextureBase::SetDrawOrder(order); };

	//画像のパスを設定する
	void SetPath(const std::wstring& path)override { TextureBase::SetPath(path); };

	//アニメーションの情報を設定する
	void SetAnimation(const std::string& name, const RECT& rect)override { Animation::SetAnimation(name, rect); }

	//次に再生するアニメーションの情報を設定する
	void SetNextAnimation(const std::string& now, const std::string& next)override { Animation::SetNextAnimation(now, next); }

	//アニメーションの終了時間を設定する
	void SetEndTime(const std::string& name, const int& time)override { Animation::SetEndTime(name, time); }

	//現在のアニメーションを設定する
	void SetNowAnimation(const RECT& rect)override { Animation::SetNowAnimation(rect); }

	//現在のアニメーションの情報を設定する
	void SetNowInfo(const std::string& info)override { Animation::SetNowInfo(info); }

	//アニメーションのループ情報を設定する
	void SetLoopInfo(const std::string& loop)override { Animation::SetLoopInfo(loop); }

	//アニメーションの更新フラグを設定する
	void SetAniFlag(const bool& flag)override { Animation::SetAniFlag(flag); }

	//アニメーションのループフラグを設定する
	void SetLoopFlag(const bool& flag)override { Animation::SetLoopFlag(flag); }

	//アニメーションのパラメーターを引き継ぐ
	void SetNewAnimation(Animation* ani)override { Animation::SetNewAnimation(ani); }

	//アニメーションのタイマーを初期化する
	void SetInitTimer(const int& timer)override { Animation::SetInitTimer(timer); }

	//ステータスを設定する
	void SetStatus(const int& hp, const int& mp, const int& atack, const int& defence,
		const int& special, const int& level, const int& next)override
	{
		StatusBase::SetStatus(hp, mp, atack, defence, special, level, next);
	}

	//ステータスの最大値を設定する
	virtual void SetMaxStatus(const int& maxhp, const int& maxmp, const int& maxatack,
		const int& maxdefence, const int& maxspecial, const int& maxlevel, const int& maxnext)override
	{
		StatusBase::SetMaxStatus(maxhp, maxmp, maxatack, maxdefence, maxspecial, maxlevel, maxnext);
	}

	//HPを設定する
	virtual void SetHP(const int& hp)override { StatusBase::SetHP(hp); }

	//MPを設定する
	virtual void SetMP(const int& mp)override { StatusBase::SetMP(mp); }

	//攻撃力を設定する
	virtual void SetAtack(const int& atack)override { StatusBase::SetAtack(atack); }

	//防御力を設定する
	virtual void SetDefence(const int& defence)override { StatusBase::SetDefence(defence); }

	//スペシャルポイントを設定する
	virtual void SetSpecial(const int& special)override { StatusBase::SetSpecial(special); }

	//レベルを設定する
	virtual void SetLevel(const int& level) override { StatusBase::SetLevel(level); };

	//成長限界を設定する
	virtual void SetMaxLevel(const int& max)override { StatusBase::SetMaxLevel(max); };

	//次のレベルまでの経験値を設定する
	virtual void SetNextLevelPoint(const int& point)override { StatusBase::SetNextLevelPoint(point); };

	//----------------セッター-----------------------------------------------------------//

	//----------------ゲッター------------------------------------------------------------//

	//ハンドルを取得する
	const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetHandle()const override { return TextureBase::GetHandle(); };

	//座標を取得する
	const DirectX::SimpleMath::Vector3& GetPosition()const override { return TextureBase::GetPosition(); };

	//ベクトルを取得する
	const DirectX::SimpleMath::Vector3& GetVelocity()const override { return TextureBase::GetVelocity(); };

	//切り出す画像の情報を取得する
	const RECT& GetRect()const override { return TextureBase::GetRect(); };

	//画像の色情報を取得する
	const DirectX::SimpleMath::Color& GetColor()const override { return TextureBase::GetColor(); };

	//画像の回転角度を取得する
	const float& GetAngle()const override { return TextureBase::GetAngle(); };

	//画像の拡大倍率を取得する
	const DirectX::SimpleMath::Vector3& GetScale()const override { return TextureBase::GetScale(); };

	//画像の状態を取得する
	const ITextureBase::STATE& GetState()const override { return TextureBase::GetState(); };

	//画像の名前を取得する
	const std::string& GetName()const override { return TextureBase::GetName(); };

	//画像の描画順を取得する
	const int& GetDrawOrder()const override { return TextureBase::GetDrawOrder(); };

	//画像のパスを設定する
	const std::wstring& GetPath()const override { return TextureBase::GetPath(); };

	//アニメーションの情報を取得する
	const RECT& GetAnimation(const std::string& name)override { return Animation::GetAnimation(name); }

	//次に再生するアニメーションの情報を取得する
	const std::string& GetNextAnimation(const std::string& now)override { return Animation::GetNextAnimation(now); }

	//アニメーションの終了時間を設定する
	const int& GetEndTime(const std::string& name)override { return Animation::GetEndTime(name); }

	//現在のアニメーションを取得する
	const RECT& GetNowAnimation()const override { return Animation::GetNowAnimation(); }

	//現在のアニメーションの情報を設定する
	const std::string& GetNowInfo()const override { return Animation::GetNowInfo(); }

	//アニメーションのループ情報を設定する
	const std::string& GetLoopInfo()const override { return Animation::GetLoopInfo(); }

	//アニメーションの更新フラグを取得する
	const bool& GetAniFlag()const override { return Animation::GetAniFlag(); }

	//アニメーションのループフラグを取得する
	const bool& GetLoopFlag()const override { return Animation::GetLoopFlag(); }

	//アニメーションカウントを取得する
	const int& GetAniCount()const override { return Animation::GetAniCount(); }

	//HPを取得する
	const int& GetHP()const override { return StatusBase::GetHP(); }

	//MPを取得する
	virtual const int& GetMP()const override { return StatusBase::GetMP(); }

	//攻撃力を取得する
	virtual const int& GetAtack()const override { return StatusBase::GetAtack(); }

	//防御力を取得する
	virtual const int& GetDefence()const override { return StatusBase::GetDefence(); }

	//スペシャルポイントを取得する
	virtual const int& GetSpecial()const override { return StatusBase::GetSpecial(); }

	//レベルを設定する
	virtual const int& GetLevel()const override { return StatusBase::GetLevel(); };

	//成長限界を設定する
	virtual const int& GetMaxLevel()const override { return StatusBase::GetMaxLevel(); }

	//次のレベルまでの経験値を設定する
	virtual const int& GetNextLevelPoint()const { return StatusBase::GetNextLevelPoint(); }

	//自身のステータスを取得する
	virtual IStatusBase* GetStatus()override { return StatusBase::GetStatus(); }

	//----------------ゲッター-------------------------------------------------------------//

	//---------------------------当たり判定用の関数---------------------------------------------------//

	//他のオブジェクトと当たった時に呼ばれる関数
	void OnCollisionEnter(ITextureBase* obj)override;

	//他のオブジェクトと当たっていないときに呼ばれる関数
	void NonCollisionEnter(ITextureBase* obj)override;

	//自身の当たり判定を取得する
	ICollisionBase<ITextureBase>* GetThisCollision() { return CollisionBase::GetThisCollision(); };

	//当たったかどうかのフラグを取得する
	const bool& GetCheck()const override { return CollisionBase::GetCheck(); };

	//---------------------------当たり判定用の関数---------------------------------------------------//

public:
	//現在の状態を設定する
	void AddData1(const ENEMYSTATE& state)override { StateArray::AddData1(state); };
	void AddData2(const int& state)override { StateArray::AddData2(state); };
	void AddData3(const COLOR_CHANGE& state)override { StateArray::AddData3(state); };

	//現在の状態を取得する
	const ENEMYSTATE& GetNowData1()const override { return StateArray::GetNowData1(); };
	const int& GetNowData2()const override { return StateArray::GetNowData2(); };
	const COLOR_CHANGE& GetNowData3()const override { return StateArray::GetNowData3(); };

	//ひとつ前の状態を取得する
	const ENEMYSTATE& GetPreData1()const override { return StateArray::GetPreData1(); };
	const int& GetPreData2()const override { return StateArray::GetPreData2(); };
	const COLOR_CHANGE& GetPreData3()const override { return StateArray::GetPreData3(); };


	//ダメージを受ける
	void OnDamage(const int& atack, const int& defence, const float correction = 1.0f);
private:
	//ゲージの更新処理
	void UpdateGage();

	//ステートの更新処理
	void UpdateState();

	//State_Noneの処理
	void State_None();

	//State_Startの処理
	void State_Start();

	//State_Atackingの処理
	void State_Atacking();

	//State_AtackResultの処理
	void State_AtackResult();

	//State_Nextの処理
	void State_Next();

	//State_DeadStartの処理
	void State_DeadStart();

	//State_Deadtingの処理
	void State_Deadting();

	//State_Deadの処理
	void State_Dead();

	//回復アイテムのドロップの処理
	void HealItemDrop();

	//Playerに当たった時の処理
	void OnCollision_Player(ITextureBase* obj);

	//NeedleOverAllに当たったときの処理
	void OnCollision_NeedleOverAll(ITextureBase* obj);

	//Resurrectionに当たったときの処理
	void OnCollision_Resurrection(ITextureBase* obj);

	//VirusBeamに当たったときの処理
	void OnCollision_VirusBeam(ITextureBase* obj);

	//NormalBulletに当たった時の処理
	void OnCollision_NormalBullet(ITextureBase* obj);

public:
	//固定砲台のリストを取得する
	std::vector<std::unique_ptr<FixedTurret>>& GetTurretList() { return m_TurrentList; }

	//MasicEffectManagerを取得する
	MasicEffectManager* GetMasicEffectManager() { return m_masicEffectManager.get(); }
};

