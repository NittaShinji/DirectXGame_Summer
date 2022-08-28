#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include <math.h>
#include <WinApp.h>
#include <cassert>
#include <random>
#include <memory>

#ifndef M_PI
#define M_PI 3.14159 /* 桁数はもっと多い方がいいかも */
#endif

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
	delete modelSkydome_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("maou4.png");

	// 3Dモデルの生成
	model_ = Model::Create();
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	////自キャラの生成
	//player_ = new Player();
	////自キャラの初期化
	//player_->Initialize(model_ ,textureHandle_);
	
	//レールカメラの生成
	//RailCamera* newRailCamera = new RailCamera;
	//railCamera_.reset(newRailCamera);
	////レールカメラの初期化
	//railCamera_->Initialize(railPos, angle);

	//自キャラの生成と登録
	Player* newPlayer = new Player;
	player_.reset(newPlayer);
	////自キャラにレールカメラのアドレスを渡す
	//player_->SetRailCamera(railCamera_);
	//自キャラの初期化
	player_->Initialize(model_, textureHandle_);
	
	//敵の生成
	Enemy* newEnemy = new Enemy();
	enemy_.reset(newEnemy);	
	//敵の初期化
	enemy_->Initialize(model_,enemyPos);

	//天球の生成
	Skydome* newSkydome = new Skydome();
	skydome_.reset(newSkydome);
	//天球の初期化
	skydome_->Initialize(modelSkydome_);

	//モンスターの生成
	Monster* newMonster = new Monster();
	monster_.reset(newMonster);

	//monster_ = new Monster;

	//モンスターの初期化
	monster_->Initialize(model_, monsterPos);

	//敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);
	
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	//カメラ視点座標を設定
	//viewProjection_.eye = { 0,0,-10 };

	//カメラ注視点座標を設定
	//viewProjection_.target = { 10,0,0 };

	//カメラ上方向ベクトルを設定
	//viewProjection_.up = { 0.0f,1.0f,0.0f };

	//カメラ垂直方向視野角を設定
	//viewProjection_.fovAngleY = ToRadian(10.0f);

	//アスペクト比を設定
	//viewProjection_.aspectRatio = 1.0f;

	////ニアクリップ距離を設定
	//viewProjection_.nearZ = 52.0f;
	////ファークリップ距離を設定
	//viewProjection_.farZ = 53.0f;

	// 軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);

	// 軸方向表示が参照するビュープロジェクションを指定する (アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	
	// ライン描画が参照するビュープロジェクションを指定する (アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);
}

void GameScene::Update() 
{
	assert(player_);
	assert(enemy_);
	assert(monster_);

	debugCamera_->Update();
	player_->Update();
	enemy_->Update();
	skydome_->Update();
	monster_->Update();
	//railCamera_->Update();

	viewProjection_ = debugCamera_->GetViewProjection();
	//viewProjection_ = railCamera_->GetViewProjection();
	
	//行列の再計算
	viewProjection_.UpdateMatrix();

	////視点移動処理
	//Vector3 eyeMove = { 0.0f,0.0f,0.0f};

	////視点移動の速さ
	//const float kEyeSpeed = 0.2f;

	////押した方向で移動ベクトルを変更
	//if (input_->PushKey(DIK_W))
	//{
	//	eyeMove.z += kEyeSpeed;
	//}
	//else if(input_->PushKey(DIK_S))
	//{
	//	eyeMove.z -= kEyeSpeed;
	//}

	////視点移動(ベクトルの加算)
	//viewProjection_.eye += eyeMove;

	////注視点の移動ベクトル
	//Vector3 kTargetMove = { 0,0,0 };

	////注視点の移動の速さ
	//const float kTargetSpeed = 0.2f;

	////押した方向で移動ベクトルを変更
	//if (input_->PushKey(DIK_LEFT))
	//{
	//	kTargetMove.x -= kTargetSpeed;
	//}
	//else if (input_->PushKey(DIK_RIGHT))
	//{
	//	kTargetMove.x += kTargetSpeed;
	//}

	////注視点移動(ベクトルの加算)
	//viewProjection_.target += kTargetMove;

	CheckAllCollisions();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	player_->Draw(viewProjection_,textureHandle_);
	enemy_->Draw(viewProjection_);
	skydome_->Draw(viewProjection_);
	monster_->Draw(viewProjection_);

	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions()
{
	//判定対象AとBの座標
	Vector3 posA, posB;
	//判定対象のAとBの半径
	float radiusA,radiusB;

	//自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();

	//敵弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定
	//自キャラの座標
	posA = player_->GetLocalPosition();
	//自キャラの半径
	radiusA = player_->GetRadius();
	//自キャラと敵弾すべての当たり判定
	for (const std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullets)
	{
		//敵弾の座標
		posB = enemyBullet->GetLocalPosition();
		//敵弾の半径
		radiusB = enemyBullet->GetRadius();
		//座標AとBの距離を求める
		distance = posA - posB;

		//球と球の交差判定
		if((distance.x) * (distance.x) + (distance.y) * (distance.y) + (distance.z) * (distance.z) <= (radiusA + radiusB) * 2)
		{
			//自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			//敵弾の衝突時コールバックを呼び出す
			enemyBullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	//敵キャラの座標
	posA = enemy_->GetLocalPosition();
	//敵キャラの半径
	radiusA = enemy_->GetRadius();

	//自弾と敵キャラのすべての当たり判定
	for (const std::unique_ptr<PlayerBullet>& playerBullet : playerBullets)
	{
		//自弾の座標
		posB = playerBullet->GetLocalPosition();
		//敵弾の半径
		radiusB = playerBullet->GetRadius();
		//座標AとBの距離を求める
		distance = posA - posB;

		//球と球の交差判定
		if ((distance.x) * (distance.x) + (distance.y) * (distance.y) + (distance.z) * (distance.z) <= (radiusA + radiusB) * 2)
		{
			//自キャラの衝突時コールバックを呼び出す
			enemy_->OnCollision();
			//敵弾の衝突時コールバックを呼び出す
			playerBullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	//自弾と敵弾のすべての当たり判定
	for (const std::unique_ptr<PlayerBullet>& playerBullet : playerBullets)
	{
		//自弾の座標
		posB = playerBullet->GetLocalPosition();
		//敵弾の半径
		radiusB = playerBullet->GetRadius();

		for (const std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullets)
		{
			//敵弾の座標
			posB = enemyBullet->GetLocalPosition();
			//敵弾の半径
			radiusB = enemyBullet->GetRadius();
			//座標AとBの距離を求める
			distance = posA - posB;

			//球と球の交差判定
			if ((distance.x) * (distance.x) + (distance.y) * (distance.y) + (distance.z) * (distance.z) <= (radiusA + radiusB) * 2)
			{
				//自キャラの衝突時コールバックを呼び出す
				playerBullet->OnCollision();
				//敵弾の衝突時コールバックを呼び出す
				enemyBullet->OnCollision();
			}
		}
	}
#pragma endregion
}
