#include "OtobeLib/StackScene/Interface/ISceneBase.h"
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"
#include "OtobeLib/FrameWork/Novel/Novel.h"
#include "ChoicesState.h"
#include "TemplateState.h"

namespace OtobeLib
{
	class ChoicesScene :public ISceneBase,StateArray<CHOICESSTATE,int>
	{
	public:
		//YESを選択している
		static const int YES = 0;

		//NOを選択している
		static const int NO = 1;
	private:
		//画像の管理クラス
		std::unique_ptr<TextureManager>	m_textureManager;

		//ノベルの管理クラス
		std::unique_ptr<Novel>			m_novel;

		//はいのテクスチャ
		ITextureBase*					m_yesTexture;

		//いいえのテクスチャ
		ITextureBase*					m_noTexture;
	public:
		//コンストラクタ
		ChoicesScene();

		//デストラクタ
		~ChoicesScene()override {};

		//初期化処理
		void Initialize()override;

		//更新処理
		void Update()override;

		//描画処理
		void Render()override;

		//終了処理
		void Finalize()override;

		//生成処理
		void Create()override;

	public:
		//現在の状態を設定する
		void AddData1(const CHOICESSTATE& state)override { StateArray::AddData1(state); };
		void AddData2(const int& state)override { StateArray::AddData2(state); };

		//現在の状態を取得する
		const CHOICESSTATE& GetNowData1()const override { return StateArray::GetNowData1(); };
		const int& GetNowData2()const override { return StateArray::GetNowData2(); };

		//ひとつ前の状態を取得する
		const CHOICESSTATE& GetPreData1()const override { return StateArray::GetPreData1(); };
		const int& GetPreData2()const override { return StateArray::GetPreData2(); };

	private:
		//ステートの更新処理
		void UpdateState();

		//State_Waitの処理
		void StateStoryWait();

		//State_Inputの処理
		void StateInput();
	};
}
