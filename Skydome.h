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
	//ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
};

