#include "LevelLoader.h"
#include <toolkit/Logging.h>

// cell offset
const float cnz::Level::cellOffset = 6.25f;

cnz::Level::Level() {
	// uh, don't use this.
}

cnz::Level::Level(std::string legendPath, std::string levelPath, std::string sceneName) {
	this->sceneName = sceneName;

	if (!LoadLegend(legendPath)) { // wasn't valid
		LOG_ERROR("Legend CSV is NOT VALID. Exiting.");
		// TODO: not have a call to exit() in the final build.
		exit(1);
	}

	else {
		LOG_TRACE("Legend valid.");

		if (!LoadLevel(levelPath)) {
			LOG_ERROR("Level CSV is NOT VALID. Exiting.");
			// TODO: not have a call to exit() in the final build.
			exit(1);
		}

		else {
			LOG_TRACE("Level valid.");
			// GetObjects(); // this is commented out for now since it needs to be done AFTER creating and registering a scene and an object list.
			// Else, read access violation due to us creating objects using a scene name that does not match with the scene created by the Game default constructor.
		}
	}
}

std::string cnz::Level::GetSceneName() const {
	return this->sceneName;
}

bool cnz::Level::LoadLegend(std::string legendPath) {
	std::vector<std::string> names;
	std::vector<std::string> symbols;
	CSV legendCSV = CSV(legendPath);
	bool csvIsValid = true;

	//// Legend format: CSV[y][x], [0][0] should contain "Legend", [0][1] "Symbol", [0][2] "Name".
	// check to make sure format is valid
	if (legendCSV.GetRow(0, 0) != "Legend" || legendCSV.GetRow(0, 1) != "Symbol" || legendCSV.GetRow(0, 2) != "Name") {
		csvIsValid = false;
	}

	if (csvIsValid) {
		int numRows = legendCSV.Size();

		// because the first row and the first column do not contain data we care about, we need to skip them. (they are used for ensuring valid format)
		for (int i = 1; i < numRows; i++) { // arrays start at 0, so it has to be while < numRows, and we want to skip first row so int i = 1;
			symbols.push_back(legendCSV.GetRow(i, 1)); // since first col is always empty, names are in col 1 not 0.
			names.push_back(legendCSV.GetRow(i, 2)); // sumbols come after name col, so col 2.
		}
	}

	else {
		return false; // shit ain't valid, yo!
	}

	// sort names and symbols into legend map.
	for (int i = 0; i < symbols.size(); i++) {
		legend[symbols[i]] = names[i];
	}

	return true;
}

bool cnz::Level::LoadLevel(std::string levelPath) {
	CSV levelCSV = CSV(levelPath);
	std::vector<int> mapSize;

	//sceneName = levelCSV.GetRow(0, 0); // Don't do this, rather pass scene name as a string in constructor for easier use of scene manager
	mapSize = GetMapSize(levelCSV);

	if (mapSize.size() != 2) {
		return false;
	}

	else {
		this->map = GetMap(levelCSV);
		return true;
	}
}

std::vector<int> cnz::Level::GetMapSize(CSV level) {
	std::vector<int> wh;
	std::string temp = level.GetRow(0, 1); // this should contain a string with two ints, formatted as: int,int

	int commaIndex = temp.find('x');
	std::string widthStr = temp.substr(0, commaIndex);
	std::string heightStr = temp.substr(commaIndex + 1, temp.length());

	int width, height;

	try {
		width = std::stoi(widthStr);
		height = std::stoi(heightStr);
	}

	catch (const std::invalid_argument & ia) {
		std::cout << "couldn't convert str to int" << std::endl;
		return wh;
	}

	wh.push_back(width);
	wh.push_back(height);
	return wh;
}


/*
[[0,1,2,3,4],
 [5,6,7,8,9]]
*/


std::vector<std::vector<std::string>> cnz::Level::GetMap(CSV level) {
	std::vector<std::vector<std::string>> tempMap;
	std::vector<int> size = GetMapSize(level);

	for (int y = 1; y <= size[1]; y++) { // row iteration
		std::vector<std::string> row;
		for (int x = 1; x <= size[0]; x++) { // column iterator
			row.push_back(level.GetRow(y, x));
		}
		std::reverse(row.begin(), row.end()); // rows were backwards for some reason
		tempMap.push_back(row);
	}

	return tempMap;
}

