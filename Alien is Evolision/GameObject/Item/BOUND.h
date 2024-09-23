#pragma once
enum class BOUND :int
{
	NONE = -1,//何もしない
	FALL,//落下中
	BOUND,//バウンド中
	RESULT,//終了
};