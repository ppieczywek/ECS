#pragma once
#include "stdafx.h"
#include <windows.h>
#include <string>
#include <iostream>  
#include <fstream> 
#include <vector> 
#include <GL/glew.h>
#include <GL/wglew.h>
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")


class Shader
{
	GLuint				ShaderID;
	std::string			ShaderCode;
	std::vector<char>	VertexShaderErrorMessage;
	bool				Load(const char*);
	bool				Compile();
public:
						Shader();
						~Shader();
	
	bool				Create(GLenum shaderType, const char* shader_file_path);
	bool				Create(GLenum shaderType, const std::string&);
	GLuint				GetShaderID();
	std::vector<char>&	GetShaderErrorMessage();
	void				SetShaderCode(const std::string&);
	void				Delete();
};