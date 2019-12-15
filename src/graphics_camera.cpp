#include "sys.h"


void Camera::init(
	glm::vec3 pos,
	glm::vec2 rot,
	float fov,
	float aspect,
	float znear,
	float zfar,
	float rotSpeed,
	float walkingSpeed) {
	this->pos = pos;
	this->rot = rot;
	this->fov = fov;
	this->apsect = aspect;
	this->znear = znear;
	this->zfar = zfar;
}

void Camera::update(float delta) {
	if (input_getGrab()) {
		int x, y;

		input_getMousePos(x, y);

		rot.x += this->rotSpeed * y * ((GRAPHICS_FPS_60 - delta > 0) ? delta : GRAPHICS_FPS_60);
		rot.y += this->rotSpeed * x * ((GRAPHICS_FPS_60 - delta > 0) ? delta : GRAPHICS_FPS_60);

		if (rot.y <= -360.0f) {
			rot.y += 360.0f;
		}

		if (rot.y >= 360.0f) {
			rot.y -= 360.0f;
		}

		if (rot.x < -90.0f) {
			rot.x = -90.0f;
		}

		if (rot.x > 90.0f) {
			rot.x = 90.0f;
		}

		float yrad = glm::radians(rot.y);

		float sp = this->walkingSpeed;

		if (input_isIMFromConfPress("move-run")) {
			sp *= 3.0f;
		}

		if (input_isIMFromConfPress("move-forward")) {
			pos.x += sp * glm::sin(yrad) * delta;
			pos.z -= sp * glm::cos(yrad) * delta;
		}

		if (input_isIMFromConfPress("move-backward")) {
			pos.x -= sp * glm::sin(yrad) * delta;
			pos.z += sp * glm::cos(yrad) * delta;
		}

		if (input_isIMFromConfPress("strafe-left")) {
			pos.x -= sp * glm::cos(yrad) * delta;
			pos.z -= sp * glm::sin(yrad) * delta;
		}

		if (input_isIMFromConfPress("strafe-right")) {
			pos.x += sp * glm::cos(yrad) * delta;
			pos.z += sp * glm::sin(yrad) * delta;
		}

		if (input_isIMFromConfPress("move-up")) {
			pos.y += sp * delta;
		}

		if (input_isIMFromConfPress("move-down")) {
			pos.y -= sp * delta;
		}
	}
}

glm::mat4 Camera::getProjection() {
	return glm::perspective(glm::radians(this->fov), this->apsect, this->znear, this->zfar);
}

glm::mat4 Camera::getView() {
	return
		glm::rotate(glm::mat4(1.0f), glm::radians(this->rot.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(this->rot.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::translate(glm::mat4(1.0f), -this->pos);
}

