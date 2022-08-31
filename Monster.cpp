#include "Monster.h"
#include "Player.h"
#include "Block.h"
#include <cassert>

Monster::Monster()
{
}

Monster::~Monster()
{
}

//������
void Monster::Initialize(Model* model, const Vector3& position, const Vector3& velocity, bool birthMonster)
{
	// NULL�`�F�b�N
	assert(model);
	
	Model_ = model;

	//�e�N�X�`���ǂݍ���
	monsterHandle_ = TextureManager::Load("mario.jpg");

	//���񂾂��ǂ����̏����R�s�[����
	birthMonster_ = birthMonster;

	////�����Ŏ󂯎�������x�������o�ϐ��ɑ��
	//enemyVelocity_ = velocity;

	// �V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();

	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;

	ApproachInitialize();
}

//�X�V
void Monster::Update()
{
	//assert(block_);

	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
		});

	//�������W���擾
	/*Vector3 boxPos = block_->GetMonsterPosition();
	birthMonster_ = block_->GetBirthMonster();
	*/	
	/*switch (phase_)
	{
	case Phase::Approach:
	default:
		PhsaeApproach();

		break;

	case Phase::Leave:

		PhsaeLeave();
		break;
	}*/

	
	//�i�߂�Ȃ�ړ��֐����Ăяo��
	if (isMoved == true)
	{
		//�����X�^�[�̏Փˎ��R�[���o�b�N���Ăяo��
		OnCollisionMove();
	}
	//�i�߂Ȃ��Ȃ�����]���֐����Ăт���
	else if (isMoved == false)
	{
		ChangeDirection();
	}

	if (firstMove == true)
	{
		Vector3 Velocity_ = { kEnemySpeed, 0, 0 };
		//���W���ړ�������(1�t���[�����̈ړ��ʂ𑫂�����)
		worldTransform_.translation_ += Velocity_;
	}
	
#pragma region �L�����N�^�[�̍U��

	//�e�X�V
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

#pragma endregion


#pragma region �s��̍X�V
	//�X�P�[�����O�p�s���錾
	Matrix4 matScale;
	//�����p��]�s���錾
	Matrix4 matRot;
	//�e���p��]�p�s���錾
	Matrix4 matRotX, matRotY, matRotZ;
	//���W�p�s���錾
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	//�L�����N�^�[�ړ�����
	float scaleX = 5.0f;
	float scaleY = 5.0f;
	float scaleZ = 5.0f;

	//�X�P�[�����O�{�����s��ɐݒ�
	matScale.Matrix4Scaling(scaleX, scaleY, scaleZ);

	//��]�s����s��ɐݒ�
	matRot.Matrix4RotationX(worldTransform_.rotation_.x);
	matRot.Matrix4RotationY(worldTransform_.rotation_.y);
	matRot.Matrix4RotationZ(worldTransform_.rotation_.z);

	worldTransform_.matWorld_.IdentityMatrix();

	//���W���s��ɐݒ�
	matTrans.Matrix4Translation(
		worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);

	worldTransform_.matWorld_ = matScale;
	worldTransform_.matWorld_ *= matRot;
	worldTransform_.matWorld_ *= matTrans;

	//WorldMatrix(enemyworldTransform_.matWorld_, matScale, matRot, matTrans);

	//���[���h�s���]��
	worldTransform_.TransferMatrix();

#pragma endregion

	//if (birthMonster_ == true)
	//{
	//	//���Ԍo�߂Ńf�X
	//	if (--deathTimer_ <= 0)
	//	{
	//		isDead_ = true;
	//	}
	//}

	debugText_->SetPos(50, 580);
	debugText_->Printf("worldTranslation:(%f,%f,%f)",
		worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);

	debugText_->SetPos(50, 280);
	debugText_->Printf("isMoved:(%d)",
		isMoved);

	debugText_->SetPos(50, 310);
	debugText_->Printf("direction_:(%d)",
		direction_);
	/*debugText_->SetPos(150, 380);
	debugText_->Printf("worldTranslation:(%f,%f,%f)",
		worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);*/

	debugText_->SetPos(50, 540);
	debugText_->Printf("deathTimer_:(%d)",
		deathTimer_);


}

Vector3 Monster::GetLocalPosition()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldPos = worldTransform_.translation_;
	return worldPos;
}

