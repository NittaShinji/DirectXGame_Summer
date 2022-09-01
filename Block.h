#pragma once

#include "Audio.h"
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "Debugtext.h"
#include "Monster.h"
#include <list>
#include <memory>

static const int blockWidth = 13;
static const int blockHeight = 13;

class Block
{
public:

	
	//�`�ԃt�F�[�Y
	enum class Form
	{
		Block = 0,		//���܂�Ă��Ȃ����
		IsSelected,		//�I������Ă�����(�u���b�N��Ԃ̂܂�)
		WasSelected,	//�I�����ꂽ���	(�u���b�N���Ȃ����)
		None,			//�����Ȃ����
		Slime,			//�X���C��
	};

	//�s���t�F�[�Y
	enum class Phase
	{
		Approach,	//�ڋ߂���
		Leave,		//���E����
	};

	Block();
	~Block();

	//������
	void Initialize(Model* model, const Vector3& position);
	//�X�V
	void Update();
	//�`��
	void Draw(const ViewProjection& viewProjection);

	//���[���h���W���擾
	Vector3 GetSelectPosition();

	void GetWorldPosition(Vector3 blockPos[blockWidth][blockHeight]);

	//�����X�^�[�o���p�ɍ��W��n���֐�
	//Vector3 GetBlockPosition();


	//�����X�^�[���o�������Ă��������ǂ���������t���O�֐�
	bool GetBirthMonster();

	// �Փ˂����m������Ăяo�����R�[���o�b�N�֐�
	//bool OnCollision(Vector3 wallPos[blockWidth][blockHeight],const int x,const int y);
	bool OnCollision(const int x,const int y);


	//�����X�^�[���X�g���擾
	const std::list<std::unique_ptr<Monster>>& GetMonsters() { return monsters_; }

	//�����X�^�[�𐶐�����
	void Birth();

	//���a���擾
	float GetRadius();

	//�ǂ̉��̒�����n��
	int GetBlockWidth();
	//�ǂ̏c�̒�����n��
	int GetBlockHight();

	//�X�R�A��n��
	int GetScore();

	int GetBreakWallCount();

	bool GetBreakAll();

	void IsSceneChanged();

	void IsMonsterDead();

private:

	// ���[���h�ϊ��f�[�^
	//WorldTransform worldTransforms_[blockWidth][blockHeight];
	WorldTransform worldTransforms_[blockWidth][blockHeight];

	// ���f��
	Model* model_ = nullptr;
	// �C���v�b�g
	Input* input_ = nullptr;
	// �f�o�b�N�e�L�X�g
	DebugText* debugText_ = nullptr;

	Audio* audio_ = nullptr;

	// �e�N�X�`���n���h��
	uint32_t blockHandle_ = 0u;
	uint32_t selectHandle_ = 0u;
	uint32_t wasSelectHandle_ = 0u;
	uint32_t slimeHandle_ = 0u;

	//���y
	uint32_t iceAxAudio_ = 0u;

	//��ԕω��ϐ�
	Form form_[blockWidth][blockHeight] = { Form::Block };
	//Form form_ = { Form::Block };

	//�L�����N�^�[�̑傫��
	float scaleX = 8.0f;
	float scaleY = 8.0f;
	float scaleZ = 8.0f;

	//�O�̃t���[���ɂ����đI��p�̃u���b�N�̏���ۑ����Ă����ϐ�
	int prevBlockX;
	int prevBlockY;

	//�I���\���ǂ����̕ϐ�
	bool changedSelect;
	bool wasChangedSelect;

	//�I���J�[�\���̃N�[���^�C�}�[�̐ݒ莞��
	static const int32_t kSelectTime = 17;

	//�I���J�[�\���̃N�[���^�C�}�[
	int32_t selectTimer_ = kSelectTime;
	int32_t wasSelectTimer_ = kSelectTime;

	//�����X�^�[
	std::list<std::unique_ptr<Monster>> monsters_;

	//�u���b�N���j�󂳂ꂽ�t���O
	bool breakBlock[blockWidth][blockHeight];
	//bool birthMonster;

	//�����X�^�[�����܂ꂽ�t���O
	bool birthMonster;

	//�����X�^�[�������p�̕ϐ�
	Vector3 worldMonsterTransform;

	//bool breakBlock[blockWidth][blockHeight] = { 0 };

	//bool breakBlock2[blockWidth][blockHeight] = { 0 };


	//�u���b�N�j���̛z���p�̐ݒ莞��
	static const int32_t kBirthTime = 90;
	//�z������
	int32_t birthTimer_[blockWidth][blockHeight] = { 0 };
	//int32_t wasSelectTimer_ = kSelectTime;


	//�u���b�N���I������Ă��邩
	//int isSelected[blockWidth][blockHeight] = { 0 };

	////�X�̃u���b�N�p�̏�ԕω��ϐ�
	//int formChange[blockWidth][blockHeight] = { 0 };

	//���[���h���W������ϐ�
	Vector3 monsterPos;

	//�Փ˂����ǂ�����p�̕ϐ�
	Vector3 collisionPos;

	//���a
	const float radius = 4.0f;

	//�X�R�A
	int score;

	//�ǂ�������ꂽ��
	int breakWallCount;

	//�f�X�t���O�̊Ǘ�
	int comeDeath;

	//int isBreakWall[blockWidth][blockHeight];

	
};

