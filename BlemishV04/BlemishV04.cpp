#include "BlemishV04.h"
#include "ui_BlemishV04.h"
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <opencv2/opencv.hpp>
#include <QTextCodec>
#include <afx.h>
#include <string>
#include<iostream>     
#include<fstream>   
#include<math.h>
#include<LargeBlemish.h>
#include<ji_libdef.h>

#define MaxDefect 50
#define MaxDefectSize 512
#define MaxDefectRange 64

using namespace cv;
using namespace std;

int rectSize = 20;
int defectCntSpec = 5;
int smallCnt[4] = { 0 }, bigCnt[4] = { 0 };
int lessC1 = 2;
uchar avg = 0;
int minV = 0;
int maxV = 255;
bool NG = false;
string th[4] = { "" ,"" ,"" ,"" }, bth[2][4] = { "" ,"" ,"" ,"" ,"" ,"" ,"" ,"" };
string in_Size = "", out_Size = "",secondThd="";
int Allow[4] = { 0 }, Spec[4] = { 0 }, Limit[4] = { 0 }, Array[4] = { 0 };
int lineThickness = 8;
int knownDefectCnt = 0;
int WBcnt = 0;
int Region_A = 60, Region_B = 80, Region_C = 100;
float nowTh = 0;
int ret = 0;

unsigned int Width=0,Height=0;

CString strDllName = _T("Ji_Lib.dll");
HINSTANCE his = LoadLibrary(strDllName);
typedef int(*LargeBlemish2)(UINT, UINT, BYTE *, BYTE*,
	const int, const int, const int, const int, const int,
	const float, const float, const float, const float,
	const float, const float, const float, const float,
	const float, const float, const float, const float,
	int *, int *, int *, COLORREF);


LargeBlemish2 LB2 = (LargeBlemish2)GetProcAddress(his, "JLib_CAL_LargeBlemish2");

//  1-3  4-7  8-15  16-31  32-63  64-128
//    4   16    64    128    256    512

typedef struct {
	int sum, cnt;
	float avg;

}New_Blemish_ROI;

New_Blemish_ROI Blemish_ROI[128][1024];

typedef struct
{
	bool TEST_LARGEBLEM = false;
	int	ROI_XSIZE = 64;
	int ROI_YSIZE = 48;
	int INNER_SIZE = 60;
	int OUTER_SIZE = 80;
	int EDGE_SIZE = 100;
	int CORNER_SIZE = 5;

} BlemishSize;

BlemishSize Semco_Blemish_Size[6];
SigmaInfo Semco_Blemish_Spec[6];
BlemishResult Semco_Blemish_Result[6];

int LB_No=0;

int leftValue, rightValue;

ofstream fout(".\\Test_Data.txt");

typedef struct {
	int i, j, v;
}SmallDefectData;
SmallDefectData defectData[MaxDefect + 1];
SmallDefectData ArrayMin, ArrayMax, bSize[2], WBData[MaxDefectSize + 5];

void logWr() {
	//ofstream fout(".\\Test_Data.txt");

	fout << "defectCount=" << WBcnt << " , width=" << ArrayMax.j - ArrayMin.j + 1
		<< " , Height=" << ArrayMax.i - ArrayMin.i + 1 << endl;

	for (int i = 0; i < WBcnt; i++) {
		fout << "x=" << WBData[i].j << " , y=" << WBData[i].i << endl;
	}
	fout << endl;

}


void BlemishV04::DisplayOutput() {

	ui->log->clear();
	ui->log->setFontPointSize(9);
	ifstream in(".\\Test_Data.txt");
	ostringstream outStr;
	outStr << in.rdbuf();
	string outContent = outStr.str();
	ui->log->insertPlainText(outContent.c_str());

}


bool BlemishV04::loadParameter() {

	//	QString tmp = ui->in_Size->document()->toPlainText();
	//	in_Size = string((const char *)tmp.toLocal8Bit());
	in_Size = string((const char *)ui->in_Size->document()->toPlainText().toLocal8Bit());
	out_Size = string((const char *)ui->out_Size->document()->toPlainText().toLocal8Bit());

	th[0] = string((const char *)ui->Threshold_A->document()->toPlainText().toLocal8Bit());
	th[1] = string((const char *)ui->Threshold_B->document()->toPlainText().toLocal8Bit());
	th[2] = string((const char *)ui->Threshold_C->document()->toPlainText().toLocal8Bit());
	th[3] = string((const char *)ui->Threshold_D->document()->toPlainText().toLocal8Bit());
	secondThd= string((const char *)ui->SecondThd->document()->toPlainText().toLocal8Bit());

	Allow[0] = ui->inner_Allow->document()->toPlainText().toInt();
	Spec[0] = ui->inner_Spec->document()->toPlainText().toInt();
	Limit[0] = ui->inner_Limit->document()->toPlainText().toInt();
	Array[0] = ui->inner_Array->document()->toPlainText().toInt();

	Allow[1] = ui->outer_Allow->document()->toPlainText().toInt();
	Spec[1] = ui->outer_Spec->document()->toPlainText().toInt();
	Limit[1] = ui->outer_Limit->document()->toPlainText().toInt();
	Array[1] = ui->outer_Array->document()->toPlainText().toInt();

	Allow[3] = Allow[2] = ui->edge_Allow->document()->toPlainText().toInt();
	Spec[3] = Spec[2] = ui->edge_Spec->document()->toPlainText().toInt();
	Limit[3] = Limit[2] = ui->edge_Limit->document()->toPlainText().toInt();
	Array[3] = Array[2] = ui->edge_Array->document()->toPlainText().toInt();

	Region_A = ui->Region_A->document()->toPlainText().toInt();
	Region_B = ui->Region_B->document()->toPlainText().toInt();
	Region_C = ui->Region_C->document()->toPlainText().toInt();

	bSize[0].j = ui->Size1_Width->document()->toPlainText().toInt();
	bSize[0].i = ui->Size1_Height->document()->toPlainText().toInt();
	bSize[1].j = ui->Size2_Width->document()->toPlainText().toInt();
	bSize[1].i = ui->Size2_Height->document()->toPlainText().toInt();

	bth[0][0] = string((const char *)ui->Size1_A_Spec->document()->toPlainText().toLocal8Bit());
	bth[0][1] = string((const char *)ui->Size1_B_Spec->document()->toPlainText().toLocal8Bit());
	bth[0][2] = string((const char *)ui->Size1_C_Spec->document()->toPlainText().toLocal8Bit());
	bth[0][3] = string((const char *)ui->Size1_D_Spec->document()->toPlainText().toLocal8Bit());

	bth[1][0] = string((const char *)ui->Size2_A_Spec->document()->toPlainText().toLocal8Bit());
	bth[1][1] = string((const char *)ui->Size2_B_Spec->document()->toPlainText().toLocal8Bit());
	bth[1][2] = string((const char *)ui->Size2_C_Spec->document()->toPlainText().toLocal8Bit());
	bth[1][3] = string((const char *)ui->Size2_D_Spec->document()->toPlainText().toLocal8Bit());

	LB_No= ui->LB->document()->toPlainText().toInt();

	for (int x = 0; x < 4; x++) {
		float thf = atof(th[x].c_str());
		if (thf <= 0 || thf >= 1)
			return false;
	}
	float infloat = atof(in_Size.c_str());
	if (infloat > 0.9 || infloat < 0.05)
		return false;

	float outfloat = atof(out_Size.c_str());
	if (infloat + outfloat > 1 || outfloat < 0)
		return false;

	return true;
}


LPTSTR lptstr2int(int v) {
	CString ctmp;
	ctmp.Format(_T("%d"), v);
	LPTSTR lptstr_tmp = new TCHAR(ctmp.GetLength() + 1);
	lstrcpy(lptstr_tmp, ctmp);
	return lptstr_tmp;
}

LPTSTR str2lpt(string v) {
	CString ctmp = v.c_str();
	//	ctmp.Format(_T("%f"), v);
	LPTSTR lptstr_tmp = new TCHAR(ctmp.GetLength() + 1);
	lstrcpy(lptstr_tmp, ctmp);
	return lptstr_tmp;
}


