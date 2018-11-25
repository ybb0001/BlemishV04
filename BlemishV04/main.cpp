#include "BlemishV04.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	BlemishV04 w;
	w.show();
//	w.readWBValue();
//	w.readBlemishValue();
	return a.exec();
}
