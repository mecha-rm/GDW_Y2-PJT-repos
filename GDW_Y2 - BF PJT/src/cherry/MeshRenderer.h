#pragma once
#include "Material.h"
#include "Mesh.h"

namespace cherry
{
	struct MeshRenderer {
		Material::Sptr Material;
		Mesh::Sptr Mesh;
	};
}