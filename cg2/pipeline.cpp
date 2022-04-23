#include "pipeline.h"

glm::mat4 Pipeline::initScalingMatrix(float x, float y, float z) {
	return glm::mat4{
	x,			0.0f,		0.0f,		0.0f,
	0.0f,		y,			0.0f,		0.0f,
	0.0f,		0.0f,		z,			0.0f,
	0.0f,		0.0f,		0.0f,		1.0f };
}

glm::mat4 Pipeline::initRotationMatrix(float x, float y, float z) {

	x = glm::radians(x);
	y = glm::radians(y);
	z = glm::radians(z);

	glm::mat4 matX{
	1.0f, 0.0f,		0.0f,		0.0f,
	0.0f, cosf(x),	-sinf(x),	0.0f,
	0.0f, sinf(x),	cosf(x),	0.0f,
	0.0f, 0.0f,		0.0f,		1.0f };

	glm::mat4 matY{
	cosf(y),	0.0f, -sinf(y),		0.0f,
	0.0f,		1.0f,  0.0f,		0.0f,
	sinf(y),	0.0f,  cosf(y),		0.0f,
	0.0f,		0.0f,  0.0f,		1.0f };

	glm::mat4 matZ{
	cosf(z),	-sinf(z),	0.0f,	0.0f,
	sinf(z),	cosf(z),	0.0f,	0.0f,
	0.0f,		0.0f,		1.0f,	0.0f,
	0.0f,		0.0f,		0.0f,	1.0f };

	return matZ * matY* matX;
}

glm::mat4 Pipeline::initTranslationMatrix(float x, float y, float z) {
	return glm::mat4{ 1.0f,	0.0f,	0.0f,	 x,
						0.0f,	1.0f,	0.0f,	 y,
						0.0f,	0.0f,	1.0f,	 z,
						0.0f,	0.0f,	0.0f,	 1.0f };
}

glm::mat4 Pipeline::initProjectionMatrix(float FOV, float width, float height, float zNear, float zFar) {
	const float ar = width / height;
	const float zRange = zNear - zFar;
	const float tanHalfFOV = tanf(glm::radians(projection.FOV / 2.0f));

	return glm::mat4{ 1.0f / (tanHalfFOV * ar), 0.0f,				0.0f,						0.0f,
					  0.0f,						1.0f / tanHalfFOV,	0.0f,						0.0f,
					  0.0f,						0.0f,				(-zNear - zFar) / zRange,	2.0f * zFar * zNear / zRange,
					  0.0f,						0.0f,				1.0f,						0.0f };
}

glm::mat4 Pipeline::initCamera(glm::vec3 target, glm::vec3 up) {
	glm::vec3 N = target;
	N = glm::normalize(N);
	glm::vec3 U = up;
	U = glm::normalize(U);
	U = glm::cross(U, target);
	glm::vec3 V = glm::cross(N, U);

	return glm::mat4{ U.x,		U.y,	U.z,	0.0f,
					   V.x,		V.y,	V.z,	0.0f,
					   N.x,		N.y,	N.z,	0.0f,
					   0.0f,	0.0f,	0.0f,	1.0f };
}

const glm::mat4* Pipeline::getTransformations() {
	glm::mat4 scMatrix, rotMatrix, transMatrix, projMatrix, camTransMatrix, camRotMatrix;

	scMatrix = initScalingMatrix(scale.x, scale.y, scale.z);
	rotMatrix = initRotationMatrix(rotation.x, rotation.y, rotation.z);
	transMatrix = initTranslationMatrix(position.x, position.y, position.z);
	projMatrix = initProjectionMatrix(projection.FOV, projection.width, projection.height, projection.zNear, projection.zFar);

	camTransMatrix = initTranslationMatrix(-camera.position.x, -camera.position.y, -camera.position.z);
	camRotMatrix = initCamera(camera.target, camera.up);

	matrix = scMatrix * rotMatrix * transMatrix * camTransMatrix * camRotMatrix * projMatrix;

	return &matrix;
}