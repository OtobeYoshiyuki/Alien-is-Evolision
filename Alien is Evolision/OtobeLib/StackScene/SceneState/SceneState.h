#pragma once
#include "OtobeLib/FrameWork/Graphics/Graphics.h"
#include "OtobeLib/Texture/Interface/ITextureBase.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"

namespace OtobeLib
{
	class SceneState
	{
	public:
		enum FADESTATE :int
		{
			FADENONE = -1,
			FADEIN,//フェードイン
			WAITKEY,//ゲームの処理
			FADEOUT,//フェードアウト
			FADERESULT,//フェード終了
		};

	private:
		//フェードのステート
		FADESTATE m_FadeState;

		//フェードのテクスチャ
		ITextureBase* m_Texture;

		//画像の管理クラス
		std::unique_ptr<TextureManager> m_TextureManager;

		//フェードの加算の速度
		float	m_fadeSpeed;

	public:
		//毎フレームのアルファ値を加算する速度
		static const float ALFA_SPEED;

		//アルファ値の限界値
		static const float LIMITTE_ALFA;

	public:
		//コンストラクタ
		SceneState();

		//デストラクタ
		~SceneState() {};
		
		//初期化
		void Initialize();

		//更新
		void Update();

		//描画
		void Render();

//-----------------------セッター-----------------------------------------------------//
		void SetFadeState(const FADESTATE& fade) { m_FadeState = fade; };
//-----------------------セッター-----------------------------------------------------//

//-----------------------ゲッター-----------------------------------------------------//
		const FADESTATE& GetFade()const { return m_FadeState; };
//-----------------------ゲッター-----------------------------------------------------//

	private:
		//フェードイン
		void Fade_In();

		//フェードアウト
		void Fade_Out();

	public:
		//アルファ値を設定する
		void SetAlfa(const float& alfa);

		//フェードの加算値を設定する
		void SetFadeSpeed(const float& speed) { m_fadeSpeed = speed; }
	};
}