void BlemishV04::saveValue_clicked() {

	if (!loadParameter()) {
		QMessageBox msgBox;
		msgBox.setText(tr("Value set not proper"));
		msgBox.exec();
	}
	else {

		/*CString str = ".\\specValue.ini";
		LPTSTR lpPath =new TCHAR(str.GetLength()+1);
		lstrcpy(lpPath, str);
		delete[] lpPath;
		*/

		WritePrivateProfileString(TEXT("RegionSetup"), TEXT("in_Size"), CA2CT(in_Size.c_str()), TEXT(".\\specValue.ini"));
		WritePrivateProfileString(TEXT("RegionSetup"), TEXT("out_Size"), CA2CT(out_Size.c_str()), TEXT(".\\specValue.ini"));

		WritePrivateProfileString(TEXT("Threshold"), TEXT("inner_Threshold"), CA2CT(th[0].c_str()), TEXT(".\\specValue.ini"));
		WritePrivateProfileString(TEXT("Threshold"), TEXT("outer_Threshold"), CA2CT(th[1].c_str()), TEXT(".\\specValue.ini"));
		WritePrivateProfileString(TEXT("Threshold"), TEXT("edge_Threshold"), CA2CT(th[2].c_str()), TEXT(".\\specValue.ini"));
		WritePrivateProfileString(TEXT("Threshold"), TEXT("corner_Threshold"), CA2CT(th[3].c_str()), TEXT(".\\specValue.ini"));

		WritePrivateProfileString(TEXT("Inner"), TEXT("Allow_A"), lptstr2int(Allow[0]), TEXT(".\\specValue.ini"));
		WritePrivateProfileString(TEXT("Inner"), TEXT("Spec_A"), lptstr2int(Spec[0]), TEXT(".\\specValue.ini"));
		WritePrivateProfileString(TEXT("Inner"), TEXT("Limit_A"), lptstr2int(Limit[0]), TEXT(".\\specValue.ini"));
		WritePrivateProfileString(TEXT("Inner"), TEXT("Array_A"), lptstr2int(Array[0]), TEXT(".\\specValue.ini"));

		WritePrivateProfileString(TEXT("Outer"), TEXT("Allow_B"), lptstr2int(Allow[1]), TEXT(".\\specValue.ini"));
		WritePrivateProfileString(TEXT("Outer"), TEXT("Spec_B"), lptstr2int(Spec[1]), TEXT(".\\specValue.ini"));
		WritePrivateProfileString(TEXT("Outer"), TEXT("Limit_B"), lptstr2int(Limit[1]), TEXT(".\\specValue.ini"));
		WritePrivateProfileString(TEXT("Outer"), TEXT("Array_B"), lptstr2int(Array[1]), TEXT(".\\specValue.ini"));

		WritePrivateProfileString(TEXT("Edge"), TEXT("Allow_C"), lptstr2int(Allow[2]), TEXT(".\\specValue.ini"));
		WritePrivateProfileString(TEXT("Edge"), TEXT("Spec_C"), lptstr2int(Spec[2]), TEXT(".\\specValue.ini"));
		WritePrivateProfileString(TEXT("Edge"), TEXT("Limit_C"), lptstr2int(Limit[2]), TEXT(".\\specValue.ini"));
		WritePrivateProfileString(TEXT("Edge"), TEXT("Array_C"), lptstr2int(Array[2]), TEXT(".\\specValue.ini"));

		//blemish value set
		WritePrivateProfileString(TEXT("Blemish_Region"), TEXT("Region_A"), lptstr2int(Region_A), TEXT(".\\specValue.ini"));
		WritePrivateProfileString(TEXT("Blemish_Region"), TEXT("Region_B"), lptstr2int(Region_B), TEXT(".\\specValue.ini"));
		WritePrivateProfileString(TEXT("Blemish_Region"), TEXT("Region_C"), lptstr2int(Region_C), TEXT(".\\specValue.ini"));
		//Size1 Spec
		WritePrivateProfileString(TEXT("Blemish_Size1"), TEXT("Size1_Width"), lptstr2int(bSize[0].j), TEXT(".\\specValue.ini"));
		WritePrivateProfileString(TEXT("Blemish_Size1"), TEXT("Size1_Height"), lptstr2int(bSize[0].i), TEXT(".\\specValue.ini"));

		WritePrivateProfileString(TEXT("Blemish_Size1"), TEXT("Size1_SpecA"), CA2CT(bth[0][0].c_str()), TEXT(".\\specValue.ini"));
		WritePrivateProfileString(TEXT("Blemish_Size1"), TEXT("Size1_SpecB"), CA2CT(bth[0][1].c_str()), TEXT(".\\specValue.ini"));
		WritePrivateProfileString(TEXT("Blemish_Size1"), TEXT("Size1_SpecC"), CA2CT(bth[0][2].c_str()), TEXT(".\\specValue.ini"));
		WritePrivateProfileString(TEXT("Blemish_Size1"), TEXT("Size1_SpecD"), CA2CT(bth[0][3].c_str()), TEXT(".\\specValue.ini"));

		//Size2 Spec
		WritePrivateProfileString(TEXT("Blemish_Size2"), TEXT("Size2_Width"), lptstr2int(bSize[1].j), TEXT(".\\specValue.ini"));
		WritePrivateProfileString(TEXT("Blemish_Size2"), TEXT("Size2_Height"), lptstr2int(bSize[1].i), TEXT(".\\specValue.ini"));

		WritePrivateProfileString(TEXT("Blemish_Size2"), TEXT("Size2_SpecA"), CA2CT(bth[1][0].c_str()), TEXT(".\\specValue.ini"));
		WritePrivateProfileString(TEXT("Blemish_Size2"), TEXT("Size2_SpecB"), CA2CT(bth[1][1].c_str()), TEXT(".\\specValue.ini"));
		WritePrivateProfileString(TEXT("Blemish_Size2"), TEXT("Size2_SpecC"), CA2CT(bth[1][2].c_str()), TEXT(".\\specValue.ini"));
		WritePrivateProfileString(TEXT("Blemish_Size2"), TEXT("Size2_SpecD"), CA2CT(bth[1][3].c_str()), TEXT(".\\specValue.ini"));

		WritePrivateProfileString(TEXT("Threshold"), TEXT("secondth"), CA2CT(secondThd.c_str()), TEXT(".\\specValue.ini"));
	}
}


void BlemishV04::readWBValue() {

	TCHAR lpTexts[9];
	GetPrivateProfileString(TEXT("RegionSetup"), TEXT("in_Size"), TEXT("0.333"), lpTexts, 8, TEXT(".\\specValue.ini"));
	in_Size = CT2A(lpTexts);
	ui->in_Size->setText(QString::fromStdString(in_Size));

	GetPrivateProfileString(TEXT("RegionSetup"), TEXT("out_Size"), TEXT("0.333"), lpTexts, 8, TEXT(".\\specValue.ini"));
	out_Size = CT2A(lpTexts);
	ui->out_Size->setText(QString::fromStdString(out_Size));

	GetPrivateProfileString(TEXT("Threshold"), TEXT("inner_Threshold"), TEXT("0.19"), lpTexts, 8, TEXT(".\\specValue.ini"));
	th[0] = CT2A(lpTexts);
	ui->Threshold_A->setText(QString::fromStdString(th[0]));

	GetPrivateProfileString(TEXT("Threshold"), TEXT("outer_Threshold"), TEXT("0.19"), lpTexts, 8, TEXT(".\\specValue.ini"));
	th[1] = CT2A(lpTexts);
	ui->Threshold_B->setText(QString::fromStdString(th[1]));

	GetPrivateProfileString(TEXT("Threshold"), TEXT("edge_Threshold"), TEXT("0.19"), lpTexts, 8, TEXT(".\\specValue.ini"));
	th[2] = CT2A(lpTexts);
	ui->Threshold_C->setText(QString::fromStdString(th[2]));

	GetPrivateProfileString(TEXT("Threshold"), TEXT("corner_Threshold"), TEXT("0.24"), lpTexts, 8, TEXT(".\\specValue.ini"));
	th[3] = CT2A(lpTexts);
	ui->Threshold_D->setText(QString::fromStdString(th[3]));

	GetPrivateProfileString(TEXT("Threshold"), TEXT("secondth"), TEXT("1.000"), lpTexts, 8, TEXT(".\\specValue.ini"));
	secondThd = CT2A(lpTexts);
	ui->SecondThd->setText(QString::fromStdString(secondThd));

	Allow[0] = GetPrivateProfileInt(_T("Inner"), TEXT("Allow_A"), 2, TEXT(".\\specValue.ini"));
	ui->inner_Allow->setText(QString::number(Allow[0], 10));
	Allow[1] = GetPrivateProfileInt(_T("Outer"), TEXT("Allow_B"), 4, TEXT(".\\specValue.ini"));
	ui->outer_Allow->setText(QString::number(Allow[1], 10));
	Allow[3]=Allow[2] = GetPrivateProfileInt(_T("Edge"), TEXT("Allow_C"), 4, TEXT(".\\specValue.ini"));
	ui->edge_Allow->setText(QString::number(Allow[2], 10));

	Spec[0] = GetPrivateProfileInt(_T("Inner"), TEXT("Spec_A"), 5, TEXT(".\\specValue.ini"));
	ui->inner_Spec->setText(QString::number(Spec[0], 10));
	Spec[1] = GetPrivateProfileInt(_T("Outer"), TEXT("Spec_B"), 5, TEXT(".\\specValue.ini"));
	ui->outer_Spec->setText(QString::number(Spec[1], 10));
	Spec[3] = Spec[2] = GetPrivateProfileInt(_T("Edge"), TEXT("Spec_C"), 5, TEXT(".\\specValue.ini"));
	ui->edge_Spec->setText(QString::number(Spec[2], 10));

	Limit[0] = GetPrivateProfileInt(_T("Inner"), TEXT("Limit_A"), 1, TEXT(".\\specValue.ini"));
	ui->inner_Limit->setText(QString::number(Limit[0], 10));
	Limit[1] = GetPrivateProfileInt(_T("Outer"), TEXT("Limit_B"), 1, TEXT(".\\specValue.ini"));
	ui->outer_Limit->setText(QString::number(Limit[1], 10));
	Limit[3]=Limit[2] = GetPrivateProfileInt(_T("Edge"), TEXT("Limit_C"), 1, TEXT(".\\specValue.ini"));
	ui->edge_Limit->setText(QString::number(Limit[2], 10));

	Array[0] = GetPrivateProfileInt(_T("Inner"), TEXT("Array_A"), 2, TEXT(".\\specValue.ini"));
	ui->inner_Array->setText(QString::number(Array[0], 10));
	Array[1] = GetPrivateProfileInt(_T("Outer"), TEXT("Array_B"), 2, TEXT(".\\specValue.ini"));
	ui->outer_Array->setText(QString::number(Array[1], 10));
	Array[3]=Array[2] = GetPrivateProfileInt(_T("Edge"), TEXT("Array_C"), 2, TEXT(".\\specValue.ini"));
	ui->edge_Array->setText(QString::number(Array[2], 10));
}


