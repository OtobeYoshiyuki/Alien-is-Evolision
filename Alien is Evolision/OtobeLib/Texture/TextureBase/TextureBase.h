#pragma once
#include "OtobeLib/Texture/Interface/ITextureBase.h"
#include "OtobeLib/Texture/Blinking/Blinking.h"
#include <functional>
#include <vector>
namespace OtobeLib
{
	class TextureBase:public ITextureBase,Blinking
	{
		//----------------変数-------------------------------------------------------------------//
	private:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_Handle;	//グラフィックハンドル

		DirectX::SimpleMath::Vector3						m_Position;	//座標

		DirectX::SimpleMath::Vector3						m_Velocity;	//ベクトル

		RECT												m_Rect;		//始点及び終点

		DirectX::SimpleMath::Color							m_Color;	//色

		float												m_Angle;	//回転（デグリー）

		DirectX::SimpleMath::Vector3						m_Scale;	//拡大倍率

		std::string											m_Name;		//名前

		STATE												m_State;	//状態

		int													m_DrawOrder;//描画順

		std::wstring										m_Path;		//画像のパス

		bool												m_IsVal;	//削除フラグ

		DirectX::SimpleMath::Vector2						m_Origin;	//画像の中心位置

		DirectX::SpriteEffects								m_Effect;	//画像用のエフェクト

		bool												m_Active;	//更新関数

		float												m_LayerDenpth;//描画順（リストに影響されない）
		//----------------変数--------------------------------------------------------------------//
	public:
		//コンストラクタ
		TextureBase();

		//コンストラクタ
		TextureBase(ITextureBase* texture);

		//デストラクタ
		virtual ~TextureBase();

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
		virtual void SetTexture(ITextureBase* texture)override;

		//ハンドルを設定する
		virtual void SetHandle(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& handle)override { m_Handle = handle; }

		//座標を設定する
		virtual void SetPosition(const DirectX::SimpleMath::Vector3& pos)override { m_Position = pos; };

		//ベクトルを設定する
		virtual void SetVelocity(const DirectX::SimpleMath::Vector3& vel)override { m_Velocity = vel; };

		//切り出す画像の情報を設定する
		virtual void SetRect(const RECT& rect)override { m_Rect = rect; };

		//画像の色情報を設定する
		virtual void SetColor(const DirectX::SimpleMath::Color& color)override { m_Color = color; };
		virtual void SetColor(const DirectX::XMVECTORF32& color)override { m_Color = color; };

		
		//画像の回転角度を設定する
		virtual void SetAngle(const float& angle)override { m_Angle = angle; };

		//画像の拡大倍率を設定する
		virtual void SetScale(const DirectX::SimpleMath::Vector3& scale)override { m_Scale = scale; };

		//画像の状態を設定する
		virtual void SetState(const ITextureBase::STATE& state)override { m_State = state; };

		//画像の名前を設定する
		virtual void SetName(const std::string& name)override { m_Name = name; };

		//画像の描画順を設定する
		virtual void SetDrawOrder(const int& order)override { m_DrawOrder = order; };

		//画像のパスを設定する
		virtual void SetPath(const std::wstring& path)override { m_Path = path; };

		//削除フラグを設定する
		virtual void SetIsVal(const bool& val)override { m_IsVal = val; }

		//画像の中心位置を設定する
		virtual void SetOrigin(const DirectX::SimpleMath::Vector2& origin)override { m_Origin = origin; };

		//現在の状態を設定する
		virtual void SetSpriteEffect(const DirectX::SpriteEffects& effect)override { m_Effect = effect; };

		//描画順を変更する(こっちは、リストに関係なく並び替えされる)
		virtual void SetLayerDenpth(const float& denpth) { m_LayerDenpth = denpth; };

		//更新関数を設定する
		virtual void SetActive(const bool& active)override { m_Active = active; };

		//点滅の設定をする
		virtual void SetBlink(const bool& blink)override { Blinking::SetBlink(blink); }

		//点滅する間隔を設定する
		virtual void SetTarget(const float& target) { Blinking::SetTarget(target); }

