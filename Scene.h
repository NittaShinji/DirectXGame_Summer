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

	//ワールド変換データ
	WorldTransform titleTransform_;

	WorldTransform startTransform_;

	WorldTransform controlTransform_;

	// モデル
	Model* model_ = nullptr;

	//フェーズ
	int sceneNow;

	//タイトル
	uint32_t titleHandle_ = 0u;
	//スタート
	uint32_t startHandle_ = 0u;
	//ゲームクリア
	uint32_t clearHandle_ = 0u;
	//ゲームオーバー
	uint32_t loseHandle_ = 0u;
	//操作方法
	uint32_t controlHandle_ = 0u;
	//SPACE
	uint32_t spaceHandle_ = 0u;




};

