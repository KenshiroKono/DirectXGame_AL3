#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

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

	//X.Y.Z方向のスケーリングを設定
	worldTransform_.scale_ = { 5.0f, 5.0f, 5.0f };
	//X.Y.Z方向の回転角
	worldTransform_.rotation_ = { XM_PI / 4,XM_PI / 4,0.0f };
	//X.Y.Z方向の平行移動
	worldTransform_.translation_ = { 10.0f,10.0f,10.0f };

	//ワールドトランスフォーム
	worldTransform_.Initialize();
	//ビュープロジェクション
	viewProjection_.Initialize();



}

void GameScene::Update() {



	//デバッグテキスト--------------

	//位置
	//debugNum = worldTransform_.translation_;
	std::string srtingDebug = std::string("Translation:")
		+ std::to_string(worldTransform_.translation_.x)
		+ std::to_string(worldTransform_.translation_.y)
		+ std::to_string(worldTransform_.translation_.z);

	debugText_->Print(srtingDebug, 50, 50, 1.0f);

	//回転角

	srtingDebug = std::string("Rotation:")
		+ std::to_string(worldTransform_.rotation_.x)
		+ std::to_string(worldTransform_.rotation_.y)
		+ std::to_string(worldTransform_.rotation_.z);


	debugText_->Print(srtingDebug, 50, 70, 1.0f);
	//スケール


	srtingDebug = std::string("Scale:")
		+ std::to_string(worldTransform_.scale_.x)
		+ std::to_string(worldTransform_.scale_.y)
		+ std::to_string(worldTransform_.scale_.z);



	debugText_->Print(srtingDebug, 50, 90, 1.0f);


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
	//3Dモデルの描画
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);





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
