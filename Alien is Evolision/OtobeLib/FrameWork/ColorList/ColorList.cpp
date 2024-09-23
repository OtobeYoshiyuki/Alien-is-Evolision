#include "pch.h"
#include "ColorList.h"
#include "OtobeLib/FrameWork/Stream/Stream.h"

using namespace OtobeLib;
using namespace DirectX;
using namespace DirectX::SimpleMath;

ColorList::ColorList():m_color_number(NULL),m_time(NULL)
{
}


ColorList::~ColorList()
{
}

void ColorList::Create()
{
}

void ColorList::Initialize()
{
	if (Stream::GetInstance()->Open(L"FileData/ColorList_Infomation.csv", Stream::GetInstance()->STREAM_IFSTREAM))
	{
		std::vector<std::vector<std::string>> Color_Data = Stream::GetInstance()->Load_Data();
		m_ColorList = this->Cast(Color_Data);
		Stream::GetInstance()->Close();
	}
}

void ColorList::CountUp()
{
	m_time++;
}

const Color& ColorList::GetColorList(bool flag, int list) const
{

	if (flag)
	{
		return m_ColorList[m_color_number];
	}
	return m_ColorList[list];
}

void ColorList::NumberUp()
{
	if (m_time % TIMER_INTERVAL == NULL)
	{
		if (m_color_number >= static_cast<int>(ColorList::COLORLIST::NUM))
		{
			m_color_number = NULL;
		}
		m_color_number++;
	}
}

std::vector<Color> ColorList::Cast(std::vector<std::vector<std::string>>& input)
{
	std::vector<Color> color;
	color.resize(input.size());
	std::vector<std::vector<float>>color_data;
	color_data.resize(input.size());

	for (size_t i = 0; i < input.size(); i++)
	{
		for (size_t j = 0; j < input[i].size(); j++)
		{
			color_data[i].push_back(std::stof(input[i][j]));
		}
	}

	for (size_t i = 0; i < color.size(); i++)
	{
		color[i] = Color{ color_data[i][0],color_data[i][1], color_data[i][2],1.0f};
	}
	return color;
}


