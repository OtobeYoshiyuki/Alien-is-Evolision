#pragma once
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "GameObject/MoveState/MoveState.h"
#include "GameObject/MoveState/AutoMoveState/AutoMoveState.h"
#include "GameObject/MoveState/JumpState/JumpState.h"
#include <list>
class Star:public OtobeLib::TextureBase
{
private:
	//AutoMoveState
	std::unique_ptr<AutoMoveState>	m_AutoMoveState;

	//JumpState
	std::unique_ptr<JumpState>		m_JumpState;

	//MoveStateのリスト
	std::list<MoveState*>			m_MoveStateList;

	//1フレームに回転する速度
	float m_AngleSpeed;
public:
	//コンストラクタ
	Star(OtobeLib::ITextureBase* texture);

	//デストラクタ
	~Star()override {};

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
	//----------------ゲッター-------------------------------------------------------------//

public:
	//1フレームに回転する速度を設定する
	void SetAngleSpeed(const float& speed) { m_AngleSpeed = speed; }

	//1フレームに回転する速度を取得する
	const float& GetAngleSpeed()const { return m_AngleSpeed; }

	//AutoMoveStateを取得する
	std::unique_ptr<AutoMoveState>& GetAutoMoveState() { return m_AutoMoveState; }

	//JumpStateを取得する
	std::unique_ptr<JumpState>& GetJumpState() { return m_JumpState; }

private:
	//ステートの更新を行う
	void UpdateState();

	//角度の更新を行う
	void UpdateAngle();
};

