// Con_XLBLEM.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#include "LargeBlemish.h"

/* helper function */
void writeBmp(char * fileName, unsigned char * image, int width, int height, int bpp);
int readRaw(char * filename, int width, int height, WORD * pBuffer);
int writeBlemishRatio(char * filename, int width, int height, BlemishResult * darkResult, BlemishResult * brightResult);

int main(int argc, char* argv[])
{

	char * filename;
	int width = 640, height = 480;

	if (argc > 3)
	{
		filename = argv[1];
		width = atoi(argv[2]);
		height = atoi(argv[3]);
	}
	else
	{
		printf("usage : XLBLEM filename width height\n");
		return -1;
	}

	int num_x_roi = 100;	// X축을 100개로 분할
	int num_y_roi = 0;		// 100 * 3 / 4, 0 일경우 정사각형으로 자동 계산한다
	if (argc > 5)
	{
		num_x_roi = atoi(argv[4]);
		num_y_roi = atoi(argv[5]);
	}

	int num_multi_scale = 1;
	if (argc > 6)
	{
		num_multi_scale = atoi(argv[6]);
	}

	float inner_sigma = 6.0;		// inner 영역 sigma, blemish로 판단할 sigma 값을 설정한다. threshold = avg + std * sigma로 내부에서 계산된다.
	float outer_sigma = 6.0;		// 각 영역별로 Simga를 넣는다.대략 5 ~ 10 사이가 적정값
	float edge_sigma = 7.0;
	float corner_sigma = 8.0;
	float edge_corner_sigma = 8.0;
	float corner_corner_sigma = 10.0;
	if (argc > 12)
	{
		inner_sigma = atof(argv[7]);
		outer_sigma = atof(argv[8]);
		edge_sigma = atof(argv[9]);
		corner_sigma = atof(argv[10]);
		edge_corner_sigma = atof(argv[11]);
		corner_corner_sigma = atof(argv[12]);
	}

	float inner_sigma_bright = 6.0;		// inner 영역 sigma, blemish로 판단할 sigma 값을 설정한다. threshold = avg - std * sigma로 내부에서 계산된다.
	float outer_sigma_bright = 6.0;		// 각 영역별로 Simga를 넣는다.대략 5 ~ 10 사이가 적정값
	float edge_sigma_bright = 7.0;
	float corner_sigma_bright = 8.0;
	float edge_corner_sigma_bright = 8.0;
	float corner_corner_sigma_bright = 10.0;
	if (argc > 18)
	{
		inner_sigma = atof(argv[13]);
		outer_sigma = atof(argv[14]);
		edge_sigma = atof(argv[15]);
		corner_sigma = atof(argv[16]);
		edge_corner_sigma = atof(argv[17]);
		corner_corner_sigma = atof(argv[18]);
	}

	int inner_region_ratio = 50;
	int outer_region_ratio = 75;
	int edge_region_ratio = 100;
	int corner_length_ratio = 5;	// 영역 폭 대비 4 코너의 크기 비율(퍼센트)를 설정한다. Edge 영역 폭의 5%가 코너 크기
	if (argc > 22)
	{
		inner_region_ratio = atoi(argv[19]);
		outer_region_ratio = atoi(argv[20]);
		edge_region_ratio = atoi(argv[21]);
		corner_length_ratio = atoi(argv[22]);
	}

	WORD *pBuf	= (WORD*)malloc(width*height*sizeof(WORD));	
	BYTE *pLum = (BYTE*)malloc(width*height*sizeof(BYTE));		// 이미지 Y/Lumin 
	BYTE *pDibDark	= (BYTE*)malloc(width*height*3*sizeof(BYTE));	// 이미지 RGB(DIB)
	BYTE *pDibBright = (BYTE*)malloc(width*height * 3 * sizeof(BYTE));	// 이미지 RGB(DIB)
	BYTE *pDibBoth = (BYTE*)malloc(width*height * 3 * sizeof(BYTE));	// 이미지 RGB(DIB)

	const int threshold_size = 12;		// returning 12 threshold values from the end of threshold ratio
	int est_num_y_roi = num_y_roi > 0 ? num_y_roi : height * num_x_roi / width;
	float * pBlemishRatioDark = (float *)malloc(num_x_roi * est_num_y_roi * sizeof(float));	
	float * pBlemishRatioBright = (float *)malloc(num_x_roi * est_num_y_roi * sizeof(float));

	if (readRaw(filename, width, height, pBuf))
	{
		free(pBuf);
		free(pLum);
		free(pDibDark);
		free(pDibBright);
		free(pDibBoth);
		free(pBlemishRatioDark);
		free(pBlemishRatioBright);
		return -1;
	}

	for (int i = 0; i < width * height; i++)
	{
		pLum[i] = pBuf[i] * 256 / 1023;	// 10 bits to 8 bits
		pDibDark[i * 3] = pDibDark[i * 3 + 1] = pDibDark[i * 3 + 2] = pLum[i];
		pDibBright[i * 3] = pDibBright[i * 3 + 1] = pDibBright[i * 3 + 2] = pLum[i];
		pDibBoth[i * 3] = pDibBoth[i * 3 + 1] = pDibBoth[i * 3 + 2] = pLum[i];
	}
	free(pBuf);

	// 사용 예제
	float fInner = inner_sigma;
	float fOuter	= outer_sigma;
	float fEdge		= edge_sigma;
	float fCorner	= corner_sigma;
	float fEdgeLT	= edge_corner_sigma;
	float fEdgeRT	= edge_corner_sigma;
	float fEdgeLB	= edge_corner_sigma;
	float fEdgeRB	= edge_corner_sigma;
	float fCornLT	= corner_corner_sigma;
	float fCornRT	= corner_corner_sigma;
	float fCornLB	= corner_corner_sigma;
	float fCornRB	= corner_corner_sigma;

	COLORREF boxColorTbl[] = { RGB(0x33,0xff,0x33),  RGB(0xa5,0x2a,0x2a),  RGB(0xff,0xd7,0x00) };	// green, brown, gold
	int LB_Inner, LB_Outer, LB_Edge;
	char buffer[100];
	int num_blemish = 0;
	char * sensorid = "1234567890123456";
	for (int n = 0; n < num_multi_scale; n++)
	{
		// for testig purpose enable this
		clock_t begin, end;
		int nResult;
#if 1
		// check dark blemish
		begin = clock() * CLK_TCK;
		sprintf(buffer, "%s_dark_result_%d.txt", filename, num_x_roi);	// save error roi info to a file
		nResult = JLib_CAL_LargeBlemish3_Dark(width, height, pLum, pDibDark,
			num_x_roi, num_y_roi,
			inner_region_ratio, outer_region_ratio, edge_region_ratio, corner_length_ratio,
			fInner, fOuter, fEdge, fCorner,
			fEdgeLT, fEdgeRT, fEdgeLB, fEdgeRB,
			fCornLT, fCornRT, fCornLB, fCornRB,
			&LB_Inner, &LB_Outer, &LB_Edge, boxColorTbl[n], buffer, sensorid);
		end = clock() * CLK_TCK;

		printf("filename : %s, number of dark blemish points : inner = %d, outer = %d, edge = %d\n", filename, LB_Inner, LB_Outer, LB_Edge);
		printf("algoritm running time = %f msec\n", (end - begin) / 1000.0f);
		num_blemish = LB_Inner + LB_Outer + LB_Edge;
		if (n == num_multi_scale - 1)
		{
			sprintf(buffer, "%s_dark.bmp", filename);
			writeBmp(buffer, pDibDark, width, height, 24);
		}

		// check bright blemish
		sprintf(buffer, "%s_bright_result_%d.txt", filename, num_x_roi);	// save error roi info to a file
		begin = clock() * CLK_TCK;
		nResult = JLib_CAL_LargeBlemish3_Bright(width, height, pLum, pDibBright,
			num_x_roi, num_y_roi,
			inner_region_ratio, outer_region_ratio, edge_region_ratio, corner_length_ratio,
			fInner, fOuter, fEdge, fCorner,
			fEdgeLT, fEdgeRT, fEdgeLB, fEdgeRB,
			fCornLT, fCornRT, fCornLB, fCornRB,
			&LB_Inner, &LB_Outer, &LB_Edge, boxColorTbl[n], buffer, sensorid);
		end = clock() * CLK_TCK;

		printf("filename : %s, number of bright blemish points : inner = %d, outer = %d, edge = %d\n", filename, LB_Inner, LB_Outer, LB_Edge);
		printf("algoritm running time = %f msec\n", (end - begin) / 1000.0f);
		num_blemish = LB_Inner + LB_Outer + LB_Edge;
		if (n == num_multi_scale - 1)
		{
			sprintf(buffer, "%s_bright.bmp", filename);
			writeBmp(buffer, pDibBright, width, height, 24);
		}
#endif
		// check both blemish with new interface
		float threshold_dark[threshold_size];
		float threshold_bright[threshold_size];
		BlemishResult result_dark = { 0, 0, 0, pBlemishRatioDark, threshold_dark }, result_bright = { 0, 0, 0, pBlemishRatioBright, threshold_bright };
		SigmaInfo sigma_dark = { fInner, fOuter, fEdge, fCorner, fEdgeLT, fEdgeRT, fEdgeLB, fEdgeRB, fCornLT, fCornRT, fCornLB, fCornRB };
		SigmaInfo sigma_bright = sigma_dark;

		sprintf(buffer, "%s_both_result_%d.txt", filename, num_x_roi);	// save error roi info to a file
		begin = clock() * CLK_TCK;
		nResult = JLib_CAL_LargeBlemish3_Both(width, height, pLum, pDibBoth,
			num_x_roi, num_y_roi,
			inner_region_ratio, outer_region_ratio, edge_region_ratio, corner_length_ratio,
			BLEMISH_TYPE_BOTH, &sigma_dark, &sigma_bright, &result_dark, &result_bright, boxColorTbl[n], buffer, sensorid);
		end = clock() * CLK_TCK;

		printf("filename : %s, number of both blemish points : inner = %d, outer = %d, edge = %d\n", filename, result_dark.num_blemish_inner + result_bright.num_blemish_inner, 
			result_dark.num_blemish_outer + result_bright.num_blemish_outer, result_dark.num_blemish_edge + result_bright.num_blemish_edge);
		printf("algoritm running time = %f msec\n", (end - begin) / 1000.0f);

		if (n == num_multi_scale - 1)
		{
			sprintf(buffer, "%s_both.bmp", filename);
			writeBmp(buffer, pDibBoth, width, height, 24);
		}
		sprintf(buffer, "%s_%d.txt", filename, num_x_roi);
		writeBlemishRatio(buffer, num_x_roi, est_num_y_roi, &result_dark, &result_bright);
		num_x_roi = num_x_roi / 2;
		num_blemish += result_dark.num_blemish_inner + result_bright.num_blemish_inner + result_dark.num_blemish_outer + result_bright.num_blemish_outer + result_dark.num_blemish_edge + result_bright.num_blemish_edge;
	}


	free(pBlemishRatioDark);
	free(pBlemishRatioBright);
	free(pLum);
	free(pDibDark);
	free(pDibBright);
	free(pDibBoth);

	return num_blemish;
}


