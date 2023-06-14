struct TargaFileHeader
{
	signed char			ImageIdLength;
	signed char			ColorMapType;    
	signed char			ImageTypeCode;     
    signed char			ColorMapOrigin[2];       /* 03h  Color map origin */
	signed char			ColorMapLength[2];      /* 05h  Color map length */
	signed char			ColorMapDepth;       /* 07h  Depth of color map entries */
	unsigned short int	ImageXOrigin;         /* 08h  X origin of image */
	unsigned short int	ImageYOrigin;         /* 0Ah  Y origin of image */
	unsigned short int	ImageWidth;           /* 0Ch  Width of image */
	unsigned short int	ImageHeight;          /* 0Eh  Height of image */
	signed char			BitCount;      /* 10h  Image pixel size */
	signed char			ImageDescriptor; /* 11h  Image descriptor byte */
};

