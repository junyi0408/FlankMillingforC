#ifndef MWINDOW_H
#define MWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtWidgets>
#include "MainViewerWidget.h"

class MWindow : public QMainWindow
{
	Q_OBJECT
public:
	MWindow(QWidget* parent = 0);
	~MWindow();

private:
	void initWindow();
	void createActions();
	void createToolBars();

private slots:

private:
	// File Actions.
	QAction* openAction;
	QAction* saveAction;
	QAction* updateAction;

	// ToolBars.
	QToolBar* toolBar;

private:
	MainViewerWidget* viewer;

};

#endif // MWINDOW_H