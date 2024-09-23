#include "pch.h"
#include "TextWindow.h"
#include "OtobeLib/FrameWork/Novel/Novel.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/FrameWork/SoundManager/SoundManager.h"
#include "OtobeLib/FrameWork/Stream/Stream.h"
#include "OtobeLib/FrameWork/Input/Input.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//���s���X�V��~
const std::string TextWindow::NEWLINE_AND_STOP = "��";

//���s
const std::string TextWindow::NEWLINE = "//";

//�e�L�X�g�̃e�N�X�`���̏����ʒu
const Vector3 TextWindow::INIT_POS = Vector3(64.0f, 455.0f, 0.0f);

//���ɕ\������e�N�X�`���̃C���^�[�o��
const float TextWindow::MAX_INTERBAL = 64.0f;

TextWindow::TextWindow(ITextureBase* texture):
	TextureBase(texture),//���N���X�ɏ��𑗐M����
	m_drawCheck(true),//�`�悷��t���O
	m_drawCount(NULL),//�`�悷��J�E���g
	m_keyCheck(true),//�L�[���͂̃t���O
	m_setupID(NULL),//�`��̐擪
	m_frameCount(NULL),//�t���[����
	m_drawInterval(5)//�J�E���g���X�V���銴�o
{
}

void TextWindow::Initialize()
{
	//�t�@�C�����擾����
	Stream* File = Stream::GetInstance();

	//�t�@�C�����J���̂ɐ���������J��
	if (File->Open(L"FileData/NoDrawList.csv", Stream::FILE::STREAM_IFSTREAM))
	{
		//vector�ɓo�^���閼�O�̏���ǂݍ���
		std::vector<std::vector<std::string>>NoDraw = File->Load_Data();
		m_noDrawText = NoDraw[0];
		//�t�@�C�������
		File->Close();
	}

	//�C�x���g�֐���o�^����
	m_ivent["��"] = std::bind(&TextWindow::Ivent_��,this);
	m_ivent["%"] = std::bind(&TextWindow::Ivent_��, this);
	m_ivent["FS"] = std::bind(&TextWindow::Ivent_FS, this);
	m_ivent["EOF"] = std::bind(&TextWindow::Ivent_EOF, this);
	m_ivent["END"] = std::bind(&TextWindow::Ivent_END, this);

	//���s�֐����N����
	this->SetActive(true);
}

void TextWindow::Update()
{
	//�t���O��false�̂Ƃ��A�������Ȃ�
	if (!this->GetActive())return;

	Story* story = this->GetOwner()->GetStory();

	//�摜�̍X�V����
	TextureBase::Update();

	//�L�[���́��}�E�X�̏���
	bool check = this->InPut();

	//�C�x���g�֘A�̏���(Key�̏��������s����Ȃ������Ƃ��̂�)
	if(!check)this->Ivent_Start();
}

void TextWindow::Render()
{
	//�t���O��false�̂Ƃ��A���b�Z�[�W�֘A�͉����`�悵�Ȃ�
	if (!m_drawCheck) return; 

	//�e�L�X�g�E�B���h�E�̕`��
	TextureBase::Render();

	//ResourceManager���擾����
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	//Story���擾����
	Story* story = this->GetOwner()->GetStory();

	//�X�g�[���[�̃e�L�X�g���擾����
	const std::vector<std::string> TextData = story->GetStoryArray();

	//DrawCount���͈͊O���Q�Ƃ��悤�Ƃ�����A�������Ȃ�
	if (m_drawCount >= (int)TextData.size())return;

	//���W�̏�����
	Vector3 pos = INIT_POS;

	//�e�L�X�g�̕`��
	for (int i = m_setupID;i < (int)TextData.size();i++)
	{
		//�e�L�X�g���擾
		const std::string& text = TextData[i];

		//�e�L�X�g���g���Ȃ��e�N�X�`�����������΂�
		if (!this->FindText(text) && !this->FindLine(text))continue;

		//���s����Ƃ�
		if (this->FindLine(text)) { pos.x = INIT_POS.x; pos.y += MAX_INTERBAL; continue; }

		//�K�v�ȃe�N�X�`�����擾����
		ITextureBase* UseTexture = resourceManager->GetTexture(text);

		//�`���1�t���[����m_DrawCount���܂�
		if (m_drawCount > i)
		{
			//�g���e�N�X�`���𐶐�����
			std::unique_ptr<ITextureBase> texture = std::make_unique<TextureBase>(UseTexture);
			//���W��ݒ肷��
			texture->SetPosition(pos);
			//�F��ݒ肷��
			texture->SetColor(Colors::Black);
			//���s���Ȃ��Ƃ�
			pos.x += MAX_INTERBAL;
			//�e�N�X�`���̕`��
			texture->Render();
		}
	}
}

