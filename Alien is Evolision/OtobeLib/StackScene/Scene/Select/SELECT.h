#pragma once
enum class SELECT :int
{
	NONE = -1,//何もしない
	WAITKEY,//キー入力待ち
	CALCING,//計算中
	CHANGE_CUTIN,//Sceneの切り替え
	CHANGE_AND_DELETE,//Sceneの切り替えと削除
};