#ifndef BlemishV04_H
#define BlemishV04_H

#include <QWidget>
#include "core.hpp"
#include"highgui.hpp"
#include "BlemishV04.h"
#include "ui_BlemishV04.h"
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <opencv2/opencv.hpp>
#include <QTextCodec>
#include <afx.h>
#include <string>
#include "ISM_Test.h"

namespace Ui {
	class BlemishV04;
}

class BlemishV04 : public QWidget
{
	Q_OBJECT

public:
	explicit BlemishV04(QWidget *parent = 0);
	~BlemishV04();

	public slots:
	bool loadParameter();
	void readWBValue();
	void readBlemishValue();
	void saveValue_clicked();
	void on_pushButton_open_image_clicked();
	void on_pushButton_image_processing_clicked();
	void on_pushButton_image_processing_2_clicked();
	void on_pushButton_blemishcheck2_clicked();
	void on_pushButton_BlemishCheckNew_clicked();
	void on_pushButton_OC_clicked();
	void on_pushButton_clear_clicked();
	void on_pushButton_circle_Detect_clicked();
	void on_pushButton_HQ_Blemish_clicked();


	void saveGrayImage_clicked();
	void drawDefectRect(int i,int j, int mode);
	void drawDefectCircle(int i, int j, int b, int g, int r);
	void WBdotCheck(int x, int y, int width, int height,int area);
	void blemishBlockCheck(float map[300][300], int area, int j, int i, int mode,int Mx, int My);
	void blemishModeCheck(int mode);
	void displayResult();


private:
	Ui::BlemishV04 *ui;
	cv::Mat image,img2,img3;
	cv::Mat imageCopy;
	cv::Mat temp_image;
	cv::Mat gray_image;
	cv::Mat green_image;

	QImage img;
	QImage imgScaled;
};

#endif // WIDGET_H
