#pragma once

#include "Model.h"

class Enemy
{
public:
	Enemy();
	~Enemy();

	//初期化
	void Initialize(Model* model, const Vector3& position);
	//更新
	void Update();
	//描画
	void Draw(const ViewProjection& viewProjection);

private:

	// ワールド変換データ
	WorldTransform enemyWorldTransform_;
	// モデル
	Model* enemyModel_ = nullptr;

	// テクスチャハンドル
	uint32_t enemyHandle_ = 0u;

	// 敵の速度
	const float kEnemySpeed = 0.1f;
	
};


