#pragma once
#include <string>
#include "cherry/objects/Object.h"

class Obstacle : public cherry::Object {
public:
	//// constructors
	// default (dont use)
	Obstacle();
	
	// load in obj file and specify whether to load its associated mtl file
	Obstacle(std::string filePath, std::string scene, bool loadMtl = false);

	// same as above with option to specifiy a physics body size. This does not create the physics body!
	Obstacle(std::string filePath, std::string scene, cherry::Vec3 pbodySize, bool loadMtl = false);

	// Update (probably wont need unless we have animated/moving/rotating obstacles
	void Update(float deltaTime);

	//// angle stuff. Should maybe be used in level editor. TODO: get collision working for non axis alligned boudning boxes.
	// sets object angle in degrees or radians. bool is true if degrees, false, if radians
	void SetAngle(float angle, bool isDegrees);

	// sets object angle in vec3
	void SetAngle(glm::vec3 angle);

	// set draw pbody
	bool setDrawPBody(bool draw);

	// get draw pbody
	bool getDrawPBody();

	// get pbody size
	cherry::Vec3 getPBodySize();

	// get pbody width, height and depth.
	float getPBodyWidth();
	float getPBodyHeight();
	float getPBodyDepth();

private:
	// pbody stuff. self explanatory
	cherry::Vec3 pBodySize = cherry::Vec3(4, 4, 4);
	bool drawPBody = false;

	// TODO: possibly delete these variables? Object has its own rotation now.
	// object angle in screen space (degrees or radians)
	float degreeAngle, radianAngle;

	// object angle in world space (vec3, so 3d angle)
	glm::vec3 worldAngle;
};