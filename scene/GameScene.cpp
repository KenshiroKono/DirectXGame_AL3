#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include<random>
using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	textureHandle_ = TextureManager::Load("mario.jpg");
	model_ = Model::Create();


	//乱数シード設定
	std::random_device speed_gen;
	//メルセンヌ・ツイスター
	std::mt19937_64 engine(speed_gen());
	//乱数範囲(回転角用)
	std::uniform_real_distribution<float> rotDist(0.0f, XM_2PI);
	//乱数範囲(座標用)
	std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);



	//X.Y.Z方向のスケーリングを設定
	worldTransform_[0].scale_ = { 1.0f, 1.0f, 1.0f };
	//X.Y.Z方向の回転角
	worldTransform_[0].rotation_ = { 0,0,0 };
	//X.Y.Z方向の平行移動
	worldTransform_[0].translation_ = { 0,0,0 };

	//ワールドトランスフォーム 初期化

	//大元	
	worldTransform_[PartId::Root].Initialize();

	//脊椎
	worldTransform_[PartId::Spine].translation_ = { 0,-4.0f,0 };
	worldTransform_[PartId::Spine].parent_ = &worldTransform_[PartId::Root];
	worldTransform_[PartId::Spine].Initialize();

	//上半身

	//胴体
	worldTransform_[PartId::Chest].translation_ = { 0,1.5f,0 };
	worldTransform_[PartId::Chest].parent_ = &worldTransform_[PartId::Spine];
	worldTransform_[PartId::Chest].Initialize();
	//頭
	worldTransform_[PartId::Head].translation_ = { 0,2.5f,0 };
	worldTransform_[PartId::Head].parent_ = &worldTransform_[PartId::Chest];
	worldTransform_[PartId::Head].Initialize();
	//左腕
	worldTransform_[PartId::ArmL].translation_ = { 2.5f,0,0 };
	worldTransform_[PartId::ArmL].parent_ = &worldTransform_[PartId::Chest];
	worldTransform_[PartId::ArmL].Initialize();
	//右腕
	worldTransform_[PartId::ArmR].translation_ = { -2.5f,0,0 };
	worldTransform_[PartId::ArmR].parent_ = &worldTransform_[PartId::Chest];
	worldTransform_[PartId::ArmR].Initialize();


	//下半身

	//腰部
	worldTransform_[PartId::Hip].translation_ = { 0,-2.5f,0 };
	worldTransform_[PartId::Hip].parent_ = &worldTransform_[PartId::Spine];
	worldTransform_[PartId::Hip].Initialize();
	//左足
	worldTransform_[PartId::LegL].translation_ = { 2.5f,-1.5,0 };
	worldTransform_[PartId::LegL].parent_ = &worldTransform_[PartId::Hip];
	worldTransform_[PartId::LegL].Initialize();
	//右足
	worldTransform_[PartId::LegR].translation_ = { -2.5f,-1.5,0 };
	worldTransform_[PartId::LegR].parent_ = &worldTransform_[PartId::Hip];
	worldTransform_[PartId::LegR].Initialize();


	/*
	//カメラの視点座標を設定
	viewProjection_.eye = { 0,0,-60 };
	//カメラの注視点座標を設定
	viewProjection_.target = { 10,0,0 };
	//カメラの上方向ベクトルを設定(右上45度指定)
	viewProjection_.up = { cosf(XM_PI / 4.0f),sinf(XM_PI / 4.0f),0.0f };
	*/

	//カメラの垂直方向視野角を設定
	viewProjection_.fovAngleY = XMConvertToRadians(60.0f);
	//アス比を設定
	//wviewProjection_.aspectRatio = 1.0f;
	//ニアクリップ距離を設定
	viewProjection_.nearZ = 30.0f;
	//ファークリップ距離を設定
	viewProjection_.farZ = 53.0f;
	//ビュープロジェクション 初期化
	viewProjection_.Initialize();
}

