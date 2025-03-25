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
		selected_faces.clear();
		readDepthBuffer();
		float radTheta = theta * M_PI / 180.0f;
		float radPhi = phi * M_PI / 180.0f;
		float camX = radius * sin(radPhi) * sin(radTheta);
		float camY = radius * cos(radPhi);
		float camZ = radius * sin(radPhi) * cos(radTheta);
		OpenMesh::Vec3f camera(camX, camY, camZ);
		for (const auto& fh : mesh.faces())
		{
			/*if (isTriangleVisible(fh))
			{
				selected_faces.push_back(fh.idx());
			}*/
			double a = abs(camera.normalize() | mesh.normal(fh));
			if (a < 0.1)
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

protected:
	// Qt mouse events
	virtual void mousePressEvent(QMouseEvent*);
	virtual void mouseReleaseEvent(QMouseEvent*);
	virtual void mouseMoveEvent(QMouseEvent*);

private:
	bool LoadMeshSuccess;
	Mesh mesh;
	double radius = 1;
	double theta = 0, phi = 90;

	std::vector<float> depthImage;
	GLdouble projection[16];
	GLdouble modelview[16];

	std::vector<int> selected_faces;

private:
	void load_mesh(const QString& fileName);
	void save_mesh(const QString& fileName);
	void readDepthBuffer();
	bool isTriangleVisible(OpenMesh::FaceHandle fh);

	void rotation(QPoint p);

protected:
	bool map_to_sphere(const QPoint& _point, OpenMesh::Vec3d& _result);
	int mouse_mode_;
	QPoint           last_point_2D_;
	OpenMesh::Vec3d  last_point_3D_;
	bool             last_point_ok_;
};
#endif // !MAINVIEWERWIDGET_H
