#include "MyMatrix.h"
#include<cmath>	//sqrt

Matrix4 matIdentity() {
	//�P�ʍs��
	Matrix4 matIdentity;
	matIdentity.m[0][0] = 1;
	matIdentity.m[1][1] = 1;
	matIdentity.m[2][2] = 1;
	matIdentity.m[3][3] = 1;

	return matIdentity;
}
//�X�P�[�����O
Matrix4 Mat_size(WorldTransform& w) {
	Matrix4 matScale;
	matScale.m[0][0] = w.scale_.x;
	matScale.m[1][1] = w.scale_.y;
	matScale.m[2][2] = w.scale_.z;
	matScale.m[3][3] = 1;

	w.matWorld_ = matIdentity();

	return matScale;
}

//��]
Matrix4 Mat_rot(WorldTransform& w) {
	//�����p��]�s���錾
	Matrix4 matRot = matIdentity();

	Matrix4 matRotX = matIdentity();
	Matrix4 matRotY = matIdentity();
	Matrix4 matRotZ = matIdentity();

	// z����]�s���錾
	matRotZ.m[0][0] = cos(w.rotation_.z);
	matRotZ.m[0][1] = sin(w.rotation_.z);
	matRotZ.m[1][0] = -sin(w.rotation_.z);
	matRotZ.m[1][1] = cos(w.rotation_.z);

	// x����]�s���錾
	matRotX.m[1][1] = cos(w.rotation_.x);
	matRotX.m[1][2] = sin(w.rotation_.x);
	matRotX.m[2][1] = -sin(w.rotation_.x);
	matRotX.m[2][2] = cos(w.rotation_.x);

	// y����]�s���錾
	matRotY.m[0][0] = cos(w.rotation_.y);
	matRotY.m[0][2] = -sin(w.rotation_.y);
	matRotY.m[2][0] = sin(w.rotation_.y);
	matRotY.m[2][2] = cos(w.rotation_.y);

	matRot *= matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;

	return matRot;
}

//���s�ړ�
Matrix4 Mat_move(WorldTransform& w) {

	//���s�ړ��s���錾
	Matrix4 matTrans = matIdentity();

	matTrans.m[3][0] = w.translation_.x;
	matTrans.m[3][1] = w.translation_.y;
	matTrans.m[3][2] = w.translation_.z;
	matTrans.m[3][3] = 1;

	return matTrans;
}

//����
Matrix4 Mat(WorldTransform w) {
	Matrix4 matComb;
	matComb = matIdentity();
	matComb *= Mat_size(w);
	matComb *= Mat_rot(w);
	matComb *= Mat_move(w);

	return matComb;
};

Vector3 Vec_rot(Vector3 velocity, Matrix4 amount) {
	Vector3 Rot;

	Rot.x = velocity.x * amount.m[0][0];
	Rot.x += velocity.y * amount.m[1][0];
	Rot.x += velocity.z * amount.m[2][0];

	Rot.y = velocity.x * amount.m[0][1];
	Rot.y += velocity.y * amount.m[1][1];
	Rot.y += velocity.z * amount.m[2][1];

	Rot.z = velocity.x * amount.m[0][2];
	Rot.z += velocity.y * amount.m[1][2];
	Rot.z += velocity.z * amount.m[2][2];

	return Rot;
}

float Length(Vector3 vec) {
	return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}
Vector3 normalize(Vector3 vec) {
	float len = Length(vec);
	if (len != 0) {
		return vec /= len;
	}
	return vec;
}
Vector3 Distance(Vector3 vec_a, Vector3 vec_b) {
	Vector3 vec = vec_a -= vec_b;
	return vec;
}