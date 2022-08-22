#pragma once

#include "WorldTransform.h"
#include "Model.h"
#include "DebugCamera.h"
#include "ViewProjection.h"
#include "Input.h"
#include "Debugtext.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>

/// <summary>
/// ���L����
/// </summary>
class Player
{
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="textureHandle">�e�N�X�`���n���h��</param>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	//�U��
	void Attack();

	Vector3 Root(Vector3 velocity, WorldTransform worldTransform_);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection& viewProjection, uint32_t textureHandle);

private:
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
	//�e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	/*std::unique_ptr<PlayerBullet> bullet_;*/
	/*PlayerBullet* bullet_ = nullptr;*/
};