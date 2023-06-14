#pragma once
#include "stdafx.h"
#include <windows.h>
#include <string>
#include <iostream>  
#include <fstream> 
#include <vector> 
#include <GL/glew.h>
#include <GL/wglew.h>
#include "Shader.h"
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")



class Program
{
	GLuint				ProgramID;
	std::vector<char>	ProgramErrorMessage;
	void				AttachShader(Shader&);
	void				DetachShader(Shader&);
	bool				Link();
public:

						Program();
						~Program();

	GLuint				GetProgramID();
	bool				Create(Shader& VertShader, Shader& FragShader);

	void				Use();
	void				Delete();
};