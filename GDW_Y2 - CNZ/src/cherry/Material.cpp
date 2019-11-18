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
}
