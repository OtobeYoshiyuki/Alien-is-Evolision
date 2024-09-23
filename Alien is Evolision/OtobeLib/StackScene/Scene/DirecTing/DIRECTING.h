#pragma once
//演出関連のステート
enum class DIRECTING :int
{
	NONE = -1,//何もしない
	WAVE_ANIME,//オーラのアニメーション中
	TIMER_UP,//プレイヤーのアニメーションまでの時間更新
	PLAYER_ANIME,//プレイヤーのアニメーション中
	BACK_FADE,//プレイヤーの裏面の透明度を計算中
	EXPROSION_ANIME,//爆発のアニメーション中
	END,//終了
};
