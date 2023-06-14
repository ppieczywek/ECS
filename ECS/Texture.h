#pragma once
#include <string>
#include <GL/glew.h>
#include <GL/wglew.h>
#include "Bitmap.h"
#include "Resource.h"

#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

class Texture : public ResourceBase
{
		GLuint							Id;
		std::string						FileName;
		unsigned int					Width;
		unsigned int					Height;
		unsigned int					ColorDepth;
			bool						Clear();
			bool						Load(const Bitmap&);

protected:

		void							Dispose();

public:
										Texture(const std::string&, const std::string&);
										Texture(const std::string&, const Bitmap&);
										~Texture();
			
		inline const GLuint&			GetId() const;
		inline const std::string&		GetFileName() const;
		inline const unsigned int&		GetWidth() const;
		inline const unsigned int&		GetHeight() const;
		inline const unsigned int&		GetColorDepth() const;
};

inline const GLuint& Texture::GetId() const
{
	return Id;
};


inline const std::string& Texture::GetFileName() const
{
	return FileName;
};


inline const unsigned int& Texture::GetWidth() const
{
	return Width;
};


inline const unsigned int& Texture::GetHeight() const
{
	return Height;
};


inline const unsigned int& Texture::GetColorDepth() const
{
	return ColorDepth;
};