		//点滅の終了時間の設定をする
		virtual void SetFinal(const int& Final) { Blinking::SetFinal(Final); }
		
		//点滅の終了時間を適応するか
		virtual void SetFinalOption(const bool& finish) { Blinking::SetFinalOption(finish); }
		//----------------セッター-----------------------------------------------------------//

		//----------------ゲッター------------------------------------------------------------//

		//ハンドルを取得する
		virtual const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetHandle()const override { return m_Handle; }

		//座標を取得する
		virtual const DirectX::SimpleMath::Vector3& GetPosition()const override { return m_Position; };

		//ベクトルを取得する
		virtual const DirectX::SimpleMath::Vector3& GetVelocity()const override { return m_Velocity; };

		//切り出す画像の情報を取得する
		virtual const RECT& GetRect()const override { return m_Rect; };

		//画像の色情報を取得する
		virtual const DirectX::SimpleMath::Color& GetColor()const override { return m_Color; };

		//画像の回転角度を取得する
		virtual const float& GetAngle()const override { return m_Angle; };

		//画像の拡大倍率を取得する
		virtual const DirectX::SimpleMath::Vector3& GetScale()const override { return m_Scale; };

		//画像の状態を取得する
		virtual const ITextureBase::STATE& GetState()const override { return m_State; };

		//画像の名前を取得する
		virtual const std::string& GetName()const override { return m_Name; };

		//画像の描画順を取得する
		virtual const int& GetDrawOrder()const override { return m_DrawOrder; };

		//画像のパスを設定する
		virtual const std::wstring& GetPath()const override { return m_Path; };

		//削除フラグを設定する
		virtual const bool& GetIsVal()const override { return m_IsVal; };

		//画像の中心位置を取得する
		virtual const DirectX::SimpleMath::Vector2& GetOrigin()const override { return m_Origin; };

		//現在の状態を取得する
		virtual const DirectX::SpriteEffects& GetSpriteEffect()const override { return m_Effect; };

		//描画順を取得する(こっちは、リストに関係なく並び替えされる)
		virtual const float& GetLayerDenpth() { return m_LayerDenpth; }

		//更新関数を取得する
		virtual const bool& GetActive()const { return m_Active; };

		//点滅の取得
		virtual bool NowBlinking()override { return Blinking::NowBlinking(); };

		//点滅中かどうか
		virtual const bool& GetBlink()const override{ return Blinking::GetBlink(); }

		//フレーム数を取得する
		virtual const float& GetFrameCount()const override { return Blinking::GetFrameCount(); }

		//点滅の終了時間を取得する
		virtual const int& GetFinal()const override { return Blinking::GetFinal(); }

		//点滅の間隔を取得する
		virtual const float& GetTarget()const override { return Blinking::GetTarget(); }

		//点滅のフレーム数を取得する
		virtual const float& GetTargetCount()const { return Blinking::GetTargetCount(); }

		//----------------ゲッター-------------------------------------------------------------//

		//画像の切り出し位置から画像の中心点を求める
		const DirectX::SimpleMath::Vector2 AskOrigin()const override;

		//画像を押し出す
		void Extrude(ITextureBase* texture)override;

		void ExtrudeX(ITextureBase* texture)override;

		void ExtrudeY(ITextureBase* texture)override;

		//描画系を反転させる
		void ChangeSpriteEffect();

		//座標の修正
		void PositionCorrection(ITextureBase* texture)override;

		//自身の当たり判定を取得する
		virtual ICollisionBase<ITextureBase>* GetThisCollision()override { return nullptr; }

		//アニメーションを取得する
		virtual Animation* GetThisAnimation()override { return nullptr; }

		//自身のステータスを取得する
		virtual IStatusBase* GetStatus()override { return nullptr; };

		//右向きか、左向きかを取得する
		const int DirectX()override;

	public:
	//テクスチャの描画
		void DrawTexture();

	public:
		//変換
		static const std::vector<TextureBase> Cast(const std::vector<std::vector<std::string>>& input);
		//std::stringからstd::wstringへの変換
		static const std::wstring Convert(const std::string byteString);
	};
}

