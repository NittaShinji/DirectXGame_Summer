#pragma once

#include "WorldTransform.h"
#include "Model.h"
#include "DebugCamera.h"
#include "ViewProjection.h"
#include "Input.h"
#include "Debugtext.h"
#include "EnemyBullet.h"
#include <memory>
#include <list>
#include <cassert>

//���@�N���X�̑O���錾
class Player;

//GameScene�̑O���錾
class GameScene;

class Enemy
{
	
public:

	//�s���t�F�[�Y
	enum class Phase
	{
		Approach,	//�ڋ߂���
		Leave,		//���E����
	};

	Enemy();
	~Enemy();

	//������
	void Initialize(Model* model, const Vector3& position);
	//�X�V
	void Update();
	void Fire();
	//�����v�Z
	Vector3 Root(Vector3 velocity, WorldTransform worldTransform_);
	//���ˊԊu
	static const int kFireInterval = 60;
	//�`��
	void Draw(const ViewProjection& viewProjection);

	//�ڋ߃t�F�[�Y
	void PhsaeApproach();
	//�ڋ߃t�F�[�Y������
	void ApproachInitialize();
	void PhsaeLeave();

	//void SetPlayer(std::unique_ptr<Player> player) { player_ = player; }
	//void SetPlayer(std::unique_ptr<Player> player) { player_ = player; }
	void SetPlayer(std::shared_ptr<Player> player) { player_ = player; }
	//void SetgameScene(std::shared_ptr<GameScene> gameScene) { gameScene_ = gameScene; }
	void SetgameScene(GameScene *gameScene) { gameScene_ = gameScene; }


	// �Փ˂����m������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	////�e���X�g���擾
	//const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	//���[���h���W���擾
	Vector3 GetLocalPosition();

	//���a���擾
	float GetRadius();

	bool GetIsDead() { return isDead_; }

private:

	// ���[���h�ϊ��f�[�^
	WorldTransform enemyWorldTransform_;

	// ���f��
	Model* enemyModel_ = nullptr;
	// �C���v�b�g
	Input* input_ = nullptr;
	// �f�o�b�N�e�L�X�g
	DebugText* debugText_ = nullptr;
	// �e�N�X�`���n���h��
	uint32_t enemyHandle_ = 0u;

	//�Q�[���V�[��
	GameScene* gameScene_ = nullptr;
	//std::unique_ptr<GameScene> gameScene_ = nullptr;
	//std::shared_ptr<GameScene> gameScene_ = nullptr;

	//���L����
	std::shared_ptr<Player> player_;


	// �G�̑��x
	const float kEnemySpeed = 0.1f;
	
	//�t�F�[�Y
	Phase phase_ = Phase::Approach;
	//�ڋ߃t�F�[�Y�̑��x
	Vector3 ApproachSpeed = { 0.1,0,0 };
	//���E�t�F�[�Y�̑��x
	Vector3 LeaveSpeed = { 0.1,0,0 };

	//���˃^�C�}�[
	int32_t bulletCoolTimer = 0;

	//�f�X�t���O
	bool isDead_ = false;
	
	//���a
	const float radius = 0.5f;
	
};

