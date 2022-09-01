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
	textureHandle_ = TextureManager::Load("maou.png");

	//audio_->LoadWave("GetSE.wav");
	clickAudio_ = audio_->LoadWave("GetSE.wav");
	

	// 3Dモデルの生成
	model_ = Model::Create();
	modelSkydome_ = Model::CreateFromOBJ("BackGround", true);

	////自キャラの生成
	//player_ = new Player();
	////自キャラの初期化
	//player_->Initialize(model_ ,textureHandle_);

	//レールカメラの生成
	//RailCamera* newRailCamera = new RailCamera;
	//railCamera_.reset(newRailCamera);
	////レールカメラの初期化
	//railCamera_->Initialize(railPos, angle);

	//シーン関連のもの
	Scene* newScene = new Scene;
	scene_.reset(newScene);

	//ブロックの生成
	Block* newBlock = new Block;
	block_.reset(newBlock);

	//カーソルの生成
	Select* newSelect = new Select;
	select_.reset(newSelect);

	//自キャラの生成と登録
	Player* newPlayer = new Player;
	player_.reset(newPlayer);

	//天球の生成
	Skydome* newSkydome = new Skydome;
	skydome_.reset(newSkydome);

	//スコアの生成
	Score* newScore = new Score;
	score_.reset(newScore);

	AirCamera* newAirCamera = new AirCamera;
	airCamera_.reset(newAirCamera);

	////自キャラにレールカメラのアドレスを渡す
	//player_->SetRailCamera(railCamera_);

	//敵の生成
	//Monster* newMonster = new Monster();
	//Monster_.reset(newMonster);	
	////敵の初期化
	//Monster_->Initialize(model_,enemyPos);
	//monster_ = new Monster;
	//敵キャラに自キャラのアドレスを渡す
	//Monster_->SetPlayer(player_);


	airCamera_->Initialize(airPos, airAngle);
	//スコアの初期化
	score_->Initialize(model_);
	//シーン関連のものを初期化
	scene_->Initialize(model_);
	//ブロックの初期化
	block_->Initialize(model_, monsterPos);
	//選択カーソルの初期化
	select_->Initialize(model_);
	//自キャラの初期化
	player_->Initialize(model_, textureHandle_);
	//天球の初期化
	skydome_->Initialize(modelSkydome_);

	//カーソルにブロックのアドレスを渡す
	select_->SetBlock(block_);

	/*Monster* monster__ = new Monster;
	monster__->SetPlayer(player_);
	delete monster__;*/

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//カメラ視点座標を設定
	viewProjection_.eye = { 0,20,-20 };

	limitTimer_ = kLifeTime;

	//// 軸方向表示を有効にする
	//AxisIndicator::GetInstance()->SetVisible(true);

	//// 軸方向表示が参照するビュープロジェクションを指定する (アドレス渡し)
	//AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//// ライン描画が参照するビュープロジェクションを指定する (アドレス渡し)
	//PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);


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


}

