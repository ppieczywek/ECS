#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <array>
#include "Vector2.h"
#include "Texture.h"

#include <GL/glew.h>
#include <GL/wglew.h>

#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

const unsigned int	DefaultRowPitch = 16;
const unsigned int	DefaultColPitch = 16;

class BitmapFont : public ResourceBase
{
			std::string						FileName;
			unsigned int					FontImageWidth;
			unsigned int					FontImageHeight;
			unsigned int					CellWidth;
			unsigned int					CellHeight;
			unsigned int					RowPitch;
			unsigned int					ColPitch;
			unsigned int					FontImageColorDepth;
			unsigned int					BaseCharOffset;
			float							RowFactor;
			float							ColFactor;
			float							CharSpacingFactor;

			Vector2							ScaleFactor;
			std::vector<Vector2>			CharCoordinates;
			GLuint							FontTextureId;
			const bool						LoadBitmapFont(const std::string&);
			bool							Clear();

protected:

			virtual void					Dispose();

public:
											BitmapFont(const std::string&, const std::string&);		
											virtual ~BitmapFont();

			inline const Vector2&			GetScaleFactor() const;
			inline const Vector2			GetCharCoordinates(const char&) const;
			inline const unsigned int&		GetFontTextureId() const;

			inline const float&				GetCharSpacingFactor() const;
			inline void						SetCharSpacingFactor(const float&);

			void							SetRowPitch(const unsigned int&);
			void							SetColPitch(const unsigned int&);
			inline void						SetBaseCharOffset(const char&);
			
};


inline const Vector2& BitmapFont::GetScaleFactor() const
{
	return ScaleFactor;
};


inline const Vector2 BitmapFont::GetCharCoordinates(const char& Char) const
{
	int Index = static_cast<int>(Char) - BaseCharOffset;
	if(Index >= 0 && Index < static_cast<int>(CharCoordinates.size()))
	{
		return CharCoordinates[Index];
	}
	else
	{
		return ZeroVector;
	}
};


inline const unsigned int&	BitmapFont::GetFontTextureId() const
{
	return FontTextureId;
};


inline void	BitmapFont::SetBaseCharOffset(const char& Char)
{
	BaseCharOffset = static_cast<int>(Char);
};


inline const float&	BitmapFont::GetCharSpacingFactor() const
{
	return CharSpacingFactor;
};


inline void	BitmapFont::SetCharSpacingFactor(const float& Value)
{
	CharSpacingFactor = Value;
};