void Monster::Fire()
{
	//assert(player_);

	//�@�e�̑��x
	const float kBulletSpeed = -1.0f;
	Vector3 velocity(kBulletSpeed, 0, 0);

	//���@�̃��[���h���W���擾
	//Vector3 playerPos = player_->GetLocalPosition();
	//�G�̃��[���h���W���擾
	Vector3 enemyPos = worldTransform_.translation_;

	//�����x�N�g��
	Vector3 difVec;
	//�����x�N�g�������߂�
	//difVec = playerPos - enemyPos;
	//�����x�N�g���𐳋K��
	difVec.normalize();
	//�x�N�g���̒����𑁂��ɍ��킹��
	const float bulletSpeed = 0.2f;
	difVec *= bulletSpeed;

	// ���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
	//velocity = Root(velocity, enemyworldTransform_);

	//�e�𐶐����A������
		//PlayerBullet* newBullet = new PlayerBullet();
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(Model_, worldTransform_.translation_, difVec);

	//�e��o�^����
	//bullet_.reset(newBullet);
	bullets_.push_back(std::move(newBullet));

}

//�`��
void Monster::Draw(const ViewProjection& viewProjection)
{
	Model_->Draw(worldTransform_, viewProjection, monsterHandle_);
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}
}

void Monster::ApproachInitialize()
{
	//���˃^�C�}�[��������
	bulletCoolTimer = kFireInterval;
}

void Monster::PhsaeApproach()
{
	//�ړ�(�x�N�g�������Z)
	//enemyworldTransform_.translation_ -= ApproachSpeed;
	//����̈ʒu�ɓ��B�����痣�E
	if (worldTransform_.translation_.x < -40.0f)
	{
		phase_ = Phase::Leave;
	}

	//���˃^�C�}�[���J�E���g�_�E��
	bulletCoolTimer--;
	//�w�莞�ԂɒB����
	if (bulletCoolTimer <= 0)
	{
		//�e�𔭎�
		Fire();
		//���˃^�C�}�[��������
		bulletCoolTimer = kFireInterval;
	}
}

void Monster::PhsaeLeave()
{
	//�ړ�(�x�N�g�������Z)
	//enemyworldTransform_.translation_ += LeaveSpeed;
	if (worldTransform_.translation_.x > 40.0f)
	{
		phase_ = Phase::Approach;
	}
}

Vector3 Monster::Root(Vector3 velocity, WorldTransform worldTransform_)
{
	//�P�ʃx�N�g��
	Vector3 frontVec = { 0.0f, 0.0f, 1.0f };
	//���ʗp�̃x�N�g��
	Vector3 resultVec = { 0.0f, 0.0f, 0.0f };

	resultVec.x = (cos(worldTransform_.rotation_.y) * frontVec.x +
		sin(worldTransform_.rotation_.y) * frontVec.z);
	resultVec.z = (-sinf(worldTransform_.rotation_.y) * frontVec.x +
		cosf(worldTransform_.rotation_.y) * frontVec.z);

	return resultVec;
}

void Monster::OnCollision()
{
	//�f�X�t���O�𗧂Ă�
	isDead_ = true;
}

void Monster::OnCollisionMove()
{
	firstMove = false;

	Vector3 Velocity_ = { 0, 0, 0 };

	if (direction_ == Direction::Right)
	{
		Velocity_ = { kEnemySpeed, 0, 0 };

		//���W���ړ�������(1�t���[�����̈ړ��ʂ𑫂�����)
		worldTransform_.translation_ += Velocity_;
	}

	else if (direction_ == Direction::Left)
	{
		Velocity_ = { kEnemySpeed, 0, 0 };
		//���W���ړ�������(1�t���[�����̈ړ��ʂ𑫂�����)
		worldTransform_.translation_ -= Velocity_;
	}
	else
	{

	}
}

float Monster::GetRadius()
{
	//���a������ϐ�
	float bulletRadius;
	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	bulletRadius = radius;
	return bulletRadius;
}

void Monster::Move()
{}

//�Ԃ������ۂɕ�����ς���
void Monster::ChangeDirection()
{
	if (direction_ == Direction::Right && checkDirectionR == false)
	{
		direction_ = Direction::Left;
		checkDirectionR = true;
		checkDirectionL = false;
	}
	else if(direction_ == Direction::Left && checkDirectionL == false)
	{
		direction_ = Direction::Right;
		checkDirectionL = true;
		checkDirectionR = false;
	}
}

void Monster::GetIsMove(bool possibleMove)
{
	isMoved = possibleMove;
}