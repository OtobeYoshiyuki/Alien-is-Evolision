#pragma once
#include <vector>
#include "GameObject/Item/Item.h"
#include "TemplateState.h"
#include "GameObject/Item/BOUND.h"
#include "GameObject/MoveState/JumpState/JumpState.h"

class Heal :public Item,State<BOUND>
{
public:
	//1フレームに加算する速度
	static const float	JUMP_SPEED;

	//ジャンプの限界値(最初)
	static const float JUMP_LIMIT_FIRST;

	//ジャンプの回数
	static const int JUMP_COUNT = 9;

	//上昇から、落下までの補正値
	static const float CORRECTION;

	//バウンド係数
	static const float BOUND_POWER;

	//初期位置
	static const DirectX::SimpleMath::Vector3 INIT_POS;

	//初期倍率
	static const DirectX::SimpleMath::Vector3 INIT_SCALE;
private:
	//ステートのリスト
	std::vector<MoveState*> m_stateList;

	//JumpState
	std::unique_ptr<JumpState> m_jumpState;

	//回復量
	float						m_healPower;
public:
	//コンストラクタ
	Heal(OtobeLib::ITextureBase* texture);

	//デストラクタ
	~Heal()override {};

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
	void SetTexture(ITextureBase* texture)override { Item::SetTexture(texture); };

	//ハンドルを設定する
	void SetHandle(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& handle)override { Item::SetHandle(handle); };

	//座標を設定する
	void SetPosition(const DirectX::SimpleMath::Vector3& pos)override { Item::SetPosition(pos); };

	//ベクトルを設定する
	void SetVelocity(const DirectX::SimpleMath::Vector3& vel)override { Item::SetVelocity(vel); };

	//切り出す画像の情報を設定する
	void SetRect(const RECT& rect)override { Item::SetRect(rect); };

	//画像の色情報を設定する
	void SetColor(const DirectX::SimpleMath::Color& color)override { Item::SetColor(color); };

	//画像の回転角度を設定する
	void SetAngle(const float& angle)override { Item::SetAngle(angle); };

	//画像の拡大倍率を設定する
	void SetScale(const DirectX::SimpleMath::Vector3& scale)override { Item::SetScale(scale); };

	//画像の状態を設定する
	void SetState(const ITextureBase::STATE& state)override { Item::SetState(state); };

	//画像の名前を設定する
	void SetName(const std::string& name)override { Item::SetName(name); };

	//画像の描画順を設定する
	void SetDrawOrder(const int& order)override { Item::SetDrawOrder(order); };

	//画像のパスを設定する
	void SetPath(const std::wstring& path)override { Item::SetPath(path); };

	//画像の中心位置を設定する
	void SetOrigin(const DirectX::SimpleMath::Vector2& origin)override { Item::SetOrigin(origin); };

	//アイテムの取得済みフラグを設定する
	void SetItemFlag(const bool& flag)override { Item::SetItemFlag(flag); }

	//----------------セッター-----------------------------------------------------------//

	//----------------ゲッター------------------------------------------------------------//

	//ハンドルを取得する
	const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetHandle()const override { return Item::GetHandle(); };

	//座標を取得する
	const DirectX::SimpleMath::Vector3& GetPosition()const override { return Item::GetPosition(); };

	//ベクトルを取得する
	const DirectX::SimpleMath::Vector3& GetVelocity()const override { return Item::GetVelocity(); };

	//切り出す画像の情報を取得する
	const RECT& GetRect()const override { return Item::GetRect(); };

	//画像の色情報を取得する
	const DirectX::SimpleMath::Color& GetColor()const override { return Item::GetColor(); };

	//画像の回転角度を取得する
	const float& GetAngle()const override { return Item::GetAngle(); };

	//画像の拡大倍率を取得する
	const DirectX::SimpleMath::Vector3& GetScale()const override { return Item::GetScale(); };

	//画像の状態を取得する
	const ITextureBase::STATE& GetState()const override { return Item::GetState(); };

	//画像の名前を取得する
	const std::string& GetName()const override { return Item::GetName(); };

	//画像の描画順を取得する
	const int& GetDrawOrder()const override { return Item::GetDrawOrder(); };

	//画像のパスを設定する
	const std::wstring& GetPath()const override { return Item::GetPath(); };

	//画像の中心位置を取得する
	const DirectX::SimpleMath::Vector2& GetOrigin()const override { return Item::GetOrigin(); };

	//アイテムの取得済みフラグを取得する
	const bool& GetItemFlag()const override { return Item::GetItemFlag(); }

	//----------------ゲッター-------------------------------------------------------------//

	//---------------------------当たり判定用の関数---------------------------------------------------//

	//他のオブジェクトと当たった時に呼ばれる関数
	void OnCollisionEnter(ITextureBase* obj)override;

	//他のオブジェクトと当たっていないときに呼ばれる関数
	void NonCollisionEnter(ITextureBase* obj)override;

	//自身の当たり判定を取得する
	ICollisionBase<ITextureBase>* GetThisCollision() { return Item::GetThisCollision(); };

	//当たったかどうかのフラグを取得する
	const bool& GetCheck()const override { return Item::GetCheck(); };

	//---------------------------当たり判定用の関数---------------------------------------------------//

	//画像の切り出し位置から画像の中心点を求める
	const DirectX::SimpleMath::Vector2 AskOrigin()const override { return Item::AskOrigin(); };
public:
	//現在の状態を設定する
	void SetNowState(const BOUND& now)override { State::SetNowState(now); };

	//現在の状態を取得する
	const BOUND& GetNowState()const override { return State::GetNowState(); };

	//ひとつ前の状態を取得する
	const BOUND& GetPreState()const override { return State::GetPreState(); };

public:
	//落下処理
	void OnFall(const DirectX::SimpleMath::Vector3& pos);

	//回復量を設定する
	void SetHealPower(const float& heal) { m_healPower = heal; }

	//回復量を取得する
	const float& GetHealPower()const { return m_healPower; }

private:
	//ステートの更新処理
	void UpdateState();

	//State_Boundの処理
	void State_Bound();

	//State_RESULTの処理
	void State_Result();

	//Playerに当たった時の処理
	void OnCollision_Player(ITextureBase* obj);

	//floorに当たったときの処理
	void OnCollision_Floor(ITextureBase* obj);

	//wallに当たったときの処理
	void OnCollision_Wall(ITextureBase* obj);
};