void GameScene::Update() {

	//視点移動処理
	/*
			#pragma region Eye
				{
					//視点の移動ベクトル
					XMFLOAT3 move = { 0,0,0 };

					//視点移動の速さ
					const float kEyespeed = 0.2f;

					//押した方向にベクトルを変更
					if (input_->PushKey(DIK_W)) {
						move = { 0,0,kEyespeed };
					}
					else if (input_->PushKey(DIK_S)) {
						move = { 0,0,-kEyespeed };
					}

					if (input_->PushKey(DIK_D)) {
						move = { kEyespeed,0,0 };
					}
					else if (input_->PushKey(DIK_A)) {
						move = { -kEyespeed,0,0 };
					}

					//視点の移動(ベクトルを加算)
					viewProjection_.eye.x += move.x;
					viewProjection_.eye.y += move.y;
					viewProjection_.eye.z += move.z;

					//行列の再計算
					viewProjection_.UpdateMatrix();
				}
			#pragma endregion
	*/

	//注視点移動処理
	/*
	#pragma region Target
		{
			//注視点のベクトル
			XMFLOAT3 move = { 0,0,0 };

			//注視点の移動速度
			const float kTargetSpeed = 0.2f;

			//押した方向にベクトルを変更
			if (input_->PushKey(DIK_LEFT)) {
				move = { -kTargetSpeed,0,0 };
			}
			else if (input_->PushKey(DIK_RIGHT)) {
				move = { kTargetSpeed,0,0 };
			}

			//注視点を移動(ベクトルを加算)
			viewProjection_.target.x += move.x;
			viewProjection_.target.y += move.y;
			viewProjection_.target.z += move.z;

			//行列の再計算
			viewProjection_.UpdateMatrix();
		}
	#pragma endregion
	*/

	//上方向回転処理
	/*
		#pragma region UpRot
			{
				//上方向の回転の速さ[ラジアン/f]
				const float kUpRotSpeed = 0.05f;

				//押した方向で移動ベクトルを変更
				if (input_->PushKey(DIK_SPACE)) {
					viewAngle += kUpRotSpeed;
					//2πを超えたら0に戻す
					viewAngle = fmodf(viewAngle, XM_2PI);
				}

				//上方向のベクトルを計算(半径１の円周上の座標)
				viewProjection_.up = { cosf(viewAngle),sinf(viewAngle),0.0f };

				//行列の再計算
				viewProjection_.UpdateMatrix();
			}

		#pragma endregion
	*/

	//FoV変更処理
#pragma region Fov
	{
		if (input_->PushKey(DIK_S)) {//Sキーで視野角が広がる
			viewProjection_.fovAngleY += 0.01f;
			viewProjection_.fovAngleY = min(viewProjection_.fovAngleY, XM_PI);
		}
		else if (input_->PushKey(DIK_W)) {//Wキーで視野角が狭まる
			viewProjection_.fovAngleY -= 0.01f;
			viewProjection_.fovAngleY = max(viewProjection_.fovAngleY, 0.01f);
		}
		//行列の再計算
		viewProjection_.UpdateMatrix();
	}

#pragma endregion

	//クリップ距離変更処理

#pragma region Near
	{
		if (input_->PushKey(DIK_UP)) {
			viewProjection_.nearZ += 0.1f;
		}
		else if (input_->PushKey(DIK_DOWN)) {
			viewProjection_.nearZ -= 0.1f;
		}
		//行列の再計算
		viewProjection_.UpdateMatrix();
	}
#pragma endregion

	//キャラクターの移動処理
#pragma region Character

	//キャラクターの移動ベクトル
	XMFLOAT3 move = { 0, 0, 0 };
	//キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;
	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_LEFT)) {
		move = { -kCharacterSpeed, 0,0 };
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		move = { kCharacterSpeed, 0,0 };
	}

	//回転処理
	const float  kChestRotSpeed = 0.05f;
	//上半身の回転
	if (input_->PushKey(DIK_U)) {
		worldTransform_[PartId::Chest].rotation_.y += kChestRotSpeed;
	}
	else if (input_->PushKey(DIK_I)) {
		worldTransform_[PartId::Chest].rotation_.y -= kChestRotSpeed;
	}

	//下半身の回転
	if (input_->PushKey(DIK_J)) {
		worldTransform_[PartId::Hip].rotation_.y += kChestRotSpeed;
	}
	else if (input_->PushKey(DIK_K)) {
		worldTransform_[PartId::Hip].rotation_.y -= kChestRotSpeed;
	}


	//注視点移動(ベクトルの加算)
	worldTransform_[PartId::Root].translation_.x += move.x;
	worldTransform_[PartId::Root].translation_.y += move.y;
	worldTransform_[PartId::Root].translation_.z += move.z;

	//デバッグ用表示
	debugText_->SetPos(50, 150);
	debugText_->Printf(
		"Root: (%f , %f %f)",
		worldTransform_[PartId::Root].translation_.x,
		worldTransform_[PartId::Root].translation_.y,
		worldTransform_[PartId::Root].translation_.z);



	worldTransform_[Root].UpdateMatrix();
	worldTransform_[Spine].UpdateMatrix();
	worldTransform_[Chest].UpdateMatrix();
	worldTransform_[Head].UpdateMatrix();
	worldTransform_[ArmL].UpdateMatrix();
	worldTransform_[ArmR].UpdateMatrix();
	worldTransform_[Hip].UpdateMatrix();
	worldTransform_[LegL].UpdateMatrix();
	worldTransform_[LegR].UpdateMatrix();