/* helpfer function */
/* write blemish ratios on a text file */
int writeBlemishRatio(char * filename, int width, int height, BlemishResult * darkResult = NULL, BlemishResult * brightResult = NULL)
{
	FILE * fp;
	errno_t error = fopen_s(&fp, filename, "wt");
	const int threshold_size = 12;
	if (fp)
	{
		BlemishResult * result = darkResult;
		for (int k = 0; k < 2; k++)
		{
			if (!result) continue;

			if (k == 1) fprintf(fp, "\nBright Blemish Ratio :\n");
			else fprintf(fp, "Dark Blemish Ratio :\n");

			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					fprintf(fp, "% 6.4f, ", result->blemish_ratio[i * width + j]);
				}
				fprintf(fp, "\n");
			}
			if (result->blemish_threshold)
			{
				fprintf(fp, "\nthreshold inner = %6.4f, outer = %6.4f, edge = %6.4f, corner = %6.4f\n", result->blemish_threshold[0], result->blemish_threshold[1], result->blemish_threshold[2], result->blemish_threshold[3]);
				fprintf(fp, "threshold edgeLT = %6.4f, edgeRT = %6.4f, edgeLB = %6.4f, edgeRB = %6.4f\n", result->blemish_threshold[4], result->blemish_threshold[5], result->blemish_threshold[6], result->blemish_threshold[7]);
				fprintf(fp, "threshold cornerLT = %6.4f, cornerRT = %6.4f, cornerLB = %6.4f, cornerRB = %6.4f\n", result->blemish_threshold[8], result->blemish_threshold[9], result->blemish_threshold[10], result->blemish_threshold[11]);
				fprintf(fp, "num of inner blemish = %d, outer blemish = %d, edge blemish = %d\n", result->num_blemish_inner, result->num_blemish_outer, result->num_blemish_edge);
			}
			result = brightResult;
		}
	}
	else
	{
		printf("cannot open file %s\n", filename);
		return -1;
	}
	fclose(fp);
	return 0;
}


