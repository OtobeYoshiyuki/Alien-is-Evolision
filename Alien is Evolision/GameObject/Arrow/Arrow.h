#pragma once
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "TemplateState.h"
#include "ArrowScale.h"

class Arrow:public OtobeLib::TextureBase,State<ARROWSCALE>
{
public:
	//プラス
	static const int POSITIVE = 1;

	//マイナス
	static const int NEGATIVE = -1;
private:
	//最大の拡大倍率
	DirectX::SimpleMath::Vector3 m_maxScale;

	//最小の拡大倍率
	DirectX::SimpleMath::Vector3 m_minScale;

	//1フレームの加算ベクトル
	DirectX::SimpleMath::Vector3 m_frameVel;

	//ベクトルの向き
	int							 m_direction;
public:
	//コンストラクタ
	Arrow(OtobeLib::ITextureBase* texture);

	//デストラクタ
	~Arrow()override {};

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
	void SetColor(const DirectX::XMVECTORF32& color)override { TextureBase::SetColor(color); };

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

	//----------------ゲッター-------------------------------------------------------------//

public:
	//現在の状態を設定する
	void SetNowState(const ARROWSCALE& now)override { State::SetNowState(now); };

	//現在の状態を取得する
	const ARROWSCALE& GetNowState()const override { return State::GetNowState(); };

	//ひとつ前の状態を取得する
	const ARROWSCALE& GetPreState()const override { return State::GetPreState(); };

public:
	//Arrowの初期化
	void InitArrow(const DirectX::SimpleMath::Vector3& now, const DirectX::SimpleMath::Vector3& max,
		const DirectX::SimpleMath::Vector3& min, const float& time, ARROWSCALE arrowState);


private:
	//拡大倍率の更新処理
	void UpdateScale();

	//State_UpScaleの処理
	void State_UpScale();

	//State_DownScaleの処理
	void State_DownScale();
};

