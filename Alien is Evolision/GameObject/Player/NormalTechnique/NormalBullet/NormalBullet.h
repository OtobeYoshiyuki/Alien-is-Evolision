#pragma once
#include "Effect/Particle/Particle.h"
#include "TemplateState.h"
#include "ENERGYCHAGE.h"
class NormalBullet:public Particle,State<ENERGY_CHAGE>
{
public:
	//取得スコア
	static const int BASE_SCORE = 30;
private:
	//入れ替え用のベクトル
	DirectX::SimpleMath::Vector3 m_Temp;

	//プレイヤーの弾を発射した時点の座標
	DirectX::SimpleMath::Vector3 m_PlayerTarget;
public:
	//コンストラクタ
	NormalBullet(OtobeLib::ITextureBase* texture);

	//デストラクタ
	~NormalBullet()override {};

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
	void SetTexture(ITextureBase* texture)override { Particle::SetTexture(texture); };

	//ハンドルを設定する
	void SetHandle(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& handle)override { Particle::SetHandle(handle); };

	//座標を設定する
	void SetPosition(const DirectX::SimpleMath::Vector3& pos)override { Particle::SetPosition(pos); };

	//ベクトルを設定する
	void SetVelocity(const DirectX::SimpleMath::Vector3& vel)override { Particle::SetVelocity(vel); };

	//切り出す画像の情報を設定する
	void SetRect(const RECT& rect)override { Particle::SetRect(rect); };

	//画像の色情報を設定する
	void SetColor(const DirectX::SimpleMath::Color& color)override { Particle::SetColor(color); };
	void SetColor(const DirectX::XMVECTORF32& color)override { Particle::SetColor(color); }

	//画像の回転角度を設定する
	void SetAngle(const float& angle)override { Particle::SetAngle(angle); };

	//画像の拡大倍率を設定する
	void SetScale(const DirectX::SimpleMath::Vector3& scale)override { Particle::SetScale(scale); };

	//画像の状態を設定する
	void SetState(const ITextureBase::STATE& state)override { Particle::SetState(state); };

	//画像の名前を設定する
	void SetName(const std::string& name)override { Particle::SetName(name); };

	//画像の描画順を設定する
	void SetDrawOrder(const int& order)override { Particle::SetDrawOrder(order); };

	//画像のパスを設定する
	void SetPath(const std::wstring& path)override { Particle::SetPath(path); };

	//画像の原点を設定する
	void SetOrigin(const DirectX::SimpleMath::Vector2& origin)override { Particle::SetOrigin(origin); }

	//画像のエフェクトを設定する
	void SetSpriteEffect(const DirectX::SpriteEffects& effect)override { Particle::SetSpriteEffect(effect); }

	//画像の更新関数を設定する
	void SetActive(const bool& active)override { Particle::SetActive(active); }

	//----------------------セッター------------------------------------------------------------//


	//----------------ゲッター------------------------------------------------------------//

	//ハンドルを取得する
	const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetHandle()const override { return Particle::GetHandle(); };

	//座標を取得する
	const DirectX::SimpleMath::Vector3& GetPosition()const override { return Particle::GetPosition(); };

	//ベクトルを取得する
	const DirectX::SimpleMath::Vector3& GetVelocity()const override { return Particle::GetVelocity(); };

	//切り出す画像の情報を取得する
	const RECT& GetRect()const override { return Particle::GetRect(); };

	//画像の色情報を取得する
	const DirectX::SimpleMath::Color& GetColor()const override { return Particle::GetColor(); };

	//画像の回転角度を取得する
	const float& GetAngle()const override { return Particle::GetAngle(); };

	//画像の拡大倍率を取得する
	const DirectX::SimpleMath::Vector3& GetScale()const override { return Particle::GetScale(); };

	//画像の状態を取得する
	const ITextureBase::STATE& GetState()const override { return Particle::GetState(); };

	//画像の名前を取得する
	const std::string& GetName()const override { return Particle::GetName(); };

	//画像の描画順を取得する
	const int& GetDrawOrder()const override { return Particle::GetDrawOrder(); };

	//画像のパスを設定する
	const std::wstring& GetPath()const override { return Particle::GetPath(); };

	//画像の原点を設定する
	const DirectX::SimpleMath::Vector2& GetOrigin()const override { return Particle::GetOrigin(); }

	//画像のエフェクトを設定する
	const DirectX::SpriteEffects& GetSpriteEffect()const override { return Particle::GetSpriteEffect(); }

	//画像の更新関数を設定する
	const bool& GetActive()const override { return Particle::GetActive(); }

	//----------------ゲッター------------------------------------------------------------//

	//他のオブジェクトと当たった時に呼ばれる関数
	void OnCollisionEnter(ITextureBase* obj)override;

	//他のオブジェクトと当たっていないときに呼ばれる関数
	void NonCollisionEnter(ITextureBase* obj)override;

	//自身の当たり判定を取得する
	ICollisionBase<ITextureBase>* GetThisCollision()override { return Particle::GetThisCollision(); }

public:

	//現在の状態を設定する
	void SetNowState(const ENERGY_CHAGE& now)override { State::SetNowState(now); };

	//現在の状態を取得する
	const ENERGY_CHAGE& GetNowState()const override { return State::GetNowState(); };

	//ひとつ前の状態を取得する
	const ENERGY_CHAGE& GetPreState()const override { return State::GetPreState(); };

public:

	//入れ替え用のベクトルを設定する
	void SetTempVel(const DirectX::SimpleMath::Vector3& vel) { m_Temp = vel; }

	//発射した時点の座標を設定する
	void SetPlayerTarget(const DirectX::SimpleMath::Vector3& target) { m_PlayerTarget = target; }

	//入れ替え用のベクトルを取得する
	const DirectX::SimpleMath::Vector3& GetTempVel()const { return m_Temp; }

	//発射した時点の座標を設定する
	const DirectX::SimpleMath::Vector3& GetPlayerTarget()const { return m_PlayerTarget; }
};

