#pragma once

#include "Model.h"

//���L�����̒e
class PlayerBullet
{
public:
	PlayerBullet();
	~PlayerBullet();

	void Initialize(Model* model, const Vector3& position,const Vector3& velocity);
	void Update();

	//�Q�b�^�[
	bool IsDead() const { return isDead_; }

	//�`��
	void Draw(const ViewProjection& viewProjection);

	// �Փ˂����m������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	//���a���擾
	float GetRadius();

	//���[���h���W���擾
	Vector3 GetWorldPosition();

private:

	// ���[���h�ϊ��f�[�^
	WorldTransform bulletWorldTransform_;
	// ���f��
	Model* bulletModel_ = nullptr;

	// �e�N�X�`���n���h��
	uint32_t bulletHandle_ = 0u;

	//���x
	Vector3 velocity_;

	//����
	static const int32_t kLifeTime = 60 * 5;

	//�f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTime;
	//�f�X�t���O
	bool isDead_ = false;

	//���a
	const float radius = 0.5f;

};