void GameScene::Update()
{
	assert(player_);
	//assert(Monster_);
	assert(block_);



	switch (scene)
	{
	case::TITLE:
		debugCamera_->Update();
		scene_->Update();

		if (input_->PushKey(DIK_SPACE))
		{
			audio_->PlayWave(clickAudio_, false, 1.0f);
			scene = GAME;
		}
		viewProjection_ = debugCamera_->GetViewProjection();
		//行列の再計算
		viewProjection_.UpdateMatrix();


		break;

	case::GAME:
		airCamera_->Update();
		debugCamera_->Update();
		scene_->Update();
		player_->Update();
		block_->Update();
		//Monster_->Update();
		skydome_->Update();
		select_->Update();
		//CheckScore();
		score_->Update();


		//railCamera_->Update();
		//スコアを受け取る
		//scoreMonster = monster->SetScore();
		//scoreBlock = block_->GetScore();
		//score = score_->GetScore();

		CheckAllCollisions();
		//viewProjection_ = debugCamera_->GetViewProjection();
		if (input_->PushKey(DIK_Q))
		{
			viewProjection_ = airCamera_->GetViewProjection();
		}
		
		//行列の再計算
		viewProjection_.UpdateMatrix();

		if (block_->GetBreakAll() == true && deadMonsterCount >= (blockWidth * blockHeight))
		{
			scene = GAMECLEAR;
		}


		/*if (input_->PushKey(DIK_T))
		{
			audio_->PlayWave(clickAudio_, false, 1.0f);
			scene = GAMECLEAR;
		}
		if (input_->PushKey(DIK_Y))
		{
			audio_->PlayWave(clickAudio_, false, 1.0f);
			scene = GAMEOVER;
		}*/

		if (player_->loseGame() == true)
		{
			viewProjection_ = debugCamera_->GetViewProjection();
			scene = GAMEOVER;
		}

		/*if (--limitTimer_ <= 0)
		{
			scene = GAMEOVER;
		}*/

		break;

	case::GAMECLEAR:
		
		if (input_->PushKey(DIK_SPACE))
		{
			resetDeath();
			reset();
			scene = TITLE;
		}

		break;
	case::GAMEOVER:
		
		debugCamera_->Update();
		if (input_->PushKey(DIK_SPACE))
		{
			resetDeath();
			reset();
			scene = TITLE;
		}

		break;

	default:
		break;
	}
	//switch (scene)
	//{
	//case::Title:
	//	debugCamera_->Update();
	//	scene_->Update();
	//	break;

	//case::Game:

	//	player_->Update();
	//	block_->Update();
	//	//Monster_->Update();
	//	skydome_->Update();
	//	select_->Update();
	//	//railCamera_->Update();


	//	CheckAllCollisions();

	//	debugText_->SetPos(50, 640);
	//	debugText_->Printf("possibleMove:(%d)",
	//		possibleMove);

	//	break;

	//default:
	//	break;
	//}


	//viewProjection_ = railCamera_->GetViewProjection();




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

	debugText_->SetPos(50, 640);
	debugText_->Printf("scene:(%d)",
		scene);
	debugText_->SetPos(50, 670);
	debugText_->Printf(" deadMonsterCount:(%d)",
		deadMonsterCount);
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

	switch (scene)
	{
	case::TITLE:
		scene_->TitleDraw(viewProjection_);
		break;
	case::GAME:
		scene_->GameDraw(viewProjection_);
		player_->Draw(viewProjection_, textureHandle_);
		//Monster_->Draw(viewProjection_);
		skydome_->Draw(viewProjection_);
		block_->Draw(viewProjection_);
		select_->Draw(viewProjection_);
		score_->Draw(viewProjection_);
		break;
	case::GAMECLEAR:
		scene_->ClearDraw(viewProjection_);
		break;
	case::GAMEOVER:
		scene_->LoseDraw(viewProjection_);
		break;

	default:
		break;
	}

	//switch 

	//switch (scene)
	//{
	//case::Title:
	//	scene_->Draw(viewProjection_);

	//	break;
	//case::Game:
	//	player_->Draw(viewProjection_, textureHandle_);
	//	//Monster_->Draw(viewProjection_);
	//	skydome_->Draw(viewProjection_);
	//	block_->Draw(viewProjection_);
	//	select_->Draw(viewProjection_);

	//	break;
	//default:
	//	break;
	//}


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

void GameScene::resetDeath()
{
	//モンスターリストの取得
	const std::list<std::unique_ptr<Monster>>& monsters = block_->GetMonsters();

	//モンスターすべて
	for (const std::unique_ptr<Monster>& monster : monsters)
	{
		monster->Dead();
		//敵弾リストの取得
		const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = monster->GetBullets();
		for (const std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullets)
		{
			enemyBullet->Dead();
		}
	}

}

void GameScene::CheckAllCollisions()
{
	//判定対象AとBの座標
	Vector3 posA, posB;

	//壁用の配列変数
	int wallWidth = block_->GetBlockWidth();
	int wallHeight = block_->GetBlockHight();

	int onCollisionX = 0;
	int onCollisionY = 0;

	//壁用の座標
	//Vector3 posWall[wallWidth][wallHeight];
	Vector3 posWall;

	//判定対象のAとBの半径
	float radiusA, radiusB;

	//勇者の弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();

	//モンスターリストの取得
	const std::list<std::unique_ptr<Monster>>& monsters = block_->GetMonsters();



#pragma region 勇者とモンスターの当たり判定
	//勇者の座標
	posA = player_->GetLocalPosition();
	//勇者の半径
	radiusA = player_->GetRadius();
	//勇者とモンスターすべての当たり判定
	for (const std::unique_ptr<Monster>& monster : monsters)
	{
		//モンスターの座標
		posB = monster->GetLocalPosition();
		//モンスターの半径
		radiusB = monster->GetRadius();
		//座標AとBの距離を求める
		distance = posA - posB;

		//球と球の交差判定
		if ((distance.x) * (distance.x) + (distance.y) * (distance.y) + (distance.z) * (distance.z) <= (radiusA + radiusB) * 2)
		{
			//自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			//敵弾の衝突時コールバックを呼び出す
			monster->OnCollision();
		}
	}
#pragma endregion
	//
#pragma region 勇者の弾とモンスターの当たり判定
	////モンスターの座標
	//posA = monsters->GetLocalPosition();
	////モンスターの半径
	//radiusA = monsters->GetRadius();

	////勇者の弾と敵キャラのすべての当たり判定
	//for (const std::unique_ptr<PlayerBullet>& playerBullet : playerBullets)
	//{
	//	//勇者の弾の座標
	//	posB = playerBullet->GetLocalPosition();
	//	//敵弾の半径
	//	radiusB = playerBullet->GetRadius();
	//	//座標AとBの距離を求める
	//	distance = posA - posB;

	//	//球と球の交差判定
	//	if ((distance.x) * (distance.x) + (distance.y) * (distance.y) + (distance.z) * (distance.z) <= (radiusA + radiusB) * 2)
	//	{
	//		//自キャラの衝突時コールバックを呼び出す
	//		Monster_->OnCollision();
	//		//敵弾の衝突時コールバックを呼び出す
	//		playerBullet->OnCollision();
	//	}
	//}
#pragma endregion
//
#pragma region 勇者の弾とモンスターの当たり判定
	//勇者の弾とモンスターのすべての当たり判定
	for (const std::unique_ptr<PlayerBullet>& playerBullet : playerBullets)
	{
		//勇者の弾の座標
		posA = playerBullet->GetLocalPosition();
		//敵弾の半径
		radiusA = playerBullet->GetRadius();

		for (const std::unique_ptr<Monster>& monster : monsters)
		{
			//モンスターの座標
			posB = monster->GetLocalPosition();
			//モンスターの半径
			radiusB = monster->GetRadius();
			//座標AとBの距離を求める
			distance = posA - posB;

			//球と球の交差判定
			if ((distance.x) * (distance.x) + (distance.y) * (distance.y) + (distance.z) * (distance.z) <= (radiusA + radiusB) * 2)
			{

				//モンスターの衝突時コールバックを呼び出す
				monster->OnCollision();
				//勇者の弾の衝突時コールバックを呼び出す
				playerBullet->OnCollision();
				nowCollision = true;
			}
		}
	}

	if (nowCollision == true)
	{
		deadMonsterCount += 1;
		nowCollision = false;
	}
#pragma endregion

#pragma region モンスターと壁の当たり判定
	//壁の座標
	block_->GetWorldPosition(wallPos);

	//壁の半径
	radiusA = block_->GetRadius();

	//勇者とモンスターすべての当たり判定
	for (const std::unique_ptr<Monster>& monster : monsters)
	{
		for (int i = 0; i < blockWidth; i++)
		{
			for (int j = 0; j < blockHeight; j++)
			{
				//モンスターの座標
				posB = monster->GetWorldPosition();
				//モンスターの半径
				radiusB = monster->GetRadius();
				//座標AとBの距離を求める
				distance = wallPos[i][j] - posB;

				//壁の状態で分岐させる

				//球と球の交差判定
				if ((distance.x) * (distance.x) + (distance.y) * (distance.y) + (distance.z) * (distance.z) <= (radiusA + radiusB) * 2)
				{
					//当たった時のブロックのX番号,Y番号を保存
					onCollisionX = i;
					onCollisionY = j;

					//壁の衝突時コールバックを呼び出す
					//possibleMove = block_->OnCollision(wallPos,onCollisionX,onCollisionY);
					possibleMove[1] = block_->OnCollision(onCollisionX, onCollisionY);
					monster->GetIsMove(possibleMove);
				}
			}
		}

		//possibleMove = block_->OnCollision(onCollisionX, onCollisionY);

		////進めるなら移動関数を呼び出す
		//if (possibleMove == true)
		//{
		//	//モンスターの衝突時コールバックを呼び出す
		//	monster->OnCollisionMove();
		//}
		////進めないなら方向転換関数を呼びだす
		//else if (possibleMove == false)
		//{
		//	monster->ChangeDirection();
		//}
	}
#pragma endregion

#pragma region モンスターの弾と勇者の当たり判定ok

	//勇者の座標
	posA = player_->GetWorldPosition();
	//勇者の半径
	radiusA = player_->GetRadius();

	for (const std::unique_ptr<Monster>& monster : monsters)
	{
		//敵弾リストの取得
		const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = monster->GetBullets();
		for (const std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullets)
		{
			//モンスターの弾の座標
			posB = enemyBullet->GetWorldPosition();
			//モンスターの弾の半径
			radiusB = enemyBullet->GetRadius();

			//座標AとBの距離を求める
			distance = posA - posB;

			//球と球の交差判定
			if ((distance.x) * (distance.x) + (distance.y) * (distance.y) + (distance.z) * (distance.z) <= (radiusA + radiusB) * 12)
			{
				enemyBullet->OnCollision();
				player_->OnCollision();
			}
		}
	}
#pragma endregion

#pragma region モンスターの弾と壁の当たり判定ok

	//壁の座標
	block_->GetWorldPosition(wallPos);

	//壁の半径
	radiusA = block_->GetRadius();

	//壁とモンスタの弾のすべての当たり判定
	for (const std::unique_ptr<Monster>& monster : monsters)
	{
		//敵弾リストの取得
		const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = monster->GetBullets();
		for (const std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullets)
		{
			//モンスターの弾の座標
			posB = enemyBullet->GetWorldPosition();
			//モンスターの弾の半径
			radiusB = enemyBullet->GetRadius();

			for (int i = 0; i < blockWidth; i++)
			{
				for (int j = 0; j < blockHeight; j++)
				{
					//座標AとBの距離を求める
					distance = wallPos[i][j] - posB;

					//球と球の交差判定
					if ((distance.x) * (distance.x) + (distance.y) * (distance.y) + (distance.z) * (distance.z) <= (radiusA + radiusB) * 2)
					{
						//当たった時のブロックのX番号,Y番号を保存
						onCollisionX = i;
						onCollisionY = j;

						//壁の衝突時コールバックを呼び出す
						//possibleMove = block_->OnCollision(wallPos,onCollisionX,onCollisionY);
						possibleMove[2] = block_->OnCollision(onCollisionX, onCollisionY);
						//monster->GetIsMove(possibleMove);
						if (possibleMove[2] == false)
						{
							enemyBullet->OnCollisionStop();
							break;
						}
					}
				}
			}
		}
	}
#pragma endregion

#pragma region プレイヤーの弾と壁の当たり判定

	//壁の座標
	block_->GetWorldPosition(wallPos);

	//壁の半径
	radiusA = block_->GetRadius();

	//壁とモンスタの弾のすべての当たり判定

	for (const std::unique_ptr<PlayerBullet>& playerBullet : playerBullets)
	{
		//勇者の弾の座標
		posB = playerBullet->GetWorldPosition();
		//敵弾の半径
		radiusB = playerBullet->GetRadius();

		for (int i = 0; i < blockWidth; i++)
		{
			for (int j = 0; j < blockHeight; j++)
			{
				//座標AとBの距離を求める
				distance = wallPos[i][j] - posB;

				//球と球の交差判定
				if ((distance.x) * (distance.x) + (distance.y) * (distance.y) + (distance.z) * (distance.z) <= (radiusA + radiusB) * 19)
				{
					//当たった時のブロックのX番号,Y番号を保存
					onCollisionX = i;
					onCollisionY = j;

					//壁の衝突時コールバックを呼び出す
					//possibleMove = block_->OnCollision(wallPos,onCollisionX,onCollisionY);
					possibleMove[3] = block_->OnCollision(onCollisionX, onCollisionY);

					if (possibleMove[3] == false)
					{
						playerBullet->OnCollisionStop();
						break;
					}
				}
			}
		}
	}
#pragma endregion
		//
		//		//モンスターの座標
		//		posB = monster->GetWorldPosition();
		//		//モンスターの半径
		//		radiusB = monster->GetRadius();
		//		//座標AとBの距離を求める
		//		distance = wallPos[i][j] - posB;

		//		//壁の状態で分岐させる

		//		//球と球の交差判定
		//		if ((distance.x) * (distance.x) + (distance.y) * (distance.y) + (distance.z) * (distance.z) <= (radiusA + radiusB) * 2)
		//		{
		//			//当たった時のブロックのX番号,Y番号を保存
		//			onCollisionX = i;
		//			onCollisionY = j;

		//			//壁の衝突時コールバックを呼び出す
		//			//possibleMove = block_->OnCollision(wallPos,onCollisionX,onCollisionY);
		//			possibleMove = block_->OnCollision(onCollisionX, onCollisionY);
		//			monster->GetIsMove(possibleMove);
		//		}
		//	}
		//}

		//possibleMove = block_->OnCollision(onCollisionX, onCollisionY);

		////進めるなら移動関数を呼び出す
		//if (possibleMove == true)
		//{
		//	//モンスターの衝突時コールバックを呼び出す
		//	monster->OnCollisionMove();
		//}
		////進めないなら方向転換関数を呼びだす
		//else if (possibleMove == false)
		//{
		//	monster->ChangeDirection();
		//}
	


	//for (const std::unique_ptr<Monster>& monster : monsters)
	//{
	//	//モンスターの座標
	//	posA = monster->GetLocalPosition();
	//	//モンスターの半径
	//	radiusA = monster->GetRadius();

	//	//敵弾リストの取得
	//	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = monster->GetBullets();

	//	for (const std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullets)
	//	{
	//		//勇者の弾の座標
	//		posB = enemyBullet->GetLocalPosition();
	//		//敵弾の半径
	//		radiusB = enemyBullet->GetRadius();
	//	}

	//	//座標AとBの距離を求める
	//	distance = posA - posB;

	//	//球と球の交差判定
	//	if ((distance.x) * (distance.x) + (distance.y) * (distance.y) + (distance.z) * (distance.z) <= (radiusA + radiusB) * 2)
	//	{
	//		//敵弾の衝突時コールバックを呼び出す
	//		monster->OnCollision();
	//	}

	//}

}

void GameScene::CheckScore()
{
	//モンスターリストの取得
	const std::list<std::unique_ptr<Monster>>& monsters = block_->GetMonsters();

	for (const std::unique_ptr<Monster>& monster : monsters)
	{
		scoreMonster += monster->GetScore();
	}

	//scoreBlock = block_->GetScore()
	score_->GetScore(scoreMonster);

}

void GameScene::reset()
{
	airCamera_->Initialize(airPos, airAngle);
	//スコアの初期化
	score_->Initialize(model_);
	//シーン関連のものを初期化
	scene_->Initialize(model_);
	//ブロックの初期化
	block_->Initialize(model_, monsterPos);
	//選択カーソルの初期化
	select_->Initialize(model_);
	//自キャラの初期化
	player_->Initialize(model_, textureHandle_);
	//天球の初期化
	skydome_->Initialize(modelSkydome_);
	//カメラ視点座標を設定
	viewProjection_.eye = { 0,0,0 };

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	limitTimer_ = kLifeTime;
	deadMonsterCount = 0;
}