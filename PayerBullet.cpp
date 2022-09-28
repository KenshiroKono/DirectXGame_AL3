#include"PayerBullet.h"
#include<assert.h>
#include "MyMatrix.h"

void PlayerBullet::OnCollision() { 
	isDead_ = true; }

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	assert(model);
	model_ = model;
	velocity_ = velocity;
	textureHandle_ = TextureManager::Load("black.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}

Vector3 PlayerBullet::GetBulletPosition() {
	//座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void PlayerBullet::Update() {
	worldTransform_.matWorld_ = matIdentity();
	worldTransform_.matWorld_ *= Mat(worldTransform_);
	worldTransform_.translation_ += velocity_;
	//行列の転送
	worldTransform_.TransferMatrix();
	//時間経過で消える
	if (--deathTimer <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}