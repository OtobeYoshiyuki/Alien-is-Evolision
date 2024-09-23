#include "pch.h"
#include "ExperienceGage.h"
#include "OtobeLib/StackScene/Scene/Play/PlayScene.h"
#include "Effect/LevelUp/LevelUpState.h"
#include "GameObject/Player/CoroKun.h"
#include "GameContext.h"
#include "OtobeLib/Status/StatusCalc/StatusCalc.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

ExperienceGage::ExperienceGage(ITextureBase* texture) :
	TextureBase(texture),//基底クラスに情報を送信する
	m_Max(NULL),//画像の縦か横の最大値
	m_calcSpeed(NULL),//1フレーム単位の計算値
	m_ratio(0.0f)//経験値と画像の比率
{
}

void ExperienceGage::Initialize()
{
}

void ExperienceGage::Update()
{
	//ステートの更新処理
	this->UpdateState();

	//画像の更新処理
	TextureBase::Update();
}

void ExperienceGage::Render()
{
	//画像の描画処理
	TextureBase::Render();
}

void ExperienceGage::Finalize()
{
}

const bool ExperienceGage::CheckExp() const
{
	//取得する経験値がない場合
	if (m_expList.empty() || m_expList.front() <= 0.0f)
	{
		return false;
	}

	//経験値が存在する場合
	return true;
}

void ExperienceGage::UpdateState()
{
	//Switch文で処理を分ける

	switch (this->GetNowState())
	{
	case CALC::CALCING:
		this->State_Calcking();
		break;
	case CALC::STOP:
		this->State_Stop();
		break;
	}
}

void ExperienceGage::State_Calcking()
{
	//SoundManagerを取得する
	SoundManager* soundManager = SoundManager::GetInstance();

	//取得する経験値がない場合は、何もしない
	if (!this->CheckExp())
	{
		soundManager->Stop(CRI_CUESHEET_0__33_ゲージ上昇);
		return;
	}

	//プレイヤーを取得する
	CoroKun* player = GameContext::Get<CoroKun>();

	//RECTを取得する
	RECT rect = this->GetRect();

	//最初の経験値を取得する
	int& exp = m_expList.front();

	//プレイヤーのレベルが成長限界に達していないとき
	if (player->GetLevel() < CoroKun::MAX_LEVEL)
	{
		//経験値を蓄積する
		rect.right += m_calcSpeed;

		//経験値が最大値を超えたら
		if (rect.right > m_Max)
		{
			//プレイヤーのレベルが19の時は、レベルアップの際にゲージを最大に固定する
			if (player->GetLevel() < CoroKun::MAX_LEVEL - 1)
			{
				//経験値を初期化する
				rect.right = NULL;

				//HPゲージのアニメーションを開始する
				player->SetDamageCheck(true);

				//HPゲージの裏面を設定する
				ITextureBase* backHpTexture = player->GetHpGage()->GetBackGageTexture();

				//HPゲージの裏面の色を設定する
				backHpTexture->SetColor(Colors::Aqua);

				//時間を設定する
				player->GetHpGage()->SetEasingTime(0.0f);
			}
			else {
				//経験値を固定する
				rect.right = m_Max;
			}

			//ステートを切り替える
			this->SetNowState(CALC::STOP);

			//レベルアップ前のステータスを取得する
			StatusImporter* importer1 = StatusBase::FindStatus(player->GetName(), player->GetLevel());

			//レベルアップ後のステータスを取得する
			StatusImporter* importer2 = StatusBase::FindStatus(player->GetName(), player->GetLevel() + 1);

			//HPの差分を求める
			int difference1 = importer2->GetHp() - importer1->GetHp();

			//MPの差分を求める
			int difference2 = importer2->GetMp() - importer1->GetMp();

			//Specialの差分を求める
			int difference3 = importer2->GetSpecial() - importer1->GetSpecial();

			//ステータスを再設定する
			player->SetStatus(player->GetHP(), player->GetMP(), importer2->GetAtack(), importer2->GetDefence(),
				player->GetSpecial(), importer2->GetLevel(), player->GetNextLevelPoint());

			player->SetMaxStatus(importer2->GetHp(), importer2->GetMp(), importer2->GetAtack(), importer2->GetDefence(),
				importer2->GetSpecial(), CoroKun::MAX_LEVEL, importer2->GetNextLevelPoint());

			//HPの切り取り位置を取得する
			RECT hpRect = player->GetHpGage()->GetRect();

			//MPの切り取り位置を取得する
			RECT mpRect = player->GetTechniqueGage()->GetRect();

			//Specialの切り取り位置を取得する
			RECT spRect = player->GetDeathblowGage()->GetRect();

			//現在のHPを取得する
			int hp = player->GetHP() + difference1;

			//現在のMPを取得する
			int mp = player->GetMP() + difference2;

			//現在のスペシャルポイントを取得する
			int special = player->GetSpecial() + difference3;

			//HP情報を補正する
			StatusCalc::CalcDamage(hp, importer2->GetHp(), importer1->GetHp(),
				player->GetHpGage()->GetMaxTexLength(), player->GetMaxHP(), hpRect.right);

			//MP情報を修正する
			StatusCalc::CalcDamage(mp, importer2->GetMp(), importer1->GetMp(),
				player->GetTechniqueGage()->GetMaxTexLength(), player->GetMaxMP(), mpRect.right);

			//SP情報を修正する
			StatusCalc::CalcDamage(special, importer2->GetSpecial(), importer1->GetSpecial(),
				player->GetDeathblowGage()->GetMaxTexLength(), player->GetMaxSpecialPoint(), spRect.right);

			//HPを設定する
			player->SetHP(hp);

			//MPを設定する
			player->SetMP(mp);

			//スペシャルポイントを設定する
			player->SetSpecial(special);

			//画像の幅を設定する
			player->GetHpGage()->SetRect(hpRect);
			player->GetTechniqueGage()->SetRect(mpRect);
			player->GetDeathblowGage()->SetRect(spRect);

			for (int& e : m_expList)
			{
				//経験値の値を元に戻す
				e /= m_ratio;
			}

			//比率を再設定する
			m_ratio = (float)m_Max / (float)player->GetMaxNextLevelPoint();

			for (int& e : m_expList)
			{
				//経験値を再設定する
				e *= m_ratio;
			}

		}
		
		//画像の切り取り位置を設定する
		this->SetRect(rect);

		//経験値を得る
		exp -= m_calcSpeed;

		if (exp <= 0.0f)
		{
			//経験値をリストから削除する
			m_expList.pop_front();
		}
	}
}

void ExperienceGage::State_Stop()
{
	//レベルアップを取得する
	LevelUp* levelUp = GameContext::Get<PlayScene>()->GetLevelUp();

	//ステートを切り替える
	if (levelUp->GetNowState() == LEVELUP::NONE)levelUp->SetNowState(LEVELUP::LEVELUP);
}

