#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	this->model_ = model;
	this->textureHandle_ = textureHandle;
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
}

void Player::Update() {
	Vector3 move = { 0, 0, 0 };

	const float speed = 0.2f;
#pragma region 移動処理
	if (input_->PushKey(DIK_UP)) {
		move = { 0, speed, 0 };
	}
	else if (input_->PushKey(DIK_DOWN)) {
		move = { 0, -speed, 0 };
	}
	else if (input_->PushKey(DIK_LEFT)) {
		move = { -speed, 0, 0 };
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		move = { speed, 0, 0 };
	}

	worldTransform_.translation_ += move;
	const float kMoveLimitX = 32.0f;
	const float kMoveLimitY = 18.0f;
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);


	//単位行列
	Matrix4 matIdentity;
	matIdentity.m[0][0] = 1;
	matIdentity.m[1][1] = 1;
	matIdentity.m[2][2] = 1;
	matIdentity.m[3][3] = 1;

#pragma region 平行移動
	// x,y,z軸周りの平行移動を設定

	//平行移動行列を宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	matTrans.m[3][0] = worldTransform_.translation_.x;
	matTrans.m[3][1] = worldTransform_.translation_.y;
	matTrans.m[3][2] = worldTransform_.translation_.z;
	matTrans.m[3][3] = 1;

#pragma endregion
	worldTransform_.matWorld_ = matIdentity;
	worldTransform_.matWorld_ *= matTrans;

	//行列の転送
	worldTransform_.TransferMatrix();

	debugText_->SetPos(50, 70);
	debugText_->Printf(
		"Player:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
		worldTransform_.translation_.z);
#pragma endregion
}


void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}