#pragma once
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "OtobeLib/FrameWork/Collision/CollisionTemplate/CollisionBase.h"
#include "GameObject/Technique/VirusBeam/MainParticle/MainParticle.h"
#include "GameObject/Technique/VirusBeam/TrackingParticle/TrackingParticle.h"
#include "TemplateState.h"
#include "ENERGYCHAGE.h"
#include <list>
#include <vector>
class VirusBeam :public OtobeLib::TextureBase,OtobeLib::CollisionBase<OtobeLib::ITextureBase>,State<ENERGY_CHAGE>
{
public:
	//周囲のパーティクルの数
	static const int OTHER_PARTICLE_NUM = 10;

	//ゲージの回復量(コマンド技)
	static const int TECHNIQUE_HEAL_COMMAND = 25;

	//ゲージの回復量(必殺技)
	static const int TECHNIQUE_HEAL_SPECIAL = 50;

	//取得スコア
	static const int BASE_SCORE = 250;

	//コマンド技のゲージの消費量
	static const int GAGE_COST = 100;
private:
	//ビームの中心のパーティクル
	MainParticle* m_MainParticle;
	//中心に集まってくるパーティクル
	std::vector<TrackingParticle*> m_OtherParticleList;
	//全てのパーティクル
	std::vector<Particle*> m_AllParticleList;
	//描画する時間
	int					m_DrawTimeLimit;
	//描画するXの終点
	int					m_DrawFinal;
	//描画する限界値
	int					m_DrawLimit;
	//フレームカウント
	int					m_FrameCount;
	//ビームの更新速度
	int					m_BeamSpeed;
	//IDの管理クラス
	std::vector<int>	m_IdManager;
public:
	//コンストラクタ
	VirusBeam(OtobeLib::ITextureBase* texture);

	//デストラクタ
	~VirusBeam()override {};

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
	void SetPosition(const DirectX::SimpleMath::Vector3& pos)override { TextureBase::SetPosition(pos);};

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

	//---------------------------当たり判定用の関数---------------------------------------------------//

	//他のオブジェクトと当たった時に呼ばれる関数
	void OnCollisionEnter(ITextureBase* obj)override;

	//他のオブジェクトと当たっていないときに呼ばれる関数
	void NonCollisionEnter(ITextureBase* obj)override;

	//自身の当たり判定を取得する
	ICollisionBase<ITextureBase>* GetThisCollision() { return CollisionBase::GetThisCollision(); };

	//---------------------------当たり判定用の関数---------------------------------------------------//


	//プレイヤーと同じ行動をとる
	void WithPlayer(OtobeLib::ITextureBase* texture);

public:
	//ビームの中心のパーティクルを取得する
	MainParticle* GetMainParticle() { return m_MainParticle; }

	//中心に集まってくるパーティクルを取得する
	std::vector<TrackingParticle*>& GetParticleList() { return m_OtherParticleList; }

	//全てのパーティクルを取得する
	std::vector<Particle*>& GetAllParticleList() { return m_AllParticleList; }

	//描画の限界値を設定する
	void SetDrawLimit(const int& limit) { m_DrawLimit = limit; }

	//終点を設定する
	void SetDrawFinal(const int& Final) { m_DrawFinal = Final; }

	//描画する時間を設定する
	void SetDrawTimeLimit(const int& limit) { m_DrawTimeLimit = limit; }

	//ビームのスピードを設定する
	void SetBeamSpeed(const int& speed) { m_BeamSpeed = speed; }

	//ビームの初期化をする
	void InitBeam();

	//パーティクルの座標の設定をする
	void SetParticlesPos(const DirectX::SimpleMath::Vector3& pos)
	{
		//中心のパーティクルの座標を設定する
		m_MainParticle->SetPosition(pos);

		//角度を初期化する
		float angle = 0.0f;

		//算出したベクトルから、座標を設定する
		for (TrackingParticle*& particle : m_OtherParticleList)
		{
			//ベクトルを初期化する
			DirectX::SimpleMath::Vector3 vel = DirectX::SimpleMath::Vector3(0.0f, -50.0f, 0.0f);

			//ベクトルを更新
			this->AskCircleVectol(vel, angle);

			//座標が中心のパーティクルの周囲に来るようにする
			particle->SetPosition(m_MainParticle->GetPosition() + vel);
		}
	}

public:
	//現在の状態を設定する
	void SetNowState(const ENERGY_CHAGE& now)override { State::SetNowState(now); };

	//現在の状態を取得する
	const ENERGY_CHAGE& GetNowState()const override { return State::GetNowState(); };

	//ひとつ前の状態を取得する
	const ENERGY_CHAGE& GetPreState()const override { return State::GetPreState(); };

private:
	//ビーム用のパーティクルの更新処理
	void UpdateParticle();

	//中心から円状になるようにベクトルを算出する
	static void AskCircleVectol(DirectX::SimpleMath::Vector3& vel,float& angle);
};

