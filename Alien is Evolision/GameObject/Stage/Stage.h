#pragma once
#include "GameObject/Stage/Maptip/MapTip.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"
#include "GameObject/Player/CoroKun.h"
#include <string>
#include <vector>

class Floor; class Wall; class Babul; class HoleSwitch; class HoleFloor; class Rebound;
class NormalEnemy; class SuperEnemy; class HyperEnemy; class CheckPoint;
class Stage
{
public:
	//最初に遊ぶステージ
	static const int FIRST_STAGE = 1;
	//用意されているステージの数
	static const int MAX_STAGE = 1;
	//1フレームにいくつステージを移動させるか
	static const float SCROLL_SPEED;
private:
	//----------------------メインステージ--------------------//
	//変換前のデータ
	std::vector<std::vector<std::string>> m_mainBeforeData;
	//変換後のデータ
	std::vector<std::vector<int>>m_mainAfterData;
	//----------------------メインステージ--------------------//

	//----------------------サブステージ----------------------//
	//変換前のデータ
	std::vector<std::vector<std::string>> m_subBeforeData;
	//変換後のデータ
	std::vector<std::vector<int>>m_subAfterData;
	//----------------------サブステージ----------------------//

	//マップチップの配列(全部)
	std::vector<std::vector<OtobeLib::ITextureBase*>>m_MapTip;

	//Floorの配列
	std::vector<Floor*>m_Floors;

	//Wallの配列
	std::vector<Wall*>m_Walls;

	//Babulの配列
	std::vector<Babul*>m_Babuls;

	//Reboundの配列
	std::vector<Rebound*>m_Rebounds;

	//HoleSwitchの配列
	std::vector<HoleSwitch*>m_Switches;

	//HoleFloorの配列
	std::vector<HoleFloor*>m_HoleFloors;

	//NormalEnemyの配列
	std::vector<NormalEnemy*>m_NormalEnemies;

	//SuperEnemyの配列
	std::vector<SuperEnemy*>m_SuperEnemies;

	//HyperEnemyの配列
	std::vector<HyperEnemy*>m_HyperEnemies;

	//CheckPointの配列
	std::vector<CheckPoint*>m_CheckPoints;

	//プレイヤークラス
	CoroKun* m_CoroKun;

	//ロードするファイルの連番
	int								m_StageFile;
public:
	//コンストラクタ
	Stage();

	//デストラクタ
	~Stage(){}
	//ステージの初期化
	void Initialize(const std::wstring& filenamemain,const std::wstring& filenamesub);

	//ステージの更新
	void Update();

	//ロードするファイルの連番を取得する
	const int& GetStageFile()const { return m_StageFile; }

	//Floorの配列を取得
	std::vector<Floor*>& GetFloors() { return m_Floors; }

	//Wallの配列を取得
	std::vector<Wall*>& GetWalls() { return m_Walls; }

	//Babulの配列を取得
	std::vector<Babul*>& GetBabuls() { return m_Babuls; }

	//Reboundの配列を取得
	std::vector<Rebound*>& GetRebounds() { return m_Rebounds; }
	
	//HoleSwitchの配列を取得
	std::vector<HoleSwitch*>& GetHoleSwitches() { return m_Switches; }

	//HoleFloorの配列を取得
	std::vector<HoleFloor*>& GetHoleFloors() { return m_HoleFloors; }

	//NormalEnemyの配列を取得
	std::vector<NormalEnemy*>& GetNormalEnemies() { return m_NormalEnemies; }

	//SuperEnemyの配列を取得
	std::vector<SuperEnemy*>& GetSuperEnemies() { return m_SuperEnemies; }

	//HyperEnemyの配列を取得
	std::vector<HyperEnemy*>& GetHyperEnemies() { return m_HyperEnemies; }

	//CheckPointの配列を取得
	std::vector<CheckPoint*>& GetCheckPoints() { return m_CheckPoints; }

	//マップ全体の配列を取得する
	std::vector<std::vector<OtobeLib::ITextureBase*>>& GetMap() { return m_MapTip; }

	//------------------------------------------メインステージ------------------------------------//

	//変換前のデータを取得する
	std::vector<std::vector<std::string>>& GetMainBeforeData() { return m_mainBeforeData; }

	//変換後のデータを取得する
	std::vector<std::vector<int>>& GetMainAfterData() { return m_mainAfterData; }

	//------------------------------------------メインステージ------------------------------------//

	//------------------------------------------サブステージ------------------------------------//

	//変換前のデータを取得する
	std::vector<std::vector<std::string>>& GetSubBeforeData() { return m_subBeforeData; }

	//変換後のデータを取得する
	std::vector<std::vector<int>>& GetSubAfterData() { return m_subAfterData; }

	//------------------------------------------サブステージ------------------------------------//

private:
	//StringからintにConvertする
	const std::vector<std::vector<int>> FileDataConvert(const std::vector<std::vector<std::string>>& Input);
	//マップチップを生成する
	void CreateMapTip(const std::vector<std::vector<int>>& Input);
	//役割ごとに、リストにポインタを登録する
	void AddVector();

public:
	//真下のマップチップを調べる
	OtobeLib::ITextureBase* DirectlyBlowMap(OtobeLib::ITextureBase* target,MapTip* mapType);

	//動くものの実行関数を設定する
	void MoveMapActive(const bool& active);
};
