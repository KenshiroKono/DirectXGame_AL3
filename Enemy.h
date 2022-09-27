#pragma once


#include "DebugText.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include<assert.h>
#include"PayerBullet.h"
#include<memory>
#include<list>

//行動フェーズ
enum class Phase {
	Approach, //接近する
	Leave,    //離脱する
};

class Enemy {
public:
	//初期化
	void Initialize(Model* model, uint32_t textureHandle);
	//更新処理
	void Update();
	//描画処理
	void Draw(ViewProjection& viewProjection);

	//各パターンの処理
	void Approach_move();
	void Leave_move();

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//デバックテキスト
	DebugText* debugText_ = nullptr;

	//フェーズ
	Phase phase_ = Phase::Approach;

};