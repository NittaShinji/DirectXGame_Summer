#include "Block.h"
#include <cassert>

Block::Block() {}
Block::~Block() {}

//初期化
void Block::Initialize(Model* model, const Vector3& position)
{
	// NULLチェック
	assert(model);

	//モデルをインプット
	model_ = model;

	//テクスチャ読み込み
	blockHandle_ = TextureManager::Load("testBlock.png");
	selectHandle_ = TextureManager::Load("testBlock2.png");
	wasSelectHandle_ = TextureManager::Load("testBlock3.png");
	slimeHandle_ = TextureManager::Load("suraimu.png");

	////引数で受け取った速度をメンバ変数に代入
	//enemyVelocity_ = velocity;

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			worldTransforms_[i][j].Initialize();
			//引数で受け取った初期座標をセット
			worldTransforms_[i][j].translation_ = position;

			//カーソルの初期値を設定
			if (i == 1 && j == 1)
			{
				form_[i][j] = Form::IsSelected;
			}

			//モンスターの生成時間の設定
			birthTimer_[i][j] = kBirthTime;

			breakBlock[i][j] = false;
			//form_[i][j] = Form::IsSelected;

			if (i > 0)
			{
				worldTransforms_[i][j].translation_.x += i * (scaleX * 2);
				//if()
			}
			if (j > 0)
			{
				//if() 
				worldTransforms_[i][j].translation_.z += j * (scaleZ * 2);
			}

		}
	}

	wasChangedSelect = true;
	changedSelect = true;
	prevBlockX = 0;
	birthMonster = false;

}

