#pragma once

#include <glad/glad.h>

#include <cstddef>
#include <vector>
#include <unordered_map>
#include <string>

#include "Shader.hpp"
#include "Texture.hpp"

class Program
{
	struct UniformData
	{

		GLenum glType;
		GLuint glLocation;
		size_t count;
		std::byte* data;

		// ~UniformData(){delete data;}

	};
	struct TextureData
	{
		std::string name;
		GLuint glId;
	};

	VertexShader m_vert;
	FragmentShader m_frag;

	GLint m_glId;

	std::unordered_map<std::string,Program::UniformData> m_uniformPool;

	std::vector<Program::TextureData> m_texturePool;

public:
	Program(VertexShader&& vert_shad, FragmentShader&& frag_shad);
	Program(const char* vert_shad_src, const char* frag_shad_src);
	Program(Program&& rvalue);

	//Uniforms

	const VertexShader& getVertShader() const;
	const FragmentShader& getFragShader() const;
	GLint getUniformLocation(const char* name);

	void setUniform1b(const char* name, bool value);
	void setUniform1i(const char* name, int value);
	void setUniform1u(const char* name, unsigned int value);
	void setUniform1f(const char* name, float value);

	void setUniform2b(const char* name, bool v1, bool v2 );
	void setUniform2i(const char* name, int v1, int v2 );
	void setUniform2u(const char* name, unsigned int v1, unsigned int v2);
	void setUniform2f(const char* name, float v1, float v2);

	void setUniform2b(const char* name, bool value[2]);
	void setUniform2i(const char* name, int value[2]);
	void setUniform2u(const char* name, unsigned int value[2]);
	void setUniform2f(const char* name, float value[2]);

	void setUniform2b(const char* name, glm::vec<2,bool> value) 			{setUniform2b(name,value.x,value.y);}
	void setUniform2i(const char* name, glm::vec<2,int> value) 			{setUniform2i(name,value.x,value.y);}
	void setUniform2u(const char* name, glm::vec<2,unsigned int> value) 	{setUniform2u(name,value.x,value.y);}
	void setUniform2f(const char* name, glm::vec<2,float> value)			{setUniform2f(name,value.x,value.y);}

	void setUniform3b(const char* name, bool v1, bool v2 , bool v3);
	void setUniform3i(const char* name, int v1, int v2 , int v3);
	void setUniform3u(const char* name, unsigned int v1, unsigned int v2, unsigned int v3);
	void setUniform3f(const char* name, float v1, float v2, float v3);

	void setUniform3b(const char* name, bool value[3]);
	void setUniform3i(const char* name, int value[3]);
	void setUniform3u(const char* name, unsigned int value[3]);
	void setUniform3f(const char* name, float value[3]);

	void setUniform3b(const char* name, glm::vec<3,bool> value) 		{setUniform3b(name,value.x,value.y,value.z);}
	void setUniform3i(const char* name, glm::vec<3,int> value) 			{setUniform3i(name,value.x,value.y,value.z);}
	void setUniform3u(const char* name, glm::vec<3,unsigned int> value) {setUniform3u(name,value.x,value.y,value.z);}
	void setUniform3f(const char* name, glm::vec<3,float> value)		{setUniform3f(name,value.x,value.y,value.z);}

	void setUniformMat4fv(const char* name, float matrix[16]);

	void setUniformTexture2D(const char* name,Texture& texture);

	void clearUniforms();

	void useProgram();

	~Program();

	Program(Program&) = delete;

private:
	static void attachGlShader(GLint glProgramId, VertexShader& vs, FragmentShader& fs);

	template<typename T> void setUniformData(const char* name, GLenum glType, size_t number ,T* data);
	void useUniformData();

	static void useUniformData1 (GLuint glLocation,GLenum glType, std::byte* data);
	static void useUniformData2 (GLuint glLocation,GLenum glType, std::byte* data);
	static void useUniformData3 (GLuint glLocation,GLenum glType, std::byte* data);
	static void useUniformData4 (GLuint glLocation,GLenum glType, std::byte* data);
	static void useUniformData9 (GLuint glLocation,GLenum glType, std::byte* data);
	static void useUniformData16(GLuint glLocation,GLenum glType, std::byte* data);

};
