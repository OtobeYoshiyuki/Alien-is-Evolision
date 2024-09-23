#pragma once
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "OtobeLib/FrameWork/Owner/Owner.h"
#include <list>
#include <vector>
#include <map>
#include <functional>

namespace OtobeLib
{
	//前方宣言
	class Novel;

	class TextWindow:public TextureBase,Owner<Novel>
	{
	public:
		//改行＆更新停止
		static const std::string NEWLINE_AND_STOP;

		//改行
		static const std::string NEWLINE;

		//テキストのテクスチャの初期位置
		static const DirectX::SimpleMath::Vector3 INIT_POS;

		//次に表示するテクスチャのインターバル
		static const float MAX_INTERBAL;

	public:
		//コンストラクタ
		TextWindow(ITextureBase* texture);

		//デストラクタ
		~TextWindow()override {};

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

		//所有者のポインタを設定する
		void SetOwner(Novel* novel)override { Owner::SetOwner(novel); }

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

		//所有者のポインタを取得する
		Novel* GetOwner()override { return Owner::GetOwner(); }

		//----------------ゲッター-------------------------------------------------------------//

	public:
		//描画のフラグを設定
		void SetDrawCheck(const bool& check) { m_drawCheck = check; }

		//描画のフラグを取得
		const bool& GetDrawCheck()const { return m_drawCheck; }

		//描画のカウンタを設定
		void SetDrawCount(const int& count) { m_drawCount = count; }

		//描画のカウンタを取得
		const int& GetDrawCount()const { return m_drawCount; }

		//フレーム数のカウントを設定する
		void SetFrameCount(const int& frame) { m_frameCount = frame; }
	private:
		//文字の描画するタイミングのカウンタ
		int												m_drawCount;

		//描画するフラグ
		bool											m_drawCheck;

		//フレーム数
		int												m_frameCount;

		//更新間隔
		int												m_drawInterval;

		//描画しない文字のリスト
		std::vector<std::string>						m_noDrawText;

		//キー入力の判定をとる
		bool											m_keyCheck;

		//イベント関数のマップ
		std::map<std::string, std::function<void()>>	m_ivent;

		//最初に登録するID
		int												m_setupID;

	private:
		//キー入力＆マウスの処理
		const bool InPut();

		//テキストが更新されているときの処理
		const bool TextUpdate();

		//テキストが停止しているときの処理
		const bool TextStop();

		//対象のテキストの配列の要素を取得する
		const int Convert(const std::string& text);

		//テクスチャとして使用しない文字を調べる
		const bool	FindText(const std::string& text);

		//検索する文字が改行コードかどうか調べる
		const bool FindLine(const std::string& text);

		//-------------------------イベント関連の関数---------------------------------//
		//＃イベント
		void Ivent_＃();

		//％イベント
		void Ivent_％();

		//FSイベント
		void Ivent_FS();

		//EOFイベント
		void Ivent_EOF();

		//ENDイベント
		void Ivent_END();

		//イベント実行関数
		void Ivent_Start();

		//イベントが実行できるか確認する
		const bool Find_Ivent(const std::string& ivent);
		//-------------------------イベント関連の関数---------------------------------//

	};
}

