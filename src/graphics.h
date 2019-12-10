#pragma once

// Program
struct Program {
	uint32_t id = 0;

	struct Shader {
		uint32_t id = 0;

		void init(GLenum type, std::string path);
		void release();
	};

	std::vector<Shader> shaders;

	uint32_t attrID = 0;
	std::map<std::string, int> attributes;

	std::map<std::string, int> uniforms;


	struct ProgramData {
		std::string type;
		std::string version;

		struct ShaderData {
			GLenum type;
			std::string path;
		};

		std::vector<ShaderData> shaderDataList;

		struct AttributeData {
			std::string name;
			int location;
		};

		std::vector<AttributeData> attributeDataList;

		struct UniformData {
			std::string name;
			bool hasDefaultData = false;

			enum MethodTokens {
				MT_SET1I = 0,
				MT_SET2I,
				MT_SET3I,
				MT_SET4I,
				MT_SET1F,
				MT_SET2F,
				MT_SET3F,
				MT_SET4F,
				MT_SETMAT2,
				MT_SETMAT3,
				MT_SETMAT4,
				MT_SIZE
			};

			struct DefaultData {
				std::string method;
				// Section Contains Data

				// Integer
				int data1i;
				glm::ivec2 data2i;
				glm::ivec3 data3i;
				glm::ivec4 data4i;

				// Float
				float data1f;
				glm::vec2 data2f;
				glm::vec3 data3f;
				glm::vec4 data4f;

				// Matrices
				glm::mat2 dataMat2;
				glm::mat3 dataMat3;
				glm::mat4 dataMat4;
			};

			DefaultData defData;
		};

		std::vector<UniformData> uniformDataList;
	};

	void loadData(std::string path, ProgramData& data);

	void init(std::string path);

	void bind();

	void unbind();

	void release();


	// Attributes
	void setAttr(std::string name, int id);

	void enableAttr(std::string name);

	void disableAttr(std::string name);

	void pointerAttr(
		std::string name, 
		uint32_t size, 
		GLenum type);

	void bindAttr();

	void unbindAttr();

	// Uniforms
	void createUniform(std::string name);

	// Integer Uniforms
	void set1i(std::string name, int x);
	void set2i(std::string name, const glm::ivec2& v);
	void set3i(std::string name, const glm::ivec3& v);
	void set4i(std::string name, const glm::ivec4& v);

	// Float Uniforms
	void set1f(std::string name, float x);
	void set2f(std::string name, const glm::vec2& v);
	void set3f(std::string name, const glm::vec3& v);
	void set4f(std::string name, const glm::vec4& v);

	// Matrices Uniforms
	void setMat2(std::string name, const glm::mat2& m);
	void setMat3(std::string name, const glm::mat2& m);
	void setMat4(std::string name, const glm::mat2& m);

};
// VertexBuffer

// IndexBuffer

// Texture2D