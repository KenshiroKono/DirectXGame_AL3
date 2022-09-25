#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "WinApp.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include<random>
#define PI  3.14159

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
	delete player_;
}



float GameScene::Angle(float angle) {
	return angle * PI / 180;
}


float Degree(const float& degree) {
	float n = degree * PI / 180;
	return n;
}
/*
#pragma region Mat
Matrix4 matIdentity() {
	//単位行列
	Matrix4 matIdentity;
	matIdentity.m[0][0] = 1;
	matIdentity.m[1][1] = 1;
	matIdentity.m[2][2] = 1;
	matIdentity.m[3][3] = 1;

	return matIdentity;
}
Matrix4 Mat_size(WorldTransform& w) {
	//スケーリング行列を宣言
	Matrix4 matScale;
	matScale.m[0][0] = w.scale_.x;
	matScale.m[1][1] = w.scale_.y;
	matScale.m[2][2] = w.scale_.z;
	matScale.m[3][3] = 1;

	w.matWorld_ = matIdentity();

	return matScale;
}

Matrix4 Mat_rot(WorldTransform& w) {
	//合成用回転行列を宣言
	Matrix4 matRot = matIdentity();

	Matrix4 matRotX = matIdentity();
	Matrix4 matRotY = matIdentity();
	Matrix4 matRotZ = matIdentity();

	// z軸回転行列を宣言
	matRotZ.m[0][0] = cos(w.rotation_.z);
	matRotZ.m[0][1] = sin(w.rotation_.z);
	matRotZ.m[1][0] = -sin(w.rotation_.z);
	matRotZ.m[1][1] = cos(w.rotation_.z);

	// x軸回転行列を宣言
	matRotX.m[1][1] = cos(w.rotation_.x);
	matRotX.m[1][2] = sin(w.rotation_.x);
	matRotX.m[2][1] = -sin(w.rotation_.x);
	matRotX.m[2][2] = cos(w.rotation_.x);

	// y軸回転行列を宣言
	matRotY.m[0][0] = cos(w.rotation_.y);
	matRotY.m[0][2] = -sin(w.rotation_.y);
	matRotY.m[2][0] = sin(w.rotation_.y);
	matRotY.m[2][2] = cos(w.rotation_.y);

	matRot *= matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;

	w.matWorld_ = matIdentity();

	return matRot;
}

Matrix4 Mat_move(WorldTransform& w) {

	//平行移動行列を宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	matTrans.m[3][0] = w.translation_.x;
	matTrans.m[3][1] = w.translation_.y;
	matTrans.m[3][2] = w.translation_.z;
	matTrans.m[3][3] = 1;

	w.matWorld_ = matIdentity();
	return matTrans;
}

Matrix4 Mat(WorldTransform w) {
	Matrix4 mat;
	mat = matIdentity();
	mat *= Mat_size(w);
	mat *= Mat_rot(w);
	mat *= Mat_move(w);

	return mat;
};
#pragma endregion
*/


