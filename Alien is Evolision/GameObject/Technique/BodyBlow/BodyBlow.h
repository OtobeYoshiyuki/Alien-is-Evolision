#pragma once
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "OtobeLib/Texture/Animation/Animation.h"
class BodyBlow:public OtobeLib::TextureBase,OtobeLib::Animation
{
public:
	//ゲージの回復量(コマンド技)
	static const int TECHNIQUE_HEAL_COMMAND = 30;

	//ゲージの回復量(必殺技)
	static const int TECHNIQUE_HEAL_SPECIAL = 40;

	//取得スコア
	static const int BASE_SCORE = 300;

	//コマンド技のゲージの消費量
	static const int GAGE_COST = 50;
public:
	//コンストラクタ
	BodyBlow(OtobeLib::ITextureBase* texture);

	//デストラクタ
	~BodyBlow()override {};

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

	//アニメーションの情報を設定する
	void SetAnimation(const std::string& name, const RECT& rect)override { Animation::SetAnimation(name,rect); }

	//次に再生するアニメーションの情報を設定する
	void SetNextAnimation(const std::string& now, const std::string& next)override { Animation::SetNextAnimation(now,next); }

	//アニメーションの終了時間を設定する
	void SetEndTime(const std::string& name, const int& time)override { Animation::SetEndTime(name,time); }

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
	const std::string& GetNowInfo()const override{ return Animation::GetNowInfo(); }

	//アニメーションのループ情報を設定する
	const std::string& GetLoopInfo()const override{ return Animation::GetLoopInfo(); }

	//アニメーションの更新フラグを取得する
	const bool& GetAniFlag()const override{ return Animation::GetAniFlag(); }

	//アニメーションのループフラグを取得する
	const bool& GetLoopFlag()const override{ return Animation::GetLoopFlag(); }

	//アニメーションカウントを取得する
	const int& GetAniCount()const override{ return Animation::GetAniCount(); }

	//----------------ゲッター-------------------------------------------------------------//

	//プレイヤーと同じ行動をとる
	void WithPlayer(OtobeLib::ITextureBase* texture);
};

