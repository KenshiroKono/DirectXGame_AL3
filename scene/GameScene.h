#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"	
#include "Player.h"
#include"Enemy.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

	DebugCamera* debugCamera_ = nullptr;


public:
	enum PartId {
		kRoot,	//大元
		kSpine,	//脊椎	
		kChest,	//胴体
		kHead,	//頭部
		kArmL,	//左腕
		kArmR,	//右腕
		kHip,	//腰部
		kLegL,	//左足
		kLegR,	//右足	

		kNumPartId
	};




public: // メンバ関数
  /// <summary>
  /// コンストクラタ
  /// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();


	/// <summary>
	/// 度数法を返す
	/// </summary>
	float Angle(float angle);
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;

	uint32_t textureHandle_ = 0;
	uint32_t enemyTexHandle_ = 0;
	Model* model_ = nullptr;
	//ワールドトランスフォーム
	WorldTransform worldTransforms_[100];
	//ビュープロジェクション
	ViewProjection viewProjection_;
	//カメラの上方向の角度
	float viewAngle = 0.0f;


	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
