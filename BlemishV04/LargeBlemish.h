// LargeBlemish.h
#pragma once

#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <conio.h>

/* blemish detection type */
typedef enum
{
	BLEMISH_TYPE_BRIGHT = 1,	// bit 01
	BLEMISH_TYPE_DARK = 2,		// bit 10
	BLEMISH_TYPE_BOTH = BLEMISH_TYPE_BRIGHT + BLEMISH_TYPE_DARK,	// bit 11
} BLEMISH_TYPE;

/* sigma info */
typedef struct
{
	float sigma_inner;
	float sigma_outer;
	float sigma_edge;
	float sigma_corner;
	float sigma_edgeLT;
	float sigma_edgeRT;
	float sigma_edgeLB;
	float sigma_edgeRB;
	float sigma_cornerLT;
	float sigma_cornerRT;
	float sigma_cornerLB;
	float sigma_cornerRB;
} SigmaInfo;

/* blemish result */
typedef struct
{
	int num_blemish_inner;
	int num_blemish_outer;
	int num_blemish_edge;
	float * blemish_ratio;		// roi_xnum * roi_ynum blemish ratio, if not required, set null
	float * blemish_threshold;	// 12 blemish thresholds, if not required set null
} BlemishResult;

/*************** Large blemish detection function ********************************************************************************
*
* Width : width of image
* Height : height of image
* pSrc8 : source image
* pDib24 : 24bpp image to mark blemish rois
* roi_xnum : number of x rois
* roi_ynum : number of y rois, if set zero, auto calculated as a sequre size roi
* inner_size : inner region percentage
* outer_size : outer region percentage
* edge_size : edge region percentage
* corner_size : define 4 corner length percent over region's width
* sigma_inner : inner region's blemish sigma, sigma decides how much variance of standard deviation is allowed. ex) 5, 6, 7...
* sigma_outer... sigma_conerRB : each region's blemish sigma
* inner_Blemish...Edge_Blemish : counted blemish number
* color : color to mark blemish rois
* debugFileName : a filename to save the blemish result
* sensorId : sensor id to identify above blemish result
* --------------------------------------------------------------------------------------------------------------------------------
* blemishType : BRIGHT, DARK, or BOTH, both searches bright blemish and dark blemish simulataneously
* sigma_dark : sigma setting for dark blemish, if only searching BRIGHT set this null
* sigma_bright : sigma setting for bright blemish, if only searching DARK set this null
* blemish_result_dark : result of detected blemishes, if ony searching BRIGHT set this null
* blemish_result_bright : result of detected blemishes, if only searching DARK set this null
/**********************************************************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
		/* dark blemish detection */
		__declspec(dllimport) int JLib_CAL_LargeBlemish3_Dark(UINT Width, UINT Height, BYTE *pSrc8, BYTE *pDib24,
			const int roi_xnum, const int roi_ynum,
			const int inner_size, const int outer_size, const int edge_size, const int corner_size,
			const float sigma_inner, const float sigma_outer, const float sigma_edge, const float sigma_corner,
			const float sigma_edgeLT, const float sigma_edgeRT, const float sigma_edgeLB, const float sigma_edgeRB,
			const float sigma_cornerLT, const float sigma_cornerRT, const float sigma_cornerLB, const float sigma_cornerRB,
			int *Inner_Blemish = NULL, int *Outer_Blemish = NULL, int *Edge_Blemish = NULL, COLORREF color = RGB(0, 0, 0), 
			char * debugFileName = NULL, char * sensorId = NULL);

		/* bright blemish detection */
		__declspec(dllimport) int JLib_CAL_LargeBlemish3_Bright(UINT Width, UINT Height, BYTE *pSrc8, BYTE *pDib24,
			const int roi_xnum, const int roi_ynum,
			const int inner_size, const int outer_size, const int edge_size, const int corner_size,
			const float sigma_inner, const float sigma_outer, const float sigma_edge, const float sigma_corner,
			const float sigma_edgeLT, const float sigma_edgeRT, const float sigma_edgeLB, const float sigma_edgeRB,
			const float sigma_cornerLT, const float sigma_cornerRT, const float sigma_cornerLB, const float sigma_cornerRB,
			int *Inner_Blemish = NULL, int *Outer_Blemish = NULL, int *Edge_Blemish = NULL, COLORREF color = RGB(0, 0, 0), 
			char * debugFileName = NULL, char * sensorId = NULL);

		/* new inteface to both dark and bright blemish detection simultaneously */
		__declspec(dllimport) int JLib_CAL_LargeBlemish3_Both(UINT Width, UINT Height, BYTE *pSrc8, BYTE *pDib24,
			const int roi_xnum, const int roi_ynum,
			const int inner_size, const int outer_size, const int edge_size, const int corner_size,
			const BLEMISH_TYPE blemishType, const SigmaInfo * sigma_dark = NULL, const SigmaInfo * sigma_bright = NULL,
			BlemishResult * blemish_result_dark = NULL, BlemishResult * blemish_result_bright = NULL, COLORREF color = RGB(0, 0, 0), 
			char * debugFileName = NULL, char * sensorId = NULL);
#ifdef __cplusplus
}
#endif