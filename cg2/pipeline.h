#ifndef PIPELINE_H
#define	PIPELINE_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Pipeline
{
private:
	glm::vec3 scale;
	glm::vec3 position;
	glm::vec3 rotation;

	glm::mat4 matrix;
	struct {
		float zFar, zNear, FOV, width, height;
	} projection;

	struct {
		glm::vec3 position, target, up;
	} camera;

	glm::mat4  initScalingMatrix(float x, float y, float z);
	glm::mat4  initTranslationMatrix(float x, float y, float z);
	glm::mat4  initRotationMatrix(float x, float y, float z);
	glm::mat4  initProjectionMatrix(float FOV, float width, float height, float zNear, float zFar);
	glm::mat4  initCamera(glm::vec3 target, glm::vec3 up);

public:
	Pipeline()
	{
		scale = glm::vec3(1.0f, 1.0f, 1.0f);
		position = glm::vec3(0.0f, 0.0f, 0.0f);
		rotation = glm::vec3(0.0f, 0.0f, 0.0f);

		matrix = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };

		projection.FOV = 0.0;
		projection.width = 0.0;
		projection.height = 0.0;
		projection.zFar = 0.0;
		projection.zNear = 0.0;

		camera.position = glm::vec3(0.0f, 0.0f, 0.0f);
		camera.target = glm::vec3(0.0f, 0.0f, 0.0f);
		camera.up = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	void setScale(float ScaleX, float ScaleY, float ScaleZ)
	{
		scale.x = ScaleX;
		scale.y = ScaleY;
		scale.z = ScaleZ;
	}

	void setPosition(float x, float y, float z)
	{
		position.x = x;
		position.y = y;
		position.z = z;
	}

	void setRotation(float RotateX, float RotateY, float RotateZ)
	{
		rotation.x = RotateX;
		rotation.y = RotateY;
		rotation.z = RotateZ;
	}

	void setProjection(float FOV, float width, float height, float zNear, float zFar) {
		projection.FOV = FOV;
		projection.width = width;
		projection.height = height;
		projection.zFar = zFar;
		projection.zNear = zNear;
	}

	void setCamera(glm::vec3 pos, glm::vec3 target, glm::vec3 up) {
		camera.position = pos;
		camera.target = target;
		camera.up = up;
	}

	const glm::mat4* getTransformations();
};

#endif