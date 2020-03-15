#include "LevelLoader.h"
#include "cherry/scenes/SceneManager.h"
#include <toolkit/Logging.h>

// sources
cnz::Player* cnz::Level::sourcePlayer = nullptr;
cnz::Sentry* cnz::Level::sourceSentry = nullptr;
cnz::Oracle* cnz::Level::sourceOracle = nullptr;
cnz::Marauder* cnz::Level::sourceMarauder = nullptr;
cnz::Bastion* cnz::Level::sourceBastion = nullptr;
cnz::Mechaspider* cnz::Level::sourceSpider = nullptr;

// cell offset
const float cnz::Level::cellOffset = 6.25f;

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
	return sceneName;
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

// generates objects
std::vector<cherry::Object*> cnz::Level::GenerateObjects() {
	int offsetX = 0, offsetY = 0;
	cherry::Vec3 objBodySize;

	// gets the scene object list and light object list
	cherry::ObjectList* objectList = cherry::ObjectManager::GetSceneObjectListByName(sceneName);
	cherry::LightList* lightList = cherry::LightManager::GetSceneLightListByName(sceneName);

	// object list does not exist, so it must be created.
	if (objectList == nullptr)
	{
		cherry::ObjectManager::CreateSceneObjectList(sceneName);
		objectList = cherry::ObjectManager::GetSceneObjectListByName(sceneName);
	}

	// light list does not exist, so it must be created.
	if (lightList)
	{
		cherry::LightManager::CreateSceneLightList(sceneName);
		lightList = cherry::LightManager::GetSceneLightListByName(sceneName);
	}

	// clearing the lists before giving them values
	objects.clear();
	obstacles.clear();

	// put objects into list
	for (int y = 0; y < this->map.size(); y++) {
		for (int x = 0; x < this->map[0].size(); x++) {
			std::string curObj = this->map[y][x].substr(0, 1);
			
			if (legend[curObj] == "Origin") { // origin point
				offsetX = x;
				offsetY = y;
				
				// if the source player hasn't been made yet.
				if (sourcePlayer == nullptr)
					GenerateSources();

				playerObj = new Player(sourcePlayer, sceneName);

				// original
				// playerObj = cnz::Player::GenerateDefault(sceneName);
				
				// heroes
				// playerObj = new cnz::Player("res/objects/hero/charactoereee.obj", this->sceneName); // creates the player.
				
				
				playerObj->SetPosition(glm::vec3(cellOffset * (x), cellOffset * (y), 0));

				// new
				// cnz::Player* playerObj = cnz::Player::GenerateDefault(sceneName, 
				// 	glm::vec3(cellOffset * (x + offsetX), cellOffset * (y + offsetY), 0)); // creates the player.
				
				// adding them to the lists
				objectList->AddObject(playerObj);
				objects.push_back(playerObj);
			}
			else if (legend[curObj] == "Wall") { // wall // TODO: orient walls properly.
				Obstacle* obj;
				obj = new Obstacle("res/objects/GDW_1_Y2 - Wall Tile.obj", this->sceneName, true);
				// if (wall == nullptr) {
				// 	wall = new Obstacle("res/objects/GDW_1_Y2 - Wall Tile.obj", this->sceneName, true);
				// 	obj = wall;
				// 	// obj = new Obstacle("res/objects/GDW_1_Y2 - Wall Tile.obj", this->sceneName, true);
				// }
				// else {
				// 	obj = new Obstacle(wall, sceneName);
				// }

				obj->SetPBodySize(UnFlipVec3((obj->GetMeshBodyMaximum() - obj->GetMeshBodyMinimum())));
				obj->AddPhysicsBody(new cherry::PhysicsBodyBox(obj->GetPosition(), obj->GetPBodySize()));

				std::vector<float> properties = GetObjectProps(y, x);
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

				// adding them to the lists
				objectList->AddObject(obj);
				objects.push_back(obj);
				obstacles.push_back(obj);
			}
			else if (legend[curObj] == "Dumpster") { // Dumpster
				Obstacle* obj;
				obj = new Obstacle("res/objects/props/Dumpster.obj", this->sceneName, true);
				// if (this->dumpster == nullptr) {
				// 	dumpster = new Obstacle("res/objects/props/Dumpster.obj", this->sceneName, true);
				// 	obj = dumpster;
				// 
				// 	// obj = new Obstacle("res/objects/props/Dumpster.obj", this->sceneName, true);
				// }
				// else {
				// 	obj = new Obstacle(this->dumpster, sceneName);
				// }
				obj->SetPBodySize(UnFlipVec3((obj->GetMeshBodyMaximum() - obj->GetMeshBodyMinimum())));
				obj->AddPhysicsBody(new cherry::PhysicsBodyBox(obj->GetPosition(), obj->GetPBodySize()));

				std::vector<float> properties = GetObjectProps(y, x);
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

				// adding to the lists 
				objectList->AddObject(obj);
				objects.push_back(obj);
				obstacles.push_back(obj);
			}
			else if (legend[curObj] == "Lamp post") { // Lamp post
				Obstacle* obj;
				obj = new Obstacle("res/objects/props/Lamp_Side.obj", this->sceneName, true);
				// if (this->lampPost == nullptr) {
				// 	lampPost = new Obstacle("res/objects/props/Lamp_Side.obj", this->sceneName, true);
				// 	obj = lampPost;
				// 	// obj = new Obstacle("res/objects/props/Lamp_Side.obj", this->sceneName, true);
				// }
				// else {
				// 	obj = new Obstacle(lampPost, sceneName);
				// }
				//obj->SetPBodySize(UnFlipVec3((obj->GetMeshBodyMaximum() - obj->GetMeshBodyMinimum())));
				//obj->SetPBodySize((obj->GetMeshBodyMaximum() - obj->GetMeshBodyMinimum()));
				cherry::Vec3 objpbsize = (obj->GetMeshBodyMaximum() - obj->GetMeshBodyMinimum());
				obj->AddPhysicsBody(new cherry::PhysicsBodyBox(obj->GetPosition(), objpbsize));

				std::vector<float> properties = GetObjectProps(y, x);
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
				obj->GetPhysicsBodies()[0]->SetLocalRotationDegrees(cherry::Vec3(0, 0, 0));
				obj->GetPhysicsBodies()[0]->SetVisible(false);

				// adding a light
				lightList->AddLight(new cherry::Light(sceneName, obj->GetPosition(), cherry::Vec3(1.0f, 1.0f, 1.0f), cherry::Vec3(0.5f, 0.5f, 0.5f), 0.1f, 0.7f, 0.6f, 1.0f / 100.0f));


				// adding to the lists
				objectList->AddObject(obj);
				objects.push_back(obj);
				obstacles.push_back(obj);
			}
			else if (legend[curObj] == "Lamp post corner") { // Lamp post corner
				Obstacle* obj;
				obj = new Obstacle("res/objects/props/Lamp_Corner.obj", this->sceneName, true);
				// if (lampPostCorner == nullptr) {
				// 	lampPostCorner = new Obstacle("res/objects/props/Lamp_Corner.obj", this->sceneName, true);
				// 	obj = lampPostCorner;
				// }
				// else {
				// 	obj = new Obstacle(lampPostCorner, sceneName);
				// }
				obj->SetPBodySize(UnFlipVec3((obj->GetMeshBodyMaximum() - obj->GetMeshBodyMinimum())));
				obj->AddPhysicsBody(new cherry::PhysicsBodyBox(obj->GetPosition(), obj->GetPBodySize()));

				std::vector<float> properties = GetObjectProps(y, x);
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
				lightList->AddLight(new cherry::Light(sceneName, obj->GetPosition(), cherry::Vec3(1.0f, 1.0f, 1.0f), cherry::Vec3(0.5f, 0.5f, 0.5f), 0.1f, 0.7f, 0.6f, 1.0f / 100.0f));

				// adding to the lists
				objectList->AddObject(obj);
				objects.push_back(obj);
				obstacles.push_back(obj);
			}
			else if (legend[curObj] == "Lamp post middle") { // lamp post middle
				Obstacle* obj;
				obj = new Obstacle("res/objects/props/Lamp_Center.obj", this->sceneName, true);
				// if (lampPostMiddle == nullptr) {
				// 	lampPostMiddle = new Obstacle("res/objects/props/Lamp_Center.obj", this->sceneName, true);
				// 	obj = lampPostMiddle;
				// }
				// else {
				// 	obj = new Obstacle(lampPostMiddle, sceneName);
				// }
				obj->SetPBodySize(UnFlipVec3((obj->GetMeshBodyMaximum() - obj->GetMeshBodyMinimum())));
				obj->AddPhysicsBody(new cherry::PhysicsBodyBox(obj->GetPosition(), obj->GetPBodySize()));

				std::vector<float> properties = GetObjectProps(y, x);
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

				// light
				lightList->AddLight(new cherry::Light(sceneName, obj->GetPosition(), cherry::Vec3(1.0f, 1.0f, 1.0f), cherry::Vec3(0.5f, 0.5f, 0.5f), 0.1f, 0.7f, 0.6f, 1.0f / 100.0f));


				// adding to hte lists
				objectList->AddObject(obj);
				objects.push_back(obj);
				obstacles.push_back(obj);
			}
			else if (legend[curObj] == "Barrel") { // barrel
				Obstacle* obj;
				obj = new Obstacle("res/objects/props/drum.obj", this->sceneName, true);
				// if (this->barrel == nullptr) {
				// 	barrel = new Obstacle("res/objects/props/drum.obj", this->sceneName, true);
				// 	obj = barrel;
				// }
				// else {
				// 	obj = new Obstacle(barrel, sceneName);
				// }
				obj->SetPBodySize(UnFlipVec3((obj->GetMeshBodyMaximum() - obj->GetMeshBodyMinimum())));
				obj->AddPhysicsBody(new cherry::PhysicsBodyBox(obj->GetPosition(), obj->GetPBodySize()));

				std::vector<float> properties = GetObjectProps(y, x);
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

				// adding to the lists
				objectList->AddObject(obj);
				objects.push_back(obj);
				obstacles.push_back(obj);
			}
			else if (legend[curObj] == "Katana") { // katana
				Obstacle* obj;
				obj = new Obstacle("res/objects/weapons/katana.obj", this->sceneName, true);
				// if (this->katana == nullptr) {
				// 	katana = new Obstacle("res/objects/weapons/katana.obj", this->sceneName, true);
				// 	obj = katana;
				// }
				// else {
				// 	obj = new Obstacle(katana, sceneName);
				// }
				obj->SetPBodySize(UnFlipVec3((obj->GetMeshBodyMaximum() - obj->GetMeshBodyMinimum())));
				obj->AddPhysicsBody(new cherry::PhysicsBodyBox(obj->GetPosition(), obj->GetPBodySize()));

				std::vector<float> properties = GetObjectProps(y, x);
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

				// adding to the lists
				objectList->AddObject(obj);
				objects.push_back(obj);
				obstacles.push_back(obj);
			}
			else if (legend[curObj] == "Pillar") { // pillar
				Obstacle* obj;
				obj = new Obstacle("res/objects/GDW_1_Y2 - Pillar.obj", this->sceneName, true);
				// if (pillar == nullptr) {
				// 	pillar = new Obstacle("res/objects/GDW_1_Y2 - Pillar.obj", this->sceneName, true);
				// 	obj = pillar;
				// }
				// else {
				// 	obj = new Obstacle(pillar, sceneName);
				// }
				obj->SetPBodySize(UnFlipVec3((obj->GetMeshBodyMaximum() - obj->GetMeshBodyMinimum())));
				obj->AddPhysicsBody(new cherry::PhysicsBodyBox(obj->GetPosition(), obj->GetPBodySize()));

				std::vector<float> properties = GetObjectProps(y, x);
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

				// adding to the lists
				objectList->AddObject(obj);
				objects.push_back(obj);
				obstacles.push_back(obj);
			}
			else if (legend[curObj] == "Manhole cover") { // manhole cover
				Obstacle* obj;
				obj = new Obstacle("res/objects/props/manhole.obj", this->sceneName, true);
				// if (manHoleCover == nullptr) {
				// 	manHoleCover = new Obstacle("res/objects/props/manhole.obj", this->sceneName, true);
				// 	obj = manHoleCover;
				// }
				// else {
				// 	obj = new Obstacle(manHoleCover, sceneName);
				// }
				obj->SetPBodySize(UnFlipVec3((obj->GetMeshBodyMaximum() - obj->GetMeshBodyMinimum())));
				obj->AddPhysicsBody(new cherry::PhysicsBodyBox(obj->GetPosition(), obj->GetPBodySize()));

				std::vector<float> properties = GetObjectProps(y, x);
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

				// adding to the lists
				objectList->AddObject(obj);
				objects.push_back(obj);
				obstacles.push_back(obj);
			}
			else if (legend[curObj] == "Road") { // road
				Obstacle* obj;
				obj = new Obstacle("res/objects/props/Road.obj", this->sceneName, true);
				// if (this->road == nullptr) {
				// 	road = new Obstacle("res/objects/props/Road.obj", this->sceneName, true);
				// 	obj = road;
				// }
				// else {
				// 	obj = new Obstacle(road, sceneName);
				// }
				// obj->SetPBodySize(UnFlipVec3((obj->GetMeshBodyMaximum() - obj->GetMeshBodyMinimum())));
				obj->SetPBodySize(obj->GetMeshBodyMaximum() - obj->GetMeshBodyMinimum());
				obj->AddPhysicsBody(new cherry::PhysicsBodyBox(obj->GetPosition(), obj->GetPBodySize()));

				std::vector<float> properties = GetObjectProps(y, x);
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
				// moved down because the player kept colliding with it.
				// obj->GetPhysicsBodies()[0]->SetLocalPosition(cherry::Vec3(0, 0, 0.125));
				obj->GetPhysicsBodies()[0]->SetLocalPosition(cherry::Vec3(0, 0, -1.0));
				obj->GetPhysicsBodies()[0]->SetVisible(false);

				// adding to the lists
				objectList->AddObject(obj);
				objects.push_back(obj);
				obstacles.push_back(obj);
			}
			else if (legend[curObj] == "Sidewalk") { // sidewalk
				Obstacle* obj;
				obj = new Obstacle("res/objects/props/sidewalk.obj", this->sceneName, true);
				// if (sidewalk == nullptr) {
				// 	sidewalk = new Obstacle("res/objects/props/sidewalk.obj", this->sceneName, true);
				// 	obj = sidewalk;
				// }
				// else {
				// 	obj = new Obstacle(sidewalk, sceneName);
				// }
				obj->SetPBodySize(UnFlipVec3((obj->GetMeshBodyMaximum() - obj->GetMeshBodyMinimum())));
				obj->AddPhysicsBody(new cherry::PhysicsBodyBox(obj->GetPosition(), obj->GetPBodySize()));

				std::vector<float> properties = GetObjectProps(y, x);
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

				// adding to the lists
				objectList->AddObject(obj);
				objects.push_back(obj);
				obstacles.push_back(obj);
			}
		}
	}

	return objects;
}

