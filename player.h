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

class RailCamera;

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

	//���[�J�����W���擾
	Vector3 GetLocalPosition();
	////���[���h���W���擾
	//Vector3 GetWorldPosition();

	//���a���擾
	float GetRadius();

	// �Փ˂����m������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	//�e���X�g���擾
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	//�J�����̃��[���h�s��(�|�C���^)��n�����߂�Setter
	void SetRailCamera(std::shared_ptr<RailCamera> railCamera) { railCamera_ = railCamera; }

private:
	// ���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���[���J�����̃��[���h�s����擾
	WorldTransform railWorldTransform;
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
	//���a
	const float radius = 0.5f;
	//���L����
	std::shared_ptr<RailCamera> railCamera_;
	//���[���h���W�v�Z�p�̕ϐ�
	Vector3 worldResultTransform;
};