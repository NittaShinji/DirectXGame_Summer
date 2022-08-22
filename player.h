#pragma once

#include "WorldTransform.h"
#include "Model.h"
#include "DebugCamera.h"
#include "Input.h"
#include "Debugtext.h"

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
	void PlayerMatrixSet(WorldTransform& worldTransform);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(DebugCamera* debugCamera_, uint32_t textureHandle);

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
};