void BlemishV04::readBlemishValue() {

	TCHAR lpTexts[9];
	//Blemish value load
	GetPrivateProfileString(TEXT("Blemish_Size1"), TEXT("Size1_SpecA"), TEXT("0.005"), lpTexts, 8, TEXT(".\\specValue.ini"));
	bth[0][0] = CT2A(lpTexts);
	ui->Size1_A_Spec->setText(QString::fromStdString(bth[0][0]));

	GetPrivateProfileString(TEXT("Blemish_Size1"), TEXT("Size1_SpecB"), TEXT("0.006"), lpTexts, 8, TEXT(".\\specValue.ini"));
	bth[0][1] = CT2A(lpTexts);
	ui->Size1_B_Spec->setText(QString::fromStdString(bth[0][1]));

	GetPrivateProfileString(TEXT("Blemish_Size1"), TEXT("Size1_SpecC"), TEXT("0.006"), lpTexts, 8, TEXT(".\\specValue.ini"));
	bth[0][2] = CT2A(lpTexts);
	ui->Size1_C_Spec->setText(QString::fromStdString(bth[0][2]));

	GetPrivateProfileString(TEXT("Blemish_Size1"), TEXT("Size1_SpecD"), TEXT("0.15"), lpTexts, 8, TEXT(".\\specValue.ini"));
	bth[0][3] = CT2A(lpTexts);
	ui->Size1_D_Spec->setText(QString::fromStdString(bth[0][3]));

	GetPrivateProfileString(TEXT("Blemish_Size2"), TEXT("Size2_SpecA"), TEXT("1.000"), lpTexts, 8, TEXT(".\\specValue.ini"));
	bth[1][0] = CT2A(lpTexts);
	ui->Size2_A_Spec->setText(QString::fromStdString(bth[1][0]));

	GetPrivateProfileString(TEXT("Blemish_Size2"), TEXT("Size2_SpecB"), TEXT("1.000"), lpTexts, 8, TEXT(".\\specValue.ini"));
	bth[1][1] = CT2A(lpTexts);
	ui->Size2_B_Spec->setText(QString::fromStdString(bth[1][1]));

	GetPrivateProfileString(TEXT("Blemish_Size2"), TEXT("Size2_SpecC"), TEXT("0.009"), lpTexts, 8, TEXT(".\\specValue.ini"));
	bth[1][2] = CT2A(lpTexts);
	ui->Size2_C_Spec->setText(QString::fromStdString(bth[1][2]));

	GetPrivateProfileString(TEXT("Blemish_Size2"), TEXT("Size2_SpecD"), TEXT("0.15"), lpTexts, 8, TEXT(".\\specValue.ini"));
	bth[1][3] = CT2A(lpTexts);
	ui->Size2_D_Spec->setText(QString::fromStdString(bth[1][3]));


	Region_A = GetPrivateProfileInt(_T("Blemish_Region"), TEXT("Region_A"), 60, TEXT(".\\specValue.ini"));
	ui->Region_A->setText(QString::number(Region_A, 10));

	Region_B = GetPrivateProfileInt(_T("Blemish_Region"), TEXT("Region_B"), 80, TEXT(".\\specValue.ini"));
	ui->Region_B->setText(QString::number(Region_B, 10));

	Region_C = GetPrivateProfileInt(_T("Blemish_Region"), TEXT("Region_C"), 100, TEXT(".\\specValue.ini"));
	ui->Region_C->setText(QString::number(Region_C, 10));

	bSize[0].j = GetPrivateProfileInt(_T("Blemish_Size1"), TEXT("Size1_Width"), 48, TEXT(".\\specValue.ini"));
	bSize[0].i = GetPrivateProfileInt(_T("Blemish_Size1"), TEXT("Size1_Height"), 36, TEXT(".\\specValue.ini"));
	bSize[1].j = GetPrivateProfileInt(_T("Blemish_Size2"), TEXT("Size2_Width"), 24, TEXT(".\\specValue.ini"));
	bSize[1].i = GetPrivateProfileInt(_T("Blemish_Size2"), TEXT("Size2_Height"), 21, TEXT(".\\specValue.ini"));



}


void Load_Sensor_Blemish() {

	TCHAR lpTexts[9];

	for (int i = 0; i < 6; i++) {
		CString keyName = "LARGEBLEMISH";
		if (i != 0) {
			keyName += '_';
			CString n;
			n.Format(_T("%d"),i);
			keyName += n;
		}    
		LPCTSTR lpt = (LPCTSTR)keyName;

		Semco_Blemish_Size[i].ROI_XSIZE = GetPrivateProfileInt(lpt, TEXT("ROI_XSIZE"), 64, TEXT(".\\Sensor.ini"));
		Semco_Blemish_Size[i].ROI_YSIZE = GetPrivateProfileInt(lpt, TEXT("ROI_YSIZE"), 48, TEXT(".\\Sensor.ini"));
		Semco_Blemish_Size[i].INNER_SIZE = GetPrivateProfileInt(lpt, TEXT("INNER_SIZE"), 48, TEXT(".\\Sensor.ini"));
		Semco_Blemish_Size[i].OUTER_SIZE = GetPrivateProfileInt(lpt, TEXT("OUTER_SIZE"), 48, TEXT(".\\Sensor.ini"));
		Semco_Blemish_Size[i].EDGE_SIZE = GetPrivateProfileInt(lpt, TEXT("EDGE_SIZE"), 48, TEXT(".\\Sensor.ini"));

		GetPrivateProfileString(lpt, TEXT("TEST_LARGEBLEM"), TEXT("false"), lpTexts, 8, TEXT(".\\Sensor.ini"));
		if (lpTexts[0] == 't' || lpTexts[0] == 'T')
			Semco_Blemish_Size[i].TEST_LARGEBLEM = true;
		else
			Semco_Blemish_Size[i].TEST_LARGEBLEM = false;

		GetPrivateProfileString(lpt, TEXT("THRESHOLD_INNER"), TEXT("0.005"), lpTexts, 8, TEXT(".\\Sensor.ini"));
		Semco_Blemish_Spec[i].sigma_inner = atof(CT2A(lpTexts));
		GetPrivateProfileString(lpt, TEXT("THRESHOLD_OUTER"), TEXT("0.01"), lpTexts, 8, TEXT(".\\Sensor.ini"));
		Semco_Blemish_Spec[i].sigma_outer = atof(CT2A(lpTexts));
		GetPrivateProfileString(lpt, TEXT("THRESHOLD_EDGE"), TEXT("0.01"), lpTexts, 8, TEXT(".\\Sensor.ini"));
		Semco_Blemish_Spec[i].sigma_edge = atof(CT2A(lpTexts));
		GetPrivateProfileString(lpt, TEXT("THRESHOLD_CORNER"), TEXT("0.22"), lpTexts, 8, TEXT(".\\Sensor.ini"));
		Semco_Blemish_Spec[i].sigma_corner = atof(CT2A(lpTexts));

		GetPrivateProfileString(lpt, TEXT("THRESHOLD_EDGE_LT"), TEXT("0.5"), lpTexts, 8, TEXT(".\\Sensor.ini"));
		Semco_Blemish_Spec[i].sigma_edgeLT = atof(CT2A(lpTexts));
		GetPrivateProfileString(lpt, TEXT("THRESHOLD_EDGE_RT"), TEXT("0.5"), lpTexts, 8, TEXT(".\\Sensor.ini"));
		Semco_Blemish_Spec[i].sigma_edgeRT = atof(CT2A(lpTexts));
		GetPrivateProfileString(lpt, TEXT("THRESHOLD_EDGE_LB"), TEXT("0.5"), lpTexts, 8, TEXT(".\\Sensor.ini"));
		Semco_Blemish_Spec[i].sigma_edgeLB = atof(CT2A(lpTexts));
		GetPrivateProfileString(lpt, TEXT("THRESHOLD_EDGE_RB"), TEXT("0.5"), lpTexts, 8, TEXT(".\\Sensor.ini"));
		Semco_Blemish_Spec[i].sigma_edgeRB = atof(CT2A(lpTexts));

		GetPrivateProfileString(lpt, TEXT("THRESHOLD_CORNER_LT"), TEXT("0.5"), lpTexts, 8, TEXT(".\\Sensor.ini"));
		Semco_Blemish_Spec[i].sigma_cornerLT = atof(CT2A(lpTexts));
		GetPrivateProfileString(lpt, TEXT("THRESHOLD_CORNER_RT"), TEXT("0.5"), lpTexts, 8, TEXT(".\\Sensor.ini"));
		Semco_Blemish_Spec[i].sigma_cornerRT = atof(CT2A(lpTexts));
		GetPrivateProfileString(lpt, TEXT("THRESHOLD_CORNER_LB"), TEXT("0.5"), lpTexts, 8, TEXT(".\\Sensor.ini"));
		Semco_Blemish_Spec[i].sigma_cornerLB = atof(CT2A(lpTexts));
		GetPrivateProfileString(lpt, TEXT("THRESHOLD_CORNER_RB"), TEXT("0.5"), lpTexts, 8, TEXT(".\\Sensor.ini"));
		Semco_Blemish_Spec[i].sigma_cornerRB = atof(CT2A(lpTexts));

	}

}


BlemishV04::BlemishV04(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::BlemishV04)
{

	ui->setupUi(this);
	connect(ui->saveValue, SIGNAL(clicked()), this, SLOT(saveValue_clicked()));
	connect(ui->saveGray, SIGNAL(clicked()), this, SLOT(saveGrayImage_clicked()));
	connect(ui->BlemishCheck2, SIGNAL(clicked()), this, SLOT(on_pushButton_blemishcheck2_clicked()));
	readWBValue();
	readBlemishValue();
	Load_Sensor_Blemish();
	LB_No = ui->LB->document()->toPlainText().toInt();

}

BlemishV04::~BlemishV04()
{
	fout << flush;
	FreeLibrary(his);
	fout.close();
	delete ui;
}


void BlemishV04::display_Image() {

	cvtColor(image, dst, CV_BGR2RGB);
	ui->textBrowser->setAlignment(Qt::AlignCenter);
	QImage showImage((const uchar*)dst.data, dst.cols, dst.rows, dst.cols*dst.channels(), QImage::Format_RGB888);

	//imgScaled = showImage.scaledToHeight(600, Qt::FastTransformation);
	imgScaled = showImage.scaled(ui->label_show_image->size(), Qt::KeepAspectRatio);
	ui->label_show_image->setPixmap(QPixmap::fromImage(imgScaled));

}


void BlemishV04::on_pushButton_open_image_clicked()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image File(*.bmp *.jpg *.jpeg *.png *.pbm *.pgm *.ppm)"));
	QTextCodec *code = QTextCodec::codecForName("gb18030");
	std::string name = code->fromUnicode(filename).data();

	ui->textBrowser->setTextColor(QColor(0, 0, 0, 255));
	ui->textBrowser->setFontPointSize(48);
	ui->textBrowser->setText("NA");
	ui->textBrowser->setAlignment(Qt::AlignCenter);

	temp_image = cv::imread(name);
	if (!temp_image.data)
	{
		QMessageBox msgBox;
		msgBox.setText(tr("image data is null"));
		msgBox.exec();
	}
	else
	{
		image = temp_image.clone();
		NG = false;
		//	defectCnt = 0;
		knownDefectCnt = 0;
		imageCopy = image.clone();
		cvtColor(image, gray_image, CV_BGR2GRAY);

		Width = image.cols;
		Height = image.rows;

		rectSize = image.cols / 1000 * 10 + 10;
		lineThickness = image.cols / 1000 * 2 + 2;
		
		ui->label_show_image->clear();
		ui->pushButton_image_processing->setEnabled(true);
		ui->pushButton_image_processing_2->setEnabled(true);
		ui->BlemishCheck2->setEnabled(true);
		ui->saveGray->setEnabled(true);
		ui->pushButton_HQ_Blemish_2->setEnabled(true);
		ui->pushButton_OC->setEnabled(true);
		ui->pushButton_circle_Detect->setEnabled(true);
		ui->pushButton_HQ_Blemish_3->setEnabled(true);

		display_Image();
	}
}


