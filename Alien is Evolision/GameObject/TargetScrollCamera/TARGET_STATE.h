#pragma once
enum class TARGET_STATE :int
{
	NONE = -1,
	TRACKING,
	WAIT,
	FADEOUT,
	FADEIN,
	CHANGE,
};