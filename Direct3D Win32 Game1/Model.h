#pragma once
#include "SimpleMath.h"
#include <vector>
#include <cstdint>

using namespace std;
using namespace DirectX::SimpleMath;

struct aiMesh;
struct aiScene;
struct aiNode;

namespace RemotingHostSample
{
	struct Vertex
	{
		Vector3 Position;
		Vector3 Normal;
		//Vector2 TexCoords;
		//Vector3 Tangent;
		//Vector3 Bitangent;
	};

	struct Texture {
		unsigned int id;
		string type;
		string path;
	};

	struct Mesh
	{
		vector<Vertex> Vertices;
		vector<uint32_t> Indices;
		vector<Texture> Textures;
	};

	class Model
	{
	public:
		vector<Mesh> meshes;
	public:
		Model();
		~Model();
		void Load(string const &path, bool gamma = false);

	private:
		void ParserMesh(aiMesh * mesh, const aiScene * scene, Mesh * outMesh);
		void ParserNode(aiNode * node, const aiScene * scene);
	};

}
