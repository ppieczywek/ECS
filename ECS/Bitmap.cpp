#pragma once
#include "stdafx.h"
#include "Bitmap.h"



Bitmap::Bitmap()
{
	Width = 0;
	Height = 0;
	ColorDepth = 0;
	Channels = 0;
};
	

const bool Bitmap::Load(const std::string& ImageName)
{
	std::fstream BitmapFile;
	BitmapFile.open(ImageName , std::ios::in | std::ios::binary);

	if (BitmapFile.is_open())
    {
		if( BitmapFile.good() )
		{
			int StringLength =  ImageName.length();
			std::string Extension = ImageName.substr( StringLength-3, StringLength );
			
			if( Extension.compare("tga") == 0 )
			{
				if( LoadTargaBitmap(BitmapFile) )
				{		
					FileName = ImageName;
					std::cout << "		Bitmap file: " << FileName << " succefully loaded." << std::endl;
					BitmapFile.close();
					return true;
				}
				else
				{
					std::cout << "		Error while loading bitmap file: " << ImageName << " Unable to read image data." << std::endl;
				}
			}
			else
			{
				std::cout << "		Error while loading bitmap file: " << ImageName << " Unsupported file typen" << std::endl;
			}
			BitmapFile.close();
		}
		else
		{
			std::cout << "		Error while loading bitmap file: " << ImageName << " File corruptn" << std::endl;
		}
	}
	else
	{
		std::cout << "		Error while loading bitmap file: " << ImageName << " File not found." << std::endl;
	}
	return false;
	
};


const bool Bitmap::LoadTargaBitmap(std::fstream& BitmapFile)
{	
	TargaFileHeader TgaHeader;
	BitmapFile.read( reinterpret_cast<char*>(&TgaHeader), sizeof(TargaFileHeader));
			
	if( TgaHeader.ImageTypeCode == 2 || TgaHeader.ImageTypeCode == 3)
	{
		Width		= TgaHeader.ImageWidth;
		Height		= TgaHeader.ImageHeight;
		ColorDepth	= TgaHeader.BitCount;
		Channels	= TgaHeader.BitCount / 8;
			
		long ImageSize = Width * Height * Channels;
						
		DataBuffer.resize(ImageSize);
		BitmapFile.read( reinterpret_cast<char*>(&DataBuffer[0]), sizeof(unsigned char) * ImageSize);
					
		for(int ii=0; ii < ImageSize; ii+=Channels)
		{
			std::swap(DataBuffer[ii], DataBuffer[ii+2]);
		}
		return true;
	}
	return false;
};