#pragma once
#include "GL/glew.h"
#include "GL/wglew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

struct Vertex
{
public:
	GLfloat Position[3];
	GLfloat TexCoord[2];
};