void GameScene::Initialize() {

	textureHandle_ = TextureManager::Load("mario.jpg");
	model_ = Model::Create();

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	player_ = new Player();
	player_->Initialize(model_, textureHandle_);

	//デバッグカメラの生成
	//debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);



	//カメラ垂直方向視野角を設定
	viewProjection_.fovAngleY = Angle(50.0f);
	//アス比を設定
	//viewProjection_.aspectRatio = 1.0f;
	//ニアクリップ距離を設定
	viewProjection_.nearZ = 1.0f;
	//ファークリップ距離を設定
	viewProjection_.farZ = 160.0f;

	//ビュープロジェクション 初期化
	viewProjection_.Initialize();





	//乱数シード設定
	std::random_device speed_gen;
	//メルセンヌ・ツイスター
	std::mt19937_64 engine(speed_gen());

	//乱数範囲(回転角用)
	std::uniform_real_distribution<float> rotDist(0.0f, 3.141592654f);
	//乱数範囲(座標用)
	std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);
	//乱数エンジンを渡し、指定範囲からランダムで数値を得る
	float rotValue = rotDist(engine);
	float posValue = posDist(engine);

	//軸方向表示を有効に
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定(アドレス)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//ライン描画が参照するビュープロジェクションを指定(アドレス)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);



	//ワールドトランスフォーム 初期化
	Matrix4 IdentityMat4;
	IdentityMat4.m[0][0] = 1;
	IdentityMat4.m[1][1] = 1;
	IdentityMat4.m[2][2] = 1;
	IdentityMat4.m[3][3] = 1;



	/*
	

	//X.Y.Z方向のスケーリングを設定
	worldTransforms_[0].scale_ = { 1.0f, 1.0f, 1.0f };
	//スケーリング行列を宣言
	Matrix4 matScale;
	//スケーリング倍率を設定
	matScale.m[0][0] = worldTransforms_[0].scale_.x;
	matScale.m[1][1] = worldTransforms_[0].scale_.y;
	matScale.m[2][2] = worldTransforms_[0].scale_.z;
	matScale.m[3][3] = 1.0f;
	//X.Y.Z方向の回転角
	worldTransforms_[0].rotation_ = { 0,0,0 };
	//X.Y.Z方向の平行移動
	worldTransforms_[0].translation_ = { 0,0,0 };
	worldTransforms_[0].Initialize();

	// 各軸用回転行列を宣言
	Matrix4 matRotX, matRotY, matRotZ;
	// 合成回転行列を宣言
	Matrix4 matRot = IdentityMat4;
	// Z軸の回転行列を設宣言
	matRotZ = IdentityMat4;
	matRotZ.m[0][0] = cos(worldTransforms_[0].rotation_.z);
	matRotZ.m[0][1] = sin(worldTransforms_[0].rotation_.z);
	matRotZ.m[1][0] = -sin(worldTransforms_[0].rotation_.z);
	matRotZ.m[1][1] = cos(worldTransforms_[0].rotation_.z);
	matRotX = IdentityMat4;
	matRotX.m[1][1] = cos(worldTransforms_[0].rotation_.x);
	matRotX.m[1][2] = sin(worldTransforms_[0].rotation_.x);
	matRotX.m[2][1] = -sin(worldTransforms_[0].rotation_.x);
	matRotX.m[2][2] = cos(worldTransforms_[0].rotation_.x);
	matRotY = IdentityMat4;
	matRotY.m[0][0] = cos(worldTransforms_[0].rotation_.y);
	matRotY.m[0][2] = -sin(worldTransforms_[0].rotation_.y);
	matRotY.m[2][0] = sin(worldTransforms_[0].rotation_.y);
	matRotY.m[2][2] = cos(worldTransforms_[0].rotation_.y);
	matRot *= matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;

	//平行移動行列を宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();
	//移動量を行列に設定
	matTrans.m[3][0] = worldTransforms_[0].translation_.x;
	matTrans.m[3][1] = worldTransforms_[0].translation_.y;
	matTrans.m[3][2] = worldTransforms_[0].translation_.z;
	matTrans.m[3][3] = 1;




	//単位行列を設定
	worldTransforms_[0].matWorld_ = IdentityMat4;
	//ワールド行列と掛け算
	worldTransforms_[0].matWorld_ *= matScale;
	worldTransforms_[0].matWorld_ *= matRot;
	worldTransforms_[0].matWorld_ *= matTrans;

	//大元	
	worldTransforms_[PartId::kRoot].Initialize();
	//脊椎
	worldTransforms_[PartId::kSpine].Initialize();
	worldTransforms_[PartId::kSpine].translation_ = { 0,4.0f,0 };
	worldTransforms_[PartId::kSpine].parent_ = &worldTransforms_[PartId::kRoot];

	//上半身
	//胴体
	worldTransforms_[PartId::kChest].Initialize();
	worldTransforms_[PartId::kChest].translation_ = { 0,1.5f,0 };
	worldTransforms_[PartId::kChest].parent_ = &worldTransforms_[PartId::kSpine];
	//頭
	worldTransforms_[PartId::kHead].Initialize();
	worldTransforms_[PartId::kHead].translation_ = { 0,2.5f,0 };
	worldTransforms_[PartId::kHead].parent_ = &worldTransforms_[PartId::kChest];
	//左腕
	worldTransforms_[PartId::kArmL].Initialize();
	worldTransforms_[PartId::kArmL].translation_ = { 2.5f,0,0 };
	worldTransforms_[PartId::kArmL].parent_ = &worldTransforms_[PartId::kChest];
	//右腕
	worldTransforms_[PartId::kArmR].Initialize();
	worldTransforms_[PartId::kArmR].translation_ = { -2.5f,0,0 };
	worldTransforms_[PartId::kArmR].parent_ = &worldTransforms_[PartId::kChest];

	//下半身
	//腰部
	worldTransforms_[PartId::kHip].Initialize();
	worldTransforms_[PartId::kHip].translation_ = { 0,-2.5f,0 };
	worldTransforms_[PartId::kHip].parent_ = &worldTransforms_[PartId::kSpine];
	//左足
	worldTransforms_[PartId::kLegL].Initialize();
	worldTransforms_[PartId::kLegL].translation_ = { 2.5f,-1.5,0 };
	worldTransforms_[PartId::kLegL].parent_ = &worldTransforms_[PartId::kHip];
	//右足
	worldTransforms_[PartId::kLegR].Initialize();
	worldTransforms_[PartId::kLegR].translation_ = { -2.5f,-1.5,0 };
	worldTransforms_[PartId::kLegR].parent_ = &worldTransforms_[PartId::kHip];
	*/





}

