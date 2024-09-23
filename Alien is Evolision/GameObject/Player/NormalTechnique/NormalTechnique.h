#pragma once
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "OtobeLib/FrameWork/Collision/CollisionTemplate/CollisionBase.h"
#include <list>
#include "GameObject/Player/NormalTechnique/NormalBullet/NormalBullet.h"

class NormalGage;

class NormalTechnique
{
public:
	//通常弾の弾数
	static const int BULLET_NUM = 10;

	//ゲージの回復量(コマンド技)
	static const int TECHNIQUE_HEAL_COMMAND = 10;

	//ゲージの回復量(必殺技)
	static const int TECHNIQUE_HEAL_SPECIAL = 20;

private:
	//ゲージの裏面
	OtobeLib::ITextureBase* m_BackGage;

	//ゲージの表面
	NormalGage*		m_NormalGage;

	//通常弾
	std::vector<NormalBullet*>	m_NormalBulletsList;

	//発射する玉
	NormalBullet*	m_FrontBullet;

	//実行関数フラグ
	bool			m_Active;

	//当たったかどうかの判定フラグ
	bool			m_checkPush;

	//当たったかどうかの判定フラグ
	bool			m_checkRelease;

	//チャージ中に当たった際の再発射の時間
	float			m_time;

public:
	//コンストラクタ
	NormalTechnique();

	//デストラクタ
	~NormalTechnique() {};

	//初期化
	void Initialize();

	//更新
	void Update();

public:
	//先頭の弾を設定する
	void SetFrontBullet(NormalBullet* bullet) { m_FrontBullet = bullet; }

	//NomalGageを取得する
	NormalGage* GetNormalGage() { return m_NormalGage; }

	//先頭の弾を取得する
	NormalBullet* GetFrontBullet() { return m_FrontBullet; }

	//先頭の発射待機している弾を取得する
	NormalBullet* GetAllBulletFront();

	//弾のリストを取得する
	std::vector<NormalBullet*>& GetBulletsList() { return m_NormalBulletsList; }

	//実行関数フラグを設定する
	void SetActive(const bool& active) { m_Active = active; }

	//実行関数フラグを取得する
	const bool& GetActive()const { return m_Active; }

	//ダメージ判定用のフラグを設定する
	void SetCheckPush(const bool& push) {m_checkPush = push;}

	//ダメージ判定用のフラグを設定する
	void SetCheckRelease(const bool& release) { m_checkRelease = release; }

	//時間を設定する
	void SetTime(const float& time) { m_time = time; }

	//ダメージ判定用のフラグを取得する
	const bool& GetCheckPush()const { return m_checkPush; }

	//ダメ－ジ判定用のフラグを取得する
	const bool& GetCheckRelease()const { return m_checkRelease; }

	//時間を取得する
	const float& GetTime()const { return m_time; }

	//ゲージの裏面を取得する
	OtobeLib::ITextureBase* GetBackGage() { return m_BackGage; }
};

