#include "pch.h"
#include "Stream.h"
#include <iomanip>

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

Stream::Stream()
{
}


Stream::~Stream()
{
}

bool Stream::Open(const wchar_t * file_name,FILE mode)
{
	m_stream = mode;

	if (m_stream == STREAM_IFSTREAM)//入力モードの時
	{
		m_ifs.open(file_name);//csvファイルを開く
		if (m_ifs.is_open() == false)
		{
			return false;
		}
	}
	else
	{
		if (m_stream == STREAM_OFSTREAM)
		{
			m_ofs.open(file_name);//出力モードの時
			if (m_ofs.is_open() == false)
			{
				return false;
			}
		}
	}
	return true;
}

void Stream::Close()
{
	if (m_stream == STREAM_IFSTREAM)//入力モードの時
	{
		m_ifs.close();
	}
	else
	{
		//出力モードの時
		m_ofs.close();
	}
}

std::vector<std::vector<std::string>> OtobeLib::Stream::split(std::vector<std::string>& input, char delimiter)
{
	std::string field;
	std::vector<std::vector<std::string>> result;
	result.resize(input.size());
	for (size_t i = 0; i < input.size(); i++)
	{
		std::istringstream stream(input[i]);
		while (getline(stream, field, delimiter))
		{
			result[i].push_back(field);
		}
	}
	return result;
}


std::vector<std::vector<std::string>> OtobeLib::Stream::Load_Data()
{
	std::string Load_Data;
	std::vector<std::string> strvec;
	std::vector<std::vector<std::string>> result;

	while (getline(m_ifs, Load_Data))
	{
		strvec.push_back(Load_Data);
	}

	result = split(strvec, ',');

	return result;
}

void Stream::Data_Input(std::vector<std::vector<std::string>>& input)
{
	for (size_t i = 0; i < input.size(); i++)
	{
		for (size_t j = 0; j < input[i].size(); j++)
		{
			m_ofs << input[i][j];
			m_ofs << ",";
		}
		m_ofs << std::endl;
	}
}
