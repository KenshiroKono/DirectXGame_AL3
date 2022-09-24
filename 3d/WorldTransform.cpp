#include "DirectXCommon.h"
#include "WorldTransform.h"
#include <cassert>
#include <d3dx12.h>

//using namespace DirectX;
/*
void WorldTransform::Initialize() {
	CreateConstBuffer();
	Map();
	UpdateMatrix_();
}

void WorldTransform::CreateConstBuffer() {
	HRESULT result;

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataWorldTransform) + 0xff) & ~0xff);

	// 定数バッファの生成
	result = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProps, // アップロード可能
		D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));
}

void WorldTransform::Map() {
	// 定数バッファとのデータリンク
	HRESULT result = constBuff_->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));
}
*/

void WorldTransform::UpdateMatrix_() {
	Matrix4 matScale, matRot, matTrans;
	// 各軸用回転行列を宣言
	Matrix4 matRotX, matRotY, matRotZ;

	Matrix4 IdentityMat4;
	IdentityMat4.m[0][0] = 1;
	IdentityMat4.m[1][1] = 1;
	IdentityMat4.m[2][2] = 1;
	IdentityMat4.m[3][3] = 1;

	// スケール、回転、平行移動行列の計算


	matScale.m[0][0] = scale_.x;
	matScale.m[1][1] = scale_.y;
	matScale.m[2][2] = scale_.z;
	matScale.m[3][3] = 1.0f;

	matRot = IdentityMat4;

	matRotZ = IdentityMat4;
	matRotZ.m[0][0] = cos(rotation_.z);
	matRotZ.m[0][1] = sin(rotation_.z);
	matRotZ.m[1][0] = -sin(rotation_.z);
	matRotZ.m[1][1] = cos(rotation_.z);
	matRotX = IdentityMat4;
	matRotX.m[1][1] = cos(rotation_.x);
	matRotX.m[1][2] = sin(rotation_.x);
	matRotX.m[2][1] = -sin(rotation_.x);
	matRotX.m[2][2] = cos(rotation_.x);
	matRotY = IdentityMat4;
	matRotY.m[0][0] = cos(rotation_.y);
	matRotY.m[0][2] = -sin(rotation_.y);
	matRotY.m[2][0] = sin(rotation_.y);
	matRotY.m[2][2] = cos(rotation_.y);
	matRot *= matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;

	
	//移動量を行列に設定
	matTrans.m[3][0] =translation_.x;
	matTrans.m[3][1] =translation_.y;
	matTrans.m[3][2] =translation_.z;
	matTrans.m[3][3] = 1;

	// ワールド行列の合成
	matWorld_ = IdentityMat4; // 変形をリセット
	matWorld_ *= matScale;          // ワールド行列にスケーリングを反映
	matWorld_ *= matRot;            // ワールド行列に回転を反映
	matWorld_ *= matTrans;          // ワールド行列に平行移動を反映


	// 親行列の指定がある場合は、掛け算する
	if (parent_) {
		matWorld_ *= parent_->matWorld_;
	}

	// 定数バッファに書き込み
	constMap->matWorld = matWorld_;
	TransferMatrix();
	//matWorld_ = IdentityMat4;

}
