#include "Enemy.h"
#include <cassert>
#include "MyMatrix.h"


void Enemy::Approach_move() {
	//ˆÚ“®
	Vector3 move = { 0, 0, -0.2f };
	worldTransform_.translation_ += move;
	//ˆê’è‚ÌˆÊ’u‚É’B‚µ‚½‚ç—£’E
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}
void Enemy::Leave_move() {
	//ˆÚ“®
	Vector3 move = { 0, 0, 0.2f };
	worldTransform_.translation_ += move;
	if (worldTransform_.translation_.z > 100.0f) {
		phase_ = Phase::Approach;
	}
}

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	this->model_ = model;
	this->textureHandle_ = textureHandle;
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0, 5.0f, 30.0f };
}

void Enemy::Update() {

	switch (phase_) {
	case Phase::Approach:
	default:
		Approach_move();
		break;
	case Phase::Leave:
		Leave_move();

		break;
	}


#pragma region ˆÚ“®ˆ—
	worldTransform_.matWorld_ = matIdentity();
	worldTransform_.matWorld_ = Mat(worldTransform_);
	//s—ñ‚Ì“]‘—
	worldTransform_.TransferMatrix();

	debugText_->SetPos(50, 90);
	debugText_->Printf(
		"Player:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
		worldTransform_.translation_.z);

#pragma endregion

}

void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}