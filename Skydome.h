#pragma once
#include "WorldTransform.h"
#include "Model.h"

class Skydome
{
public:
	Skydome();
	~Skydome();

	void Initialize(Model* model);

	void Update();

	void Draw(ViewProjection& viewProjection);

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	// ���f��
	Model* model_ = nullptr;
};

