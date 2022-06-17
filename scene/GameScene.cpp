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
}



float GameScene::Angle(float angle) {
	return angle * PI / 180;
}


float Degree(const float& degree) {
	float n = degree * PI / 180;
	return n;
}

void GameScene::Initialize() {

	textureHandle_ = TextureManager::Load("mario.jpg");
	model_ = Model::Create();

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);



	//カメラ垂直方向視野角を設定
	viewProjection_.fovAngleY = Angle(75.0f);
	//アス比を設定
	//viewProjection_.aspectRatio = 1.0f;
	//ニアクリップ距離を設定
	viewProjection_.nearZ = 1.0f;
	//ファークリップ距離を設定
	viewProjection_.farZ = 60.0f;

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

	for (WorldTransform& worldTransform : worldTransforms) {
		worldTransform.Initialize();
		//4*4の単位行列を生成
		Matrix4 IdentityMat4;
		IdentityMat4.m[0][0] = 1;
		IdentityMat4.m[1][1] = 1;
		IdentityMat4.m[2][2] = 1;
		IdentityMat4.m[3][3] = 1;
		//x,y,zのスケーリングを決定
		worldTransform.scale_ = { 1.0f,1.0f,1.0f };
		//スケーリング行列を宣言
		Matrix4 matScale;
		//スケーリング倍率を設定
		matScale.m[0][0] = worldTransform.scale_.x;
		matScale.m[1][1] = worldTransform.scale_.y;
		matScale.m[2][2] = worldTransform.scale_.z;
		matScale.m[3][3] = 1.0f;
		// X, Y, Z 軸の周りの回転角を設定
		worldTransform.rotation_ = { rotDist(engine),rotDist(engine),rotDist(engine) };
		// 各軸用回転行列を宣言
		Matrix4 matRotX, matRotY, matRotZ;
		// 合成回転行列を宣言
		Matrix4 matRot = IdentityMat4;
		// Z軸の回転行列を設宣言
		matRotZ = IdentityMat4;
		matRotZ.m[0][0] = cos(worldTransform.rotation_.z);
		matRotZ.m[0][1] = sin(worldTransform.rotation_.z);
		matRotZ.m[1][0] = -sin(worldTransform.rotation_.z);
		matRotZ.m[1][1] = cos(worldTransform.rotation_.z);
		matRotX = IdentityMat4;
		matRotX.m[1][1] = cos(worldTransform.rotation_.x);
		matRotX.m[1][2] = sin(worldTransform.rotation_.x);
		matRotX.m[2][1] = -sin(worldTransform.rotation_.x);
		matRotX.m[2][2] = cos(worldTransform.rotation_.x);
		matRotY = IdentityMat4;
		matRotY.m[0][0] = cos(worldTransform.rotation_.y);
		matRotY.m[0][2] = -sin(worldTransform.rotation_.y);
		matRotY.m[2][0] = sin(worldTransform.rotation_.y);
		matRotY.m[2][2] = cos(worldTransform.rotation_.y);
		matRot *= matRotZ;
		matRot *= matRotX;
		matRot *= matRotY;
		//X,Y,Zの平行移動を設定
		worldTransform.translation_ = { posDist(engine), posDist(engine), posDist(engine) };
		//平行移動行列を宣言
		Matrix4 matTrans = MathUtility::Matrix4Identity();
		//移動量を行列に設定
		matTrans.m[3][0] = worldTransform.translation_.x;
		matTrans.m[3][1] = worldTransform.translation_.y;
		matTrans.m[3][2] = worldTransform.translation_.z;
		matTrans.m[3][3] = 1;
		//単位行列を設定
		worldTransform.matWorld_ = IdentityMat4;
		//ワールド行列と掛け算
		worldTransform.matWorld_ *= matScale;
		worldTransform.matWorld_ *= matRot;
		worldTransform.matWorld_ *= matTrans;
		//行列を転送
		worldTransform.TransferMatrix();
	}






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
		}
		else if (input_->PushKey(DIK_DOWN)) {
			viewProjection_.nearZ -= 0.1f;
		}
		//行列の再計算
		viewProjection_.UpdateMatrix();

		// デバック用表示
		debugText_->SetPos(50, 130);
		debugText_->Printf("nearZ: % f", viewProjection_.nearZ);
	}
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
	/// ここに3Dオブジェクトの描画処理を追加できる-----------------
	/// </summary>


	for (WorldTransform& worldTransform : worldTransforms) {

		model_->Draw(worldTransform, viewProjection_, textureHandle_);

	}



	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region PrimitiveDrawer

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
