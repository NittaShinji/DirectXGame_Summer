#pragma once

#include "Model.h"
#

//自キャラの弾
class PlayerBullet
{
public:
	PlayerBullet();
	~PlayerBullet();

	void Initialize(Model* model, const Vector3& position);
	void Update();

	//描画
	void Draw(const ViewProjection& viewProjection);

private:

	// ワールド変換データ
	WorldTransform bulletWorldTransform_;
	// モデル
	Model* buleetModel_ = nullptr;

	// テクスチャハンドル
	uint32_t bulletHandle_ = 0u;

};

