#pragma once
enum class LEVELUP :int
{
	NONE = -1,
	LEVELUP,//レベルアップ
	ANIMATION,//アニメーション中
	DOWN,//アルファ値減少中
	RESULT,//ステートの終了
};