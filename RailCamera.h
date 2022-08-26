#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Debugtext.h"

class RailCamera
{
public:
	RailCamera();
	~RailCamera();

	//������
	void Initialize(const Vector3& position,const Vector3& radian);
	//�X�V
	void Update();

	//�Q�b�^�[
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	//���[���h�ϊ��f�[�^���擾
	WorldTransform GetWorldTransform();

	//���[���h�s����擾
	Matrix4 GetMatWorld();
	
private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;
	// �f�o�b�N�e�L�X�g
	DebugText* debugText_ = nullptr;
};

