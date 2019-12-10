#include "sys.h"


const static std::string PROG_DATA_TYPE = "program";
const static std::string PROG_DATA_VERSION = "1.0";
// Program

// Shaders
void Program::Shader::init(GLenum type, std::string path) {
	id = glCreateShader(type);

	std::ifstream in(path);
	std::stringstream ss;
	std::string temp;

	while (std::getline(in, temp)) {
		ss << temp << std::endl;
	}

	in.close();

	std::string src = ss.str();

	logger_output("%s\n", src.c_str());

	const char* c_src = src.c_str();

	glShaderSource(id, 1, &c_src, 0);
	glCompileShader(id);

	char log[1024];
	int len;

	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
	
	if (len > 0) {
		glGetShaderInfoLog(id, len, 0, log);
		logger_output("%s\n", log);
	}
}

void Program::Shader::release() {
	glDeleteShader(this->id);
	this->id = 0;
}

// Program functions
void Program::loadData(
	std::string path, 
	ProgramData& data) {

	std::ifstream in(path);

	Json::Value root;

	in >> root;

	Json::Value type = root["type"];
	Json::Value version = root["version"];

	data.type = type.asString();
	data.version = type.asString();

	if (
		type.asString() == PROG_DATA_TYPE && 
		version.asString() == PROG_DATA_VERSION) {

		// Shaders
		std::map<std::string, GLenum> shaderTypes = {
			{"vertex", GL_VERTEX_SHADER},
			{"fragment", GL_FRAGMENT_SHADER} 
		};

		Json::Value shaders = root["shaders"];

		if (shaders.isArray()) {
			for (int i = 0; i < shaders.size(); i++) {
				Json::Value temp = shaders[i];

				ProgramData::ShaderData shaderData;

				shaderData.type = shaderTypes[temp["type"].asString()];
				shaderData.path = temp["path"].asString();

				data.shaderDataList.push_back(shaderData);
			}
		}

		// Attributes
		Json::Value attributes = root["attributes"];
		if (attributes.isArray()) {
			for (int i = 0; i < attributes.size(); i++) {
				Json::Value t1 = attributes[i];

				ProgramData::AttributeData attrData;

				attrData.name = t1["name"].asString();
				attrData.location = t1["location"].asInt();

				data.attributeDataList.push_back(attrData);
			}
		}

		// Uniforms
		Json::Value uniforms = root["uniforms"];
		if (uniforms.isArray()) {
			for (int i = 0; i < uniforms.size(); i++) {
				Json::Value t1 = uniforms[i];

				ProgramData::UniformData uniformData;

				uniformData.name = t1["name"].asString();
				uniformData.hasDefaultData = t1["default"].isNull();

				if (uniformData.hasDefaultData) {
					Json::Value d = t1["default"];

					std::string check = uniformData.defData.method = d["method"].asString();

					std::map<std::string, ProgramData::UniformData::MethodTokens> methodTokens = {
						{"set1i", ProgramData::UniformData::MethodTokens::MT_SET1I},
						{"set2i", ProgramData::UniformData::MethodTokens::MT_SET2I},
						{"set3i", ProgramData::UniformData::MethodTokens::MT_SET3I},
						{"set4i", ProgramData::UniformData::MethodTokens::MT_SET4I},
						{"set1f", ProgramData::UniformData::MethodTokens::MT_SET1F},
						{"set2f", ProgramData::UniformData::MethodTokens::MT_SET2F},
						{"set3f", ProgramData::UniformData::MethodTokens::MT_SET3F},
						{"set4f", ProgramData::UniformData::MethodTokens::MT_SET4F},
						{"setMat2", ProgramData::UniformData::MethodTokens::MT_SETMAT2},
						{"setMat3", ProgramData::UniformData::MethodTokens::MT_SETMAT3},
						{"setMat4", ProgramData::UniformData::MethodTokens::MT_SETMAT4}
					};


						/*
							"method": "set1i",
							"data": 0,
						*/
						/*
							"method": "set2i",
							"data": [1, 1],
						*/
						/*
							// set3i
							"method": "set3i",
							"data": [1, 1, 1],
						*/
						/*
						// set4i
						"method": "set4i",
						"data" : [1, 1, 1, 1],
						*/
				/*
					// set4i
					"method": "set4i",
					"data": [1, 1, 1, 1],
					// set1f
					"method": "set1f",
					"data": 1.0,
					// set2f
					"method": "set2f",
					"data": [1.0, 1.0],
					// set3f
					"method": "set3f",
					"data": [1.0, 1.0, 1.0],
					// set4f
					"method": "set3f",
					"data": [1.0, 1.0, 1.0, 1.0],
					// setMat2
					"method": "setMat2",
					"data": [
						1.0, 1.0,
						1.0, 1.0
					],
					// setMat3
					"method": "setMat3",
					"data": [
						1.0, 1.0, 1.0,
						1.0, 1.0, 1.0,
						1.0, 1.0, 1.0
					],
					// setMat4
					"method": "setMat4",
					"data": [
						1.0, 1.0, 1.0, 1.0,
						1.0, 1.0, 1.0, 1.0,
						1.0, 1.0, 1.0, 1.0,
						1.0, 1.0, 1.0, 1.0
					]
				*/


				}

				data.uniformDataList.push_back(uniformData);
			}
		}
	}
	in.close();
}