//更新
void Block::Update()
{
	//monster_.remove_if([]);

	monsters_.remove_if([](std::unique_ptr<Monster>& monster) {
		return monster->IsDead();
		});


	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
#pragma region カーソル処理
			if (input_->PushKey(DIK_RIGHT) && i < blockWidth && changedSelect == true)
			{
				//前カーソルの状態からカーソル状態をなくして、カーソルがつく前の状態にする。
				if (form_[i][j] == Form::IsSelected)
				{
					//ブロック
					form_[i][j] = Form::Block;
					if (form_[i + 1][j] == Form::Block)
					{
						//ブロックカーソル
						form_[i + 1][j] = Form::IsSelected;
						changedSelect = false;
					}
					else if (form_[i + 1][j] == Form::None)
					{
						//空間カーソル
						form_[i + 1][j] = Form::WasSelected;
						changedSelect = false;
					}

				}
				else if (form_[i][j] == Form::WasSelected)
				{
					//空間
					form_[i][j] = Form::None;
					if (form_[i + 1][j] == Form::Block)
					{
						//ブロックカーソル
						form_[i + 1][j] = Form::IsSelected;
						changedSelect = false;
					}
					else if (form_[i + 1][j] == Form::None)
					{
						//空間カーソル
						form_[i + 1][j] = Form::WasSelected;
						changedSelect = false;
					}
				}
			}
			else if (input_->PushKey(DIK_LEFT) && i > 0 && changedSelect == true)
			{
				//if (form_[i][j] == Form::IsSelected || form_[i][j] == Form::WasSelected)
				//{
				//	form_[i][j] = Form::Block;
				//	//form_[i - 1][j] = Form::IsSelected;
				//	form_[prevBlockX][j] = Form::IsSelected;

				//	changedSelect = false;
				//}

				//前カーソルの状態からカーソル状態をなくして、カーソルがつく前の状態にする。
				if (form_[i][j] == Form::IsSelected)
				{
					//ブロック
					form_[i][j] = Form::Block;
					if (form_[prevBlockX][j] == Form::Block)
					{
						//ブロックカーソル
						form_[prevBlockX][j] = Form::IsSelected;
						changedSelect = false;
					}
					else if (form_[prevBlockX][j] == Form::None)
					{
						//空間カーソル
						form_[prevBlockX][j] = Form::WasSelected;
						changedSelect = false;
					}
				}
				else if (form_[i][j] == Form::WasSelected)
				{
					//空間
					form_[i][j] = Form::None;
					if (form_[prevBlockX][j] == Form::Block)
					{
						//ブロックカーソル
						form_[prevBlockX][j] = Form::IsSelected;
						changedSelect = false;
					}
					else if (form_[prevBlockX][j] == Form::None)
					{
						//空間カーソル
						form_[prevBlockX][j] = Form::WasSelected;
						changedSelect = false;
					}
				}
			}
			else if (input_->PushKey(DIK_UP) && j < blockHeight && changedSelect == true)
			{
				//前カーソルの状態からカーソル状態をなくして、カーソルがつく前の状態にする。
				if (form_[i][j] == Form::IsSelected)
				{
					//ブロック
					form_[i][j] = Form::Block;

					if (form_[i][j + 1] == Form::Block)
					{
						//ブロックカーソル
						form_[i][j + 1] = Form::IsSelected;
						changedSelect = false;
					}
					else if (form_[i][j + 1] == Form::None)
					{
						//空間カーソル
						form_[i][j + 1] = Form::WasSelected;
						changedSelect = false;
					}

				}
				else if (form_[i][j] == Form::WasSelected)
				{
					//空間
					form_[i][j] = Form::None;

					if (form_[i][j + 1] == Form::Block)
					{
						//ブロックカーソル
						form_[i][j + 1] = Form::IsSelected;
						changedSelect = false;
					}
					else if (form_[i][j + 1] == Form::None)
					{
						//空間カーソル
						form_[i][j + 1] = Form::WasSelected;
						changedSelect = false;
					}

				}

				/*if (form_[i][j] == Form::IsSelected || form_[i][j] == Form::WasSelected)
				{
					form_[i][j] = Form::Block;
					form_[i][j + 1] = Form::IsSelected;
					changedSelect = false;
				}*/
			}
			else if (input_->PushKey(DIK_DOWN) && j > 0 && changedSelect == true)
			{
				if (form_[i][j] == Form::IsSelected)
				{
					//ブロック
					form_[i][j] = Form::Block;

					if (form_[i][prevBlockY] == Form::Block)
					{
						//ブロックカーソル
						form_[i][prevBlockY] = Form::IsSelected;
						changedSelect = false;
					}
					else if (form_[i][prevBlockY] == Form::None)
					{
						//空間カーソル
						form_[i][prevBlockY] = Form::WasSelected;
						changedSelect = false;
					}
				}
				else if (form_[i][j] == Form::WasSelected)
				{
					//空間
					form_[i][j] = Form::None;

					if (form_[i][prevBlockY] == Form::Block)
					{
						//ブロックカーソル
						form_[i][prevBlockY] = Form::IsSelected;
						changedSelect = false;
					}
					else if (form_[i][prevBlockY] == Form::None)
					{
						//空間カーソル
						form_[i][prevBlockY] = Form::WasSelected;
						changedSelect = false;
					}

				}
			}
			else
			{

			}
#pragma endregion カーソル処理

			if (input_->PushKey(DIK_SPACE) && form_[i][j] == Form::IsSelected && wasChangedSelect == true)
			{
				form_[i][j] = Form::WasSelected;
				wasChangedSelect = false;
				//breakBlock[i][j] = true;
				breakBlock[i][j] = true;
				//breakBlock = true;
			}

			if (input_->PushKey(DIK_1))
			{
				if (j > 0 && j < 3)
				{
					if (i > 0 && i < 3)
					{
						form_[i][j] = Form::None;
					}
				}
			}

			if (breakBlock[i][j] == true)
			{
				if (form_[i][j] == Form::WasSelected)
				{
					//ワールド行列の平行移動成分を取得(ワールド座標)
					monsterPos = worldTransforms_[i][j].translation_;
				}

				birthMonster = true;
				breakBlock[i][j] = false;
			}

			//if (breakBlock = true)
			//{
			//	if (form_[i][j] == Form::WasSelected)
			//	{
			//		//ワールド行列の平行移動成分を取得(ワールド座標)
			//		monsterPos = worldTransforms_[i][j].translation_;
			//	}

			//	birthMonster[i][j] = true;
			//	//birthTimer_[i][j] = kBirthTime;
			//	//breakBlock = false;
			//}

			//if (breakBlock[i][j] == true)
			//{
			//	if (--birthTimer_[i][j] <= 0)
			//	{
			//		birthMonster[i][j] = true;
			//		Birth();
			//		//birthTimer_[i][j] = kBirthTime;
			//		breakBlock[i][j] = false;
			//	}
			//}

			//if (birthMonster[i][j] == true)
			//{
			//	
			//	//birthMonster[i][j] = false;
			//}

			//X座標の一つ前の番号を保存
			prevBlockY = j;
		}
		//Y座標の一つ前のブロック番号を保存
		prevBlockX = i;
	}

	if (birthMonster == true)
	{
		Birth();
		//birthTimer_[i][j] = kBirthTime;
		birthMonster = false;
	}

	//モンスター更新
	for (std::unique_ptr<Monster>& monster : monsters_)
	{
		monster->Update();
	}

	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{

#pragma region 行列の更新
			//スケーリング用行列を宣言
			Matrix4 matScale;
			//合成用回転行列を宣言
			Matrix4 matRot;
			//各軸用回転用行列を宣言
			Matrix4 matRotX, matRotY, matRotZ;
			//座標用行列を宣言
			Matrix4 matTrans = MathUtility::Matrix4Identity();

			//スケーリング倍率を行列に設定
			matScale.Matrix4Scaling(scaleX, scaleY, scaleZ);

			//回転行列を行列に設定
			matRot.Matrix4RotationX(worldTransforms_[i][j].rotation_.x);
			matRot.Matrix4RotationY(worldTransforms_[i][j].rotation_.y);
			matRot.Matrix4RotationZ(worldTransforms_[i][j].rotation_.z);

			worldTransforms_[i][j].matWorld_.IdentityMatrix();

			//座標を行列に設定
			matTrans.Matrix4Translation(
				worldTransforms_[i][j].translation_.x,
				worldTransforms_[i][j].translation_.y,
				worldTransforms_[i][j].translation_.z);

			worldTransforms_[i][j].matWorld_ = matScale;
			worldTransforms_[i][j].matWorld_ *= matRot;
			worldTransforms_[i][j].matWorld_ *= matTrans;

			//WorldMatrix(worldTransform_.matWorld_, matScale, matRot, matTrans);

			//親行列と掛け算代入
			/*if (worldTransform_.parent_ != nullptr)
			{
				worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;
			}*/

			//レールカメラのワールド行列を取得
			/*Matrix4 railCameraMatWorld = railCamera_->GetMatWorld();
			worldTransform_.matWorld_ *= railCameraMatWorld;*/

			//worldTransform_.matWorld_ *= 

			//ワールド行列を転送
			worldTransforms_[i][j].TransferMatrix();


#pragma endregion

		}
	}

