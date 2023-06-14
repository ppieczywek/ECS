#pragma once
#include "stdafx.h"
#include "Program.h"



Program::Program()
{

};

Program::~Program()
{

};
				

GLuint	Program::GetProgramID()
{
	return ProgramID;
};

bool Program::Create(Shader& VertShader, Shader& FragShader)
{
	ProgramID = glCreateProgram();
	if(ProgramID==0)
	{
		MessageBox(NULL, TEXT("Unable to create Shader Program."), TEXT("OpenGL Message"), MB_OK | MB_ICONERROR);
		return false;
	}

	AttachShader(VertShader);
	AttachShader(FragShader);
	
	if(!Link())
	{
		MessageBox(NULL, TEXT("Unable to link Shader Program."), TEXT("OpenGL Message"), MB_OK | MB_ICONERROR);
		return false;
	}
		
	DetachShader(VertShader);
	DetachShader(FragShader);

	return true;
};


void Program::AttachShader(Shader& Shader)
{
	glAttachShader(ProgramID, Shader.GetShaderID());	
};

void Program::DetachShader(Shader& Shader)
{
	 glDetachShader(ProgramID, Shader.GetShaderID());	
};


bool Program::Link()
{
	
    GLint Result = GL_FALSE;
    int InfoLogLength;

    glLinkProgram(ProgramID);
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    ProgramErrorMessage.resize( max(InfoLogLength, int(1)) );
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    //fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);	
	if (InfoLogLength > 1) {
		MessageBoxA(NULL, &ProgramErrorMessage[0], " ", MB_OK | MB_ICONERROR);
	};

	return (Result == 1);
};

void Program::Use()
{
	glUseProgram(ProgramID);
};


void Program::Delete()
{
	if(ProgramID != 0)
	{
		GLsizei maxCount = 10;
 		GLsizei count = 0;
 		GLuint	shaders[10];

		glGetAttachedShaders( ProgramID, maxCount, &count, shaders);
	
		for (auto ii = 0; ii < count; ++ii)
		{
			glDetachShader(ProgramID, shaders[ii]);
			glDeleteShader(shaders[ii]);
		}

		glDeleteProgram(ProgramID);
	}
};


    
