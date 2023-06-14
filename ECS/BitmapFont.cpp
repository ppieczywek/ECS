#pragma once
#include "stdafx.h"
#include "BitmapFont.h"


BitmapFont::BitmapFont(const std::string& Tag, const std::string& FontFileName) : ResourceBase(Tag)
{
	FontTextureId = 0;
	
	if( LoadBitmapFont(FontFileName) )
	{
		std::cout << "		Texture data from file: " << FileName << " succesfully loaded." << std::endl;
		SetValid(true);
	}
	else
	{
		SetValid(false);
	}
};


BitmapFont::~BitmapFont()
{

};


const bool BitmapFont::LoadBitmapFont(const std::string& FontFileName)
{	
	Bitmap Bitmap;
	if(Bitmap.Load(FontFileName))
	{
		if(!Bitmap.Empty())
		{
			if( FontTextureId == 0 )
			{
				glActiveTexture(GL_TEXTURE0);
				glGenTextures(1, &FontTextureId);	
				glBindTexture(GL_TEXTURE_2D, FontTextureId);
	
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
			}
			
			FileName = FontFileName;	
			FontImageWidth = Bitmap.GetWidth();
			FontImageHeight = Bitmap.GetHeight();
			FontImageColorDepth = Bitmap.GetColorDepth();
			BaseCharOffset = 32;
			CharSpacingFactor = 0.8f;
			RowPitch = DefaultRowPitch;
			ColPitch = DefaultColPitch;
			CellWidth =  FontImageWidth / ColPitch;
			CellHeight = FontImageHeight / RowPitch;
			ScaleFactor.x = ColFactor = 1.0f / static_cast<float>(ColPitch);
			ScaleFactor.y = RowFactor = 1.0f / static_cast<float>(RowPitch);

	
			CharCoordinates.resize(RowPitch*ColPitch);
			for(int ii=0; ii < static_cast<int>(RowPitch*ColPitch); ++ii)
			{
				auto row = floor( static_cast<float>(ii) / static_cast<float>(RowPitch) );
				auto col = static_cast<float>(ii) - (row * static_cast<float>(RowPitch));

				CharCoordinates[ii].x = col * ColFactor;
				CharCoordinates[ii].y = (static_cast<float>(RowPitch) - (row + 1.0f))*RowFactor;
			}
			std::cout << "		Bitmap font file: " << FileName << " succefully loaded." << std::endl;
			return true;
		}
	}
	std::cout << "		Error while loading bitmap font file: " << FileName << " Unable to read image data." << std::endl;
	return false;
};


void BitmapFont::SetRowPitch(const unsigned int& Value)
{
	RowPitch = Value;
	CellHeight = FontImageHeight / RowPitch;
	ScaleFactor.y = RowFactor = 1.0f / static_cast<float>(RowPitch);

	CharCoordinates.resize(RowPitch*ColPitch);
	for(int ii=0; ii < static_cast<int>(RowPitch*ColPitch); ++ii)
	{
		auto row = floor( static_cast<float>(ii) / static_cast<float>(RowPitch) );
		auto col = static_cast<float>(ii) - (row * static_cast<float>(RowPitch));

		CharCoordinates[ii].x = col * ColFactor;
		CharCoordinates[ii].y = (static_cast<float>(RowPitch) - (row + 1.0f))*RowFactor;
	}
};


void BitmapFont::SetColPitch(const unsigned int& Value)
{
	ColPitch = Value;
	CellWidth =  FontImageWidth / ColPitch;
	ScaleFactor.x = ColFactor = 1.0f / static_cast<float>(ColPitch);
		
	CharCoordinates.resize(RowPitch*ColPitch);
	for(int ii=0; ii < static_cast<int>(RowPitch*ColPitch); ++ii)
	{
		auto row = floor( static_cast<float>(ii) / static_cast<float>(RowPitch) );
		auto col = static_cast<float>(ii) - (row * static_cast<float>(RowPitch));

		CharCoordinates[ii].x = col * ColFactor;
		CharCoordinates[ii].y = (static_cast<float>(RowPitch) - (row + 1.0f))*RowFactor;
	}
};


bool BitmapFont::Clear()
{
	if(FontTextureId > 0)
	{
		std::cout << "		Clearing bitmap font data - " << FileName << std::endl;
		FontTextureId = 0;
		FontImageWidth = 0;
		FontImageHeight = 0;
		CellWidth = 0;
		CellHeight = 0;
		RowPitch = 0;
		ColPitch = 0;
		FontImageColorDepth = 0;
		BaseCharOffset = 0;
		RowFactor = 0.0f;
		ColFactor = 0.0f;
		FileName.clear();
		CharCoordinates.clear();

		if(FontTextureId > 0)
		{
			glDeleteTextures(1, &FontTextureId);
		}
		return true;
	}
	else
	{
		return false;
	}
};

void BitmapFont::Dispose()
{
	Clear();
}