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
//class Player;
//class Block;

class Monster
{
	
public:

	//�`�ԃt�F�[�Y
	enum class MonsterForm
	{
		Slime,			//�X���C��
	};

	//�s���t�F�[�Y
	enum class Phase
	{
		Approach,	//�ڋ߂���
		Leave,		//���E����
	};

	//�����X�^�[�̌���
	enum  class Direction
	{
		Right,
		Left,
		Up,
		Down,
	};

	Monster();
	~Monster();

	//������
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity, bool birthMonster);
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
	/*void SetPlayer(std::shared_ptr<Player> player) { player_ = player; }
	void SetPlayer(std::shared_ptr<Block> block) { block_ = block; }*/

	// �E�҂̒e�Ƃ̏Փ˂����m������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	//�ړ��ł���ۂɌĂяo�����R�[���o�b�N�֐�
	void OnCollisionMove();
	//�i�߂Ȃ��ĕ����]���̍ۂɌĂяo�����R�[���o�b�N�֐�
	void ChangeDirection();

	//�e���X�g���擾
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	//���[���h���W���擾
	Vector3 GetLocalPosition();

	//�ړ�
	void Move();

	//���a���擾
	float GetRadius();

	void receive();

	bool IsDead() const { return isDead_; }

private:

	// ���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	// ���f��
	Model* Model_ = nullptr;
	// �C���v�b�g
	Input* input_ = nullptr;
	// �f�o�b�N�e�L�X�g
	DebugText* debugText_ = nullptr;
	// �e�N�X�`���n���h��
	uint32_t monsterHandle_ = 0u;

	// �G�̑��x
	const float kEnemySpeed = 0.1f;
	
	//�t�F�[�Y
	Phase phase_ = Phase::Approach;

	//�����X�^�[�̌���
	Direction direction_ = Direction::Right();

	//�ڋ߃t�F�[�Y�̑��x
	Vector3 ApproachSpeed = { 0.1,0,0 };
	//���E�t�F�[�Y�̑��x
	Vector3 LeaveSpeed = { 0.1,0,0 };

	//�e
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	//���˃^�C�}�[
	int32_t bulletCoolTimer = 0;

	////���L����
	//std::shared_ptr<Player> player_;

	////�u���b�N���
	//std::shared_ptr<Block> block_;

	//���a
	const float radius = 0.5f;

	//�����X�^�[�̐������W
	//Vector3 boxPos;
	 
	//�����X�^�[���a���������ǂ���
	bool birthMonster_;

	//����
	static const int32_t kLifeTime = 50 * 5;

	//�f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTime;
	//�f�X�t���O
	bool isDead_ = false;

	//�����Ă�����t���O
	bool isMoved = false;

	//�����ړ��t���O
	bool firstMove = true;

	//�����]������ۂ̔��Ȃ��悤�̃`�F�b�N�ϐ�
	bool checkDirectionR = false;
	bool checkDirectionL = false;

};

