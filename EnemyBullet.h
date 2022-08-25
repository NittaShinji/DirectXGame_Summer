#pragma once

#include "Model.h"

class EnemyBullet
{
public:
	EnemyBullet();
	~EnemyBullet();

	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	void Update();

	//�Q�b�^�[
	bool IsDead() const { return isDead_; }

	//�`��
	void Draw(const ViewProjection& viewProjection);

	// �Փ˂����m������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	//���[���h���W���擾
	Vector3 GetWorldPosition();

	//���a���擾
	float GetRadius();

private:

	// ���[���h�ϊ��f�[�^
	WorldTransform eBulletWorldTransform_;
	// ���f��
	Model* eBulletModel_ = nullptr;

	// �e�N�X�`���n���h��
	uint32_t eBulletHandle_ = 0u;

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