float getMatSum(const Mat &img , int mode) {

	int sum = 0,c=0;
	for (int i = 0; i < img.rows; i++) {
		const uchar* inData = img.ptr<uchar>(i);
		for (int j = 0; j < img.cols; j++) {
			sum += inData[j];
			c++;
		}
	}
	return (float)sum/c;
}

void BlemishV04::drawDefectRect(int i, int j, int mode) {

	rectangle(temp_image, Rect((j-1)*bSize[mode].j, (i-1)*bSize[mode].i, bSize[mode].j, bSize[mode].i), Scalar(0, 0, 255), lineThickness, 8, 0);
}


void BlemishV04::blemishBlockCheck(float map[300][300], int area, int j, int i, int mode, int Mx, int My) {

	if (map[i][j] < 1)
		return;

	nowTh = atof(bth[mode][area].c_str());
	float ref;

	bool ok = true;
	bool avgMode = ui->blemishMode->isChecked();

	int corner = 0;

	if (i == 0 || map[i - 1][j] < 1)
		corner++;
	if (j == 0 || map[i ][j-1] < 1)
		corner++;
	if (i == My - 1 || map[i + 1][j] <1)
		corner++;
	if (j == Mx - 1 || map[i][j + 1] <1)
		corner++;

	if (corner > 1) {
		ref = map[i][j] * (1 + nowTh*1.5);
	}
	else {
		ref = map[i][j] * (1 + nowTh);
	}


	if (avgMode) {
		float sum = 0;
		int c = 0;
		if (i > 0 && map[i - 1][j] > 1) {
			sum += map[i - 1][j];
			c++;
		}
	
		if (j > 0 && map[i][j - 1] > 1) {
			sum += map[i][j - 1];
			c++;
		}
	
		if (i < My - 1 && map[i + 1][j]  >1) {
			sum += map[i + 1][j];
			c++;
		}
	
		if (j < Mx - 1 && map[i][j + 1]  >1) {
			sum += map[i][j + 1];
			c++;
		}

		if (i > 0 && j > 0 && map[i - 1][j - 1] > 1) {
			sum += map[i - 1][j - 1];
			c++;
		}
		if (i > 0 && j < Mx - 1 && map[i - 1][j + 1] > 1) {
			sum += map[i - 1][j + 1];
			c++;
		}
		if (i < My - 1 && j > 0 && map[i + 1][j - 1] > 1) {
			sum += map[i + 1][j - 1];
			c++;
		}
		if (i < My - 1 && j < Mx - 1 && map[i + 1][j + 1] > 1) {
			sum += map[i + 1][j + 1];
			c++;
		}
		if (sum / c>ref)
			ok = false;

	}
	else {

		if (i > 0 && map[i - 1][j] > 1 && map[i - 1][j] > ref) {
			ok = false;
		}
		if (j > 0 && map[i][j - 1] > 1 && map[i][j - 1] > ref) {
			ok = false;
		}
		if (i < My - 1 && map[i + 1][j]  >1 && map[i + 1][j] > ref) {
			ok = false;
		}
		if (j < Mx - 1 && map[i][j + 1]  >1 && map[i][j + 1] > ref) {
			ok = false;
		}
		if (i > 0 && j > 0 && map[i - 1][j - 1] > 1 && map[i - 1][j - 1] > ref) {
			ok = false;
		}
		if (i > 0 && j < Mx - 1 && map[i - 1][j + 1] > 1 && map[i - 1][j + 1] > ref) {
			ok = false;
		}
		if (i < My - 1 && j > 0 && map[i + 1][j - 1] > 1 && map[i + 1][j - 1] > ref) {
			ok = false;
		}
		if (i < My - 1 && j < Mx - 1 && map[i + 1][j + 1] > 1 && map[i + 1][j + 1] > ref) {
			ok = false;
		}

	}
	if (!ok) {
		drawDefectRect(i, j, mode);
		fout << "x=" << j << " , y=" << i<< endl;
		NG = true;
	}
}

float getFloatSqrt(int x) {
	float a = x;
	return sqrtf(a/100);
}

void BlemishV04::blemishModeCheck(int mode) {

	int remainderX = image.cols % bSize[mode].j;
	int remainderY = image.rows % bSize[mode].i;

	int remainderHalfX = remainderX / 2;
	int remainderHalfY = remainderY / 2;

	int blockX = image.cols / bSize[mode].j;
	int blockY = image.rows / bSize[mode].i;
	float blcokSum = blockX*blockY;

	int oddX = blockX % 2;
	int blocksize = bSize[mode].i*bSize[mode].j;

	// X 坐标和范围
	float RA = getFloatSqrt(Region_A);
	float RB = getFloatSqrt(Region_B);
	float RC = getFloatSqrt(Region_C);

	int rangeX1 = blockX* RA;
	int rangeX2 = blockX* RB;
	int rangeX3 = blockX* RC;

	if (rangeX1 % 2 != oddX)
		rangeX1 -= 1;
	if (rangeX2 % 2 != oddX)
		rangeX2 -= 1;
	if (rangeX3 % 2 != oddX)
		rangeX3 -= 1;

	rangeX1 /= 2;
	rangeX2 /= 2;
	rangeX3 /= 2;

	int RX3 = blockX / 2 - rangeX3 + 1;
	int RX2 = blockX / 2 - rangeX2 + 1;
	int RX1 = blockX / 2 - rangeX1 + 1;

	// Y 坐标和范围
	int oddY = blockY % 2;

	int rangeY1 = blockY* RA;
	int rangeY2 = blockY* RB;
	int rangeY3 = blockY* RC;

	if (rangeY1 % 2 != oddY)
		rangeY1 -= 1;
	if (rangeY2 % 2 != oddY)
		rangeY2 -= 1;
	if (rangeY3 % 2 != oddY)
		rangeY3 -= 1;

	rangeY1 /= 2;
	rangeY2 /= 2;
	rangeY3 /= 2;

	int RY3 = blockY / 2 - rangeY3 + 1;
	int RY2 = blockY / 2 - rangeY2 + 1;
	int RY1 = blockY / 2 - rangeY1 + 1;


	Mat imageROI = gray_image(Rect(remainderHalfX, remainderHalfY, gray_image.cols - remainderX, gray_image.rows - remainderY));
	temp_image = image(Rect(remainderHalfX, remainderHalfY, gray_image.cols - remainderX, gray_image.rows - remainderY));

	float map[300][300] = { 0 };
	for (int i = 0; i < blockY; i++) {
		for (int j = 0; j < blockX; j++) {
			Mat temp = imageROI(Rect(bSize[mode].j*j, bSize[mode].i*i, bSize[mode].j, bSize[mode].i));

			//	map[i + 1][j + 1] = (float)getMatSum(temp, mode) / blocksize;
			map[i + 1][j + 1] = getMatSum(temp, mode);

		}
	}

	int rightEdge = remainderHalfX + remainderX % 2;
	int bottomEdge = remainderHalfY + remainderY % 2;

	if (remainderHalfY != 0)
		for (int j = 0; j < blockX; j++) {
			Mat temp = gray_image(Rect(bSize[mode].j*j + remainderHalfX, 0, bSize[mode].j, remainderHalfY));
			//	map[0][j + 1] = (float)getMatSum(temp, mode) / (bSize[mode].j*remainderHalfY);
			map[0][j + 1] = getMatSum(temp, mode);
		}
	if (bottomEdge != 0)
		for (int j = 0; j < blockX; j++) {
			Mat temp = gray_image(Rect(bSize[mode].j*j + remainderHalfX, gray_image.rows - bottomEdge, bSize[mode].j, bottomEdge));
			//	map[blockY + 1][j + 1] = (float)getMatSum(temp, mode) /( bSize[mode].j*bottomEdge);
			map[blockY + 1][j + 1] = getMatSum(temp, mode);
		}
	if (remainderHalfX != 0)
		for (int i = 0; i < blockY; i++) {
			Mat temp = gray_image(Rect(0, bSize[mode].i*i + remainderHalfY, remainderHalfX, bSize[mode].i));
			//	map[i + 1][0] = (float)getMatSum(temp, mode) / (remainderHalfX*bSize[mode].i);
			map[i + 1][0] = getMatSum(temp, mode);
		}
	if (rightEdge != 0)
		for (int i = 0; i < blockY; i++) {
			Mat temp = gray_image(Rect(gray_image.cols - rightEdge, bSize[mode].i*i + remainderHalfY, rightEdge, bSize[mode].i));
			//	map[i + 1][blockX + 1] = (float)getMatSum(temp, mode) / (rightEdge*bSize[mode].i);
			map[i + 1][blockX + 1] = getMatSum(temp, mode);
		}

	// 四角
	if (remainderHalfX != 0 && remainderHalfY != 0) {
		Mat temp = gray_image(Rect(0, 0, remainderHalfX, remainderHalfY));
		//	map[0][0] = (float)getMatSum(temp, mode) / (remainderHalfX*remainderHalfY);
		map[0][0] = getMatSum(temp, mode);
	}
	if (remainderHalfX != 0 && bottomEdge != 0) {
		Mat	temp = gray_image(Rect(0, gray_image.rows - bottomEdge, remainderHalfX, bottomEdge));
		//	map[blockY + 1][0] = (float)getMatSum(temp, mode) / (remainderHalfX*bottomEdge);
		map[blockY + 1][0] = getMatSum(temp, mode);
	}

	if (rightEdge != 0 && remainderHalfY != 0) {
		Mat temp = gray_image(Rect(gray_image.cols - rightEdge, 0, rightEdge, remainderHalfY));
		//	map[0][blockX + 1] = (float)getMatSum(temp, mode) / (rightEdge*remainderHalfY);
		map[0][blockX + 1] = getMatSum(temp, mode);
	}

	if (rightEdge != 0 && bottomEdge != 0) {
		Mat temp = gray_image(Rect(gray_image.cols - rightEdge, gray_image.rows - bottomEdge, rightEdge, bottomEdge));
		//	map[blockY + 1][blockX + 1] = (float)getMatSum(temp, mode) / (rightEdge*bottomEdge);
		map[blockY + 1][blockX + 1] = getMatSum(temp, mode);
	}

	int area = 0;
	int My = blockY + 2;
	int Mx = blockX + 2;
	if (map[0][1] < 1)
		RY3++;
	if (map[1][0] < 1)
		RX3++;


	for (int i = 0; i < My; i++) {
		for (int j = 0; j < Mx; j++) {
			fout << map[i][j] << "	";
			}
		fout << endl;
		}
	fout << endl;

	for (int i = 0; i < My ; i++) 
		for (int j = 0; j < Mx; j++) 
		if(map[i][j]>=1){

			if ((i < RY3 || i >= My - RY3) || (j < RX3 || j >= Mx - RX3)) {
				area = 3;
			}
			else if ((i < RY2 || i >= My - RY2) || (j < RX2 || j >= Mx - RX2)) {
				area = 2;
			}
			else if ((i < RY1 || i >= My - RY1) || (j < RX1 || j >= Mx - RX1)) {
				area = 1;
			}
			else {
				area = 0;
			}
			blemishBlockCheck(map, area, j, i, mode,Mx,My);

		}
}


