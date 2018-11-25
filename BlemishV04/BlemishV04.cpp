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

}

BlemishV04::~BlemishV04()
{
	fout << flush;
	fout.close();
	delete ui;
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

	image = cv::imread(name);
	if (!image.data)
	{
		QMessageBox msgBox;
		msgBox.setText(tr("image data is null"));
		msgBox.exec();
	}
	else
	{
		NG = false;
		//	defectCnt = 0;
		knownDefectCnt = 0;
		imageCopy = image.clone();
		rectSize = image.cols / 1000 * 10 + 10;
		lineThickness = image.cols / 1000 * 2 + 2;
		cv::cvtColor(image, temp_image, CV_BGR2RGB);
		//	img = QImage((const unsigned char*)(image.data), image.cols, image.rows, QImage::Format_RGB888);
		QImage showImage((const uchar*)temp_image.data, temp_image.cols, temp_image.rows, temp_image.cols*temp_image.channels(), QImage::Format_RGB888);

		ui->label_show_image->clear();
		ui->pushButton_image_processing->setEnabled(true);
		ui->pushButton_image_processing_2->setEnabled(true);
		ui->BlemishCheck2->setEnabled(true);
		ui->saveGray->setEnabled(true);

		imgScaled = showImage.scaled(ui->label_show_image->size(), Qt::KeepAspectRatio);
		//imgScaled = showImage.scaledToHeight(600, Qt::FastTransformation);
		//imgScaled = showImage.scaledToWidth(800, Qt::FastTransformation);

		ui->label_show_image->setPixmap(QPixmap::fromImage(imgScaled));
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


void BlemishV04::on_pushButton_image_processing_clicked()
{
	image = imageCopy.clone();
	cvtColor(image, gray_image, CV_BGR2GRAY);
	NG = false;

	blemishModeCheck(0);

	ui->textBrowser->setFontPointSize(48);

	if (NG) {
		ui->textBrowser->setTextColor(QColor(255, 0, 0, 255));
		ui->textBrowser->setText("NG");
	}
	else {
		ui->textBrowser->setTextColor(QColor(0, 255, 0, 255));
		ui->textBrowser->setText("OK");
	}
	fout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
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

	ui->textBrowser->setFontPointSize(48);

	if (NG) {
		ui->textBrowser->setTextColor(QColor(255, 0, 0, 255));
		ui->textBrowser->setText("NG");	
	}
	else {
		ui->textBrowser->setTextColor(QColor(0, 255, 0, 255));
		ui->textBrowser->setText("OK");
	}
	fout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	Mat dst;
	cvtColor(image, dst, CV_BGR2RGB);
	ui->textBrowser->setAlignment(Qt::AlignCenter);
	QImage showImage((const uchar*)dst.data, dst.cols, dst.rows, dst.cols*dst.channels(), QImage::Format_RGB888);

	//imgScaled = showImage.scaledToHeight(600, Qt::FastTransformation);
	imgScaled = showImage.scaled(ui->label_show_image->size(), Qt::KeepAspectRatio);
	ui->label_show_image->setPixmap(QPixmap::fromImage(imgScaled));
}


void BlemishV04::saveGrayImage_clicked()
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

	ui->textBrowser->setFontPointSize(48);

	if (NG) {
		ui->textBrowser->setTextColor(QColor(255, 0, 0, 255));
		ui->textBrowser->setText("NG");
	}
	else {
		ui->textBrowser->setTextColor(QColor(0, 255, 0, 255));
		ui->textBrowser->setText("OK");
	}

	Mat dst;
	cvtColor(image, dst, CV_BGR2RGB);
	ui->textBrowser->setAlignment(Qt::AlignCenter);
	QImage showImage((const uchar*)dst.data, dst.cols, dst.rows, dst.cols*dst.channels(), QImage::Format_RGB888);

	//imgScaled = showImage.scaledToHeight(600, Qt::FastTransformation);
	imgScaled = showImage.scaled(ui->label_show_image->size(), Qt::KeepAspectRatio);
	ui->label_show_image->setPixmap(QPixmap::fromImage(imgScaled));

	fout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	
}


