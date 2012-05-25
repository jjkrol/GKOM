/************************************************************
*	APRON TUTORIALS PRESENTED BY MORROWLAND					*
*************************************************************
*	Author					: Ronny André Reierstad			*
*	Web Page				: www.morrowland.com			*
*	E-Mail					: apron@morrowland.com			*
*	Version					: English (UK)					*
************************************************************/
#include "stdafx.h"
#include "jpeg.h"
#include <stdio.h>
#include <stdlib.h>



//Macros's to assist
#define DISPLAY_VARMEM(var) displayMemory((char*) &( var ), (sizeof( var )));
#define DISPLAY_PTRMEM(ptr) displayMemory((char*) ( ptr ), (sizeof( *ptr )));


void displayMemory(char *address, int length) {
	int i = 0; //used to keep track of line lengths
	char *line = (char*)address; //used to print char version of data
	unsigned char ch; // also used to print char version of data
	printf("%08X | ", (int)address); //Print the address we are pulling from
	while (length-- > 0) {
		printf("%02X ", (unsigned char)*address++); //Print each char
		if (!(++i % 16) || (length == 0 && i % 16)) { //If we come to the end of a line...
			//If this is the last line, print some fillers.
			if (length == 0) { while (i++ % 16) { printf("__ "); } }
			printf("| ");
			while (line < address) {  // Print the character version
				ch = *line++;
				printf("%c", (ch < 33 || ch == 255) ? 0x2E : ch);
			}
			// If we are not on the last line, prefix the next line with the address.
			if (length > 0) { printf("\n%08X | ", (int)address); }
		}
	}
	puts("");
}


void loadHeightMap(BYTE *map, int size, LPSTR fileName){
	if(!fileName)	return;
	 //FILE *pFile = NULL;
 
  //  // Open The File In Read / Binary Mode.
  //  pFile = fopen( fileName, "rb" );
 
  //  if ( pFile == NULL )   
  //  {
  //      return;
  //  }
  //  fread( map, 1, size, pFile );
 
  //  if (ferror( pFile )) {
		//std::cout<<"Error reading heightmap"<<std::endl;
  //  }
 
  //  fclose(pFile);
	
tImageJPG * amap = Load_JPEG(fileName);
if(amap == NULL)	exit(0);
memcpy(map, amap->data, size*size*3);
}

void BMP_Texture(UINT textureArray[], LPSTR strFileName, int ID)
{
	if(!strFileName)   return;
	
	AUX_RGBImageRec *pBitMap = auxDIBImageLoad(strFileName);
	
	if(pBitMap == NULL)	exit(0);

	glGenTextures(1, &textureArray[ID]);
	glBindTexture(GL_TEXTURE_2D, textureArray[ID]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitMap->sizeX, pBitMap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitMap->data);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	

	if (pBitMap)										
	{
		if (pBitMap->data)								
		{
			free(pBitMap->data);						
		}
		free(pBitMap);									
	}
}


void JPEG_Skybox(UINT textureArray[], LPSTR strFileName, int ID)
{
	if(!strFileName)	return;
	
	tImageJPG *pBitMap = Load_JPEG(strFileName);
	
	if(pBitMap == NULL)	exit(0);
	
	glGenTextures(1, &textureArray[ID]);
	glBindTexture(GL_TEXTURE_2D, textureArray[ID]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitMap->sizeX, pBitMap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitMap->data);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);	

	if (pBitMap)									
	{
		if (pBitMap->data)					
		{
			free(pBitMap->data);			
		}
		free(pBitMap);
	}
}

void JPEG_Texture(UINT textureArray[], LPSTR strFileName, int ID)
{
	if(!strFileName)	return;
	
	tImageJPG *pBitMap = Load_JPEG(strFileName);
	
	if(pBitMap == NULL)	exit(0);
	
	glGenTextures(1, &textureArray[ID]);
	glBindTexture(GL_TEXTURE_2D, textureArray[ID]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitMap->sizeX, pBitMap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitMap->data);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);	
	
	if (pBitMap)									
	{
		if (pBitMap->data)					
		{
			free(pBitMap->data);			
		}
		free(pBitMap);
	}
}

tImageJPG *Load_JPEG(const char *strfilename)
{
	struct jpeg_decompress_struct cInfo;
	tImageJPG *pImgData = NULL;
	FILE *pFile;
	
	if((pFile = fopen(strfilename, "rb")) == NULL) 
	{
				std::cout<<"Error loading jpg file"<<std::endl;
		return NULL;
	}
	
	jpeg_error_mgr jerr;

	cInfo.err = jpeg_std_error(&jerr);

	jpeg_create_decompress(&cInfo);

	jpeg_stdio_src(&cInfo, pFile);
	
	pImgData = (tImageJPG*)malloc(sizeof(tImageJPG));

	Decompress_JPEG(&cInfo, pImgData);

	jpeg_destroy_decompress(&cInfo);
	
	fclose(pFile);
	return pImgData;
}


void Decompress_JPEG(jpeg_decompress_struct* cInfo, tImageJPG *pImgData)
{
	jpeg_read_header(cInfo, TRUE);
	
	jpeg_start_decompress(cInfo);
	
	pImgData->rowSpan = cInfo->image_width * cInfo->num_components;
	pImgData->sizeX   = cInfo->image_width;
	pImgData->sizeY   = cInfo->image_height;
	
	pImgData->data = new unsigned char[pImgData->rowSpan * pImgData->sizeY];
	
	unsigned char** rowPtr = new unsigned char*[pImgData->sizeY];
	for (int i = 0; i < pImgData->sizeY; i++)
		rowPtr[i] = &(pImgData->data[i*pImgData->rowSpan]);
	
	int rowsRead = cInfo->output_height-1;
	while (cInfo->output_scanline < cInfo->output_height) 
	{
		rowsRead -= jpeg_read_scanlines(cInfo, &rowPtr[rowsRead], cInfo->output_height - rowsRead);
	}
	delete [] rowPtr;
	
	jpeg_finish_decompress(cInfo);
}