#pragma once

enum class PLAYERMOVE :int
{
	INPUT,//キー入力の処理
	DEAD,//死亡の処理
	SWEATANIME,//HPが0になった時点でのアニメーション
	ROTSCALE,//回転と縮小の処理
	ANIMESTART,//アニメーション開始
	ANIMEFINISH,//アニメーション終了
	CLEAR,//ゲームクリア
};
