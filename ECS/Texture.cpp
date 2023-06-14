#pragma once
#include "stdafx.h"
#include "Texture.h"


Texture::~Texture()
{

}


Texture::Texture(const std::string& Tag, const std::string& FileName) : ResourceBase(Tag)
{
	Id = 0;
	Width = 0;
	Height = 0;
	ColorDepth = 0;

	Bitmap Bitmap;
	if(Bitmap.Load(FileName))
	{
		if( Load(Bitmap) )
		{
			std::cout << "		Texture data from file: " << FileName << " succesfully loaded." << std::endl;
			SetValid(true);
		}
	}
	else
	{
		std::cout << "		Error, unable to load texture from file: " << FileName << std::endl;
		SetValid(false);
	}
};


Texture::Texture(const std::string& Tag, const Bitmap& Bitmap) : ResourceBase(Tag)
{
	Id = 0;
	Width = 0;
	Height = 0;
	ColorDepth = 0;

	if( Load(Bitmap) )
	{
			std::cout << "		Texture data from file: " << FileName << " succesfully loaded." << std::endl;
			SetValid(true);
	}
	else
	{
		std::cout << "		Error, unable to load texture from bitmap " << Bitmap.GetFileName() << std::endl;
		SetValid(false);
	}
};


bool Texture::Load(const Bitmap& Bitmap)
{
	if(!Bitmap.Empty())
	{
		if( Id == 0 )
		{
			glActiveTexture(GL_TEXTURE0);
			glGenTextures(1, &Id);	
			glBindTexture(GL_TEXTURE_2D, Id);
	
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
			if(Bitmap.GetColorDepth() == 32)
			{
				glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, Bitmap.GetWidth(), Bitmap.GetHeight(), 0,GL_RGBA, GL_UNSIGNED_BYTE, Bitmap.GetDataBuffer() );
			}
			else	
			{
				std::cout << "		Error while loading texture data from file: " << Bitmap.GetFileName() << " Only 32 bit RGBA image data accepted." << std::endl;
				Clear();
				return false;
			}
	
			glBindTexture(GL_TEXTURE_2D, 0);

			Width = Bitmap.GetWidth();
			Height = Bitmap.GetWidth();
			FileName = Bitmap.GetFileName();
			ColorDepth = Bitmap.GetColorDepth();
			return true;
		}
		else
		{
			std::cout << "		Error while loading texture data from file: " << Bitmap.GetFileName() << " Trying to overwrite texture data without releasing previous buffer." << std::endl;
			return false;
		}
	}
	else
	{
		std::cout << "		Error while loading texture data from file: " << Bitmap.GetFileName() << " Image data buffer is empty." << std::endl;
		return false;
	}
};


bool Texture::Clear()
{
	if(Id > 0)
	{
		std::cout << "		Clearing texture data - " << FileName << std::endl;
		glDeleteTextures(1, &Id);
		Width = 0;
		Height = 0;
		ColorDepth = 0;
		FileName.clear();
		
		return true;
	}
	else
	{
		std::cout << "		Texture data already cleared" << std::endl;
		return false;
	}
};


void Texture::Dispose()
{
	Clear();
};