void BlemishV04::displayResult() {

	ui->textBrowser->setFontPointSize(48);

	if (NG) {
		ui->textBrowser->setTextColor(QColor(255, 0, 0, 255));
		ui->textBrowser->setText("NG");
	}
	else {
		ui->textBrowser->setTextColor(QColor(0, 255, 0, 255));
		ui->textBrowser->setText("OK");
	}
	ui->textBrowser->setAlignment(Qt::AlignCenter);

	fout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
}

void BlemishV04::on_pushButton_image_processing_clicked()
{
	image = imageCopy.clone();
	cvtColor(image, gray_image, CV_BGR2GRAY);
	NG = false;

	blemishModeCheck(0);
	displayResult();
	
	Mat dst;
	cvtColor(image, dst, CV_BGR2RGB);
	ui->textBrowser->setAlignment(Qt::AlignCenter);
	QImage showImage((const uchar*)dst.data, dst.cols, dst.rows, dst.cols*dst.channels(), QImage::Format_RGB888);

	//imgScaled = showImage.scaledToHeight(600, Qt::FastTransformation);
	imgScaled = showImage.scaled(ui->label_show_image->size(), Qt::KeepAspectRatio);
	ui->label_show_image->setPixmap(QPixmap::fromImage(imgScaled));
}


void BlemishV04::on_pushButton_blemishcheck2_clicked()
{
	image = imageCopy.clone();
	cvtColor(image, gray_image, CV_BGR2GRAY);
	NG = false;

	blemishModeCheck(1);
	displayResult();

	Mat dst;
	cvtColor(image, dst, CV_BGR2RGB);
	ui->textBrowser->setAlignment(Qt::AlignCenter);
	QImage showImage((const uchar*)dst.data, dst.cols, dst.rows, dst.cols*dst.channels(), QImage::Format_RGB888);

	//imgScaled = showImage.scaledToHeight(600, Qt::FastTransformation);
	imgScaled = showImage.scaled(ui->label_show_image->size(), Qt::KeepAspectRatio);
	ui->label_show_image->setPixmap(QPixmap::fromImage(imgScaled));
}


void BlemishV04::on_saveGrayImage_clicked()
{
	if (imageCopy.data != NULL) {
		cvtColor(imageCopy, gray_image, CV_BGR2GRAY);

		try {
			imwrite("GrayImg.bmp", gray_image);
		}
		catch (runtime_error& ex) {
			QMessageBox msgBox;
			msgBox.setText(tr("Save Image Error"));
			msgBox.exec();
		}
	}
}


void BlemishV04::on_saveDisplay_clicked() {
	if (image.data != NULL) {
	//	cvtColor(imageCopy, gray_image, CV_BGR2GRAY);
		try {
			imwrite("DisplayImg.bmp", image);
		}
		catch (runtime_error& ex) {
			QMessageBox msgBox;
			msgBox.setText(tr("Save Image Error"));
			msgBox.exec();
		}
	}
}


uchar AvgValue(const Mat& image, int x, int y, int width, int height) {

	int s = width*height;
	unsigned int sum = 0;
	int My = y + height;
	int Mx = x + width;
	for (int k = y; k < My; k++)
	{
		const uchar* inData = image.ptr<uchar>(k);
		for (int i = x; i < Mx; i++)
		{
			sum += inData[i];
		}
	}
	return int(((float)sum) / s + 0.5);
}


void BlemishV04::drawDefectCircle(int i, int j, int b, int g, int r) {

	int m = ArrayMax.j - ArrayMin.j;
	int n = ArrayMax.i - ArrayMin.i;

	if (n > m)
		m = n;

	circle(image, Point(j, i), rectSize + m / 2, Scalar(b, g, r), lineThickness, 8, 0);
	defectData[knownDefectCnt] = { i,j,(rectSize + m) / 2 };
	if (knownDefectCnt < MaxDefect)
		knownDefectCnt++;
}

void check(int i, int j) {
	if (i > ArrayMax.i)
		ArrayMax.i = i;
	if (i < ArrayMin.i)
		ArrayMin.i = i;
	if (j > ArrayMax.j)
		ArrayMax.j = j;
	if (j < ArrayMin.j)
		ArrayMin.j = j;
}


void swop(uchar &a, uchar &b) {
	uchar c = a;
	a = b;
	b = c;
}

bool roundAvg(Mat& img, int x, int y, uchar midData, int area) {

	uchar* up_line = img.ptr<uchar>(y - 1);//指向上一行
	uchar* inData = img.ptr<uchar>(y);
	uchar a, b, c;
	a=b=c= up_line[x - 1];

	if (x > 1 && y > 1) {
		uchar* up2_line = img.ptr<uchar>(y - 2);
		a = up2_line[x - 2];
		b = up2_line[x - 1];
		c = up_line[x - 2];
	}
	else if (x > 1) {
		b = up_line[x - 2];
		c = inData[x-2];
	}
	else if (y > 1) {
		uchar* up2_line = img.ptr<uchar>(y - 2);	
		b = up2_line[x - 1];
		c = up2_line[x];
	}

	if (a > b) {
		swop(a, b);
	}
	if (c > b) {
		avg = b;
	}
	else {
		if (c > a)
			avg = c;
		else 
			avg = a;
	}

	leftValue = avg / (1 + nowTh);
	rightValue = avg*(1 + nowTh);

	if (avg >= inData[x] * (1 + nowTh) || avg <= inData[x] * (1 - nowTh)) {
		//inData[x] = avg;
		return true;
	}
	return false;
}


bool BFS(int i, int j, Mat &img, int area) {

	if (i <= 0 || i >= img.rows - 1 || j <= 0 || j >= img.cols - 1)
		return false;

	//for (int k = 0; k < WBcnt; k++)
	//	if (i == WBData[k].i&&j == WBData[k].j)
	//		return false;

	WBData[WBcnt].i = i;
	WBData[WBcnt++].j = j;

	if (WBcnt ==3) {
		float sth = atof(secondThd.c_str());
		leftValue = avg / (1 + nowTh*sth);
		rightValue = avg*(1 + nowTh*sth);
	}

	if (WBcnt > MaxDefectSize || ArrayMax.i - ArrayMin.i > MaxDefectRange || ArrayMax.j - ArrayMin.j > MaxDefectRange) {
		return true;
	}

	check(i, j);
	uchar* up_line = img.ptr<uchar>(i - 1);//指向上一行
	uchar* mid_line = img.ptr<uchar>(i);//当前行
	uchar* down_line = img.ptr<uchar>(i + 1);//下一行

	mid_line[j] = avg;

	if (mid_line[j - 1] <= leftValue || mid_line[j - 1] >= rightValue) {
		//defectCnt++;
		//if (defectCnt >= defectCntSpec) {
		//	return true;
		//}
		if (BFS(i, j - 1, img, area))
			return true;
	}

	if (mid_line[j + 1] <= leftValue || mid_line[j + 1] >= rightValue) {
		if (BFS(i, j + 1, img, area))
			return true;
	}

	if (up_line[j] <= leftValue || up_line[j] >= rightValue) {
		if (BFS(i - 1, j, img, area))
			return true;
	}

	if (down_line[j] <= leftValue || down_line[j] >= rightValue) {
		if (BFS(i + 1, j, img, area))
			return true;
	}

	if (up_line[j - 1] <= leftValue || up_line[j - 1] >= rightValue) {
		if (BFS(i - 1, j - 1, img, area))
			return true;
	}

	if (up_line[j + 1] <= leftValue || up_line[j + 1] >= rightValue) {
		if (BFS(i - 1, j + 1, img, area))
			return true;
	}

	if (down_line[j - 1] <= leftValue || down_line[j - 1] >= rightValue) {
		if (BFS(i + 1, j - 1, img, area))
			return true;
	}

	if (down_line[j + 1] <= leftValue || down_line[j + 1] >= rightValue) {
		if (BFS(i + 1, j + 1, img, area))
			return true;
	}
	return false;
}


void BlemishV04::WBdotCheck(int x, int y, int Mx, int My, int area) {

	nowTh = atof(th[area].c_str());

	for (int i = y; i < My; i++)
	{
		if (knownDefectCnt >= MaxDefect)
			break;
		uchar* inData = gray_image.ptr<uchar>(i);

		for (int j = x; j < Mx; j++) {
			if (inData[j]< inData[j - 1] - 2 || inData[j] > inData[j - 1] + 2)

				if (knownDefectCnt < MaxDefect&&i>1 && j>1)
				{
					if (roundAvg(gray_image, j, i, inData[j], area)) {

						ArrayMax.i = ArrayMin.i = i;
						ArrayMax.j = ArrayMin.j = j;
						bool knownArea = false;
						//for (int a = 0; a < knownDefectCnt; a++) {
						//	if (i >= defectData[a].i - defectData[a].v  && i <= defectData[a].i + defectData[a].v  && j >= defectData[a].j - defectData[a].v  && j <= defectData[a].j + defectData[a].v)
						//		//	if (i >= defectData[a].i - defectData[a].v * 2 && i <= defectData[a].i + defectData[a].v * 2 && j >= defectData[a].j - defectData[a].v * 2 && j <= defectData[a].j + defectData[a].v * 2)
						//	{
						//		knownArea = true;
						//		break;
						//	}
						//}
						//	if (!knownArea) {
						WBcnt = 0;
						defectCntSpec = Spec[area];
						bool isDefect = BFS(i, j, gray_image, area);
						if (WBcnt >= defectCntSpec|| ArrayMax.i - ArrayMin.i >= Array[area] || ArrayMax.j - ArrayMin.j >= Array[area])
							isDefect = true;
					
						bool use = true;

						if (WBcnt > MaxDefectSize || ArrayMax.i - ArrayMin.i > MaxDefectRange || ArrayMax.j - ArrayMin.j > MaxDefectRange)
							use = false;

						if (use) {

							for (int a = 0; a < knownDefectCnt; a++) 
								if (i >= defectData[a].i - defectData[a].v  && i <= defectData[a].i + defectData[a].v  && j >= defectData[a].j - defectData[a].v  && j <= defectData[a].j + defectData[a].v)
								{
									knownArea = true;
									break;
								}
							
							if (!knownArea) {

								int ci = (ArrayMax.i + ArrayMin.i) / 2;
								int cj = (ArrayMax.j + ArrayMin.j) / 2;
								if (isDefect){			
										bigCnt[area]++;
										drawDefectCircle(ci, cj, 0, 0, 255);
										logWr();
							
									}
									else {
										drawDefectCircle(i, j, 255, 0, 0);
										smallCnt[area]++;
										logWr();
									}
								}
						}

						if (smallCnt[area] >= Allow[area] || bigCnt[area] >= Limit[area]) {
							NG = true;
						}
						//	}
					}
				}
		}
	}
	DisplayOutput();
}


