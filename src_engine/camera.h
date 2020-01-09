#pragma once

struct Camera {
	glm::vec3 pos;
	glm::vec2 rot;

	float fov;
	float apsect;
	float znear;
	float zfar;

	float rotSpeed = 64.0f;
	float walkingSpeed = 32.0f;

	virtual void init(
		glm::vec3 pos,
		glm::vec2 rot,
		float fov,
		float aspect,
		float znear,
		float zfar,
		float rotSpeed = 64.0f,
		float walkingSpeed = 32.0f);

	virtual void update(float delta);

	virtual glm::mat4 getProjection();

	virtual glm::mat4 getView();

};