std::vector<cherry::Object*> cnz::Level::GetObjects() {

	int offsetX, offsetY;

	cherry::Vec3 objBodySize;

	// put objects into list
	for (int y = 0; y < this->map.size(); y++) {
		for (int x = 0; x < this->map[0].size(); x++) {
			std::string curObj = this->map[y][x].substr(0, 1);
			
			if (legend[curObj] == "Origin") { // origin point
				offsetX = x;
				offsetY = y;
				cnz::Player* playerObj = new cnz::Player("res/objects/hero/charactoereee.obj", this->GetSceneName()); // creates the player.
				playerObj->SetPosition(glm::vec3(cellOffset * (x + offsetX), cellOffset * (y + offsetY), 0));
				this->objList.push_back(playerObj);
			}
			else if (legend[curObj] == "Wall") { // wall
				Obstacle* obj;
				if (this->wall == nullptr) {
					obj = new Obstacle("res/objects/GDW_1_Y2 - Wall Tile.obj", this->GetSceneName(), cherry::Vec3(4, 4, 1), true);
				}
				else {
					obj = new Obstacle(*this->wall);
				}
				objBodySize = (obj->GetMeshBodyMaximum() - obj->GetMeshBodyMinimum());
				objBodySize = cherry::Vec3(objBodySize.GetX(), objBodySize.GetZ(), objBodySize.GetY()); // something is wrong...
				obj->AddPhysicsBody(new cherry::PhysicsBodyBox(obj->GetPosition(), objBodySize));

				std::vector<float> properties = GetObjProps(y, x);
				cherry::Vec3 posOffset, rot;
				if (properties.size() == 0) { // no modifiers
					obj->SetPosition(glm::vec3(cellOffset * x, cellOffset * y, 0)); // no position offset, so just use map position * cell offset.
					obj->SetRotation(cherry::Vec3(90, 0, 0), true); // no rotation data, use default rotation
				}
				else if (properties.size() == 1) { // rotation modifier
					obj->SetPosition(glm::vec3(cellOffset * x, cellOffset * y, 0));// no position offset, so just use map position * cell offset.
					obj->SetRotation(cherry::Vec3(90, 0, properties[0]), true); // add rotation offset
					obj->GetPhysicsBodies()[0]->SetLocalRotationDegrees(cherry::Vec3(90, 0, 0)); // add rot to PB
				}
				else if (properties.size() == 3) { // position modifier
					obj->SetPosition(glm::vec3(cellOffset * x + properties[0], cellOffset * y + properties[1], 0 + properties[2])); // add position offsets
					obj->SetRotation(cherry::Vec3(90, 0, 0), true); // no rotation data, use default rotation
				}
				else if (properties.size() == 4) { // rotation and position modifiers
					obj->SetPosition(glm::vec3(cellOffset * x + properties[0], cellOffset * y + properties[1], 0 + properties[2])); // add position offsets
					obj->SetRotation(cherry::Vec3(90, 0, properties[3]), true); // add rotation offset
					obj->GetPhysicsBodies()[0]->SetLocalRotationDegrees(cherry::Vec3(90, 0, 0)); // add rot to PB

				}
				obj->GetPhysicsBodies()[0]->SetLocalPosition(cherry::Vec3(0, 0, 2));
				obj->GetPhysicsBodies()[0]->SetVisible(false);

				this->objList.push_back(obj);
			}
			else if (legend[curObj] == "Dumpster") { // Dumpster
				Obstacle* obj;
				if (this->wall == nullptr) {
					obj = new Obstacle("res/objects/props/Dumpster.obj", this->GetSceneName(), cherry::Vec3(10, 2, 2), true);
				}
				else {
					obj = new Obstacle(*this->dumpster);
				}
				objBodySize = (obj->GetMeshBodyMaximum() - obj->GetMeshBodyMinimum());
				obj->AddPhysicsBody(new cherry::PhysicsBodyBox(obj->GetPosition(), objBodySize));

				std::vector<float> properties = GetObjProps(y, x);
				cherry::Vec3 posOffset, rot;
				if (properties.size() == 0) { // no modifiers
					obj->SetPosition(glm::vec3(cellOffset * x, cellOffset * y, 0)); // no position offset, so just use map position * cell offset.
					obj->SetRotation(cherry::Vec3(90, 0, 0), true); // no rotation data, use default rotation
				}
				else if (properties.size() == 1) { // rotation modifier
					obj->SetPosition(glm::vec3(cellOffset * x, cellOffset * y, 0));// no position offset, so just use map position * cell offset.
					obj->SetRotation(cherry::Vec3(90, 0, properties[0]), true); // add rotation offset
					obj->GetPhysicsBodies()[0]->SetLocalRotationDegrees(cherry::Vec3(90, 0, 0)); // add rot to PB
				}
				else if (properties.size() == 3) { // position modifier
					obj->SetPosition(glm::vec3(cellOffset * x + properties[0], cellOffset * y + properties[1], 0 + properties[2])); // add position offsets
					obj->SetRotation(cherry::Vec3(90, 0, 0), true); // no rotation data, use default rotation
				}
				else if (properties.size() == 4) { // rotation and position modifiers
					obj->SetPosition(glm::vec3(cellOffset * x + properties[0], cellOffset * y + properties[1], 0 + properties[2])); // add position offsets
					obj->SetRotation(cherry::Vec3(90, 0, properties[3]), true); // add rotation offset
					obj->GetPhysicsBodies()[0]->SetLocalRotationDegrees(cherry::Vec3(90, 0, 0)); // add rot to PB
				}
				obj->GetPhysicsBodies()[0]->SetLocalPosition(cherry::Vec3(0, 0, 1));
				obj->GetPhysicsBodies()[0]->SetVisible(false);

				this->objList.push_back(obj);
			}
			else if (legend[curObj] == "Lamp post") { // Lamp post
				Obstacle* obj;
				if (this->wall == nullptr) {
					obj = new Obstacle("res/objects/props/Lamp_Side.obj", this->GetSceneName(), cherry::Vec3(1, 1, 6), true);
				}
				else {
					obj = new Obstacle(*this->lampPost);
				}
				objBodySize = (obj->GetMeshBodyMaximum() - obj->GetMeshBodyMinimum());
				obj->AddPhysicsBody(new cherry::PhysicsBodyBox(obj->GetPosition(), objBodySize));

				std::vector<float> properties = GetObjProps(y, x);
				cherry::Vec3 posOffset, rot;
				if (properties.size() == 0) { // no modifiers
					obj->SetPosition(glm::vec3(cellOffset * x, cellOffset * y, 0)); // no position offset, so just use map position * cell offset.
					obj->SetRotation(cherry::Vec3(90, 0, 0), true); // no rotation data, use default rotation
				}
				else if (properties.size() == 1) { // rotation modifier
					obj->SetPosition(glm::vec3(cellOffset * x, cellOffset * y, 0));// no position offset, so just use map position * cell offset.
					obj->SetRotation(cherry::Vec3(90, 0, properties[0]), true); // add rotation offset
					obj->GetPhysicsBodies()[0]->SetLocalRotationDegrees(cherry::Vec3(0, 0, 0)); // add rot to PB
				}
				else if (properties.size() == 3) { // position modifier
					obj->SetPosition(glm::vec3(cellOffset * x + properties[0], cellOffset * y + properties[1], 0 + properties[2])); // add position offsets
					obj->SetRotation(cherry::Vec3(90, 0, 0), true); // no rotation data, use default rotation
				}
				else if (properties.size() == 4) { // rotation and position modifiers
					obj->SetPosition(glm::vec3(cellOffset * x + properties[0], cellOffset * y + properties[1], 0 + properties[2])); // add position offsets
					obj->SetRotation(cherry::Vec3(90, 0, properties[3]), true); // add rotation offset
					obj->GetPhysicsBodies()[0]->SetLocalRotationDegrees(cherry::Vec3(0, 0, 0)); // add rot to PB
				}
				obj->GetPhysicsBodies()[0]->SetLocalPosition(cherry::Vec3(0, 0, 3));
				obj->GetPhysicsBodies()[0]->SetVisible(false);

				this->objList.push_back(obj);
			}
			else if (legend[curObj] == "Lamp post corner") { // Lamp post corner
				Obstacle* obj;
				if (this->wall == nullptr) {
					obj = new Obstacle("res/objects/props/Lamp_Corner.obj", this->GetSceneName(), cherry::Vec3(1, 1, 6), true);
				}
				else {
					obj = new Obstacle(*this->lampPostCorner);
				}
				objBodySize = (obj->GetMeshBodyMaximum() - obj->GetMeshBodyMinimum());
				obj->AddPhysicsBody(new cherry::PhysicsBodyBox(obj->GetPosition(), objBodySize));

				std::vector<float> properties = GetObjProps(y, x);
				cherry::Vec3 posOffset, rot;
				if (properties.size() == 0) { // no modifiers
					obj->SetPosition(glm::vec3(cellOffset * x, cellOffset * y, 0)); // no position offset, so just use map position * cell offset.
					obj->SetRotation(cherry::Vec3(90, 0, 0), true); // no rotation data, use default rotation
				}
				else if (properties.size() == 1) { // rotation modifier
					obj->SetPosition(glm::vec3(cellOffset * x, cellOffset * y, 0));// no position offset, so just use map position * cell offset.
					obj->SetRotation(cherry::Vec3(90, 0, properties[0]), true); // add rotation offset
					obj->GetPhysicsBodies()[0]->SetLocalRotationDegrees(cherry::Vec3(0, 0, 0)); // add rot to PB
				}
				else if (properties.size() == 3) { // position modifier
					obj->SetPosition(glm::vec3(cellOffset * x + properties[0], cellOffset * y + properties[1], 0 + properties[2])); // add position offsets
					obj->SetRotation(cherry::Vec3(90, 0, 0), true); // no rotation data, use default rotation
				}
				else if (properties.size() == 4) { // rotation and position modifiers
					obj->SetPosition(glm::vec3(cellOffset * x + properties[0], cellOffset * y + properties[1], 0 + properties[2])); // add position offsets
					obj->SetRotation(cherry::Vec3(90, 0, properties[3]), true); // add rotation offset
					obj->GetPhysicsBodies()[0]->SetLocalRotationDegrees(cherry::Vec3(0, 0, 0)); // add rot to PB
				}
				obj->GetPhysicsBodies()[0]->SetLocalPosition(cherry::Vec3(0, 0, 3));
				obj->GetPhysicsBodies()[0]->SetVisible(false);

				this->objList.push_back(obj);
			}
			else if (legend[curObj] == "Lamp post middle") { // lamp post middle
				Obstacle* obj;
				if (this->wall == nullptr) {
					obj = new Obstacle("res/objects/props/Lamp_Center.obj", this->GetSceneName(), cherry::Vec3(1, 1, 6), true);
				}
				else {
					obj = new Obstacle(*this->lampPostMiddle);
				}
				objBodySize = (obj->GetMeshBodyMaximum() - obj->GetMeshBodyMinimum());
				obj->AddPhysicsBody(new cherry::PhysicsBodyBox(obj->GetPosition(), objBodySize));
				std::vector<float> properties = GetObjProps(y, x);
				cherry::Vec3 posOffset, rot;
				if (properties.size() == 0) { // no modifiers
					obj->SetPosition(glm::vec3(cellOffset * x, cellOffset * y, 0)); // no position offset, so just use map position * cell offset.
					obj->SetRotation(cherry::Vec3(90, 0, 0), true); // no rotation data, use default rotation
				}
				else if (properties.size() == 1) { // rotation modifier
					obj->SetPosition(glm::vec3(cellOffset * x, cellOffset * y, 0));// no position offset, so just use map position * cell offset.
					obj->SetRotation(cherry::Vec3(90, 0, properties[0]), true); // add rotation offset
					obj->GetPhysicsBodies()[0]->SetLocalRotationDegrees(cherry::Vec3(0, 0, 0)); // add rot to PB
				}
				else if (properties.size() == 3) { // position modifier
					obj->SetPosition(glm::vec3(cellOffset * x + properties[0], cellOffset * y + properties[1], 0 + properties[2])); // add position offsets
					obj->SetRotation(cherry::Vec3(90, 0, 0), true); // no rotation data, use default rotation
				}
				else if (properties.size() == 4) { // rotation and position modifiers
					obj->SetPosition(glm::vec3(cellOffset * x + properties[0], cellOffset * y + properties[1], 0 + properties[2])); // add position offsets
					obj->SetRotation(cherry::Vec3(90, 0, properties[3]), true); // add rotation offset
					obj->GetPhysicsBodies()[0]->SetLocalRotationDegrees(cherry::Vec3(0, 0, 0)); // add rot to PB
				}
				obj->GetPhysicsBodies()[0]->SetLocalPosition(cherry::Vec3(0, 0, 3));
				obj->GetPhysicsBodies()[0]->SetVisible(false);

				this->objList.push_back(obj);
			}
			else if (legend[curObj] == "Barrel") { // barrel
				Obstacle* obj;
				if (this->wall == nullptr) {
					obj = new Obstacle("res/objects/props/drum.obj", this->GetSceneName(), cherry::Vec3(2, 2, 2), true);
				}
				else {
					obj = new Obstacle(*this->barrel);
				}
				objBodySize = (obj->GetMeshBodyMaximum() - obj->GetMeshBodyMinimum());
				obj->AddPhysicsBody(new cherry::PhysicsBodyBox(obj->GetPosition(), objBodySize));
				std::vector<float> properties = GetObjProps(y, x);
				cherry::Vec3 posOffset, rot;
				if (properties.size() == 0) { // no modifiers
					obj->SetPosition(glm::vec3(cellOffset * x, cellOffset * y, 0)); // no position offset, so just use map position * cell offset.
					obj->SetRotation(cherry::Vec3(90, 0, 0), true); // no rotation data, use default rotation
				}
				else if (properties.size() == 1) { // rotation modifier
					obj->SetPosition(glm::vec3(cellOffset * x, cellOffset * y, 0));// no position offset, so just use map position * cell offset.
					obj->SetRotation(cherry::Vec3(90, 0, properties[0]), true); // add rotation offset
					obj->GetPhysicsBodies()[0]->SetLocalRotationDegrees(cherry::Vec3(90, 0, 0)); // add rot to PB
				}
				else if (properties.size() == 3) { // position modifier
					obj->SetPosition(glm::vec3(cellOffset * x + properties[0], cellOffset * y + properties[1], 0 + properties[2])); // add position offsets
					obj->SetRotation(cherry::Vec3(90, 0, 0), true); // no rotation data, use default rotation
				}
				else if (properties.size() == 4) { // rotation and position modifiers
					obj->SetPosition(glm::vec3(cellOffset * x + properties[0], cellOffset * y + properties[1], 0 + properties[2])); // add position offsets
					obj->SetRotation(cherry::Vec3(90, 0, properties[3]), true); // add rotation offset
					obj->GetPhysicsBodies()[0]->SetLocalRotationDegrees(cherry::Vec3(90, 0, 0)); // add rot to PB
				}
				obj->GetPhysicsBodies()[0]->SetLocalPosition(cherry::Vec3(0, 0, 1));
				obj->GetPhysicsBodies()[0]->SetVisible(false);

				this->objList.push_back(obj);
			}
			else if (legend[curObj] == "Katana") { // katana
				Obstacle* obj;
				if (this->wall == nullptr) {
					obj = new Obstacle("res/objects/weapons/katana.obj", this->GetSceneName(), cherry::Vec3(1, 1, 2), true);
				}
				else {
					obj = new Obstacle(*this->katana);
				}
				objBodySize = (obj->GetMeshBodyMaximum() - obj->GetMeshBodyMinimum());
				obj->AddPhysicsBody(new cherry::PhysicsBodyBox(obj->GetPosition(), objBodySize));
				std::vector<float> properties = GetObjProps(y, x);
				cherry::Vec3 posOffset, rot;
				if (properties.size() == 0) { // no modifiers
					obj->SetPosition(glm::vec3(cellOffset * x, cellOffset * y, 0)); // no position offset, so just use map position * cell offset.
					obj->SetRotation(cherry::Vec3(90, 0, 0), true); // no rotation data, use default rotation
				}
				else if (properties.size() == 1) { // rotation modifier
					obj->SetPosition(glm::vec3(cellOffset * x, cellOffset * y, 0));// no position offset, so just use map position * cell offset.
					obj->SetRotation(cherry::Vec3(90, 0, properties[0]), true); // add rotation offset
					obj->GetPhysicsBodies()[0]->SetLocalRotationDegrees(cherry::Vec3(90, 0, 0)); // add rot to PB
				}
				else if (properties.size() == 3) { // position modifier
					obj->SetPosition(glm::vec3(cellOffset * x + properties[0], cellOffset * y + properties[1], 0 + properties[2])); // add position offsets
					obj->SetRotation(cherry::Vec3(90, 0, 0), true); // no rotation data, use default rotation
				}
				else if (properties.size() == 4) { // rotation and position modifiers
					obj->SetPosition(glm::vec3(cellOffset * x + properties[0], cellOffset * y + properties[1], 0 + properties[2])); // add position offsets
					obj->SetRotation(cherry::Vec3(90, 0, properties[3]), true); // add rotation offset
					obj->GetPhysicsBodies()[0]->SetLocalRotationDegrees(cherry::Vec3(90, 0, 0)); // add rot to PB
				}
				obj->GetPhysicsBodies()[0]->SetLocalPosition(cherry::Vec3(0, 0, 1));
				obj->GetPhysicsBodies()[0]->SetVisible(false);

				this->objList.push_back(obj);
			}
			else if (legend[curObj] == "Pillar") { // pillar
				Obstacle* obj;
				if (this->wall == nullptr) {
					obj = new Obstacle("res/objects/GDW_1_Y2 - Pillar.obj", this->GetSceneName(), cherry::Vec3(2, 2, 4), true);
				}
				else {
					obj = new Obstacle(*this->pillar);
				}
				objBodySize = (obj->GetMeshBodyMaximum() - obj->GetMeshBodyMinimum());
				obj->AddPhysicsBody(new cherry::PhysicsBodyBox(obj->GetPosition(), objBodySize));
				std::vector<float> properties = GetObjProps(y, x);
				cherry::Vec3 posOffset, rot;
				if (properties.size() == 0) { // no modifiers
					obj->SetPosition(glm::vec3(cellOffset * x, cellOffset * y, 0)); // no position offset, so just use map position * cell offset.
					obj->SetRotation(cherry::Vec3(90, 0, 0), true); // no rotation data, use default rotation
				}
				else if (properties.size() == 1) { // rotation modifier
					obj->SetPosition(glm::vec3(cellOffset * x, cellOffset * y, 0));// no position offset, so just use map position * cell offset.
					obj->SetRotation(cherry::Vec3(90, 0, properties[0]), true); // add rotation offset
					obj->GetPhysicsBodies()[0]->SetLocalRotationDegrees(cherry::Vec3(90, 0, 0)); // add rot to PB
				}
				else if (properties.size() == 3) { // position modifier
					obj->SetPosition(glm::vec3(cellOffset * x + properties[0], cellOffset * y + properties[1], 0 + properties[2])); // add position offsets
					obj->SetRotation(cherry::Vec3(90, 0, 0), true); // no rotation data, use default rotation
				}
				else if (properties.size() == 4) { // rotation and position modifiers
					obj->SetPosition(glm::vec3(cellOffset * x + properties[0], cellOffset * y + properties[1], 0 + properties[2])); // add position offsets
					obj->SetRotation(cherry::Vec3(90, 0, properties[3]), true); // add rotation offset
					obj->GetPhysicsBodies()[0]->SetLocalRotationDegrees(cherry::Vec3(90, 0, 0)); // add rot to PB
				}
				obj->GetPhysicsBodies()[0]->SetLocalPosition(cherry::Vec3(0, 0, 2));
				obj->GetPhysicsBodies()[0]->SetVisible(false);

				this->objList.push_back(obj);
			}
			else if (legend[curObj] == "Manhole cover") { // manhole cover
				Obstacle* obj;
				if (this->wall == nullptr) {
					obj = new Obstacle("res/objects/props/manhole.obj", this->GetSceneName(), cherry::Vec3(1, 1, 0.5), true);
				}
				else {
					obj = new Obstacle(*this->manHoleCover);
				}
				objBodySize = (obj->GetMeshBodyMaximum() - obj->GetMeshBodyMinimum());
				obj->AddPhysicsBody(new cherry::PhysicsBodyBox(obj->GetPosition(), objBodySize));
				std::vector<float> properties = GetObjProps(y, x);
				cherry::Vec3 posOffset, rot;
				if (properties.size() == 0) { // no modifiers
					obj->SetPosition(glm::vec3(cellOffset * x, cellOffset * y, 0)); // no position offset, so just use map position * cell offset.
					obj->SetRotation(cherry::Vec3(90, 0, 0), true); // no rotation data, use default rotation
				}
				else if (properties.size() == 1) { // rotation modifier
					obj->SetPosition(glm::vec3(cellOffset * x, cellOffset * y, 0));// no position offset, so just use map position * cell offset.
					obj->SetRotation(cherry::Vec3(90, 0, properties[0]), true); // add rotation offset
					obj->GetPhysicsBodies()[0]->SetLocalRotationDegrees(cherry::Vec3(90, 0, 0)); // add rot to PB
				}
				else if (properties.size() == 3) { // position modifier
					obj->SetPosition(glm::vec3(cellOffset * x + properties[0], cellOffset * y + properties[1], 0 + properties[2])); // add position offsets
					obj->SetRotation(cherry::Vec3(90, 0, 0), true); // no rotation data, use default rotation
				}
				else if (properties.size() == 4) { // rotation and position modifiers
					obj->SetPosition(glm::vec3(cellOffset * x + properties[0], cellOffset * y + properties[1], 0 + properties[2])); // add position offsets
					obj->SetRotation(cherry::Vec3(90, 0, properties[3]), true); // add rotation offset
					obj->GetPhysicsBodies()[0]->SetLocalRotationDegrees(cherry::Vec3(90, 0, 0)); // add rot to PB
				}
				obj->GetPhysicsBodies()[0]->SetLocalPosition(cherry::Vec3(0, 0, 0.25));
				obj->GetPhysicsBodies()[0]->SetVisible(false);

				this->objList.push_back(obj);
			}
			else if (legend[curObj] == "Road") { // road
				Obstacle* obj;
				if (this->wall == nullptr) {
					obj = new Obstacle("res/objects/props/Road.obj", this->GetSceneName(), cherry::Vec3(4, 4, 0.25), true);
				}
				else {
					obj = new Obstacle(*this->road);
				}
				objBodySize = (obj->GetMeshBodyMaximum() - obj->GetMeshBodyMinimum());
				obj->AddPhysicsBody(new cherry::PhysicsBodyBox(obj->GetPosition(), objBodySize));
				std::vector<float> properties = GetObjProps(y, x);
				cherry::Vec3 posOffset, rot;
				if (properties.size() == 0) { // no modifiers
					obj->SetPosition(glm::vec3(cellOffset * x, cellOffset * y, 0)); // no position offset, so just use map position * cell offset.
					obj->SetRotation(cherry::Vec3(90, 0, 0), true); // no rotation data, use default rotation
				}
				else if (properties.size() == 1) { // rotation modifier
					obj->SetPosition(glm::vec3(cellOffset * x, cellOffset * y, 0));// no position offset, so just use map position * cell offset.
					obj->SetRotation(cherry::Vec3(90, 0, properties[0]), true); // add rotation offset
					obj->GetPhysicsBodies()[0]->SetLocalRotationDegrees(cherry::Vec3(90, 0, 0)); // add rot to PB
				}
				else if (properties.size() == 3) { // position modifier
					obj->SetPosition(glm::vec3(cellOffset * x + properties[0], cellOffset * y + properties[1], 0 + properties[2])); // add position offsets
					obj->SetRotation(cherry::Vec3(90, 0, 0), true); // no rotation data, use default rotation
				}
				else if (properties.size() == 4) { // rotation and position modifiers
					obj->SetPosition(glm::vec3(cellOffset * x + properties[0], cellOffset * y + properties[1], 0 + properties[2])); // add position offsets
					obj->SetRotation(cherry::Vec3(90, 0, properties[3]), true); // add rotation offset
					obj->GetPhysicsBodies()[0]->SetLocalRotationDegrees(cherry::Vec3(90, 0, 0)); // add rot to PB
				}
				obj->GetPhysicsBodies()[0]->SetLocalPosition(cherry::Vec3(0, 0, 0.125));
				obj->GetPhysicsBodies()[0]->SetVisible(false);

				this->objList.push_back(obj);
			}
			else if (legend[curObj] == "Sidewalk") { // sidewalk
				Obstacle* obj;
				if (this->wall == nullptr) {
					obj = new Obstacle("res/objects/props/sidewalk.obj", this->GetSceneName(), cherry::Vec3(4, 4, 0.25), true);
				}
				else {
					obj = new Obstacle(*this->sidewalk);
				}
				objBodySize = (obj->GetMeshBodyMaximum() - obj->GetMeshBodyMinimum());
				obj->AddPhysicsBody(new cherry::PhysicsBodyBox(obj->GetPosition(), objBodySize));
				std::vector<float> properties = GetObjProps(y, x);
				cherry::Vec3 posOffset, rot;
				if (properties.size() == 0) { // no modifiers
					obj->SetPosition(glm::vec3(cellOffset * x, cellOffset * y, 0)); // no position offset, so just use map position * cell offset.
					obj->SetRotation(cherry::Vec3(90, 0, 0), true); // no rotation data, use default rotation
				}
				else if (properties.size() == 1) { // rotation modifier
					obj->SetPosition(glm::vec3(cellOffset * x, cellOffset * y, 0));// no position offset, so just use map position * cell offset.
					obj->SetRotation(cherry::Vec3(90, 0, properties[0]), true); // add rotation offset
					obj->GetPhysicsBodies()[0]->SetLocalRotationDegrees(cherry::Vec3(90, 0, 0)); // add rot to PB
				}
				else if (properties.size() == 3) { // position modifier
					obj->SetPosition(glm::vec3(cellOffset * x + properties[0], cellOffset * y + properties[1], 0 + properties[2])); // add position offsets
					obj->SetRotation(cherry::Vec3(90, 0, 0), true); // no rotation data, use default rotation
				}
				else if (properties.size() == 4) { // rotation and position modifiers
					obj->SetPosition(glm::vec3(cellOffset * x + properties[0], cellOffset * y + properties[1], 0 + properties[2])); // add position offsets
					obj->SetRotation(cherry::Vec3(90, 0, properties[3]), true); // add rotation offset
					obj->GetPhysicsBodies()[0]->SetLocalRotationDegrees(cherry::Vec3(90, 0, 0)); // add rot to PB
				}
				obj->GetPhysicsBodies()[0]->SetLocalPosition(cherry::Vec3(0, 0, 0.125));
				obj->GetPhysicsBodies()[0]->SetVisible(false);

				this->objList.push_back(obj);
			}
		}
	}

	return this->objList;
}