void BlemishV04::on_pushButton_image_processing_2_clicked() {

	image = imageCopy.clone();
	bool greenmode = ui->greenMode->isChecked();

	if (greenmode) {
		vector<Mat> channels;
		split(image, channels);
		gray_image = channels.at(1);
	}
	else {
		cvtColor(image, gray_image, CV_BGR2GRAY);
	}
	float inSize = atof(in_Size.c_str());
	float outSize = atof(out_Size.c_str());

	int Rx = image.cols / 2;
	int Ry = image.rows / 2;

	int RangeX1 = Rx *inSize;
	int RangeY1 = Ry *inSize;

	int RangeX2 = Rx *(inSize + outSize);
	int RangeY2 = Ry *(inSize + outSize);

	WBdotCheck(1, 1, RangeX1, RangeY1, 3);    //D
	WBdotCheck(RangeX1, 1, image.cols - RangeX1, RangeY1, 2);		//C
	WBdotCheck(image.cols - RangeX1, 1, image.cols, RangeY1, 3);	 //D
	WBdotCheck(1, RangeY1, RangeX1, image.rows - RangeY1, 2);		//C
	WBdotCheck(RangeX1, RangeY1, image.cols - RangeX1, RangeY2, 1);	 //B
	WBdotCheck(RangeX1, RangeY2, RangeX2, image.rows - RangeY2, 1);	 //B
	WBdotCheck(RangeX2, RangeY2, image.cols - RangeX2, image.rows - RangeY2, 0);	 //A
	WBdotCheck(image.cols - RangeX2, RangeY2, image.cols - RangeX1, image.rows - RangeY2, 1);	 //B
	WBdotCheck(RangeX1, image.rows - RangeY2, image.cols - RangeX1, image.rows - RangeY1, 1);	 //B
	WBdotCheck(image.cols - RangeX1, RangeY1, image.cols, image.rows - RangeY1, 2);		//C
	WBdotCheck(1, image.rows - RangeY1, RangeX1, image.rows, 3);	 //D
	WBdotCheck(RangeX1, image.rows - RangeY1, image.cols - RangeX1, image.rows, 2);		//C
	WBdotCheck(image.cols - RangeX1, image.rows - RangeY1, image.cols, image.rows, 3);    //D

	displayResult();

	Mat dst;
	cvtColor(image, dst, CV_BGR2RGB);
	ui->textBrowser->setAlignment(Qt::AlignCenter);
	QImage showImage((const uchar*)dst.data, dst.cols, dst.rows, dst.cols*dst.channels(), QImage::Format_RGB888);

	//imgScaled = showImage.scaledToHeight(600, Qt::FastTransformation);
	imgScaled = showImage.scaled(ui->label_show_image->size(), Qt::KeepAspectRatio);
	ui->label_show_image->setPixmap(QPixmap::fromImage(imgScaled));
	
}


void BlemishV04::on_pushButton_OC_clicked() {

	cvtColor(imageCopy, gray_image, CV_BGR2GRAY);
	Point2i Center = OC(gray_image);

	string strx = to_string(Center.x);
	string stry = to_string(Center.y);
	strx = "x= " + strx + "\n";
	stry = "y= " + stry + "\n";
	ui->log->insertPlainText(strx.c_str());
	ui->log->insertPlainText(stry.c_str());

	strx = to_string(Center.x-gray_image.cols/2);
	stry = to_string(Center.y-gray_image.rows/2);
	strx = "OC_X= " + strx + "\n";
	stry = "OC_Y= " + stry + "\n";
	ui->log->insertPlainText(strx.c_str());
	ui->log->insertPlainText(stry.c_str());
	ui->log->insertPlainText("~~~~~~~~~~~~~~~~~~~~\n");

}

void BlemishV04::on_pushButton_clear_clicked() {

	ui->log->clear();

}


void BlemishV04::on_pushButton_BlemishCheckNew_clicked() {

//	nowTh = atof(bth[0][0].c_str());
	nowTh = 0.009;

	cvtColor(imageCopy, gray_image, CV_BGR2GRAY);
	NG = false;
//	Point2i Center = OC(gray_image);
	Point2i Center = Circle_Detect(gray_image,0.75);
	
	int cx = Center.x;
	int cy = Center.y;
	float dis;

	//  1 2-3  4-7  8-15  16-31  32-63  64-128
	//  4   4   16    64    128    256    512
	//    0    1     2    4      8     16

	memset(Blemish_ROI, 0, sizeof(Blemish_ROI));
	int max_dis = 0;
	for (int i = 0; i < gray_image.rows; i++)
	{
		uchar* inData = gray_image.ptr<uchar>(i);
		for (int j = 0; j < gray_image.cols; j++) {

			int px = j - cx;
			int py = cy - i;
			if (px == 0 || py == 0)
				continue;

			dis = sqrt(px*px + py*py);
			int dis_index = dis / 32;
			if (dis_index > max_dis)
				max_dis = dis_index;
			int divide = 4;

		/*	if (dis_index >= 64)
				divide = 512;
			else */
				if (dis_index >= 32)
				divide = 256;
			else if (dis_index >= 16)
				divide = 128;
			else if (dis_index >= 8)
				divide = 64;
			else if (dis_index >= 4)
				divide = 16;

			float angle = atan2(py, px);
			int angle_index = (angle + CV_PI) / (2 * CV_PI / divide);
	/*		if (py == 0){
				if(px>0)
				angle_index = divide - 1;
			}*/
			Blemish_ROI[dis_index][angle_index].sum += inData[j];
			Blemish_ROI[dis_index][angle_index].cnt++;
		}
	}

	for (int k = 0; k <= max_dis; k++) {

		float tsum = 0;
		int divide = 4;
	/*	if (k >= 64)
			divide = 512;
		else */
			if (k >= 32)
			divide = 256;
		else if (k >= 16)
			divide = 128;
		else if (k >= 8)
			divide = 64;
		else if (k >= 4)
			divide = 16;

		int cnt = 0;
		for (int i = 0; i < divide; i++) {
			if (Blemish_ROI[k][i].cnt>=128) {
				Blemish_ROI[k][i].avg = (float)Blemish_ROI[k][i].sum / Blemish_ROI[k][i].cnt;
				tsum += Blemish_ROI[k][i].avg;	
				cnt++;
			}
		}
		float avg = (float)tsum / cnt;

		for (int i = 0; i < divide; i++) {
			if (Blemish_ROI[k][i].cnt>128) {
				float total_gap = abs((Blemish_ROI[k][i].avg-avg)/avg);
				int next = i + 1;
				while (Blemish_ROI[k][next].cnt<128&& next<divide)
					next++;
				if (next >= divide)
					next = 0;
				float next_gap = abs((Blemish_ROI[k][i].avg - Blemish_ROI[k][next].avg) / avg);

				if (k >= 50)
					nowTh *= 1.5;

				if (total_gap > nowTh && next_gap>nowTh) {

					NG = true;
					int r = 32 * k;
					float angle = 2*CV_PI / divide * i - CV_PI;
	
					int x = r*cos(angle);
					int y = r*sin(angle);
							
					circle(image, Point(x+cx, y+cy), 64, Scalar(0, 0, 255), 12, 8, 0);

				}		
			}
		}
	}


	cv::cvtColor(image, temp_image, CV_BGR2RGB);
	QImage showImage((const uchar*)temp_image.data, temp_image.cols, temp_image.rows, temp_image.cols*temp_image.channels(), QImage::Format_RGB888);
	imgScaled = showImage.scaled(ui->label_show_image->size(), Qt::KeepAspectRatio);
	ui->label_show_image->setPixmap(QPixmap::fromImage(imgScaled));

	displayResult();

}


