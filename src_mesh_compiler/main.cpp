/*
	Mesh Compiler ~ This program will take a number of different mesh formats and compiles them to a format that 
	that Project Clean can use...
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <json/json.h>

const static std::string TYPE = "scene";
const static std::string VERSION = "1.0";

struct Vertex {
	glm::vec3 position;
	glm::vec2 texCoord;
	glm::vec3 normal;
};

struct Face {
	uint32_t p1;
	uint32_t p2;
	uint32_t p3;
};

struct Mesh {
	glm::mat4 matrix;
	std::vector<Vertex> vertices;
	std::vector<Face> faces;
};

struct Scene {
	std::string type;
	std::string version;
	std::vector<Mesh> meshes;
};

void openSceneFile(Scene& scene, std::string path);

void serializeSceneFile(Scene& scene, std::string path);

int main(int argc, char** argv) {
	if (argc == 3) {
		Scene scene;

		scene.type = TYPE;
		scene.version = VERSION;

		openSceneFile(scene, argv[1]);
		serializeSceneFile(scene, argv[2]);
	}
	else {
		std::cout << "mesh_compiler [input_file] [output_file without extension]" << std::endl;
	}
	return 0;
}

void toMat4(glm::mat4& m, aiMatrix4x4& ma) {
	m[0][0] = ma.a1;
	m[0][1] = ma.c1;
	m[0][2] = ma.b1;
	m[0][3] = ma.d1;

	m[1][0] = ma.a3;
	m[1][1] = ma.c3;
	m[1][2] = ma.b3;
	m[1][3] = ma.d3;

	m[2][0] = ma.a2;
	m[2][1] = ma.c2;
	m[2][2] = ma.b2;
	m[2][3] = ma.d2;

	m[3][0] = ma.a4;
	m[3][1] = ma.c4;
	m[3][2] = ma.b4;
	m[3][3] = ma.d4;
}

void openSceneFile(Scene& scene, std::string path) {
	Assimp::Importer importer;

	const aiScene* s = importer.ReadFile(
		path,
		aiProcess_Triangulate |
		aiProcess_FlipUVs
	);

	aiNode* root = s->mRootNode;

	for (uint32_t i = 0; i < root->mNumChildren; i++) {
		Mesh mesh;

		aiNode* node = root->mChildren[i];
		aiMesh* m = s->mMeshes[node->mMeshes[0]];

		toMat4(mesh.matrix, node->mTransformation);

		for (uint32_t j = 0; j < m->mNumVertices; j++) {
			// Vertices
			glm::vec3 v;
			v.x = m->mVertices[j].x;
			v.y = m->mVertices[j].z;
			v.z = m->mVertices[j].y;
			// Normal
			glm::vec3 n;
			n.x = m->mNormals[j].x;
			n.y = m->mNormals[j].z;
			n.z = m->mNormals[j].y;
			// TexCoords
			glm::vec2 tc;
			if (m->mTextureCoords[0]) {
				tc.x = m->mTextureCoords[0][j].x;
				tc.y = m->mTextureCoords[0][j].y;
			}
			else {
				tc = glm::vec2(0, 0);
			}

			Vertex vertex;
			vertex.position = v;
			vertex.texCoord = tc;
			vertex.normal = n;

			mesh.vertices.push_back(vertex);
		}

		for (uint32_t j = 0; j < m->mNumFaces; j++) {
			aiFace f = m->mFaces[j];
			Face face;

			face.p1 = f.mIndices[0];
			face.p2 = f.mIndices[1];
			face.p3 = f.mIndices[2];

			mesh.faces.push_back(face);
		}

		scene.meshes.push_back(mesh);
	}
}

void serializeSceneFile(Scene& scene, std::string path) {
	Json::Value root;

	root["type"] = scene.type;
	root["version"] = scene.version;

	// Mesh
	Json::Value meshes;

	std::for_each(scene.meshes.begin(), scene.meshes.end(), [&] (Mesh& mesh) {
		Json::Value temp;

		// Matrix
		Json::Value matrix;

		for (int y = 0; y < 4; y++) {
			for (int x = 0; x < 4; x++) {
				matrix.append(mesh.matrix[y][x]);
			}
		}

		temp["matrix"] = matrix;

		// Vertices
		Json::Value vertices;

		std::for_each(mesh.vertices.begin(), mesh.vertices.end(), [&](Vertex& vertex) {
			Json::Value temp2;

			// Vertex
			temp2["x"] = vertex.position.x;
			temp2["y"] = vertex.position.y;
			temp2["z"] = vertex.position.z;

			// TexCoords
			temp2["tx"] = vertex.texCoord.x;
			temp2["ty"] = vertex.texCoord.y;

			// Normals
			temp2["nx"] = vertex.normal.x;
			temp2["ny"] = vertex.normal.y;
			temp2["nz"] = vertex.normal.z;

			vertices.append(temp2);
		});

		temp["vertices"] = vertices;

		// Faces
		Json::Value faces;

		std::for_each(mesh.faces.begin(), mesh.faces.end(), [&](Face& face) {
			Json::Value temp2;

			temp2["p1"] = face.p1;
			temp2["p2"] = face.p2;
			temp2["p3"] = face.p3;

			faces.append(temp2);
		});

		temp["faces"] = faces;

		meshes.append(temp);
	});

	root["meshes"] = meshes;

	Json::StreamWriterBuilder builder;
	const std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());

	std::stringstream ss;

	ss << path << ".json";

	std::ofstream out(ss.str());

	writer->write(root, &out);

	out.close();
}