std::vector<float> cnz::Level::GetObjProps(int y, int x) {
	std::vector<float> properties;
	std::string cell = this->map[y][x];
	int strLen = cell.size();
	int firstDash = cell.find("~");
	int secondDash = cell.find("~", firstDash + 1); // find first period after index firstDash. Should return second dash.


	// CASES
	// no first dash, no pos offset or rotation
	// first dash, no second dash. Check if data has brackets or not. if brackets, collect three floats. if no brackets collect one float.
	// both dashs. Collect three floats from within brackets after first dash. Collect one float from after second dash

	// "P-(x,y,z)" (size = 9)
	// "P-12.66" (size = 7)
	// "P-(x,y,z)-12.66" (size = 15)
	// string.substr(inclusive, exclusive);

	if (firstDash == std::string::npos) { // no position or rotation data, return empty properties list
		return properties;
	}
	else if (firstDash != std::string::npos && secondDash == std::string::npos) { // there is one dash, figure out if data is pos offset or rotation and add it to list
		std::string data = cell.substr(firstDash + 1, std::string::npos); // get everything after dash
		int bracket = data.find('('); // check if there is a bracket in the data

		if (bracket == std::string::npos) { // no bracket, data is rotation
			properties.push_back(stof(data));
			return properties;
		}
		else { // bracket exists, data is pos offset. Collect three floats
			int secondBracket = data.find(')');
			std::string newData = data.substr(bracket + 1, secondBracket - (bracket + 1)); // remove brakets from string
			int firstUScore = newData.find('_');
			int secondUScore = newData.find('_', firstUScore + 1);
			std::string strX = newData.substr(0, firstUScore);
			std::string strY = newData.substr(firstUScore + 1, secondUScore - (firstUScore + 1));
			std::string strZ = newData.substr(secondUScore + 1, std::string::npos);

			float xOff = stof(strX);
			float yOff = stof(strY);
			float zOff = stof(strZ);

			properties.push_back(xOff);
			properties.push_back(yOff);
			properties.push_back(zOff);

			return properties;
		}
	}
	else { // neither firstDash nor secondDash are std::string::npos, there is pos offset and rotation data. Collect 4 floats
		std::string posData = cell.substr(firstDash + 1, secondDash - (firstDash + 1));
		std::string rotData = cell.substr(secondDash + 1, std::string::npos);

		int bracket = posData.find('(');
		int secondBracket = posData.find(')');
		std::string newData = posData.substr(bracket + 1, secondBracket - (bracket + 1)); // remove brakets from string
		int firstUScore = newData.find('_');
		int secondUScore = newData.find('_', firstUScore + 1);
		std::string strX = newData.substr(0, firstUScore);
		std::string strY = newData.substr(firstUScore + 1, secondUScore - (firstUScore + 1));
		std::string strZ = newData.substr(secondUScore + 1, std::string::npos);

		float xOff = stof(strX);
		float yOff = stof(strY);
		float zOff = stof(strZ);

		properties.push_back(xOff);
		properties.push_back(yOff);
		properties.push_back(zOff);

		float rot = stof(rotData);

		properties.push_back(rot);

		return properties;
	}
}
