#include "Material.h"

void cherry::Material::Apply() {

	// first value is the name, the second value is what we're actually setting.
	for (auto& kvp : myMat4s)
		myShader->SetUniform(kvp.first.c_str(), kvp.second);
	for (auto& kvp : myVec4s)
		myShader->SetUniform(kvp.first.c_str(), kvp.second);
	for (auto& kvp : myVec3s)
		myShader->SetUniform(kvp.first.c_str(), kvp.second);
	for (auto& kvp : myFloats)
		myShader->SetUniform(kvp.first.c_str(), kvp.second);
	for (auto& kvp : myInts)
		myShader->SetUniform(kvp.first.c_str(), kvp.second);

	// OLD [ REPLACED]
	//// binding the textures, and then sending hte slot it's bound to.
	//int slot = 0;
	//for (auto& kvp : myTextures) {
	//	kvp.second->Bind(slot);
	//	myShader->SetUniform(kvp.first.c_str(), slot);
	//	slot++;
	//}

	// updated in tutorial 09
	int slot = 0;
	for (auto& kvp : myTextures) {
		if (kvp.second.Sampler != nullptr)
			kvp.second.Sampler->Bind(slot);
		else
			TextureSampler::Unbind(slot);
		kvp.second.Texture->Bind(slot);
		myShader->SetUniform(kvp.first.c_str(), slot);
		slot++;
	}

	// updated in tutorial 10
	for (auto& kvp : myCubeMaps) {
		if (kvp.second.Sampler != nullptr)
			kvp.second.Sampler->Bind(slot);
		else
			TextureSampler::Unbind(slot);
		kvp.second.Texture->Bind(slot);
		myShader->SetUniform(kvp.first.c_str(), slot);
	}

	// mulitiplies everything by the source alpha so
	if (HasTransparency) {
		glEnable(GL_BLEND);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
		glDisable(GL_DEPTH_TEST);
	}
	else {
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}
}
