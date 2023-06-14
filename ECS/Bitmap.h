#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "TargaFileHeader.h"

class Bitmap
{
			std::string						FileName;
			unsigned int					Width;
			unsigned int					Height;
			unsigned int					ColorDepth;
			unsigned int					Channels;
			std::vector<unsigned char>		DataBuffer;

			const bool						LoadTargaBitmap(std::fstream&);
	public:
											Bitmap();
			const bool						Load(const std::string&);
			inline const bool				Empty() const;
			inline const std::string&		GetFileName() const;
			inline const unsigned int		GetSize() const;
			inline const unsigned int&		GetWidth() const;
			inline const unsigned int&		GetHeight() const;
			inline const unsigned int&		GetColorDepth() const; 
			inline const unsigned int&		GetChannels() const;
			inline const unsigned char*		GetDataBuffer() const;
};


inline const std::string& Bitmap::GetFileName() const
{
	return FileName;
};


inline const bool Bitmap::Empty() const
{
	return DataBuffer.empty();
};


inline const unsigned int Bitmap::GetSize() const
{
	return DataBuffer.size();
};


inline const unsigned int& Bitmap::GetWidth() const
{
	return Width;
};


inline const unsigned int& Bitmap::GetHeight() const
{
	return Height;
};


inline const unsigned int& Bitmap::GetColorDepth() const
{
	return ColorDepth;
};


inline const unsigned int& Bitmap::GetChannels() const
{
	return Channels;
};


inline const unsigned char* Bitmap::GetDataBuffer() const
{
	return &DataBuffer[0];
};
