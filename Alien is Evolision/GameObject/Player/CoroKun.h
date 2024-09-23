#pragma once
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "OtobeLib/FrameWork/Collision/CollisionTemplate/CollisionBase.h"
#include "GameObject/MoveState/MoveState.h"
#include <list>
#include <vector>
#include <functional>
#include "GameObject/MoveState/DamageState/DamageState.h"
#include "GameObject/MoveState/WalkState/WalkState.h"
#include "GameObject/MoveState/AccelState/AccelState.h"
#include "GameObject/Technique/BodyBlow/BodyBlow.h"
#include "GameObject/Technique/VirusBeam/VirusBeam.h"
#include "OtobeLib/Texture/Animation/Animation.h"
#include "GameObject/Technique/NeedleOverall/NeedleOverall.h"
#include "GameObject/Technique/Resurrection/Resurrection.h"
#include "GameObject/Player/NormalTechnique/NormalTechnique.h"
#include "OtobeLib/Status/StatusBase/StatusBase.h"
#include "GameObject/Gage/HPGage/HPGage.h"
#include "GameObject/Gage/ExperienceGage/ExperienceGage.h"
#include "GameObject/Gage/TechniqueGage/TechniqueGage.h"
#include "GameObject/Gage/DeathblowGage/DeathblowGage.h"
#include "TemplateState.h"
#include "GameObject/DamageUI/FadeState.h"
#include "PLAYERANIME.h"
#include "OtobeLib/Texture/TextureBase/ColorChange.h"
#include "OtobeLib/Texture/FadeTexture/FadeTexture.h"