void Program::init(std::string path) {
	ProgramData programData;

	this->loadData(path, programData);
}

void Program::bind() {
	glUseProgram(this->id);
}

void Program::unbind() {
	glUseProgram(0);
}

void Program::release() {
	glDeleteVertexArrays(1, &this->attrID);

	std::for_each(shaders.begin(), shaders.end(), [&](Shader& shader) {
		glDetachShader(this->id, shader.id);
		shader.release();
	});

	shaders.clear();

	glDeleteProgram(this->id);
	this->id = 0;
}

// Attributes
void Program::setAttr(std::string name, int id) {
	this->attributes[name] = id;
}

void Program::enableAttr(std::string name) {
	glEnableVertexAttribArray(this->attributes[name]);
}

void Program::disableAttr(std::string name) {
	glDisableVertexAttribArray(this->attributes[name]);
}

void Program::pointerAttr(
	std::string name,
	uint32_t size,
	GLenum type) {
	glVertexAttribPointer(
		this->attributes[name],
		size,
		type,
		GL_FALSE,
		0,
		0
	);
}

void Program::bindAttr() {
	glBindVertexArray(this->attrID);
}

void Program::unbindAttr() {
	glBindVertexArray(0);
}

// Uniforms
void Program::createUniform(std::string name) {
	this->uniforms[name] = glGetUniformLocation(this->id, name.c_str());
}

// Integer Uniforms
void Program::set1i(std::string name, int x) {
	glUniform1i(this->uniforms[name], x);
}

void Program::set2i(std::string name, const glm::ivec2& v) {
	glUniform2i(this->uniforms[name], v.x, v.y);
}

void Program::set3i(std::string name, const glm::ivec3& v) {
	glUniform3i(this->uniforms[name], v.x, v.y, v.z);
}

void Program::set4i(std::string name, const glm::ivec4& v) {
	glUniform4i(this->uniforms[name], v.x, v.y, v.z, v.w);
}


// Float Uniforms
void Program::set1f(std::string name, float x) {
	glUniform1f(this->uniforms[name], x);
}

void Program::set2f(std::string name, const glm::vec2& v) {
	glUniform2f(this->uniforms[name], v.x, v.y);
}

void Program::set3f(std::string name, const glm::vec3& v) {
	glUniform3f(this->uniforms[name], v.x, v.y, v.z);
}

void Program::set4f(std::string name, const glm::vec4& v) {
	glUniform4f(this->uniforms[name], v.x, v.y, v.z, v.w);
}


// Matrices Uniforms
void Program::setMat2(std::string name, const glm::mat2& m) {
	glUniformMatrix2fv(this->uniforms[name], 1, GL_FALSE, &m[0][0]);
}

void Program::setMat3(std::string name, const glm::mat2& m) {
	glUniformMatrix3fv(this->uniforms[name], 1, GL_FALSE, &m[0][0]);
}

void Program::setMat4(std::string name, const glm::mat2& m) {
	glUniformMatrix4fv(this->uniforms[name], 1, GL_FALSE, &m[0][0]);
}