void GameScene::Update() {//----------------------

	//視点移動処理
#pragma region Eye

//視点の移動ベクトル
	{
		/*
		Vector3 move = { 0,0,0 };
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

		*/
	}


#pragma endregion

	//注視点移動処理
#pragma region Target

	//注視点のベクトル
	{
		/*
		Vector3 move = { 0,0,0 };

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
		*/

	}
#pragma endregion

	//上方向回転処理
#pragma region UpRot
	{
		/*
				//上方向の回転の速さ[ラジアン/f]
				const float kUpRotSpeed = 0.05f;

				//押した方向で移動ベクトルを変更
				if (input_->PushKey(DIK_SPACE)) {
					viewAngle += kUpRotSpeed;
					//2πを超えたら0に戻す
					viewAngle = fmodf(viewAngle,PI*2 );
				}

				//上方向のベクトルを計算(半径１の円周上の座標)
				viewProjection_.up = { cosf(viewAngle),sinf(viewAngle),0.0f };

				//行列の再計算
				viewProjection_.UpdateMatrix();
		*/


	}

#pragma endregion

	//FoV変更処理
#pragma region Fov

	//押した方向にベクトルを変更
	{

		if (input_->PushKey(DIK_S)) {//Sキーで視野角が広がる
			viewProjection_.fovAngleY += 0.01f;
			viewProjection_.fovAngleY = min(viewProjection_.fovAngleY, PI);
		}
		else if (input_->PushKey(DIK_W)) {//Wキーで視野角が狭まる
			viewProjection_.fovAngleY -= 0.01f;
			viewProjection_.fovAngleY = max(viewProjection_.fovAngleY, 0.01f);
		}
		//行列の再計算
		viewProjection_.UpdateMatrix();

		debugText_->SetPos(50, 110);
		debugText_->Printf("fovAngleY(Degree):%f", Degree(viewProjection_.fovAngleY));

	}
#pragma endregion

	//クリップ距離変更処理
#pragma region Near
	{
		if (input_->PushKey(DIK_UP)) {
			viewProjection_.nearZ += 0.1f;
			viewProjection_.nearZ = min(viewProjection_.nearZ, 160.0f);
		}
		else if (input_->PushKey(DIK_DOWN)) {
			viewProjection_.nearZ -= 0.1f;
			viewProjection_.nearZ = max(viewProjection_.nearZ, 0.1f);

		}
		//行列の再計算
		viewProjection_.UpdateMatrix();

		// デバック用表示
		debugText_->SetPos(50, 130);
		debugText_->Printf("nearZ: % f", viewProjection_.nearZ);
	}
#pragma endregion


#pragma region オブジェクト	
	/*
	//キャラクターの移動ベクトル
	Vector3 move = { 0, 0, 0 };
	//キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;
	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_LEFT)) {
		move = { -kCharacterSpeed, 0,0 };
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		move = { kCharacterSpeed, 0,0 };
	}
	//平行移動
	worldTransforms_[0].translation_ += move;

	//回転
	const float  kChestRotSpeed = 0.05f;
	//上半身の回転
	if (input_->PushKey(DIK_U)) {
		worldTransforms_[PartId::kChest].rotation_.y += kChestRotSpeed;
	}
	else if (input_->PushKey(DIK_I)) {
		worldTransforms_[PartId::kChest].rotation_.y -= kChestRotSpeed;
	}

	//下半身の回転
	if (input_->PushKey(DIK_J)) {
		worldTransforms_[PartId::kHip].rotation_.y += kChestRotSpeed;
	}
	else if (input_->PushKey(DIK_K)) {
		worldTransforms_[PartId::kHip].rotation_.y -= kChestRotSpeed;
	}

	for (int i = 0; i < kNumPartId; i++) {
		Matrix4 matTrans = MathUtility::Matrix4Identity();

		worldTransforms_[i].matWorld_ = MathUtility::Matrix4Identity();

		//ワールド行列と掛け算
		worldTransforms_[i].matWorld_ *=Mat(worldTransforms_[i]);
		if (worldTransforms_[i].parent_ != nullptr) {
			worldTransforms_[i].matWorld_ *= worldTransforms_[i].parent_->matWorld_;
		}
		//行列を転送
		worldTransforms_[i].TransferMatrix();
	}

	worldTransforms_[1].TransferMatrix();
	// デバック用表示
	debugText_->SetPos(50, 150);
	debugText_->Printf("worldx:%f/y:%f/z:%f", worldTransforms_[0].translation_.x, worldTransforms_[0].translation_.y, worldTransforms_[0].translation_.z);
	debugText_->SetPos(50, 170);
	debugText_->Printf("speed%f", move.x);
	*/
#pragma endregion


	player_->Update();


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
	/// ここに3Dオブジェクトの描画処理を追加できる-----------------
	/// </summary>

	player_->Draw(viewProjection_);

	/*
	for (int i = 0; i < kNumPartId; i++) {
		if (i==kRoot||i==kSpine) {
			continue;
		}
		model_->Draw(worldTransforms_[i], viewProjection_, textureHandle_);
	}
	*/


	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region PrimitiveDrawer

#pragma region DrawLine
	const float LINE_MAX = 25;
	Vector3 lineStart;
	Vector3 lineEnd;
	Vector4 lineColo;

	for (float i = 0; i <= LINE_MAX; i += 1) {

		float lineW = 2.0f;
		lineStart = {
			-(LINE_MAX / 2) * lineW,
			-5,
			(-(LINE_MAX / 2) * lineW) + (lineW * i)
		};
		lineEnd = {
			 (LINE_MAX / 2) * lineW ,
			-5,
			(-(LINE_MAX / 2) * lineW) + (lineW * i)
		};
		lineColo = { 1.0f,0,0,1.0f };

		PrimitiveDrawer::GetInstance()->DrawLine3d(lineStart, lineEnd, lineColo);

		lineStart = {
			(-(LINE_MAX / 2) * lineW) + (lineW * i),
			-5,
			-(LINE_MAX / 2) * lineW,
		};
		lineEnd = {
			(-(LINE_MAX / 2) * lineW) + (lineW * i),
			-5 ,
			 (LINE_MAX / 2) * lineW };
		lineColo = { 0,0,1.0f,1.0f };

		PrimitiveDrawer::GetInstance()->DrawLine3d(lineStart, lineEnd, lineColo);

	}

#pragma endregion 


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