class CoroKun:public OtobeLib::TextureBase,OtobeLib::CollisionBase<OtobeLib::ITextureBase>,
	OtobeLib::Animation,OtobeLib::StatusBase,
	StateArray<FADE_STATE,PLAYERMOVE,COLOR_CHANGE>
{
public:
	//行ける範囲の最大値
	static const DirectX::SimpleMath::Vector3 MAX_LIMIT;

	//行ける範囲の最小値
	static const DirectX::SimpleMath::Vector3 MIN_LIMIT;

	//1フレームに加算するスピード
	static const float SPEED;

	//Easing関数に使用するトータルタイム
	static const float EASING_TOTALTIME;

	//Easing関数に使用する最大値
	static const float EASING_MAX;

	//Easing関数に使用する最小値
	static const float EASING_MIN;

	//成長限界
	static const int MAX_LEVEL = 20;

	//初期レベル
	static const int MIN_LEVEL = 1;

	//リバースアビリティの持続時間
	static const float DEATHBLOW_ABILITY_SUSTAIN_TIME;

	//背面の点滅の開始時間
	static const float BACKTEXTURE_BLINK_START;

private:
	//HPゲージ
	HPGage* m_HPGage;

	//MPゲージ
	TechniqueGage* m_mpGage;

	//Specialゲージ
	DeathblowGage* m_specialGage;

	//経験値ゲージ
	ExperienceGage* m_expGage;

	//背面の画像
	TextureBase* m_backTexture;

	//汗の画像
	TextureBase* m_sweatTexture;

	//イージング関数のカウント用の変数
	float m_easingTime;

	//経過時間
	float m_timer;

	//時間の更新
	bool  m_timerUp;

	//フレーム数のカウント
	int m_frameCount;

	//プレイヤーのステートのリスト
	std::vector<MoveState*>		m_StateList;

	//BodyBlow
	BodyBlow*					m_BodyBlow;

	//VirusBeam
	VirusBeam*					m_VirusBeam;

	//NormalTechnique
	std::unique_ptr<NormalTechnique>	m_Normal;

	//NeedleOverAll
	std::unique_ptr<NeedleOverall>				m_NeedleOverAll;

	//Resurrection
	std::unique_ptr<Resurrection>				m_Resurrection;

	//WalkState
	std::unique_ptr<WalkState>		m_WalkState;

	//DamageState
	std::unique_ptr<DamageState>	m_DamageState;

	//AccelState
	std::unique_ptr<AccelState>		m_AccelState;

	//浮遊後の座標
	DirectX::SimpleMath::Vector3	m_warpPos;

	//ダメージのチェック用のフラグ
	bool							m_damageCheck;

	//体当たりのコマンド欄のエフェクト
	ITextureBase*					m_fadeBodyBlow;

	//ビームのコマンド欄のエフェクト
	ITextureBase*					m_fadeBeam;

	//ニードルインクジェクションのコマンド欄のエフェクト
	ITextureBase*					m_fadeNeedleOverAll;

	//リザレクションのコマンド欄のエフェクト
	ITextureBase*					m_fadeResurection;

	//必殺技のUIのエフェクト
	OtobeLib::FadeTexture*			m_fadeSpecial;

public:
	//コンストラクタ
	CoroKun(OtobeLib::ITextureBase* texture);

	//デストラクタ
	~CoroKun()override {};

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
	void SetTexture(ITextureBase* texture)override { TextureBase::SetTexture(texture); }

	//ハンドルを設定する
	void SetHandle(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& handle)override { TextureBase::SetHandle(handle); }

	//座標を設定する
	void SetPosition(const DirectX::SimpleMath::Vector3& pos)override { TextureBase::SetPosition(pos); }

	//ベクトルを設定する
	void SetVelocity(const DirectX::SimpleMath::Vector3& vel)override { TextureBase::SetVelocity(vel); }

	//切り出す画像の情報を設定する
	void SetRect(const RECT& rect)override { TextureBase::SetRect(rect); }

	//画像の色情報を設定する
	void SetColor(const DirectX::SimpleMath::Color& color)override { TextureBase::SetColor(color); }
	void SetColor(const DirectX::XMVECTORF32& color)override { TextureBase::SetColor(color); }

	//画像の回転角度を設定する
	void SetAngle(const float& angle)override { TextureBase::SetAngle(angle); }

	//画像の拡大倍率を設定する
	void SetScale(const DirectX::SimpleMath::Vector3& scale)override { TextureBase::SetScale(scale); }

	//画像の状態を設定する
	void SetState(const ITextureBase::STATE& state)override { TextureBase::SetState(state); }

	//画像の名前を設定する
	void SetName(const std::string& name)override { TextureBase::SetName(name); }

	//画像の描画順を設定する
	void SetDrawOrder(const int& order)override { TextureBase::SetDrawOrder(order); }

	//画像のパスを設定する
	void SetPath(const std::wstring& path)override { TextureBase::SetPath(path); }

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

	//ステータスを設定する
	void SetStatus(const int& hp, const int& mp, const int& atack, const int& defence,
		const int& special, const int& level, const int& next)override
		{	StatusBase::SetStatus(hp,mp,atack,defence,special,level,next);	}

	//ステータスの最大値を設定する
	virtual void SetMaxStatus(const int& maxhp, const int& maxmp, const int& maxatack,
		const int& maxdefence, const int& maxspecial, const int& maxlevel,const int& maxnext)override
	{
		StatusBase::SetMaxStatus(maxhp, maxmp, maxatack, maxdefence, maxspecial, maxlevel,maxnext);
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

	//HPの最大値を設定する
	virtual void SetMaxHP(const int& max)override { StatusBase::SetMaxHP(max); }

	//次のレベルまでの経験値を設定する
	virtual void SetNextLevelPoint(const int& point)override { StatusBase::SetNextLevelPoint(point); };

	//----------------セッター-----------------------------------------------------------//

	//----------------ゲッター------------------------------------------------------------//

	//ハンドルを取得する
	const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetHandle()const override { return TextureBase::GetHandle(); }

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

	//自身のアニメーションを取得する
	Animation* GetThisAnimation()override { return Animation::GetThisAnimation(); }

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

	//レベルを取得する
	virtual const int& GetLevel()const override { return StatusBase::GetLevel(); };

	//成長限界を取得する
	virtual const int& GetMaxLevel()const override { return StatusBase::GetMaxLevel(); };

	//HPの最大値を取得する
	virtual const int& GetMaxHP()const override { return StatusBase::GetMaxHP(); }

	//MPの最大値を取得する
	virtual const int& GetMaxMP()const override { return StatusBase::GetMaxMP(); }

	//Specialの最大値を取得する
	virtual const int& GetMaxSpecialPoint()const override { return StatusBase::GetMaxSpecialPoint(); }

	//次のレベルまでの経験値を設定する
	virtual const int& GetNextLevelPoint()const { return StatusBase::GetNextLevelPoint(); };

	//経験値の最大値を設定する
	virtual const int& GetMaxNextLevelPoint()const { return StatusBase::GetMaxNextLevelPoint(); }

	//自身のステータスを取得する
	virtual IStatusBase* GetStatus()override { return StatusBase::GetStatus(); };


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
	void AddData1(const FADE_STATE& state)override { StateArray::AddData1(state); };
	void AddData2(const PLAYERMOVE& state)override { StateArray::AddData2(state); };
	void AddData3(const COLOR_CHANGE& state)override { StateArray::AddData3(state); };

	//現在の状態を取得する
	const FADE_STATE& GetNowData1()const override { return StateArray::GetNowData1(); };
	const PLAYERMOVE& GetNowData2()const override { return StateArray::GetNowData2(); };
	const COLOR_CHANGE& GetNowData3()const override { return StateArray::GetNowData3(); };

	//ひとつ前の状態を取得する
	const FADE_STATE& GetPreData1()const override { return StateArray::GetPreData1(); };
	const PLAYERMOVE& GetPreData2()const override { return StateArray::GetPreData2(); };
	const COLOR_CHANGE& GetPreData3()const override { return StateArray::GetPreData3(); };

private:
	//プレイヤーのステートの処理
	void PlayerState();

	//動く
	void Move();

	//死亡時の処理
	void SweatAnime();

	//回転と縮小の処理
	void RotScale();

	//死亡アニメーション開始の処理
	void DeadAnimeStart();

	//死亡アニメーション終了の処理
	void DeadAnimeResult();

	//ステートの更新
	void UpdateState();

	//技の更新処理
	void UpdateTechnique();

	//イージング関数の更新処理
	void UpdateEasing();

	//背面のフェードの更新処理
	void UpdateBackFade();

	//Hpゲージの更新処理
	void UpdateHpGage();

	//コマンドエフェクトの更新処理
	void UpdateCommandEffect();

	//指定したコマンドエフェクトを再生
	void PlayCommandEffect(ITextureBase* command, const int& cost);

	//必殺技エフェクトを再生
	void PlaySpecialEffect(OtobeLib::FadeTexture* special, const int& cost,const DirectX::XMVECTORF32 color);

	//床に当たった時
	void OnCollisionFloor(ITextureBase* floor);

	//壁に当たった時
	void OnCollisionWall(ITextureBase* wall);

	//泡に当たったとき
	void OnCollisionBabul(ITextureBase* babul);

	//反転台に当たったとき
	void OnCollisionRebound(ITextureBase* rebound);

	//起爆スイッチに当たったとき
	void OnCollisionSwitch(ITextureBase* switchi);

	//落とし穴に当たったとき
	void OnCollisionFoleFloor(ITextureBase* fole);

	//HyperEnemyに当たったとき
	void OnCollisionHyperEnemy(ITextureBase* hyene);

	//SuperEnemyに当たったとき
	void OnCollision_SuperEnemy(ITextureBase* super);

	//NormalEnemyに当たったとき
	void OnCollision_NormalEnemy(ITextureBase* normal);

	//UIに当たった時
	void OnCollisionWhiteBack(ITextureBase* whiteback);

	//HyperEnemyの泡に当たった時
	void OnCollisionHyperBabble(ITextureBase* highbabble);

	//SuperEnemyの泡に当たったとき
	void OnCollision_SuperBabble(ITextureBase* superbabble);

	//ゴールに当たったとき
	void OnCollision_Goal(ITextureBase* goal);

	//角度を変更する
	void InputAngle();

	//技を生成する
	void CreateTechnique();

public:
	//ダメージを受ける
	void OnDamage(const int& atack,const int& defence,const float correction = 1.0f);

	//コマンド技を使用する
	void OnCommand(const int& cost);

	//必殺技ゲージを使用する
	void OnSpesial(const int& cost);

	//コマンドゲージを回復させる
	void OnHealCommand(int& heal);

	//必殺技ゲージを回復させる
	void OnHealSpecial(int& heal);

	//HPゲージを回復させる
	void OnHealHp(int& heal);

	//経験値ゲージを回復させる
	void OnHealExp(int& heal);

	//終了時の処理
	void Finish(const PLAYERMOVE& state, const int& sound);

	//ダメージフラグを設定する
	void SetDamageCheck(const bool& check) { m_damageCheck = check; }

private:
	//必殺技を実行
	void OnExeDeathBlowHyperEnemy();
	void OnExeDeathBlowSuperEnemy();
	void OnExeDeathBlowNormalEnemy();
public:
	//全ての技が使用されているかどうか調べる
	const bool ALLTechniqueExe();

public:
	//WalkStateを取得する
	std::unique_ptr<WalkState>& GetWalkState() { return m_WalkState; }

	//BodyBlowを取得する
	BodyBlow* GetBodyBlow() { return m_BodyBlow; }

	//ExperienceGageを取得する
	ExperienceGage* GetExperienceGage() { return m_expGage; }

	//HPGageを取得する
	HPGage* GetHpGage() { return m_HPGage; }

	//TechniqueGageを取得する
	TechniqueGage* GetTechniqueGage() { return m_mpGage; }

	//DeathblowGageを取得する
	DeathblowGage* GetDeathblowGage() { return m_specialGage; }

	//必殺技のUIを取得する
	OtobeLib::FadeTexture* GetSpecialUI() { return m_fadeSpecial; }
};