void TextWindow::Finalize()
{
}

//�L�[���́��}�E�X�̏���
const bool TextWindow::InPut()
{
	//�e�L�X�g�̍X�V�����ꂽ���`�F�b�N����t���O
	bool update = false;

	//�e�L�X�g�̍X�V����
	update = this->TextUpdate();

	//�e�L�X�g�̒�~�����iKey�̃g���K�[���ύX����Ă��A�����͎��̃t���[������j
	//�������Ȃ��ƁA�L�[���͂������t���[���œK������Ă��܂�����
	bool check = false;

	if(!update)check = this->TextStop();

	//���������s���ꂽ��true�A���s����Ȃ�������false��Ԃ�
	return check;
}

const bool TextWindow::TextUpdate()
{
	//�L�[�֘A�̏����擾
	Input* input = Input::GetInstance();

	//Key�֘A�̏����擾
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//Mouse�֘A�̏����擾
	const auto& mouseState = input->GetMouse_State();

	//Key���͂�true 
	if (m_keyCheck)
	{
		//Key��Mouse�������ꂽ��A�����܂ŃX�L�b�v����
		if (keyTrakker.IsKeyPressed(Keyboard::Space)/* || 
			mouseState.leftButton == Mouse::ButtonStateTracker::ButtonState::PRESSED*/)
		{
			//�L�[���͂��~������
			m_keyCheck = false;

			//%�C�x���g��1�O�́��C�x���g�̂Ƃ���܂ŁA�߂�i�e�L�X�g���~�����邽�߁j
			m_drawCount = this->Convert("��");

			//���������s���ꂽ���Ƃ𖾎�����
			return true;
		}
		else
		{
			//���Ԋu����
			if (m_frameCount >= m_drawInterval)
			{
				//DrawCount���X�V����
				m_drawCount++;

				//�t���[������������
				m_frameCount = NULL;
			}

			//�t���[�������X�V����
			m_frameCount++;
		}
	}
	return false;
}

const bool TextWindow::TextStop()
{
	//�L�[�֘A�̏����擾
	Input* input = Input::GetInstance();

	//Key�֘A�̏����擾
	const auto& keyState = input->GetKeybord_State();
	const auto& keyTrakker = input->GetKeybord_Trakker();

	//Mouse�֘A�̏����擾
	const auto& mouseState = input->GetMouse_State();

	//Key���͂�false DrawCount���X�V����
	if (!m_keyCheck)
	{
		//Key��Mouse�������ꂽ��
		if (keyTrakker.IsKeyPressed(Keyboard::Space)/* ||
			mouseState.leftButton == Mouse::ButtonStateTracker::ButtonState::PRESSED*/)
		{
			//�L�[���͂��ĊJ����
			m_keyCheck = true;

			//�t���[������␳����
			m_frameCount = m_drawInterval;
			return true;
		}
	}

	return false;
}

