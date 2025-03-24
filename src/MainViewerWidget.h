#ifndef MAINVIEWERWIDGET_H
#define MAINVIEWERWIDGET_H

#include <QtGui>
#include <QString>
#include <QMessageBox>
#include <QFileDialog>
#include <QGLWidget>
#include <Windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

typedef OpenMesh::TriMesh_ArrayKernelT<> Mesh;

class MainViewerWidget : public QGLWidget
{
	Q_OBJECT
public:
	MainViewerWidget(QWidget* _parent = 0);
	~MainViewerWidget();

public slots:
	void open_mesh_query()
	{
		QString fileName = QFileDialog::getOpenFileName(this,
			tr("Open mesh file"),
			tr("../models/"),
			tr("OBJ Files (*.obj);;"
				"OFF Files (*.off);;"
				"PLY Files (*.ply);;"
				"STL Files (*.stl);;"
				"All Files (*)"));
		if (!fileName.isEmpty())
		{
			load_mesh(fileName);
		}
	}
	void save_mesh_query()
	{
		QString fileName = QFileDialog::getSaveFileName(this,
			tr("Save mesh file"),
			tr("../models/untitled.off"),
			tr("OFF Files (*.off);;"
				"OBJ Files (*.obj);;"
				"PLY Files (*.ply);;"
				"STL Files (*.stl);;"
				"All Files (*)"));
		if (!fileName.isEmpty())
		{
			save_mesh(fileName);
		}
	}
	void update_query()
	{
		readDepthBuffer();
		for (const auto& fh : mesh.faces())
		{
			if (isTriangleVisible(fh))
			{
				selected_faces.push_back(fh.idx());
			}
		}
		updateGL();
	}

protected:
	virtual void initViewerWindow();
	void initializeGL() override;
	void resizeGL(int width, int height)override;
	void paintGL()override;

private:
	bool LoadMeshSuccess;
	Mesh mesh;
	double radius = 1;
	double theta = 45, phi = 90;

	std::vector<float> depthImage;
	GLdouble projection[16];
	GLdouble modelview[16];

	std::vector<int> selected_faces;

private:
	void load_mesh(const QString& fileName);
	void save_mesh(const QString& fileName);
	void readDepthBuffer();
	bool isTriangleVisible(OpenMesh::FaceHandle fh);
};
#endif // !MAINVIEWERWIDGET_H
