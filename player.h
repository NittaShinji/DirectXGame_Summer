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

class RailCamera;

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

	//ローカル座標を取得
	Vector3 GetLocalPosition();
	////ワールド座標を取得
	//Vector3 GetWorldPosition();

	//半径を取得
	float GetRadius();

	// 衝突を検知したら呼び出されるコールバック関数
	void OnCollision();

	//弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	//カメラのワールド行列(ポインタ)を渡すためのSetter
	void SetRailCamera(std::shared_ptr<RailCamera> railCamera) { railCamera_ = railCamera; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	//レールカメラのワールド行列を取得
	WorldTransform railWorldTransform;
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
	//半径
	const float radius = 0.5f;
	//自キャラ
	std::shared_ptr<RailCamera> railCamera_;
	//ワールド座標計算用の変数
	Vector3 worldResultTransform;
};