#pragma once
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "OtobeLib/FrameWork/Owner/Owner.h"
#include <list>
#include <vector>
#include <map>
#include <functional>

namespace OtobeLib
{
	//�O���錾
	class Novel;

	class TextWindow:public TextureBase,Owner<Novel>
	{
	public:
		//���s���X�V��~
		static const std::string NEWLINE_AND_STOP;

		//���s
		static const std::string NEWLINE;

		//�e�L�X�g�̃e�N�X�`���̏����ʒu
		static const DirectX::SimpleMath::Vector3 INIT_POS;

		//���ɕ\������e�N�X�`���̃C���^�[�o��
		static const float MAX_INTERBAL;

	public:
		//�R���X�g���N�^
		TextWindow(ITextureBase* texture);

		//�f�X�g���N�^
		~TextWindow()override {};

		//������
		virtual void Initialize()override;

		//�X�V
		virtual void Update()override;

		//�`��
		virtual void Render()override;

		//�I��
		virtual void Finalize()override;

		//----------------------�Z�b�^�[------------------------------------------------------------//

		//�e�N�X�`����ݒ肷��
		void SetTexture(ITextureBase* texture)override { TextureBase::SetTexture(texture); };

		//�n���h����ݒ肷��
		void SetHandle(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& handle)override { TextureBase::SetHandle(handle); };

		//���W��ݒ肷��
		void SetPosition(const DirectX::SimpleMath::Vector3& pos)override { TextureBase::SetPosition(pos); };

		//�x�N�g����ݒ肷��
		void SetVelocity(const DirectX::SimpleMath::Vector3& vel)override { TextureBase::SetVelocity(vel); };

		//�؂�o���摜�̏���ݒ肷��
		void SetRect(const RECT& rect)override { TextureBase::SetRect(rect); };

		//�摜�̐F����ݒ肷��
		void SetColor(const DirectX::SimpleMath::Color& color)override { TextureBase::SetColor(color); };

		//�摜�̉�]�p�x��ݒ肷��
		void SetAngle(const float& angle)override { TextureBase::SetAngle(angle); };

		//�摜�̊g��{����ݒ肷��
		void SetScale(const DirectX::SimpleMath::Vector3& scale)override { TextureBase::SetScale(scale); };

		//�摜�̏�Ԃ�ݒ肷��
		void SetState(const ITextureBase::STATE& state)override { TextureBase::SetState(state); };

		//�摜�̖��O��ݒ肷��
		void SetName(const std::string& name)override { TextureBase::SetName(name); };

		//�摜�̕`�揇��ݒ肷��
		void SetDrawOrder(const int& order)override { TextureBase::SetDrawOrder(order); };

		//�摜�̃p�X��ݒ肷��
		void SetPath(const std::wstring& path)override { TextureBase::SetPath(path); };

		//���L�҂̃|�C���^��ݒ肷��
		void SetOwner(Novel* novel)override { Owner::SetOwner(novel); }

		//----------------�Z�b�^�[-----------------------------------------------------------//

		//----------------�Q�b�^�[------------------------------------------------------------//

		//�n���h�����擾����
		const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetHandle()const override { return TextureBase::GetHandle(); };

		//���W���擾����
		const DirectX::SimpleMath::Vector3& GetPosition()const override { return TextureBase::GetPosition(); };

		//�x�N�g�����擾����
		const DirectX::SimpleMath::Vector3& GetVelocity()const override { return TextureBase::GetVelocity(); };

		//�؂�o���摜�̏����擾����
		const RECT& GetRect()const override { return TextureBase::GetRect(); };

		//�摜�̐F�����擾����
		const DirectX::SimpleMath::Color& GetColor()const override { return TextureBase::GetColor(); };

		//�摜�̉�]�p�x���擾����
		const float& GetAngle()const override { return TextureBase::GetAngle(); };

		//�摜�̊g��{�����擾����
		const DirectX::SimpleMath::Vector3& GetScale()const override { return TextureBase::GetScale(); };

		//�摜�̏�Ԃ��擾����
		const ITextureBase::STATE& GetState()const override { return TextureBase::GetState(); };

		//�摜�̖��O���擾����
		const std::string& GetName()const override { return TextureBase::GetName(); };

		//�摜�̕`�揇���擾����
		const int& GetDrawOrder()const override { return TextureBase::GetDrawOrder(); };

		//�摜�̃p�X��ݒ肷��
		const std::wstring& GetPath()const override { return TextureBase::GetPath(); };

		//���L�҂̃|�C���^���擾����
		Novel* GetOwner()override { return Owner::GetOwner(); }

		//----------------�Q�b�^�[-------------------------------------------------------------//

	public:
		//�`��̃t���O��ݒ�
		void SetDrawCheck(const bool& check) { m_drawCheck = check; }

		//�`��̃t���O���擾
		const bool& GetDrawCheck()const { return m_drawCheck; }

		//�`��̃J�E���^��ݒ�
		void SetDrawCount(const int& count) { m_drawCount = count; }

		//�`��̃J�E���^���擾
		const int& GetDrawCount()const { return m_drawCount; }

		//�t���[�����̃J�E���g��ݒ肷��
		void SetFrameCount(const int& frame) { m_frameCount = frame; }
	private:
		//�����̕`�悷��^�C�~���O�̃J�E���^
		int												m_drawCount;

		//�`�悷��t���O
		bool											m_drawCheck;

		//�t���[����
		int												m_frameCount;

		//�X�V�Ԋu
		int												m_drawInterval;

		//�`�悵�Ȃ������̃��X�g
		std::vector<std::string>						m_noDrawText;

		//�L�[���͂̔�����Ƃ�
		bool											m_keyCheck;

		//�C�x���g�֐��̃}�b�v
		std::map<std::string, std::function<void()>>	m_ivent;

		//�ŏ��ɓo�^����ID
		int												m_setupID;

	private:
		//�L�[���́��}�E�X�̏���
		const bool InPut();

		//�e�L�X�g���X�V����Ă���Ƃ��̏���
		const bool TextUpdate();

		//�e�L�X�g����~���Ă���Ƃ��̏���
		const bool TextStop();

		//�Ώۂ̃e�L�X�g�̔z��̗v�f���擾����
		const int Convert(const std::string& text);

		//�e�N�X�`���Ƃ��Ďg�p���Ȃ������𒲂ׂ�
		const bool	FindText(const std::string& text);

		//�������镶�������s�R�[�h���ǂ������ׂ�
		const bool FindLine(const std::string& text);

		//-------------------------�C�x���g�֘A�̊֐�---------------------------------//
		//���C�x���g
		void Ivent_��();

		//���C�x���g
		void Ivent_��();

		//FS�C�x���g
		void Ivent_FS();

		//EOF�C�x���g
		void Ivent_EOF();

		//END�C�x���g
		void Ivent_END();

		//�C�x���g���s�֐�
		void Ivent_Start();

		//�C�x���g�����s�ł��邩�m�F����
		const bool Find_Ivent(const std::string& ivent);
		//-------------------------�C�x���g�֘A�̊֐�---------------------------------//

	};
}

