#pragma once
#include "Effect/Particle/Particle.h"
#include "LagState.h"
#include "TemplateState.h"
class MainParticle:public Particle,State<SCALE>
{
public:
	//1フレームにスケールに加算する速度
	static const DirectX::SimpleMath::Vector3 SCALESPEED;
private:
	//チャージする回数
	int m_ChageCount;

	//チャージする限界値
	int m_LimitChage;

	//チャージ完了のフラグ
	bool m_ChageFinish;

	//拡大・縮小する回数
	int	m_ScaleCount;

	//拡大・縮小する限界数
	int m_ScaleLimitCount;

	//どれだけ拡大するかの限界
	float m_ScaleLimit;

	//現在の拡大・縮小ベクトル
	DirectX::SimpleMath::Vector3 m_ScaleVel;
public:
	//コンストラクタ
	MainParticle(OtobeLib::ITextureBase* texture);

	//デストラクタ
	~MainParticle()override {};

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
	void SetTexture(OtobeLib::ITextureBase* texture)override { TextureBase::SetTexture(texture); };

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

	//更新関数を設定する
	void SetActive(const bool& active)override { TextureBase::SetActive(active); };

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

	//更新関数を取得する
	const bool& GetActive()const { return TextureBase::GetActive(); };

	//----------------ゲッター-------------------------------------------------------------//

	//他のオブジェクトと当たった時に呼ばれる関数
	void OnCollisionEnter(ITextureBase* obj)override;

	//他のオブジェクトと当たっていないときに呼ばれる関数
	void NonCollisionEnter(ITextureBase* obj)override;


public:
	//チャージする限界値を設定する
	void SetChageLimit(const int& limit) { m_LimitChage = limit; }

	//何回、拡大・縮小するか
	void SetScaleLimitCount(const int& limit) { m_ScaleLimitCount = limit; }

	//どれだけ、拡大・縮小するか
	void SetScaleLimit(const float& limit) { m_ScaleLimit = limit; }

	//チャージする限界値を取得する
	const int& GetChageLimit()const { return m_LimitChage; }

	//チャージ完了のフラグを取得する
	const bool& GetChageFinish()const { return m_ChageFinish; }

public:
	//現在の状態を設定する
	void SetNowState(const SCALE& now)override { State::SetNowState(now); };

	//現在の状態を取得する
	const SCALE& GetNowState()const override { return State::GetNowState(); };

	//ひとつ前の状態を取得する
	const SCALE& GetPreState()const override { return State::GetPreState(); };

private:
	//周囲のパーティクルが追尾しているときの処理
	void State_Tracking(const int& NoDrawNum);

	//中心のパーティクルが大きくなる時の処理
	void State_PowerUp(const int& NoDrawNum);

	//エネルギーがMAXの時の処理
	void State_Max();
};

