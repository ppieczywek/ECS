#pragma once
#include "stdafx.h"
#include "Shader.h"

using namespace std;

Shader::Shader()
{

};

Shader::~Shader()
{

};

bool Shader::Create(GLenum shaderType, const char* shader_file_path)
{
	vector<char> ErrorMessage;

	ShaderID = glCreateShader(shaderType);
	if(ShaderID==0)
	{
		MessageBoxA(NULL, "Unable to create Shader.", shader_file_path, MB_OK | MB_ICONERROR);
		return false;
	}

	if(!Load(shader_file_path))
	{
		MessageBoxA(NULL, "Unable to load Shader.", shader_file_path, MB_OK | MB_ICONERROR);
		return false;
	}

	if(!Compile())
	{		
		ErrorMessage = GetShaderErrorMessage();
		MessageBoxA(NULL, &ErrorMessage[0], shader_file_path, MB_OK | MB_ICONERROR);
		return false;
	}
	 
	 return true;
};

bool Shader::Create(GLenum shaderType, const std::string& ShaderCode)
{
	vector<char> ErrorMessage;

	ShaderID = glCreateShader(shaderType);
	if(ShaderID==0)
	{
		MessageBoxA(NULL, "Unable to create Shader.", " ", MB_OK | MB_ICONERROR);
		return false;
	}

	SetShaderCode(ShaderCode);
	
	if(!Compile())
	{		
		ErrorMessage = GetShaderErrorMessage();
		MessageBoxA(NULL, &ErrorMessage[0], " ", MB_OK | MB_ICONERROR);
		return false;
	}
	 
	return true;

};


bool Shader::Load(const char* shader_file_path)
{
	ShaderCode.clear();
    std::ifstream VertexShaderStream(shader_file_path, ios::in);
    if(VertexShaderStream.is_open())
    {
        std::string Line = "";
        while(getline(VertexShaderStream, Line))
            ShaderCode += "\n" + Line;
        VertexShaderStream.close();
		return true;
    }
	return false;
};

bool Shader::Compile()
{
	GLint Result = GL_FALSE;
    int InfoLogLength;
	 //   printf("Compiling shader : %s\n", vertex_file_path);
    char const * VertexSourcePointer = ShaderCode.c_str();
    glShaderSource(ShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(ShaderID);
 
    // Check Shader
    glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 1)
	{
		VertexShaderErrorMessage.resize(InfoLogLength);
		glGetShaderInfoLog(ShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		MessageBoxA(NULL, &VertexShaderErrorMessage[0], " ", MB_OK | MB_ICONERROR);
	};

	return (Result == 1);
    //fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

};


GLuint Shader::GetShaderID()
{
	return ShaderID;
};


vector<char>& Shader::GetShaderErrorMessage()
{
	return VertexShaderErrorMessage;	
};


void Shader::SetShaderCode(const string& NewCode)
{
	ShaderCode.clear();
	ShaderCode = NewCode;
};

void Shader::Delete()
{
	if(ShaderID != 0)
	{
		ShaderCode.clear();
		VertexShaderErrorMessage.clear();
		glDeleteShader(ShaderID);
		ShaderID = -1;
	}
};