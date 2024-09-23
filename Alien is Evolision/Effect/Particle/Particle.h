#pragma once
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "OtobeLib/FrameWork/Collision/CollisionTemplate/CollisionBase.h"
class Particle :public OtobeLib::TextureBase, OtobeLib::CollisionBase<OtobeLib::ITextureBase>
{
public:
	//コンストラクタ
	Particle(OtobeLib::ITextureBase* texture);

	//デストラクタ
	~Particle()override {};

	//初期化
	virtual void Initialize()override;

	//更新
	virtual void Update()override;

	//描画
	virtual void Render()override;

	//終了
	virtual void Finalize()override;

	//----------------------セッター------------------------------------------------------------//

	//テクスチャを設定する
	virtual void SetTexture(ITextureBase* texture)override { TextureBase::SetTexture(texture); };

	//ハンドルを設定する
	virtual void SetHandle(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& handle)override { TextureBase::SetHandle(handle); };

	//座標を設定する
	virtual void SetPosition(const DirectX::SimpleMath::Vector3& pos)override { TextureBase::SetPosition(pos); };

	//ベクトルを設定する
	virtual void SetVelocity(const DirectX::SimpleMath::Vector3& vel)override { TextureBase::SetVelocity(vel); };

	//切り出す画像の情報を設定する
	virtual void SetRect(const RECT& rect)override { TextureBase::SetRect(rect); };

	//画像の色情報を設定する
	virtual void SetColor(const DirectX::SimpleMath::Color& color)override { TextureBase::SetColor(color); };
	virtual void SetColor(const DirectX::XMVECTORF32& color)override { TextureBase::SetColor(color); };

	//画像の回転角度を設定する
	virtual void SetAngle(const float& angle)override { TextureBase::SetAngle(angle); };

	//画像の拡大倍率を設定する
	virtual void SetScale(const DirectX::SimpleMath::Vector3& scale)override { TextureBase::SetScale(scale); };

	//画像の状態を設定する
	virtual void SetState(const ITextureBase::STATE& state)override { TextureBase::SetState(state); };

	//画像の名前を設定する
	virtual void SetName(const std::string& name)override { TextureBase::SetName(name); };

	//画像の描画順を設定する
	virtual void SetDrawOrder(const int& order)override { TextureBase::SetDrawOrder(order); };

	//画像のパスを設定する
	virtual void SetPath(const std::wstring& path)override { TextureBase::SetPath(path); };

	//----------------セッター-----------------------------------------------------------//

	//----------------ゲッター------------------------------------------------------------//

	//ハンドルを取得する
	virtual const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetHandle()const override { return TextureBase::GetHandle(); };

	//座標を取得する
	virtual const DirectX::SimpleMath::Vector3& GetPosition()const override { return TextureBase::GetPosition(); };

	//ベクトルを取得する
	virtual const DirectX::SimpleMath::Vector3& GetVelocity()const override { return TextureBase::GetVelocity(); };

	//切り出す画像の情報を取得する
	virtual const RECT& GetRect()const override { return TextureBase::GetRect(); };

	//画像の色情報を取得する
	virtual const DirectX::SimpleMath::Color& GetColor()const override { return TextureBase::GetColor(); };

	//画像の回転角度を取得する
	virtual const float& GetAngle()const override { return TextureBase::GetAngle(); };

	//画像の拡大倍率を取得する
	virtual const DirectX::SimpleMath::Vector3& GetScale()const override { return TextureBase::GetScale(); };

	//画像の状態を取得する
	virtual const ITextureBase::STATE& GetState()const override { return TextureBase::GetState(); };

	//画像の名前を取得する
	virtual const std::string& GetName()const override { return TextureBase::GetName(); };

	//画像の描画順を取得する
	virtual const int& GetDrawOrder()const override { return TextureBase::GetDrawOrder(); };

	//画像のパスを設定する
	virtual const std::wstring& GetPath()const override { return TextureBase::GetPath(); };
	//----------------ゲッター-------------------------------------------------------------//

	//---------------------------当たり判定用の関数---------------------------------------------------//

	//他のオブジェクトと当たった時に呼ばれる関数
	virtual void OnCollisionEnter(ITextureBase* obj)override;

	//他のオブジェクトと当たっていないときに呼ばれる関数
	virtual void NonCollisionEnter(ITextureBase* obj)override;

	//自身の当たり判定を取得する
	ICollisionBase<ITextureBase>* GetThisCollision()override { return CollisionBase::GetThisCollision(); };

	//---------------------------当たり判定用の関数---------------------------------------------------//

};

