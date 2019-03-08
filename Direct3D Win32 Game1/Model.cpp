#include "pch.h"
#include "Model.h"
#include "AssetLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

RemotingHostSample::Model::Model()
{
}

RemotingHostSample::Model::~Model()
{
}

void RemotingHostSample::Model::Load(std::string const & path, bool gamma)
{
	Assimp::Importer importer;
	auto fullPath = AssetLoader::GetFullPath(path);
	auto scene = importer.ReadFile(fullPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}

	ParserNode(scene->mRootNode, scene);

}

void RemotingHostSample::Model::ParserMesh(aiMesh * mesh, const aiScene * scene, Mesh * outMesh)
{
	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.Position.x = mesh->mVertices[i].x;
		vertex.Position.y = mesh->mVertices[i].y;
		vertex.Position.z = mesh->mVertices[i].z;

		if (mesh->mNormals)
		{
			vertex.Normal.x = mesh->mNormals[i].x;
			vertex.Normal.y = mesh->mNormals[i].y;
			vertex.Normal.z = mesh->mNormals[i].z;
		}

		//if (mesh->mTextureCoords[0] != NULL)
		//{
		//	vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
		//	vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
		//}
		//else
		//{
		//	vertex.TexCoords = Vector2(0, 0);
		//}

		//// tangent
		//if (mesh->mTangents)
		//{
		//	vertex.Tangent.x = mesh->mTangents[i].x;
		//	vertex.Tangent.y = mesh->mTangents[i].y;
		//	vertex.Tangent.z = mesh->mTangents[i].z;
		//}
		//// bitangent
		//if (mesh->mBitangents)
		//{
		//	vertex.Bitangent.x = mesh->mBitangents[i].x;
		//	vertex.Bitangent.y = mesh->mBitangents[i].y;
		//	vertex.Bitangent.z = mesh->mBitangents[i].z;
		//}

		outMesh->Vertices.emplace_back(vertex);
	}

	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			outMesh->Indices.push_back(face.mIndices[j]);
	}
	// process materials
	// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
	// Same applies to other texture as the following list summarizes:
	// diffuse: texture_diffuseN
	// specular: texture_specularN
	// normal: texture_normalN

	/*
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	// 1. diffuse maps
	vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	outMesh->Textures.insert(outMesh->Textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	outMesh->Textures.insert(outMesh->Textures.end(), specularMaps.begin(), specularMaps.end());
	// 3. normal maps
	std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	outMesh->Textures.insert(outMesh->Textures.end(), normalMaps.begin(), normalMaps.end());
	// 4. height maps
	std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	outMesh->Textures.insert(outMesh->Textures.end(), heightMaps.begin(), heightMaps.end());
	*/
}

void RemotingHostSample::Model::ParserNode(aiNode * node, const aiScene * scene)
{
	for (size_t i = 0, length = node->mNumMeshes; i < length; i++)
	{
		aiMesh* aimesh = scene->mMeshes[node->mMeshes[i]];
		Mesh mesh;
		ParserMesh(aimesh, scene, &mesh);
		meshes.emplace_back(mesh);
	}
	for (size_t i = 0, length = node->mNumChildren; i < length; i++)
	{
		auto child = node->mChildren[i];
		ParserNode(child, scene);
	}
}
