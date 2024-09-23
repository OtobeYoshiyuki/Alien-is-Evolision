#pragma once
#include <vector>
#include <string>
#include "GameObject/Item/Heal/Heal.h"
#include "TemplateState.h"
#include <functional>

class HealManager:public State<int>
{
public:
	//回復アイテムの数
	static const int HEALITEM_NUM = 5;

	//アイテムをだす間隔
	static const float FIRING_INTERVAL;
private:
	//回復アイテムのリスト(時間経過で落下)
	std::vector<Heal*> m_healItem;

	//取得済みアイテムのリスト
	std::vector<std::unique_ptr<Item>> m_getItem;

	//全てのアイテム
	std::vector<Item*> m_allItem;

	//経過時間
	float				m_timer;
public:
	//コンストラクタ
	HealManager();

	//デストラクタ
	~HealManager() = default;

	//初期化
	void Initialize();

	//更新処理
	void Update();

public:
	//現在の状態を設定する
	void SetNowState(const int& now)override { State::SetNowState(now); };

	//現在の状態を取得する
	const int& GetNowState()const override { return State::GetNowState(); };

	//ひとつ前の状態を取得する
	const int& GetPreState()const override { return State::GetPreState(); };

public:
	//回復アイテムを生成する
	void CreateItem(OtobeLib::ITextureBase* refTex,std::function<void(Item*)> func);

	//回復アイテムを取得する
	std::vector<Heal*> GetHealList() { return m_healItem; }

	//全てのアイテムを取得する
	std::vector<Item*> GetAllItem() { return m_allItem; }

	//取得済みアイテムを取得する
	std::vector<std::unique_ptr<Item>>& GetItem() { return m_getItem; }

	//取得したアイテムを追加する
	void AddItem(Item* item) { m_getItem.push_back(std::make_unique<Item>(item)); }
	void AddItem(std::unique_ptr<Item>&& item) { m_getItem.push_back(std::move(item)); }

	//使用したアイテムを消す
	void EraseItem(const int& num) { m_getItem.erase(m_getItem.begin() + num); }
};