#pragma region リセット処理

	if (changedSelect == false)
	{
		if (--selectTimer_ <= 0)
		{
			changedSelect = true;
			selectTimer_ = kSelectTime;
		}
	}

	if (wasChangedSelect == false)
	{
		if (--wasSelectTimer_ <= 0)
		{
			wasChangedSelect = true;
			wasSelectTimer_ = kSelectTime;
		}
	}

#pragma endregion

#pragma region デバッグテキスト
	/*debugText_->SetPos(50, 200);
	debugText_->Printf("worldTranslation[0]:(%f,%f,%f)",
		worldTransforms_[1][1].translation_.x, worldTransforms_[1][1].translation_.y, worldTransforms_[1][1].translation_.z);*/

	debugText_->SetPos(50, 200);
	debugText_->Printf("Form:(%d)",
		form_[1][1]);

	/*debugText_->SetPos(50, 250);
	debugText_->Printf("breakBlock[1][0]:(%d)",
		breakBlock[1][0]);
	debugText_->SetPos(50, 280);
	debugText_->Printf("breakBlock[1][1]:(%d)",
		breakBlock[1][1]);
	debugText_->SetPos(50, 310);
	debugText_->Printf("breakBlock[1][2]:(%d)",
		breakBlock[1][2]);*/

	debugText_->SetPos(50, 350);
	debugText_->Printf("birthTimer_[1][0]:(%d)",
		birthTimer_[1][0]);
	debugText_->SetPos(50, 380);
	debugText_->Printf("birthTimer_[1][1]:(%d)",
		birthTimer_[1][1]);
	debugText_->SetPos(50, 410);
	debugText_->Printf("birthTimer_[1][2]:(%d)",
		birthTimer_[1][2]);

	debugText_->SetPos(50, 450);
	debugText_->Printf("birthMonster[1][0]:(%d)",
		breakBlock[1][0]);
	debugText_->SetPos(50, 480);
	debugText_->Printf("birthMonster[1][1]:(%d)",
		breakBlock[1][1]);
	debugText_->SetPos(50, 510);
	debugText_->Printf("birthMonster[1][2]:(%d)",
		breakBlock[1][2]);
#pragma endregion

}
//描画
void Block::Draw(const ViewProjection& viewProjection)
{
	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			switch (form_[i][j])
			{

			case Form::WasSelected:

				//model_->Draw(worldTransforms_[i][j], viewProjection, wasSelectHandle_);
				break;

			case Form::Block:

				model_->Draw(worldTransforms_[i][j], viewProjection, blockHandle_);

				break;

			case Form::IsSelected:

				model_->Draw(worldTransforms_[i][j], viewProjection, selectHandle_);
				break;
			case Form::None:
				break;

			default:
				break;
			}
		}
	}

	//モンスター描画
	for (std::unique_ptr<Monster>& monster : monsters_)
	{
		monster->Draw(viewProjection);
	}
}

Vector3 Block::GetSelectPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;

	//現在カーソルがある位置を渡す
	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			if (form_[i][j] == Form::IsSelected || form_[i][j] == Form::WasSelected)
			{
				//ワールド行列の平行移動成分を取得(ワールド座標)
				worldPos = worldTransforms_[i][j].translation_;
			}
		}
	}

	return worldPos;
}

