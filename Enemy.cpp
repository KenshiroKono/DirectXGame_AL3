#include "Enemy.h"
#include <cassert>
#include "MyMatrix.h"

#include "Player.h"

void Enemy::Approach_move() {
	//移動
	Vector3 move = { 0, 0, -0.2f };
	worldTransform_.translation_ += move;
	//一定の位置に達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}

	if (bullletTime-- < 0) {

		Fire();

	}

}
void Enemy::Leave_move() {
	//移動
	Vector3 move = { 0, 0, 0.2f };
	worldTransform_.translation_ += move;
	if (worldTransform_.translation_.z > 100.0f) {
		phase_ = Phase::Approach;
	}
}

void Enemy::Fire() {

	assert(player_);

	//弾の速度
	const float kBulletSpeed = 1.0f;
	Vector3 velocity;

	Vector3 pPos = player_->GetWorldPosition();
	Vector3 ePos = GetWorldPosition();

	Vector3 len ;
	len.x = ePos.x -pPos.x;
	len.y = ePos.y -pPos.y;
	len.z = ePos.z -pPos.z;

	velocity = normalize(len);

	len *= kBulletSpeed;


	//速度ベクトルを自機の向きに合わせて回転させる
	//velocity = Vec_rot(velocity, worldTransform_.matWorld_);

	//弾を生成し、初期化
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	//弾を登録する
	bullets_.push_back(std::move(newBullet));

	bullletTime = kFireInterval;

}

void Enemy::ApproachInitialize() {

	bullletTime = kFireInterval;

}

Vector3 Enemy::GetWorldPosition() {
	//座標を格納
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}
void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	this->model_ = model;
	this->textureHandle_ = textureHandle;
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
	worldTransform_.translation_ = { 10, 5.0f, 30.0f };

	ApproachInitialize();
}

void Enemy::Update() {
	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->IsDead(); });

	switch (phase_) {
	case Phase::Approach:
	default:
		Approach_move();
		break;
	case Phase::Leave:
		Leave_move();

		break;
	}


	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}

#pragma region 移動処理
	worldTransform_.matWorld_ = matIdentity();
	worldTransform_.matWorld_ = Mat(worldTransform_);
	//行列の転送
	worldTransform_.TransferMatrix();

	debugText_->SetPos(50, 90);
	debugText_->Printf(
		"Player:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
		worldTransform_.translation_.z);

#pragma endregion

}

void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}