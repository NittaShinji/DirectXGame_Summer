#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "Debugtext.h"

//�u���b�N�N���X�̑O���錾
class Block;

class Select
{
public:
	Select();
	~Select();

	//������
	void Initialize(Model* model);
	//�X�V
	void Update();

	void receive();


	//�`��
	void Draw(const ViewProjection& viewProjection);

	//�Z�b�^�[
	void SetBlock(std::shared_ptr<Block> block) { block_ = block; }

private:

	static const int blockWidth = 30;
	static const int blockHeight = 30;

	// ���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	// ���f��
	Model* model_ = nullptr;
	// �C���v�b�g
	Input* input_ = nullptr;
	// �f�o�b�N�e�L�X�g
	DebugText* debugText_ = nullptr;
	// �e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	//���f���̑傫��
	float scaleX = 8.0f;
	float scaleY = 8.0f;
	float scaleZ = 8.0f;

	std::shared_ptr<Block> block_ = nullptr;

	Vector3 boxPos;
};

