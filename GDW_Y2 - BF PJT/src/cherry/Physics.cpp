// file for general physics calculation. WIP.
#include "Physics.h"

float cherry::Vec3::getX() {
	return this->x;
}

float cherry::Vec3::getY() {
	return this->y;
}

float cherry::Vec3::getZ() {
	return this->z;
}

void cherry::Vec3::setX(float newX) {
	this->x = newX;
}

void cherry::Vec3::setY(float newY) {
	this->y = newY;
}

void cherry::Vec3::setZ(float newZ) {
	this->z = newZ;
}

cherry::Vec3 cherry::Vec3::operator+(Vec3 & vec) {
	Vec3 temp;
	temp.setX(this->getX() + vec.getX());
	temp.setY(this->getY() + vec.getY());
	temp.setZ(this->getZ() + vec.getZ());
	return temp;
}

cherry::Vec3 cherry::Vec3::operator-(Vec3 & vec) {
	Vec3 temp;
	temp.setX(this->getX() - vec.getX());
	temp.setY(this->getY() - vec.getY());
	temp.setZ(this->getZ() - vec.getZ());
	return temp;
}

float cherry::Vec2::getX() {
	return this->x;
}

float cherry::Vec2::getY() {
	return this->y;
}

void cherry::Vec2::setX(float newX) {
	this->x = newX;
}

void cherry::Vec2::setY(float newY) {
	this->y = newY;
}

cherry::Vec2 cherry::Vec2::operator+(Vec2& vec) {
	Vec2 temp;
	temp.setX(this->getX() + vec.getX());
	temp.setY(this->getY() + vec.getY());
	return temp;
}

cherry::Vec2 cherry::Vec2::operator-(Vec2& vec) {
	Vec2 temp;
	temp.setX(this->getX() - vec.getX());
	temp.setY(this->getY() - vec.getY());
	return temp;
}

//cherry::Object::Object(const char* path) {
//	FILE* file = fopen(path, "r");
//	if (file == NULL) {
//		printf("Impossible to open the file !\n");
//	}
//	else {
//		while (true) {
//			char lineHeader[128];
//			int res = fscanf(file, "%s", lineHeader);
//			if (res == EOF) {
//				break;
//			}
//
//			if (strcmp(lineHeader, "v") == 0) {
//				Vec3 vertex;
//				float x, y, z;
//				fscanf(file, "%f %f %f\n", &x, &y, &z);
//				vertex.setX(x);
//				vertex.setY(y);
//				vertex.setZ(z);
//				temp_vertices.push_back(vertex);
//			}
//			else if (strcmp(lineHeader, "vt") == 0) {
//				Vec2 uv;
//				float x, y;
//				fscanf(file, "%f %f\n", &x, &y);
//				uv.setX(x);
//				uv.setY(y);
//				temp_uvs.push_back(uv);
//			}
//			else if (strcmp(lineHeader, "vn") == 0) {
//				Vec3 normal;
//				float x, y, z;
//				fscanf(file, "%f %f %f\n", &x, &y, &z);
//				normal.setX(x);
//				normal.setY(y);
//				normal.setZ(z);
//				temp_normals.push_back(normal);
//			}
//			else if (strcmp(lineHeader, "f") == 0) {
//				std::string vertex1, vertex2, vertex3;
//				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
//				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
//				if (matches != 9) {
//					printf("oh shit\n");
//				}
//				else {
//					vertexIndices.push_back(vertexIndex[0]);
//					vertexIndices.push_back(vertexIndex[1]);
//					vertexIndices.push_back(vertexIndex[2]);
//					uvIndices.push_back(uvIndex[0]);
//					uvIndices.push_back(uvIndex[1]);
//					uvIndices.push_back(uvIndex[2]);
//					normalIndices.push_back(normalIndex[0]);
//					normalIndices.push_back(normalIndex[1]);
//					normalIndices.push_back(normalIndex[2]);
//				}
//			}
//
//			for (unsigned int i = 0; i < vertexIndices.size(); i++) {
//				unsigned int vertexIndex = vertexIndices[i];
//				Vec3 vertex = temp_vertices[vertexIndex - 1];
//				this->vertices.push_back(vertex);
//			}
//			for (unsigned int i = 0; i < uvIndices.size(); i++) {
//				unsigned int uvIndex = uvIndices[i];
//				Vec2 uv = temp_uvs[uvIndex - 1];
//				this->uvs.push_back(uv);
//			}
//			for (unsigned int i = 0; i < normalIndices.size(); i++) {
//				unsigned int normalIndex = normalIndices[i];
//				Vec3 normal = temp_normals[normalIndex - 1];
//				this->normals.push_back(normal);
//			}
//		}
//	}
//}