//モンスターの当たり判定用に壁の座標を渡す関数
//Vector3 Block::GetBlockPosition()
//{
//	//ローカル座標を入れる変数
//	Vector3 monsterPos[blockWidth][blockHeight];
//
//	//全ての壁の座標を渡す
//	for (int i = 0; i < blockWidth; i++)
//	{
//		for (int j = 0; j < blockHeight; j++)
//		{
//			//ワールド行列の平行移動成分を取得(ワールド座標)
//			monsterPos[i][j] = worldTransforms_[i][j].translation_;
//			return monsterPos[i][j];
//		}
//	}
//	
//}

void Block::GetWorldPosition(Vector3 blockPos[blockWidth][blockHeight])
{
	/*Vector3 monsterPos[blockWidth][blockHeight];*/

	Vector3 givePos;

	//全ての壁の座標を渡す
	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			//ワールド行列の平行移動成分を取得(ワールド座標)
			//blockPos[i][j] = worldTransforms_[i][j].translation_;
			blockPos[i][j].x = worldTransforms_[i][j].matWorld_.m[3][0];
			blockPos[i][j].y = worldTransforms_[i][j].matWorld_.m[3][1];
			blockPos[i][j].z = worldTransforms_[i][j].matWorld_.m[3][2];
		}
	}

	//blockPos = monsterPos;
}

bool Block::GetBirthMonster()
{
	return breakBlock;
}

void Block::Birth()
{
	//モンスターを生成し、初期化
		//PlayerBullet* newBullet = new PlayerBullet();
	std::unique_ptr<Monster> newMonster = std::make_unique<Monster>();
	//newBullet->Initialize(model_ ,worldTransform_.translation_,velocity);

	//モンスターの速度
	const float kMonsterSpeed = 1.0f;
	Vector3 velocity(0, 0, kMonsterSpeed);

	//for (int i = 0; i < blockWidth; i++)
	//{
	//	for (int j = 0; j < blockHeight; j++)
	//	{
	//		if (breakBlock == true)
	//		{
	//			if (form_[i][j] == Form::WasSelected)
	//			{
	//				//ワールド行列の平行移動成分を取得(ワールド座標)
	//				monsterPos = worldTransforms_[i][j].translation_;
	//			}
	//		}
	//	}
	//}

	////自機の平行移動成分の情報を取得
	//worldMonsterTransform.x = worldTransform_.matWorld_.m[3][0];
	//worldMonsterTransform.y = worldTransform_.matWorld_.m[3][1];
	//worldMonsterTransform.z = worldTransform_.matWorld_.m[3][2];

	//モンスターを生成
	newMonster->Initialize(model_, monsterPos, velocity, birthMonster);

	//モンスターを登録する
	monsters_.push_back(std::move(newMonster));
}

//bool Block::OnCollision(Vector3 wallPos[blockWidth][blockHeight],const int x, const int y)
bool Block::OnCollision(const int x, const int y)
{
	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			/*if (worldTransforms_[i][j].translation_.x == collisionPos.x)
			{
				if (worldTransforms_[i][j].translation_.y == collisionPos.y)
				{
					if (worldTransforms_[i][j].translation_.z == collisionPos.z)
					{
						if (form_[i][j] == Form::WasSelected || form_[i][j] == Form::None)
						{
							result = true;
							return result;
						}
						else if(form_[i][j] == Form::Block || form_[i][j] == Form::IsSelected)
						{
							result = false;
							return result;
						}
					}
				}
			}*/

			//進める
			if (form_[x][y] == Form::WasSelected || form_[x][y] == Form::None)
			{
				return true;
			}
			//進めない
			else if (form_[x][y] == Form::Block || form_[x][y] == Form::IsSelected)
			{
				return false;
			}
		}
	}
}

float Block::GetRadius()
{
	//半径を入れる変数
	float blockRadius;
	//ワールド行列の平行移動成分を取得(ワールド座標)
	blockRadius = radius;
	return blockRadius;
}

//壁の横の長さを渡す
int Block::GetBlockWidth()
{
	//壁の横の長さを入れる変数
	float blockWidth_;
	//ワールド行列の平行移動成分を取得(ワールド座標)
	blockWidth_ = blockWidth;
	return blockWidth_;
}
//壁の縦の長さを渡す
int Block::GetBlockHight()
{
	//壁の横の長さを入れる変数
	float blockHight_;
	//ワールド行列の平行移動成分を取得(ワールド座標)
	blockHight_ = blockHeight;
	return blockHight_;
}
