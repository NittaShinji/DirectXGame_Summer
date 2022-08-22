#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "Enemy.h"


/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

	enum PartID
	{
		Center,
		Right,
		Left,
		Up,
		Down,
	};

  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	/// 
	/// //単位行列
	Matrix4 frontVec;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	uint32_t textureRayHandle_ = 0;
	uint32_t textureGroundHandle_ = 0;

	//3Dモデル
	Model* model_ = nullptr;
	Model* rayModel = nullptr;

	////ワールドトランスフォーム
	// 
	//光線
	WorldTransform  ray = { 0 };

	//始点
	Vector3 startray = { 1,0,0 };
	//終点
	Vector3 endaray = { 1,0,1 };

	//オブジェクトの半径
	float objectRadius = 0.0f;

	//当たり判定フラグ
	bool rayCollision = 0;

	float moveCount = 0.0f;


	//WorldTransform rays = { 0 };

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//プレイヤー
	//Player* player_ = nullptr;

	//プレイヤーの生成
	std::unique_ptr<Player> player_;
	//敵の生成
	std::unique_ptr<Enemy> enemy_;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//カメラ上方向の角度
	float viewAngle = 0.0f;

};