/* read a raw image with jpeg output */
int readRaw(char * filename, int width, int height, WORD * pBuffer)
{
	FILE * fp;
	fopen_s(&fp, filename, "rb");
	int err_code = 0;
	if (fp)
	{
		size_t numRead = fread_s(pBuffer, width * height * 2, 2, width * height, fp);
		if (numRead != width * height)
		{
			printf("file size mismatch!!\n");
			err_code = -1;
		}
	}
	else
	{
		printf("file %s not found!!\n", filename);
		err_code = -1;
	}
	fclose(fp);
	return err_code;
}

/* helpfer function */
/* save Mat data to a bmp file */
void writeBmp(char * fileName, unsigned char * image, int width, int height, int bpp) {
	// silly macro
	unsigned char * buf = image;

	unsigned int headers[13];
	FILE * outfile;
	int extrabytes;
	int paddedsize;
	int x; int y; int n;

	extrabytes = 4 - ((width) % 4);                 // How many bytes of padding to add to each
													// horizontal line - the size of which must
													// be a multiple of 4 bytes.
	if (extrabytes == 4)
		extrabytes = 0;

	paddedsize = ((width)+extrabytes) * height;

	// Headers...
	// Note that the "BM" identifier in bytes 0 and 1 is NOT included in these "headers".

	headers[0] = paddedsize + 54 + 0x400;      // bfSize (whole file size)
	headers[1] = 0;                    // bfReserved (both)
	headers[2] = 54 + 0x400;                   // bfOffbits
	headers[3] = 40;                   // biSize
	headers[4] = width;  // biWidth
	headers[5] = height; // biHeight

						 // Would have biPlanes and biBitCount in position 6, but they're shorts.
						 // It's easier to write them out separately (see below) than pretend
						 // they're a single int, especially with endian issues...

	headers[7] = 0;                    // biCompression
	headers[8] = paddedsize;           // biSizeImage
									   //headers[9]  = 0xEC3;                // biXPelsPerMeter
									   //headers[10] = 0xEC3;                // biYPelsPerMeter
	headers[9] = 0;                // biXPelsPerMeter
	headers[10] = 0;                // biYPelsPerMeter
	headers[11] = 256;                  // biClrUsed
	headers[12] = 256;                  // biClrImportant

	if (errno_t err = fopen_s(&outfile, fileName, "wb") != 0)
	{
		printf("File open error!");
		return;
	}

	//
	// Headers begin...
	// When printing ints and shorts, we write out 1 character at a time to avoid endian issues.
	//

	fprintf(outfile, "BM");

	for (n = 0; n <= 5; n++) {
		fprintf(outfile, "%c", headers[n] & 0x000000FF);
		fprintf(outfile, "%c", (headers[n] & 0x0000FF00) >> 8);
		fprintf(outfile, "%c", (headers[n] & 0x00FF0000) >> 16);
		fprintf(outfile, "%c", (headers[n] & (unsigned int)0xFF000000) >> 24);
	}

	// These next 4 characters are for the biPlanes and biBitCount fields.

	fprintf(outfile, "%c", 1);
	fprintf(outfile, "%c", 0);
	if (bpp == 24)
		fprintf(outfile, "%c", 24); // the number of bits per pixel
	else
		fprintf(outfile, "%c", 8); // the number of bits per pixel
	fprintf(outfile, "%c", 0);

	for (n = 7; n <= 12; n++) {
		fprintf(outfile, "%c", headers[n] & 0x000000FF);
		fprintf(outfile, "%c", (headers[n] & 0x0000FF00) >> 8);
		fprintf(outfile, "%c", (headers[n] & 0x00FF0000) >> 16);
		fprintf(outfile, "%c", (headers[n] & (unsigned int)0xFF000000) >> 24);
	}

	// duplicate the way how Matlab's imwrite and Python scipy.misc.imsave create BMP files
	for (n = 0; n < 256; n++) {
		fprintf(outfile, "%c", (n & 0xFF));
		fprintf(outfile, "%c", (n & 0xFF));
		fprintf(outfile, "%c", (n & 0xFF));
		fprintf(outfile, "%c", (0x00 & 0xFF));
	}

	//
	// Headers done, now write the data...
	//

	buf += (height - 1) * width * (bpp == 24 ? 3 : 1);
	for (y = height - 1; y >= 0; y--) {     // BMP image format is written from bottom to top...
		for (x = 0; x <= width - 1; x++) {
			fprintf(outfile, "%c", *buf++);
			if (bpp == 24)
			{
				fprintf(outfile, "%c", *buf++);
				fprintf(outfile, "%c", *buf++);
			}
		}
		if (extrabytes) {      // See above - BMP lines must be of lengths divisible by 4.
			for (n = 1; n <= extrabytes; n++) {
				fprintf(outfile, "%c", 0);
			}
		}
		buf -= 2 * width * (bpp == 24 ? 3:1);
	}

	fclose(outfile);
	return;
}