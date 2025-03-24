#include "MWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	MWindow mainWin;
	
	mainWin.show();

	return app.exec();
}
