#pragma once
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "TemplateState.h"
#include "NormalState.h"
class NormalGage:public OtobeLib::TextureBase,State<NORMALSTATE>
{
public:
	//ゲージの最大値
	static const int MAX_GAGE = 100;

	//ゲージの最小値
	static const int MIN_GAGE = 0;

private:
	//計算の補正値
	int m_Correction;

public:
	//コンストラクタ
	NormalGage(OtobeLib::ITextureBase* texture);

	//デストラクタ
	~NormalGage()override {};

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

	//画像の原点を設定する
	void SetOrigin(const DirectX::SimpleMath::Vector2& origin)override { TextureBase::SetOrigin(origin); }

	//画像のエフェクトを設定する
	void SetSpriteEffect(const DirectX::SpriteEffects& effect)override { TextureBase::SetSpriteEffect(effect); }

	//画像の更新関数を設定する
	void SetActive(const bool& active)override { TextureBase::SetActive(active); }

	//----------------------セッター------------------------------------------------------------//


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

	//画像の原点を設定する
	const DirectX::SimpleMath::Vector2& GetOrigin()const override { return TextureBase::GetOrigin(); }

	//画像のエフェクトを設定する
	const DirectX::SpriteEffects& GetSpriteEffect()const override { return TextureBase::GetSpriteEffect(); }

	//画像の更新関数を設定する
	const bool& GetActive()const override { return TextureBase::GetActive(); }

	//----------------ゲッター------------------------------------------------------------//

	public:

	//現在の状態を設定する
	void SetNowState(const NORMALSTATE& now)override { State::SetNowState(now); };

	//現在の状態を取得する
	const NORMALSTATE& GetNowState()const override { return State::GetNowState(); };

	//ひとつ前の状態を取得する
	const NORMALSTATE& GetPreState()const override { return State::GetPreState(); };


public:

	//ゲージの変更を行う
	void ChangeGage();

	//補正値を取得する
	const int& GetCorrection()const { return m_Correction; }

};

