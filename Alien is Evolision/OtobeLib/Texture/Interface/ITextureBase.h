#pragma once
#include <SimpleMath.h>
#include <SpriteBatch.h>
#include "OtobeLib/FrameWork/Collision/CollisionTemplate/ICollisionBase.h"
#include "OtobeLib/Status/Interface/IStatusBase.h"
#include "OtobeLib/Texture/Animation/Animation.h"

namespace OtobeLib
{
	class ITextureBase
	{
	public:
		enum class STATE :int
		{
			NONE,//描画しない
			DRAW,//描画する
		};

	public:
		//バーチャルデストラクタ（継承先のクラスのインスタンスのデストラクタが呼ばれる際に必要）
		virtual ~ITextureBase() {};

		//初期化
		virtual void Initialize() = 0;

		//更新
		virtual void Update() = 0;

		//描画
		virtual void Render() = 0;

		//終了
		virtual void Finalize() = 0;

		//----------------------セッター------------------------------------------------------------//

		//テクスチャを設定する
		virtual void SetTexture(ITextureBase* texture) = 0;
		
		//ハンドルを設定する
		virtual void SetHandle(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& handle) = 0;

		//座標を設定する
		virtual void SetPosition(const DirectX::SimpleMath::Vector3& pos) = 0;

		//ベクトルを設定する
		virtual void SetVelocity(const DirectX::SimpleMath::Vector3& vel) = 0;

		//切り出す画像の情報を設定する
		virtual void SetRect(const RECT& rect) = 0;

		//画像の色情報を設定する
		virtual void SetColor(const DirectX::SimpleMath::Color& color) = 0;
		virtual void SetColor(const DirectX::XMVECTORF32& color) = 0;

		//画像の回転角度を設定する
		virtual void SetAngle(const float& angle) = 0;

		//画像の拡大倍率を設定する
		virtual void SetScale(const DirectX::SimpleMath::Vector3& scale) = 0;

		//画像の状態を設定する
		virtual void SetState(const STATE& state) = 0;

		//画像の名前を設定する
		virtual void SetName(const std::string& name) = 0;

		//画像の描画順を設定する
		virtual void SetDrawOrder(const int& order) = 0;

		//画像のパスを設定する
		virtual void SetPath(const std::wstring& path) = 0;

		//削除フラグを設定する
		virtual void SetIsVal(const bool& val) = 0;

		//画像の中心位置を設定する
		virtual void SetOrigin(const DirectX::SimpleMath::Vector2& origin) = 0;

		//更新関数を設定する
		virtual void SetActive(const bool& active) = 0;

		//----------------セッター-----------------------------------------------------------//

		//----------------ゲッター------------------------------------------------------------//

		//ハンドルを設定する
		virtual const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetHandle()const = 0;

		//座標を設定する
		virtual const DirectX::SimpleMath::Vector3& GetPosition()const = 0;

		//ベクトルを設定する
		virtual const DirectX::SimpleMath::Vector3& GetVelocity()const = 0;

		//切り出す画像の情報を設定する
		virtual const RECT& GetRect()const = 0;

		//画像の色情報を設定する
		virtual const DirectX::SimpleMath::Color& GetColor()const = 0;

		//画像の回転角度を設定する
		virtual const float& GetAngle()const = 0;

		//画像の拡大倍率を設定する
		virtual const DirectX::SimpleMath::Vector3& GetScale()const = 0;

		//画像の状態を設定する
		virtual const STATE& GetState()const = 0;

		//画像の名前を設定する
		virtual const std::string& GetName()const = 0;

		//画像の描画順を取得する
		virtual const int& GetDrawOrder()const = 0;

		//画像のパスを設定する
		virtual const std::wstring& GetPath()const = 0;

		//削除フラグを設定する
		virtual const bool& GetIsVal()const = 0;

		//画像の中心位置を取得する
		virtual const DirectX::SimpleMath::Vector2& GetOrigin()const = 0;

		//更新関数を取得する
		virtual const bool& GetActive()const = 0;

		//----------------ゲッター-------------------------------------------------------------//


		//----------------セッター--------------------------------------------------------------//

		//現在の状態を設定する
		virtual void SetSpriteEffect(const DirectX::SpriteEffects& effect) = 0;

		//描画順を変更する(こっちは、リストに関係なく並び替えされる)
		virtual void SetLayerDenpth(const float& denpth) = 0;

		//----------------セッター--------------------------------------------------------------//

		//----------------ゲッター--------------------------------------------------------------//

		//現在の状態を取得する
		virtual const DirectX::SpriteEffects& GetSpriteEffect()const = 0;

		//描画順を取得する(こっちは、リストに関係なく並び替えされる)
		virtual const float& GetLayerDenpth() = 0;

		//----------------ゲッター--------------------------------------------------------------//
		
		//画像の切り出し位置から画像の中心点を求める
		virtual const DirectX::SimpleMath::Vector2 AskOrigin()const = 0;

		//画像を押し出す
		virtual void Extrude(ITextureBase* texture) = 0;
		virtual void ExtrudeX(ITextureBase* texture) = 0;
		virtual void ExtrudeY(ITextureBase* texture) = 0;

		//座標の修正
		virtual void PositionCorrection(ITextureBase* texture) = 0;

		//自身の当たり判定を取得する
		virtual ICollisionBase<ITextureBase>* GetThisCollision() = 0;

		//自身のステータスを取得する
		virtual IStatusBase* GetStatus() = 0;

		//右向きか、左向きかを取得する
		virtual const int DirectX() = 0;

		//アニメーションを取得する
		virtual Animation* GetThisAnimation() = 0;
	};
	
}

