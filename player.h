#pragma once

#include "WorldTransform.h"
#include "Model.h"
#include "DebugCamera.h"
#include "ViewProjection.h"
#include "Input.h"
#include "Debugtext.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>

/// <summary>
/// 自キャラ
/// </summary>
class Player
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	//攻撃
	void Attack();

	Vector3 Root(Vector3 velocity, WorldTransform worldTransform_);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection, uint32_t textureHandle);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// インプット
	Input* input_ = nullptr;
	// デバックテキスト
	DebugText* debugText_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	/*std::unique_ptr<PlayerBullet> bullet_;*/
	/*PlayerBullet* bullet_ = nullptr;*/
};