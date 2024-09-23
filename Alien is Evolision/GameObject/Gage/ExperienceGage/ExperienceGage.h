#pragma once
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "TemplateState.h"
#include "CalcState.h"

class ExperienceGage :public OtobeLib::TextureBase,State<CALC>
{
private:
	//画像の縦か横の最大値
	int m_Max;

	//1フレーム単位で、計算する値
	int m_calcSpeed;

	//取得経験値のリスト
	std::list<int> m_expList;

	//取得した経験値との比率
	float m_ratio;
public:
	//コンストラクタ
	ExperienceGage(OtobeLib::ITextureBase* texture);

	//デストラクタ
	~ExperienceGage()override {};

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

	//----------------ゲッター------------------------------------------------------------//

public:
	//現在の状態を設定する
	void SetNowState(const CALC& now)override { State::SetNowState(now); };

	//現在の状態を取得する
	const CALC& GetNowState()const override { return State::GetNowState(); };

	//ひとつ前の状態を取得する
	const CALC& GetPreState()const override { return State::GetPreState(); };
public:
	//画像の縦か横の最大値を設定する
	void SetMaxTexLength(const int& max) { m_Max = max; }

	//1フレーム単位の計算値を設定する
	void SetFrameCalcSpeed(const int& calc) { m_calcSpeed = calc; }

	//取得経験値の値を設定する
	void PushExp(const int& exp) { m_expList.push_back(exp); }

	//取得経験値の値を取得する
	void SetRatio(const float& ratio) { m_ratio = ratio; }

	//画像の縦か横の最大値を取得する
	const int& GetMaxTexLength()const { return m_Max; }

	//1フレーム単位の計算値を取得する
	const int& GetFrameCalcSpeed()const { return m_calcSpeed; }

	//経験値があるかどうか確認する
	const bool CheckExp()const;


private:
	//Stateの更新処理
	void UpdateState();

	//State_CALCINGの処理
	void State_Calcking();

	//State_Stopの処理
	void State_Stop();
};