void BlemishV04::on_pushButton_circle_Detect_clicked() {


	image = imageCopy.clone();
	cvtColor(image, gray_image, CV_BGR2GRAY);
	NG = false;
	unsigned char bv = BV(gray_image);

	img2 = gray_image.clone();
	GaussianBlur(img2, img2, Size(9, 9), 2, 2);
//	threshold(img2, img3, 150, 220, THRESH_BINARY);  //图像二值化，
	threshold(img2, img3, 170, 220, THRESH_BINARY);  //图像二值化，

	namedWindow("detecte circles1", CV_NORMAL);
	imshow("detecte circles1", img3);

	Canny(img3, img3, 50, 100);//边缘检测
	//	namedWindow("detect circles2", CV_NORMAL);
	//	imshow("detect circles2", img3);

	vector<vector<Point>>contours;
	vector<Vec4i>hierarchy;
	findContours(img3, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//查找出所有的圆边界
/*	
	int index = 0;
	for (; index >= 0; index = hierarchy[index][0])
	{
		//Scalar color(rand() & 255, rand() & 255, rand() & 255);
		Scalar color(0, 0, 0);
		drawContours(gray_image, contours, index, color, CV_FILLED, 8, hierarchy);

		////绘制轮廓的最小外结圆
		//Point2f Center;
		//float Radius;
		//minEnclosingCircle(contours, Center, Radius);
		//circle(gray_image, Center, Radius, Scalar(255), 2);
	}

	namedWindow("detected circles3", CV_NORMAL);
	imshow("detected circles3", gray_image);

	img2 = gray_image.clone();

	GaussianBlur(img2, img2, Size(9, 9), 2, 2);
	threshold(img2, img3, 20, 250, THRESH_BINARY);  //图像二值化，，

//	namedWindow("detecte circles4", CV_NORMAL);
//	imshow("detecte circles4", img3);
	Canny(img3, img3, 50, 100);//边缘检测
//	namedWindow("detect circles5", CV_NORMAL);
//	imshow("detect circles5", img3);

	findContours(img3, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//查找出所有的圆边界

*/
	vector<Point> Circle_edge;

	for (int i = 0; i < contours.size(); i++) {
		vector<Point> ::iterator start = contours[i].begin();
		while (start != contours[i].end()){
			Circle_edge.push_back(*start);
			++start;
		}
	}

	
	//标准圆在图片上一般是椭圆，所以采用OpenCV中拟合椭圆的方法求解中心
	Mat pointsf;
//	Mat(contours[0]).convertTo(pointsf, CV_32F);
	Mat(Circle_edge).convertTo(pointsf, CV_32F);

	RotatedRect box = fitEllipse(pointsf);

	string strx = to_string(box.center.x);
	string stry = to_string(box.center.y);
	strx = "x= " + strx + "\n";
	stry = "y= " + stry + "\n";
	ui->log->insertPlainText(strx.c_str());
	ui->log->insertPlainText(stry.c_str());
	int x = box.center.x - gray_image.cols / 2;
	int y = box.center.y - gray_image.rows / 2;
	strx = to_string(x);
	stry = to_string(y);

	float d = sqrt(x*x+y*y);
	string strd = to_string(d);

	strx = "OC_X= " + strx + "\n";
	stry = "OC_Y= " + stry + "\n";
	strd = "OC_Dis= " + strd + "\n";

	ui->log->insertPlainText(strx.c_str());
	ui->log->insertPlainText(stry.c_str());
	ui->log->insertPlainText(strd.c_str());
	ui->log->insertPlainText("~~~~~~~~~~~~~~~~~~~~\n");


}


string TCHAR2STRING (TCHAR *STR) {

	int iLen = WideCharToMultiByte(CP_ACP, 0,STR, -1, NULL, 0, NULL, NULL);
	char* chRtn = new char[iLen*sizeof(char)];
	WideCharToMultiByte(CP_ACP, 0, STR, -1, chRtn, iLen, NULL, NULL);
	std::string str(chRtn);
	return str;
}


void skip_Annotation(TCHAR* s) {

	for (int i = 0; i < 9; i++) {
		if (s[i] == '/') {
			s[i] = '\0';
			break;
		}
	}

}


void BlemishV04::on_pushButton_sensor_clicked() {

	Load_Sensor_Blemish();

}


void BlemishV04::on_pushButton_open_raw_clicked() {

	LB_No = ui->LB->document()->toPlainText().toInt();
	Width = ui->width->document()->toPlainText().toInt();
	Height = ui->height->document()->toPlainText().toInt();

	QString filename = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image File(*.raw)"));
	QTextCodec *code = QTextCodec::codecForName("gb18030");
	std::string name = code->fromUnicode(filename).data();

	ui->textBrowser->setTextColor(QColor(0, 0, 0, 255));
	ui->textBrowser->setFontPointSize(48);
	ui->textBrowser->setText("NA");
	ui->textBrowser->setAlignment(Qt::AlignCenter);

	FILE *fp = NULL;

	ret = 0;
	if (Width*Height == 0) {
		QMessageBox msgBox;
		msgBox.setText(tr("Plz input Img_width and Img_height value"));
		msgBox.exec();
		return;
	}
	unsigned short *pRawData = (unsigned short *)calloc(Width*Height, sizeof(unsigned short));

	if (NULL == pRawData)
	{
		QMessageBox msgBox;
		msgBox.setText(tr("Fail to calloc buf"));
		msgBox.exec();
		return;
	}

	ifstream in(name.c_str());
	in.seekg(0, ios::end); //设置文件指针到文件流的尾部
	streampos ps = in.tellg(); //读取文件指针的位置
	in.close(); //关闭文件流

	if (NULL == (fp = fopen(name.c_str(), "rb")))
	{
		QMessageBox msgBox;
		msgBox.setText(tr("Fail to read"));
		msgBox.exec();
		return;
	}

	if (Width*Height * 2 != ps)
	{
		QMessageBox msgBox;
		msgBox.setText(tr("Width * Height Size does not match Raw Size!"));
		msgBox.exec();
		return;
	}

	ret = fread(pRawData, sizeof(unsigned short)*Width*Height, 1, fp);

	IplImage *pBayerData = cvCreateImage(cvSize(Width, Height), 16, 1);
	IplImage *pRgbDataInt8 = cvCreateImage(cvSize(Width, Height), 8, 1);

	memcpy(pBayerData->imageData, (char *)pRawData, Width*Height*sizeof(unsigned short));

	cvConvertScale(pBayerData, pRgbDataInt8, 0.25, 0);

	temp_image = cvarrToMat(pRgbDataInt8);
	img2 = temp_image.clone();
	temp_image = img2.clone();
	gray_image = img2.clone();

	bool map = ui->GR->isChecked();
	if (map) {
		cvtColor(img2, img2, CV_BayerGR2BGR);
	}

	map = ui->GB->isChecked();
	if (map) {
		cvtColor(img2, img2, CV_BayerGB2BGR);
	}

	map = ui->RG->isChecked();
	if (map) {
		cvtColor(img2, img2, CV_BayerRG2BGR);
	}

	map = ui->BG->isChecked();
	if (map) {
		cvtColor(img2, img2, CV_BayerBG2BGR);
	}

	imwrite("RAW2RGB.bmp", img2);
	image = img2.clone();
	imageCopy = img2.clone();
	display_Image();

	cvReleaseImage(&pBayerData);
	cvReleaseImage(&pRgbDataInt8);

}


void BlemishV04::on_pushButton_HQ_Blemish_3_clicked() {

	image = imageCopy.clone();

	LB_No = ui->LB->document()->toPlainText().toInt();
	BLEMISH_TYPE blemishType = BLEMISH_TYPE_BRIGHT;
	char *debugFileName = "debug.txt";

	int Blemish_result[3] = { 0 };
	ret = 0;

	if (ui->Bright->isChecked()) {
		//	blemishType = BLEMISH_TYPE_BRIGHT;
		ret = JLib_CAL_LargeBlemish3_Bright(Width, Height, (unsigned char*)temp_image.data, (unsigned char*)image.data,
			Semco_Blemish_Size[LB_No].ROI_XSIZE, Semco_Blemish_Size[LB_No].ROI_YSIZE,
			Semco_Blemish_Size[LB_No].INNER_SIZE, Semco_Blemish_Size[LB_No].OUTER_SIZE, Semco_Blemish_Size[LB_No].EDGE_SIZE, Semco_Blemish_Size[LB_No].CORNER_SIZE,
			Semco_Blemish_Spec[LB_No].sigma_inner, Semco_Blemish_Spec[LB_No].sigma_outer, Semco_Blemish_Spec[LB_No].sigma_edge, Semco_Blemish_Spec[LB_No].sigma_corner,
			Semco_Blemish_Spec[LB_No].sigma_edgeLT, Semco_Blemish_Spec[LB_No].sigma_edgeRT, Semco_Blemish_Spec[LB_No].sigma_edgeLB, Semco_Blemish_Spec[LB_No].sigma_edgeRB,
			Semco_Blemish_Spec[LB_No].sigma_cornerLT, Semco_Blemish_Spec[LB_No].sigma_cornerRT, Semco_Blemish_Spec[LB_No].sigma_cornerLB, Semco_Blemish_Spec[LB_No].sigma_cornerRB,
			&Blemish_result[0], &Blemish_result[1], &Blemish_result[2], RGB(255, 0, 0), debugFileName, "1234567890123456");

	//	ui->log->insertPlainText(to_string(ret).c_str());

	}
	else if (ui->Dark->isChecked()) {
		//	blemishType = BLEMISH_TYPE_DARK;
		ret = JLib_CAL_LargeBlemish3_Dark(Width, Height, (unsigned char*)temp_image.data, (unsigned char*)image.data,
			Semco_Blemish_Size[LB_No].ROI_XSIZE, Semco_Blemish_Size[LB_No].ROI_YSIZE,
			Semco_Blemish_Size[LB_No].INNER_SIZE, Semco_Blemish_Size[LB_No].OUTER_SIZE, Semco_Blemish_Size[LB_No].EDGE_SIZE, Semco_Blemish_Size[LB_No].CORNER_SIZE,
			Semco_Blemish_Spec[LB_No].sigma_inner, Semco_Blemish_Spec[LB_No].sigma_outer, Semco_Blemish_Spec[LB_No].sigma_edge, Semco_Blemish_Spec[LB_No].sigma_corner,
			Semco_Blemish_Spec[LB_No].sigma_edgeLT, Semco_Blemish_Spec[LB_No].sigma_edgeRT, Semco_Blemish_Spec[LB_No].sigma_edgeLB, Semco_Blemish_Spec[LB_No].sigma_edgeRB,
			Semco_Blemish_Spec[LB_No].sigma_cornerLT, Semco_Blemish_Spec[LB_No].sigma_cornerRT,Semco_Blemish_Spec[LB_No].sigma_cornerLB, Semco_Blemish_Spec[LB_No].sigma_cornerRB,
			&Blemish_result[0], &Blemish_result[1], &Blemish_result[2],RGB(255, 0, 0), debugFileName, "1234567890123456");

		ui->log->clear();
		ui->log->insertPlainText("HQ Raw Blemish Result: ");
		ui->log->insertPlainText(to_string(ret).c_str());
	
	}

	display_Image();
	imwrite( "Blemsih_Result.bmp", image);

	NG = true;
	if (Blemish_result[0] == 0 && Blemish_result[1] == 0 && Blemish_result[2] == 0)
		NG = false;

	displayResult();
	
	return;
}


void BlemishV04::on_pushButton_HQ_Blemish_2_clicked() {

	image = imageCopy.clone();
	LB_No = ui->LB->document()->toPlainText().toInt();

	if (his == NULL)
	{
		QMessageBox msgBox;
		msgBox.setText(tr("Fail to read DLL!"));
		msgBox.exec();
		return;
	}

	if (LB2 == NULL)
	{
		QMessageBox msgBox;
		msgBox.setText(tr("Fail to Load DLL Function!"));
		msgBox.exec();
		return;
	}

	int Blemish_result[3] = { 0 };
	ret = 0;

	if (Width == 0) {
		Width = image.cols;
		Height = image.rows;
	}

	ret = LB2(
		Width, Height, (unsigned char*)gray_image.data, (unsigned char*)image.data,
		Semco_Blemish_Size[LB_No].ROI_XSIZE, Semco_Blemish_Size[LB_No].ROI_YSIZE,
		Semco_Blemish_Size[LB_No].INNER_SIZE, Semco_Blemish_Size[LB_No].OUTER_SIZE, Semco_Blemish_Size[LB_No].EDGE_SIZE,
		Semco_Blemish_Spec[LB_No].sigma_inner, Semco_Blemish_Spec[LB_No].sigma_outer, Semco_Blemish_Spec[LB_No].sigma_edge, Semco_Blemish_Spec[LB_No].sigma_corner,
		Semco_Blemish_Spec[LB_No].sigma_edgeLT, Semco_Blemish_Spec[LB_No].sigma_edgeRT, Semco_Blemish_Spec[LB_No].sigma_edgeLB, Semco_Blemish_Spec[LB_No].sigma_edgeRB,
		Semco_Blemish_Spec[LB_No].sigma_cornerLT, Semco_Blemish_Spec[LB_No].sigma_cornerRT, Semco_Blemish_Spec[LB_No].sigma_cornerLB, Semco_Blemish_Spec[LB_No].sigma_cornerRB,
		&Blemish_result[0], &Blemish_result[1], &Blemish_result[2], RGB(255, 0, 0));

	ui->log->clear();
	ui->log->insertPlainText("HQ Blemish2 Result: ");
	ui->log->insertPlainText(to_string(ret).c_str());
	ui->log->insertPlainText("\n");

	display_Image();
	imwrite("Blemsih_Result.bmp", image);

	NG = true;
	if (Blemish_result[0] == 0 && Blemish_result[1] == 0 && Blemish_result[2] == 0)
		NG = false;

	displayResult();
	
	return;
}


void BlemishV04::on_pushButton_spec_search_clicked() {

	LB_No = ui->LB->document()->toPlainText().toInt();
	int Blemish_result[3] = { 0 };

	if (ui->corner->isChecked()) {

		float max_Spec = 0.5, min_Spec = 0.001, current_Spec = 0.1;

		while (max_Spec - min_Spec > 0.0001) {

			Blemish_result[2] = 0;
			current_Spec = (max_Spec + min_Spec) / 2;
			ret = LB2(
				Width, Height, (unsigned char*)gray_image.data, (unsigned char*)image.data,
				Semco_Blemish_Size[LB_No].ROI_XSIZE, Semco_Blemish_Size[LB_No].ROI_YSIZE,
				Semco_Blemish_Size[LB_No].INNER_SIZE, Semco_Blemish_Size[LB_No].OUTER_SIZE, Semco_Blemish_Size[LB_No].EDGE_SIZE,
				1, 1, 1, current_Spec,
				10, 10, 10, 10,
				10, 10, 10, 10,
				&Blemish_result[0], &Blemish_result[1], &Blemish_result[2], RGB(255, 0, 0));

			if (Blemish_result[2] == 0) {
				max_Spec = current_Spec;
			}
			else {
				min_Spec = current_Spec;
			}	
		}
		ui->log->insertPlainText("Corner Limit PASS Spec is: ");
		ui->log->insertPlainText(to_string(max_Spec).c_str());
		ui->log->insertPlainText("\n");
	}

	if (ui->edge->isChecked()) {

		float max_Spec = 0.5, min_Spec = 0.001, current_Spec = 0.1;

		while (max_Spec - min_Spec > 0.0001) {

			Blemish_result[2] = 0;
			current_Spec = (max_Spec + min_Spec) / 2;
			ret = LB2(
				Width, Height, (unsigned char*)gray_image.data, (unsigned char*)image.data,
				Semco_Blemish_Size[LB_No].ROI_XSIZE, Semco_Blemish_Size[LB_No].ROI_YSIZE,
				Semco_Blemish_Size[LB_No].INNER_SIZE, Semco_Blemish_Size[LB_No].OUTER_SIZE, Semco_Blemish_Size[LB_No].EDGE_SIZE,
				1, 1, current_Spec, 1,
				10, 10, 10, 10,
				10, 10, 10, 10,
				&Blemish_result[0], &Blemish_result[1], &Blemish_result[2], RGB(255, 0, 0));

			if (Blemish_result[2] == 0) {
				max_Spec = current_Spec;
			}
			else {
				min_Spec = current_Spec;
			}
		}
		ui->log->insertPlainText("Edge Limit PASS Spec is: ");
		ui->log->insertPlainText(to_string(max_Spec).c_str());
		ui->log->insertPlainText("\n");
	}

	if (ui->outer->isChecked()) {

		float max_Spec = 0.5, min_Spec = 0.001, current_Spec = 0.1;

		while (max_Spec - min_Spec > 0.0001) {

			Blemish_result[1] = 0;
			current_Spec = (max_Spec + min_Spec) / 2;
			ret = LB2(
				Width, Height, (unsigned char*)gray_image.data, (unsigned char*)image.data,
				Semco_Blemish_Size[LB_No].ROI_XSIZE, Semco_Blemish_Size[LB_No].ROI_YSIZE,
				Semco_Blemish_Size[LB_No].INNER_SIZE, Semco_Blemish_Size[LB_No].OUTER_SIZE, Semco_Blemish_Size[LB_No].EDGE_SIZE,
				1, current_Spec, 1, 1,
				10, 10, 10, 10,
				10, 10, 10, 10,
				&Blemish_result[0], &Blemish_result[1], &Blemish_result[2], RGB(255, 0, 0));

			if (Blemish_result[2] == 0) {
				max_Spec = current_Spec;
			}
			else {
				min_Spec = current_Spec;
			}
		}
		ui->log->insertPlainText("Outer Limit PASS Spec is: ");
		ui->log->insertPlainText(to_string(max_Spec).c_str());
		ui->log->insertPlainText("\n");
	}

	if (ui->inner->isChecked()) {

		float max_Spec = 0.5, min_Spec = 0.001, current_Spec = 0.1;

		while (max_Spec - min_Spec > 0.0001) {

			Blemish_result[0] = 0;
			current_Spec = (max_Spec + min_Spec) / 2;
			ret = LB2(
				Width, Height, (unsigned char*)gray_image.data, (unsigned char*)image.data,
				Semco_Blemish_Size[LB_No].ROI_XSIZE, Semco_Blemish_Size[LB_No].ROI_YSIZE,
				Semco_Blemish_Size[LB_No].INNER_SIZE, Semco_Blemish_Size[LB_No].OUTER_SIZE, Semco_Blemish_Size[LB_No].EDGE_SIZE,
				1, current_Spec, 1, 1,
				10, 10, 10, 10,
				10, 10, 10, 10,
				&Blemish_result[0], &Blemish_result[1], &Blemish_result[2], RGB(255, 0, 0));

			if (Blemish_result[2] == 0) {
				max_Spec = current_Spec;
			}
			else {
				min_Spec = current_Spec;
			}
		}
		ui->log->insertPlainText("Inner Limit PASS Spec is: ");
		ui->log->insertPlainText(to_string(max_Spec).c_str());
		ui->log->insertPlainText("\n");
	}
}

/*
image = imageCopy.clone();
cvtColor(image, gray_image, CV_BGR2GRAY);
NG = false;

img2 = gray_image.clone();

GaussianBlur(img2, img2, Size(9, 9), 2, 2);
threshold(img2, img3, 80, 255, THRESH_BINARY);  //图像二值化，，注意阈值变化
namedWindow("detecte circles", CV_NORMAL);
imshow("detecte circles", img3);
Canny(img3, img3, 50, 100);//边缘检测
namedWindow("detect circles", CV_NORMAL);
imshow("detect circles", img3);
vector<vector<Point>>contours;
vector<Vec4i>hierarchy;
findContours(img3, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//查找出所有的圆边界
int index = 0;
for (; index >= 0; index = hierarchy[index][0])
{
	Scalar color(rand() & 255, rand() & 255, rand() & 255);
	drawContours(gray_image, contours, index, color, CV_FILLED, 8, hierarchy);
}

namedWindow("detected circles", CV_NORMAL);
imshow("detected circles", gray_image);
//标准圆在图片上一般是椭圆，所以采用OpenCV中拟合椭圆的方法求解中心
Mat pointsf;
Mat(contours[0]).convertTo(pointsf, CV_32F);
RotatedRect box = fitEllipse(pointsf);
//	cout << box.center;
string strx = to_string(box.center.x);
string stry = to_string(box.center.y);
strx = "x= " + strx + "\n";
stry = "y= " + stry + "\n";
ui->log->insertPlainText(strx.c_str());
ui->log->insertPlainText(stry.c_str());


waitKey();
displayResult();


//	GaussianBlur(edges, edges, Size(7, 7), 2, 2);
namedWindow("Hough circles", CV_NORMAL);
imshow("Hough circles", edges);
vector<Vec3f> circles;
//	CvMemStorage* circles = cvCreateMemStorage(0);
//霍夫圆
HoughCircles(edges, circles, CV_HOUGH_GRADIENT, 1, 18, 100,100, 0, 0);
for (size_t i = 0; i < circles.size(); i++)
{
Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
int radius = cvRound(circles[i][2]);
//绘制圆心
circle(gray_image, center, 3, Scalar(0, 255, 0), -1, 8, 0);
//绘制圆轮廓
circle(gray_image, center, radius, Scalar(155, 50, 255), 3, 8, 0);
}


*/


/*
Mat img4 = img3.clone();
vector<Vec3f> circles;
//霍夫圆
HoughCircles(img4, circles, CV_HOUGH_GRADIENT, 1.5, 8000, 200, 50, 1200, 1560);
for (size_t i = 0; i < circles.size(); i++)
{
Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
int radius = cvRound(circles[i][2]);
//绘制圆心
circle(img4, center, 3, Scalar(0, 255, 0), -1, 8, 0);
//绘制圆轮廓
circle(img4, center, radius, Scalar(155, 50, 255), 3, 8, 0);
}

namedWindow("【效果图】", CV_NORMAL);
imshow("【效果图】", img4);
*/