#pragma endregion






	//デバッグテキスト--------------
#pragma region debugTxt

	////位置
	//
	//std::string srtingDebug = std::string("Translation:")
	//	+ std::to_string(worldTransform_.translation_.x)
	//	+ std::to_string(worldTransform_.translation_.y)
	//	+ std::to_string(worldTransform_.translation_.z);

	//debugText_->Print(srtingDebug, 50, 50, 1.0f);

	////回転角

	//srtingDebug = std::string("Rotation:")
	//	+ std::to_string(worldTransform_.rotation_.x)
	//	+ std::to_string(worldTransform_.rotation_.y)
	//	+ std::to_string(worldTransform_.rotation_.z);


	//debugText_->Print(srtingDebug, 50, 70, 1.0f);
	////スケール


	//srtingDebug = std::string("Scale:")
	//	+ std::to_string(worldTransform_.scale_.x)
	//	+ std::to_string(worldTransform_.scale_.y)
	//	+ std::to_string(worldTransform_.scale_.z);

	//debugText_->Print(srtingDebug, 50, 90, 1.0f);

	//カメラの位置座標
	debugText_->SetPos(50, 50);
	debugText_->Printf("eye(%.4f,%.4f,%.4f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	//カメラの注視点
	debugText_->SetPos(50, 70);
	debugText_->Printf("target(%.4f,%.4f,%.4f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);
	//カメラの上方向回転
	debugText_->SetPos(50, 90);
	debugText_->Printf("up(%.4f,%.4f,%.4f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);
	//視野角
	debugText_->SetPos(50, 110);
	debugText_->Printf("FoV(%.4f)", XMConvertToRadians(viewProjection_.fovAngleY));
	//クリップ距離
	debugText_->SetPos(50, 130);
	debugText_->Printf("nearZ(%.4f)", viewProjection_.nearZ);



#pragma endregion

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
	/// </summary>
	//3Dモデルの描画--------------


	//model_->Draw(worldTransform_[Root], viewProjection_, textureHandle_);
	//model_->Draw(worldTransform_[Spine], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[Chest], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[Head], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[ArmL], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[ArmR], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[Hip], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[LegL], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[LegR], viewProjection_, textureHandle_);
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
