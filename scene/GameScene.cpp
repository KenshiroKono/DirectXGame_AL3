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


	for (size_t i = 0; i < _countof(worldTransform_); i++) {

		//X.Y.Z方向のスケーリングを設定
		worldTransform_[i].scale_ = { 1.0f, 1.0f, 1.0f };
		//X.Y.Z方向の回転角
		worldTransform_[i].rotation_ = { rotDist(engine),rotDist(engine),rotDist(engine) };
		//X.Y.Z方向の平行移動
		worldTransform_[i].translation_ = { posDist(engine),posDist(engine),posDist(engine) };

		//ワールドトランスフォーム 初期化
		worldTransform_[i].Initialize();
	}

	//カメラの視点座標を設定
	viewProjection_.eye = { 0,0,-60 };
	//カメラの注視点座標を設定
	viewProjection_.target = { 10,0,0 };
	//カメラの上方向ベクトルを設定(右上45度指定)
	viewProjection_.up = { cosf(XM_PI / 4.0f),sinf(XM_PI / 4.0f),0.0f };



	//ビュープロジェクション 初期化
	viewProjection_.Initialize();


}

void GameScene::Update() {


	//視点移動処理
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
		}/*
		if (input_->PushKey(DIK_D)) {
			move = { kEyespeed,0,0 };
		}
		else if (input_->PushKey(DIK_A)) {
			move = { -kEyespeed,0,0 };
		}*/

		//視点の移動(ベクトルを加算)
		viewProjection_.eye.x += move.x;
		viewProjection_.eye.y += move.y;
		viewProjection_.eye.z += move.z;

		//行列の再計算
		viewProjection_.UpdateMatrix();
	}
#pragma endregion

	//注視点移動処理
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

#pragma region UpRot

	//上方向回転処理
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
	debugText_->Printf("eye(%.4f,%.4f,%.4f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);
	//カメラの上方向回転
	debugText_->SetPos(50, 90);
	debugText_->Printf("eye(%.4f,%.4f,%.4f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);

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
	for (size_t i = 0; i < _countof(worldTransform_); i++) {
		model_->Draw(worldTransform_[i], viewProjection_, textureHandle_);
	}






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
