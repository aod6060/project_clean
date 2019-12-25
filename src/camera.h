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

	void init(
		glm::vec3 pos,
		glm::vec2 rot,
		float fov,
		float aspect,
		float znear,
		float zfar,
		float rotSpeed = 64.0f,
		float walkingSpeed = 32.0f);

	void update(float delta);

	glm::mat4 getProjection();

	glm::mat4 getView();

};