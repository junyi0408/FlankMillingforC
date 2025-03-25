#include "MWindow.h"
//#include "MeshViewer/MainViewerWidget.h"
//#include "MeshViewer/Aux_Transform_Mesh_Dialog.h"

MWindow::MWindow(QWidget* parent)
	: QMainWindow(parent)
{
	viewer = new MainViewerWidget(this);
	viewer->setFixedSize(800, 800);
	setCentralWidget(viewer);
	initWindow();
	createActions();
	createToolBars();
}

MWindow::~MWindow()
{

}

void MWindow::initWindow()
{

}

void MWindow::createActions()
{
	openAction = new QAction(tr("&Open"), this);
	connect(openAction, SIGNAL(triggered()), viewer, SLOT(open_mesh_query()));

	saveAction = new QAction(tr("&Save"), this);
	connect(saveAction, SIGNAL(triggered()), viewer, SLOT(save_mesh_query()));

	updateAction = new QAction(tr("&Update"), this);
	connect(updateAction, SIGNAL(triggered()), viewer, SLOT(update_query()));
}


void MWindow::createToolBars()
{
	toolBar = addToolBar(tr("&Function"));
	toolBar->addAction(openAction);
	toolBar->addAction(saveAction);
	toolBar->addAction(updateAction);
}


