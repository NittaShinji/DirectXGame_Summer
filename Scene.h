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

	//void Draw(ViewProjection& viewProjection);

	void UpdateMatrix();

	int GetScene();

private:

	//ワールド変換データ
	WorldTransform titleTransform_;

	WorldTransform startTransform_;

	// モデル
	Model* model_ = nullptr;

	//フェーズ
	int sceneNow;

	//タイトル
	uint32_t titleHandle_ = 0u;
	//ポーズ
	uint32_t startHandle_ = 0u;
};

