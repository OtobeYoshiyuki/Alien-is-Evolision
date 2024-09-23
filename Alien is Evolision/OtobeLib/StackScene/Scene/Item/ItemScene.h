#pragma once
#include "OtobeLib/StackScene/Interface/ISceneBase.h"
#include "OtobeLib/Texture/Interface/ITextureBase.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"
#include "GameObject/Item/Item.h"
#include "GameObject/Arrow/Arrow.h"
#include <vector>
#include <string>
#include <TemplateState.h>

//長いので省略
using FontInfo = std::map<std::string, std::vector<std::pair<std::string, DirectX::SimpleMath::Vector2>>>;

namespace OtobeLib
{
	class ItemScene :public ISceneBase, State<int>
	{
	public:
		enum class ITEMFONT :int
		{
			TAG,//タグ
			MOJI,//文字
			X,//X
			Y,//Y
		};
	public:
		//背景の初期座標
		static const DirectX::SimpleMath::Vector3 INIT_POS_BACK;

		//アイテムの初期座標
		static const DirectX::SimpleMath::Vector3 INIT_POS_ITEM;

		//アイテムの裏面
		static const DirectX::SimpleMath::Vector3 INIT_POS_ITEM_BACK;

		//説明欄の裏面
		static const DirectX::SimpleMath::Vector3 INIT_POS_EXPLANATION_COLUMN_BACK;

		//背景の切り取り位置
		static const RECT INIT_RECT_BACK;

		//アイテムの裏面の切り取り位置
		static const RECT INIT_RECT_ITEM_BACK;

		//説明欄の切り取り位置
		static const RECT INIT_RECT_EXPLANATION_COLUMN_BACK;

		//アイテムを横に並べる数
		static const int ROWITEM = 4;

		//アイテムを縦に並べる数
		static const int COLITEM = 2;

		//アイテム欄の初期座標
		static const DirectX::SimpleMath::Vector3 INIT_POS_ITEMTEXT;

		//アイテムの間隔
		static const DirectX::SimpleMath::Vector3 ITEM_INTERVAL;

	private:
		//画像の管理クラス
		std::unique_ptr<TextureManager>	m_TextureManager;

		//アイテム欄の裏面
		std::vector<ITextureBase*> m_itemBackText;

		//アイテム欄のアイテム
		std::vector<Item*> m_itemText;

		//拡大して、表示されているアイテム
		ITextureBase*				m_zoomItem;

		//現在のページ数
		int							m_pageNum;

		//ペース総数
		int							m_allPageNum;

		//現在選択中の画像
		ITextureBase*				m_selectTexture;

		//フォントの情報
		FontInfo					m_fontInfo;

		//Arrowクラス
		Arrow* m_rightArrowTexture;
		Arrow* m_leftArrowTexture;
	public:
		//コンストラクタ
		ItemScene();

		//デストラクタ
		~ItemScene()override {};

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

	private:
		//取得したアイテムに対応したタグを取得する
		const std::pair<std::string, float> ChangeItemTag(const std::string& name);

		//選択しているアイテムから、元のタグと、座標を取得する
		const std::pair<std::string, DirectX::SimpleMath::Vector3> ReverseItem(const std::string& name);

		//アイテム欄の更新
		void UpdateItemText();

		//選択中の画像の更新
		void UpdateSelectTexture();

		//選択中の処理
		void InputSelect(const int& move, std::function<void(int&, DirectX::SimpleMath::Vector3&)>func);

		//選択中のものに、アイテムがあるかどうか
		const bool OnSelectItem();

		//選択されているアイテムを取得する
		Item* SelectNowItem();

		//アイテムを使用する
		void UseItem();

		//現在、選択している場所を取得する
		const int OnSelectPlace();

		//使用したアイテムを消す

	public:
		//現在の状態を設定する
		void SetNowState(const int& now)override { State::SetNowState(now); };

		//現在の状態を取得する
		const int& GetNowState()const override { return State::GetNowState(); };

		//ひとつ前の状態を取得する
		const int& GetPreState()const override { return State::GetPreState(); };

		//データを変換する
		const FontInfo Cast(const std::vector<std::vector<std::string>>& Input);
	};
}