// generates the default objects and puts them into the scene.
std::vector<cherry::Object*> cnz::Level::GenerateDefaults()
{
	// gets the scene object list and light object list
	cherry::ObjectList* objectList = cherry::ObjectManager::GetSceneObjectListByName(sceneName);
	cherry::LightList* lightList = cherry::LightManager::GetSceneLightListByName(sceneName);

	// object list does not exist, so it must be created.
	if (objectList == nullptr)
	{
		cherry::ObjectManager::CreateSceneObjectList(sceneName);
		objectList = cherry::ObjectManager::GetSceneObjectListByName(sceneName);
	}

	// light list does not exist, so it must be created.
	if (lightList)
	{
		cherry::LightManager::CreateSceneLightList(sceneName);
		lightList = cherry::LightManager::GetSceneLightListByName(sceneName);
	}

	// clearing the lists before giving them values
	objects.clear();
	obstacles.clear();

	playerObj = cnz::Player::GenerateDefault(sceneName); // creates the player.
	cnz::Player* testObj = new Player("res/objects/monkey.obj", sceneName); // creates the not player.

	// arena obstacles
	Obstacle* wall1 = new Obstacle("res/objects/GDW_1_Y2 - Wall Tile.obj", sceneName, cherry::Vec3(10, 2, 2));
	Obstacle* wall2 = new Obstacle("res/objects/GDW_1_Y2 - Wall Tile.obj", sceneName, cherry::Vec3(2, 2, 2));
	Obstacle* wall3 = new Obstacle("res/objects/GDW_1_Y2 - Wall Tile.obj", sceneName, cherry::Vec3(2, 2, 2));
	Obstacle* wall4 = new Obstacle("res/objects/GDW_1_Y2 - Wall Tile.obj", sceneName, cherry::Vec3(2, 2, 2));
	Obstacle* wall5 = new Obstacle("res/objects/GDW_1_Y2 - Wall Tile.obj", sceneName, cherry::Vec3(2, 2, 2));
	Obstacle* wall6 = new Obstacle("res/objects/GDW_1_Y2 - Wall Tile.obj", sceneName, cherry::Vec3(2, 2, 2));
	Obstacle* wall7 = new Obstacle("res/objects/GDW_1_Y2 - Wall Tile.obj", sceneName, cherry::Vec3(2, 2, 2));


	Obstacle * bow = new Obstacle("res/objects/weapons/bow.obj", sceneName, false);
	Obstacle * katana = new Obstacle("res/objects/weapons/katana.obj", sceneName, false);
	Obstacle * spear = new Obstacle("res/objects/weapons/spear.obj", sceneName, false);
	obstacles.push_back(bow);
	obstacles.push_back(katana);
	obstacles.push_back(spear);

	Obstacle * drum = new Obstacle("res/objects/props/drum.obj", sceneName, false);
	Obstacle * dumpster = new Obstacle("res/objects/props/Dumpster.obj", sceneName, false);
	Obstacle * lamp_Center = new Obstacle("res/objects/props/Lamp_Center.obj", sceneName, false);
	Obstacle * lamp_Corner = new Obstacle("res/objects/props/Lamp_Corner.obj", sceneName, false);
	Obstacle * lamp_Side = new Obstacle("res/objects/props/Lamp_Side.obj", sceneName, false);
	Obstacle * manhole = new Obstacle("res/objects/props/manhole.obj", sceneName, false);
	Obstacle * piller = new Obstacle("res/objects/GDW_1_Y2 - Pillar.obj", sceneName, false);
	Obstacle * road = new Obstacle("res/objects/props/Road.obj", sceneName, false);

	obstacles.push_back(drum);
	obstacles.push_back(dumpster);
	obstacles.push_back(lamp_Center);
	obstacles.push_back(lamp_Corner);
	obstacles.push_back(lamp_Side);
	obstacles.push_back(manhole);
	obstacles.push_back(piller);
	obstacles.push_back(road);

	// big wall bois
	obstacles.push_back(wall1);
	obstacles.push_back(wall2);
	obstacles.push_back(wall3);
	obstacles.push_back(wall4);
	obstacles.push_back(wall5);
	obstacles.push_back(wall6);
	obstacles.push_back(wall7);

	// rotations
	playerObj->SetRotation(cherry::Vec3(0, 0, 0), true);
	playerObj->SetRotationXDegrees(90);
	playerObj->SetRotationZDegrees(180);
	testObj->SetRotation(cherry::Vec3(0, 0, 0), true);

	wall1->SetRotation(cherry::Vec3(90, 0, 180), true); // top wall
	wall2->SetRotation(cherry::Vec3(90, 0, 0), true); // bottom wall
	wall3->SetRotation(cherry::Vec3(90, 0, 0), true); // bottom wall
	wall4->SetRotation(cherry::Vec3(90, 0, 90), true); // right wall
	wall5->SetRotation(cherry::Vec3(90, 0, 90), true); // right wall
	wall6->SetRotation(cherry::Vec3(90, 0, 270), true); // left wall
	wall7->SetRotation(cherry::Vec3(90, 0, 270), true); // left wall

	// positions
	testObj->SetPosition(cherry::Vec3(0, -5, 0));
	wall1->SetPosition(cherry::Vec3(15, -1, 0));
	wall2->SetPosition(cherry::Vec3(15, 14, 0));
	wall3->SetPosition(cherry::Vec3(8, 14, 0));
	wall4->SetPosition(cherry::Vec3(4, 10, 0));
	wall5->SetPosition(cherry::Vec3(4, 3, 0));
	wall6->SetPosition(cherry::Vec3(19, 10, 0));
	wall7->SetPosition(cherry::Vec3(19, 3, 0));

	// scale. if needed.


	// attach pbody
	// TODO: outdated size
	playerObj->AddPhysicsBody(new cherry::PhysicsBodyBox(playerObj->GetPosition(), playerObj->GetPBodySize()));
	testObj->AddPhysicsBody(new cherry::PhysicsBodyBox(testObj->GetPosition(), testObj->GetPBodySize()));
	wall1->AddPhysicsBody(new cherry::PhysicsBodyBox(wall1->GetPosition(), wall1->GetPBodySize()));
	wall2->AddPhysicsBody(new cherry::PhysicsBodyBox(wall2->GetPosition(), wall2->GetPBodySize()));
	wall3->AddPhysicsBody(new cherry::PhysicsBodyBox(wall3->GetPosition(), wall3->GetPBodySize()));
	wall4->AddPhysicsBody(new cherry::PhysicsBodyBox(wall4->GetPosition(), wall4->GetPBodySize()));
	wall5->AddPhysicsBody(new cherry::PhysicsBodyBox(wall5->GetPosition(), wall5->GetPBodySize()));
	wall6->AddPhysicsBody(new cherry::PhysicsBodyBox(wall6->GetPosition(), wall6->GetPBodySize()));
	wall7->AddPhysicsBody(new cherry::PhysicsBodyBox(wall7->GetPosition(), wall7->GetPBodySize()));

	// set pbody pos and maybe rotation for static objects
	//testObj->GetPhysicsBodies()[0]->SetLocalPosition(testObj->GetPosition());
	// wall1->GetPhysicsBodies()[0]->SetLocalPosition(wall1->GetPosition());
	// wall2->GetPhysicsBodies()[0]->SetLocalPosition(wall2->GetPosition());
	// wall3->GetPhysicsBodies()[0]->SetLocalPosition(wall3->GetPosition());
	// wall4->GetPhysicsBodies()[0]->SetLocalPosition(wall4->GetPosition());
	// wall5->GetPhysicsBodies()[0]->SetLocalPosition(wall5->GetPosition());
	// wall6->GetPhysicsBodies()[0]->SetLocalPosition(wall6->GetPosition());
	// wall7->GetPhysicsBodies()[0]->SetLocalPosition(wall7->GetPosition());

	// debug draw pbdody
	// wall1->GetPhysicsBodies()[0]->SetVisible(showPBs);
	// wall2->GetPhysicsBodies()[0]->SetVisible(showPBs);
	// wall3->GetPhysicsBodies()[0]->SetVisible(showPBs);
	// wall4->GetPhysicsBodies()[0]->SetVisible(showPBs);
	// wall5->GetPhysicsBodies()[0]->SetVisible(showPBs);
	// wall6->GetPhysicsBodies()[0]->SetVisible(showPBs);
	// wall7->GetPhysicsBodies()[0]->SetVisible(showPBs);

	playerObj->GetPhysicsBodies()[0]->SetVisible(true);

	// Path stuff
	cherry::Path testPath = cherry::Path();
	testPath.AddNode(1.0f, 1.0f, 0.0f);
	testPath.AddNode(0.0f, 5.0f, 0.0f);
	testPath.AddNode(-2.0f, 5.0f, 0.0f);
	testPath.AddNode(-3.0f, 7.0f, 0.0f);
	testPath.AddNode(-6.0f, 8.0f, 0.0f);
	testPath.AddNode(-6.0f, 6.0f, 0.0f);
	testPath.AddNode(-2.0f, 4.0f, 0.0f);
	testPath.AddNode(1.0f, 1.0f, 0.0f);

	testPath.SetIncrementer(0.5f);
	testPath.SetInterpolationMode(1);

	testObj->SetPath(testPath, true);

	// add objects
	objectList->AddObject(playerObj);
	objectList->AddObject(testObj);
	
	int x = -27;
	for (int i = 0; i < obstacles.size(); i++) {
		objectList->AddObject(obstacles[i]);

		if (i > 0 && i < 11) {
			obstacles[i]->SetRotationXDegrees(90);
			obstacles[i]->SetRotationZDegrees(180);
			obstacles[i]->SetPosition(x, -40, 0);
		}
		x += 5;
	}

	road->SetRotationXDegrees(90);
	road->SetRotationZDegrees(180);
	road->SetPosition(0, -30, -1);
	manhole->SetPosition(manhole->GetPosition().GetX(), manhole->GetPosition().GetY(), -1);

	// returning hte objects
	return objectList->objects;
}