const int TextWindow::Convert(const std::string& text)
{
	//Story���擾����
	Story* story = this->GetOwner()->GetStory();

	//�X�g�[���[�̃e�L�X�g���擾����
	const std::vector<std::string> TextData = story->GetStoryArray();

	//�͈͊O���Q�Ƃ��悤�Ƃ�����A�������Ȃ�
	if (m_drawCount >= (int)TextData.size())return -1;

	//�ړI����T��
	for (size_t i = m_drawCount; i < TextData.size(); i++)
	{
		const std::string& search = TextData[i];
		if (text == search)return i;
	}

	//������Ȃ������Ƃ��́A��O�Ƃ���-1��Ԃ�
	return -1;
}

const bool TextWindow::FindText(const std::string& text)
{
	//�e�N�X�`���Ƃ��Ďg���邩�ǂ������ׂ�
	for (const std::string& notext : m_noDrawText)
	{
		//�g���Ȃ����̂𔭌��������_��false��Ԃ�
		if (text == notext)return false;
	}

	//�g���Ȃ����̂���������Ȃ�������true��Ԃ�
	return true;
}

const bool TextWindow::FindLine(const std::string& text)
{
	//���s���̂Ƃ�
	if (text == NEWLINE_AND_STOP)return true;

	//���s//�̂Ƃ�
	if (text == NEWLINE)return true;

	return false;
}

//���̎��̏���������
void TextWindow::Ivent_��()
{	
	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//�����̍X�V���~�߂�
	m_keyCheck = false;

	//SE���~����
	soundManager->Stop(CRI_CUESHEET_0__47_���b�Z�[�W�\��);
}

//%�̎��̏���������
void TextWindow::Ivent_��()
{	
	//�`��̊J�n�ʒu��ύX����
	//�i���̃e�L�X�g�̐擪�Ɏ����Ă���j
	m_setupID = this->Convert("%") + 1;
}

//FS�̎��̏���������
void TextWindow::Ivent_FS()
{
	//SoundManager���擾����
	SoundManager* soundManager = SoundManager::GetInstance();

	//SE���Đ�����
	soundManager->Play(CRI_CUESHEET_0__47_���b�Z�[�W�\��);

	//�\���Ԋu��␳����
	m_frameCount = 5;
}

//EOF�̎��̏���������
void TextWindow::Ivent_EOF()
{	
	//�����̍X�V���~�߂�
	m_keyCheck = false;

	//�e�L�X�g�E�B���h�E�̍X�V���`����~�߂�
	m_drawCheck = false;

	//Update�̍X�V���~�߂�
	this->SetActive(false);
}

//END�̎��̏���������
void TextWindow::Ivent_END()
{	
	//�����̍X�V���~�߂�
	m_keyCheck = false;

	//�e�L�X�g�E�B���h�E�̍X�V���`����~�߂�
	m_drawCheck = false;

	//Update�̍X�V���~�߂�
	this->SetActive(false);
}

void TextWindow::Ivent_Start()
{
	//Story���擾����
	Story* story = this->GetOwner()->GetStory();

	//�X�g�[���[�̃e�L�X�g���擾����
	const std::vector<std::string> TextData = story->GetStoryArray();

	//�͈͊O�̂Ƃ�����Q�Ƃ��悤�Ƃ�����A�������Ȃ�
	if (m_drawCount >= (int)TextData.size())return;

	//���ݓǂ܂�Ă���e�L�X�g���擾����
	const std::string& nowText = TextData[m_drawCount];

	//���ݓǂ܂�Ă���e�L�X�g���C�x���g�Ɩ��֌W��������A�������Ȃ�
	if (!this->Find_Ivent(nowText))return;

	//�C�x���g�Ɍ��т��Ă���֐������s����
	std::function<void()> Execute = m_ivent[nowText];
	Execute();
}

const bool TextWindow::Find_Ivent(const std::string& ivent)
{
	//���C�x���g�̂Ƃ�
	if (ivent == "��")return true;

	//%�C�x���g�̂Ƃ�
	if (ivent == "%")return true;

	//FS�C�x���g�̂Ƃ�
	if (ivent == "FS")return true;

	//EOF�C�x���g�̂Ƃ�
	if (ivent == "EOF")return true;

	//END�C�x���g�̂Ƃ�
	if (ivent == "END")return true;

	return false;
}
