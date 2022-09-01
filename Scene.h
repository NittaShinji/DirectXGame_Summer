#pragma once

#include "WorldTransform.h"
#include "Model.h"

class Scene
{
public:
	Scene();
	~Scene();

	void Initialize(Model* model);

	void Update();

	void TitleDraw(ViewProjection& viewProjection);

	void LoseDraw(ViewProjection& viewProjection);

	void ClearDraw(ViewProjection& viewProjection);

	void GameDraw(ViewProjection& viewProjection);

	//void Draw(ViewProjection& viewProjection);

	void UpdateMatrix();

	int GetScene();

private:

	//���[���h�ϊ��f�[�^
	WorldTransform titleTransform_;

	WorldTransform startTransform_;

	WorldTransform controlTransform_;

	// ���f��
	Model* model_ = nullptr;

	//�t�F�[�Y
	int sceneNow;

	//�^�C�g��
	uint32_t titleHandle_ = 0u;
	//�X�^�[�g
	uint32_t startHandle_ = 0u;
	//�Q�[���N���A
	uint32_t clearHandle_ = 0u;
	//�Q�[���I�[�o�[
	uint32_t loseHandle_ = 0u;
	//������@
	uint32_t controlHandle_ = 0u;
	//SPACE
	uint32_t spaceHandle_ = 0u;




};

