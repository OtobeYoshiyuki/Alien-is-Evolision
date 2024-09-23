#pragma once
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "OtobeLib/FrameWork/Collision/CollisionTemplate/CollisionBase.h"
#include "TemplateState.h"
#include "GameObject/TargetScrollCamera/TARGET_STATE.h"
#include "GameObject/MoveState/TargetState/TrackingState.h"
#include "GameObject/MoveState/HomingState/HomingState.h"
#include "OtobeLib/FrameWork/Owner/Owner.h"
#include <vector>

class TechniqueRecovery :public OtobeLib::TextureBase, OtobeLib::CollisionBase<OtobeLib::ITextureBase>,
	State<TARGET_STATE>, OtobeLib::Owner<OtobeLib::ITextureBase>
{
private:
	//ステートのコンテナ
	std::vector<MoveState*> m_stateList;

	//追跡用のステート
	std::unique_ptr<TrackingState> m_tracking;

	//ホーミング用のステート
	std::unique_ptr<HomingState>	m_homing;

	//経過時間
	float					m_timer;

	//敵が消滅してから、動き出すまでの待ち時間
	float					m_waitTimer;

	//回復させる値
	int					m_masicPower;
public:
	//コンストラクタ
	TechniqueRecovery(OtobeLib::ITextureBase* texture);

	//デストラクタ
	~TechniqueRecovery()override {};

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
	void SetColor(const DirectX::XMVECTORF32& color)override { TextureBase::SetColor(color); };

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

	//所有者のポインタを設定する
	void SetOwner(ITextureBase* ownew)override { Owner::SetOwner(ownew); }

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

	//所有者のポインタを取得する
	ITextureBase* GetOwner() { return Owner::GetOwner(); }

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
	void SetNowState(const TARGET_STATE& now)override { State::SetNowState(now); };

	//現在の状態を取得する
	const TARGET_STATE& GetNowState()const override { return State::GetNowState(); };

	//ひとつ前の状態を取得する
	const TARGET_STATE& GetPreState()const override { return State::GetPreState(); };

public:
	//追跡用のステートを取得する
	TrackingState* GetTrackingState() { return m_tracking.get(); }

	//ホーミング用のステートを取得する
	HomingState* GetHomingState() { return m_homing.get(); }

	//待ち時間を設定する
	void SetWaitTimer(const float& wait) { m_waitTimer = wait; }

	//回復量を設定する
	void SetHeal(const int& heal) { m_masicPower = heal; }

private:
	//Stateの更新処理
	void UpdateState();

	//State_Waitの処理
	void State_Wait();

	//State_Trackingの処理
	void State_Tracking();

	//プレイヤーと当たった時の処理
	void OnCollision_Player(OtobeLib::ITextureBase* player);
};


