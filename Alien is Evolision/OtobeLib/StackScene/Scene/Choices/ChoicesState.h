#pragma once
enum class CHOICESSTATE :int
{
	NONE = -1,//何もしない
	STORYWAIT,//テキストの終了待ち
	INPUT,//キー入力の状態
};
