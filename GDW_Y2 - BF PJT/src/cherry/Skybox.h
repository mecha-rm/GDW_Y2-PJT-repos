#pragma once
#include "textures/TextureCube.h"
#include "Shader.h"
#include "Mesh.h"

namespace cherry
{
	class Scene;

	class Skybox
	{
	public:
		// default constructor
		Skybox();

		// sets the images for the skybox.
		Skybox(std::string left, std::string right, std::string down, std::string up, std::string front, std::string back);

		// gets the image for the left of the skybox.
		std::string GetLeft() const;

		// sets the image for the left side of the skybox. A 'true' is returned if the file was successfully found.
		bool SetLeft(std::string filePath);

		// gets the image for the right of the skybox.
		std::string GetRight() const;

		// sets the image for the left side of the skybox. A 'true' is returned if the file was successfully found.
		bool SetRight(std::string filePath);

		// gets the image for the up of the skybox.
		std::string GetUp() const;

		// sets the image for the left side of the skybox. A 'true' is returned if the file was successfully found.
		bool SetUp(std::string filePath);

		// gets the image for the down of the skybox.
		std::string GetDown() const;

		// sets the image for the down side of the skybox. A 'true' is returned if the file was successfully found.
		bool SetDown(std::string filePath);

		// gets the image for the front of the skybox.
		std::string GetFront() const;

		// sets the image for the front side of the skybox. A 'true' is returned if the file was successfully found.
		bool SetFront(std::string filePath);

		// gets the image for the back of the skybox.
		std::string GetBack() const;

		// sets the image for the back side of the skybox. A 'true' is returned if the file was successfully found.
		bool SetBack(std::string filePath);

		// generates the texture cube and returns it
		TextureCube::Sptr GenerateTextureCube() const;

		// generates and returns the shader for the skybox.
		Shader::Sptr GenerateShader() const;

		// generates the mesh
		Mesh::Sptr GenerateMesh() const;

		// adds the skybox to the scene provided.
		void AddSkyboxToScene(cherry::Scene * scene) const;
		

	private:
		// sets a file for the cubemap
		// [0] = lf, [1] = right. [2] = dn, [3] = up, [4] = ft, [5] = bk
		bool SetFile(unsigned int index, std::string filePath);

		/*
		 * an array for the six files used to make the cubemap
		 * [0]: left (lf)
		 * [1]: right (rt)
		 * [2]: bottom (dn)
		 * [3]: top (up)
		 * [4]: front (ft)
		 * [5]: back (bk)
		*/
		std::string files[6];

	protected:
	};
}