// generates the soruce objects.
void cnz::Level::GenerateSources()
{
	// scene name
	std::string sceneName = "rand_";

	// character to be added
	int chr = 0;

	for (int i = 0; i < 10; i++)
	{
		chr = rand() % 256;

		// sceneName
		switch (rand() % 2)
		{
		case 1:
			sceneName += std::to_string(chr); // int
			break;
		case 0:
		default:
			sceneName.push_back(chr);
		}
	}

	// creating the scene
	cherry::SceneManager::RegisterScene(sceneName);

	// generating defaults
	sourcePlayer = cnz::Player::GenerateDefault(sceneName);
	sourceSentry = new Sentry(sceneName);

	sourceOracle = new Oracle(sceneName);
	sourceMarauder = new Marauder(sceneName);

	sourceBastion = new Bastion(sceneName);
	sourceSpider = new Mechaspider(sceneName);
}

// gets the objects; will be empty if it doesn't exist yet.
std::vector<cherry::Object*> cnz::Level::GetObjects() const { return objects; }

// gets the objects for the levle
std::vector<cnz::Obstacle *> cnz::Level::GetObstacles() const { return obstacles; }

// returns if the objects have been generated or not.
bool cnz::Level::GetObjectsGenerated() const { return objectsGenerated; }

// gets the player object
cnz::Player* cnz::Level::GetPlayerObject() const { return playerObj; }

std::vector<float> cnz::Level::GetObjectProps(int y, int x) {
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


cherry::Vec3 cnz::Level::UnFlipVec3(cherry::Vec3 vecToFlip) {
	cherry::Vec3 flippedVec;
	flippedVec.SetX(vecToFlip.GetX());
	flippedVec.SetY(vecToFlip.GetZ());
	flippedVec.SetZ(vecToFlip.GetY());
	return flippedVec;
}
