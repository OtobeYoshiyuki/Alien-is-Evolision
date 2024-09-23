#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "OtobeLib/FrameWork/Singleton/Singleton.h"

namespace OtobeLib
{
	class Stream:public Singleton<Stream>
	{
		friend class Singleton<Stream>;//��{�I�Ƀ����o�ɃA�N�Z�X���Ȃ�
	public:
		enum FILE :int
		{
			STREAM_NONE,//�J���Ȃ�
			STREAM_IFSTREAM,//���̓��[�h
			STREAM_OFSTREAM,//�o�̓��[�h
		};
	private:
		//���͗p�X�g���[��
		std::ifstream m_ifs;

		//�o�͗p�X�g���[��
		std::ofstream m_ofs;

		//�t�@�C�����[�h
		FILE m_stream;
	public:
		//�R���X�g���N�^
		Stream();

		//�f�X�g���N�^
		~Stream();

		//�t�@�C�����J��
		bool Open(const wchar_t* file_name, FILE mode);

		//�t�@�C�������
		void Close();

		//�ǂݍ��񂾕������������Ƃɕ�����
		std::vector<std::vector<std::string >> split(std::vector<std::string>& input, char delimiter);

		//�t�@�C���̒��g�����ׂēǂݍ���
		std::vector<std::vector<std::string >> Load_Data();

		//�z��̒��g��S�āA�t�@�C���ɕۑ�����i���łɁA�w�肵���t�@�C��������ꍇ�́A�㏑������j
		void Data_Input(std::vector<std::vector<std::string>>& input);
	};
}
