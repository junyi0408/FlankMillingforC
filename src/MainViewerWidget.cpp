#include "MainViewerWidget.h"
const double TRACKBALL_RADIUS = 0.6;

MainViewerWidget::MainViewerWidget(QWidget* _parent/* =0 */)
{
	initViewerWindow();
	LoadMeshSuccess = false;
}
MainViewerWidget::~MainViewerWidget()
{
};

void MainViewerWidget::initViewerWindow()
{
	
}

void MainViewerWidget::initializeGL()
{
    //initializeOpenGLFunctions();
    glClearColor(1.0, 1.0, 1.0, 1.0);
    //glClearColor(0.196, 0.196, 0.196, 1.0);
    glDisable(GL_DITHER);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

    GLfloat mat_a[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat mat_d[] = { 0.88f, 0.84f, 0.76f, 1.0f };
    GLfloat mat_s[] = { 0.4f, 0.4f, 0.4f, 1.0f };
    GLfloat shine[] = { 120.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);


    glLoadIdentity();
    

    GLfloat pos1[] = { 10.0f, 10.0f, -10.0f, 0.0f };
    GLfloat pos2[] = { -10.0f, 10.0f, -10.0f, 0.0f };
    GLfloat pos3[] = { 0.0f, 0.0f, 10.0f, 0.0f };
    /*GLfloat col1[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat col2[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat col3[] = { 0.8f, 0.8f, 0.8f, 1.0f };*/
    GLfloat col1[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat col2[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat col3[] = { 0.8f, 0.8f, 0.8f, 1.0f };

    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, pos1);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, col1);
    glLightfv(GL_LIGHT0, GL_SPECULAR, col1);

    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_POSITION, pos2);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, col2);
    glLightfv(GL_LIGHT1, GL_SPECULAR, col2);

    glEnable(GL_LIGHT2);
    glLightfv(GL_LIGHT2, GL_POSITION, pos3);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, col3);
    glLightfv(GL_LIGHT2, GL_SPECULAR, col3);


    
    //glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    // scene pos and size
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}
void MainViewerWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // ��������ͶӰ�߽�
    float aspect = static_cast<float>(width) / height;
    float orthoSize = 1.5;  

    if (aspect >= 1.0f) {
        glOrtho(-orthoSize * aspect, orthoSize * aspect, -orthoSize, orthoSize, -5.0f, 5.0f);
    }
    else {
        glOrtho(-orthoSize, orthoSize, -orthoSize / aspect, orthoSize / aspect, -5.0f, 5.0f);
    }
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glMatrixMode(GL_MODELVIEW);
}
void MainViewerWidget::paintGL()
{
    //qDebug() << "paintGL() called!";  // ȷ��������
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // **������ת�ѿ�������**
    float radTheta = theta * M_PI / 180.0f;  // �Ƕ�ת����
    float radPhi = phi * M_PI / 180.0f;
    float camX = radius * sin(radPhi) * sin(radTheta);
    float camY = radius * cos(radPhi);
    float camZ = radius * sin(radPhi) * cos(radTheta);

    // **����ӽ�**
    gluLookAt(camX, camY, camZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // **������������**
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.5f, 2.0f);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_FLAT);
    for (const auto& fh : mesh.faces()) {
        glNormal3fv(mesh.normal(fh).data());
        if (std::find(selected_faces.begin(),selected_faces.end(),fh.idx()) != selected_faces.end()) {
            glColor3f(0.0f, 1.0f, 0.0f);
        }
        else {
            glColor3f(0.8f, 0.8f, 0.8f);
        }
        glBegin(GL_TRIANGLES);
        for (const auto& vh : mesh.fv_range(fh)) {
            OpenMesh::Vec3f v = mesh.point(vh);
            glVertex3fv(v.data());
        }
        glEnd();
    }
    
    glDisable(GL_POLYGON_OFFSET_FILL);
    glDisable(GL_LIGHTING);


    glColor3f(0.0f, 0.0f, 0.25f);
    glLineWidth(1.0f);  // �趨�߿�
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // �߿�ģʽ
    glBegin(GL_LINES);
    for (const auto& eh : mesh.edges()) {
        OpenMesh::Vec3f v1 = mesh.point(mesh.from_vertex_handle(mesh.halfedge_handle(eh, 0)));
        OpenMesh::Vec3f v2 = mesh.point(mesh.to_vertex_handle(mesh.halfedge_handle(eh, 0)));
        glVertex3fv(v1.data());
        glVertex3fv(v2.data());
    }
    glEnd();
    
}

static void normalizeMesh(Mesh& mesh) {
    if (mesh.n_vertices() == 0) return;

    OpenMesh::Vec3f min_corner(FLT_MAX, FLT_MAX, FLT_MAX);
    OpenMesh::Vec3f max_corner(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    // ���� mesh �� bounding box
    for (auto v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it) {
        OpenMesh::Vec3f p = mesh.point(*v_it);
        min_corner.minimize(p);
        max_corner.maximize(p);
    }

    // �������ĵ� & ���߳�
    OpenMesh::Vec3f center = (min_corner + max_corner) * 0.5f;
    float max_side = (max_corner - min_corner).max(); // �ҵ���ı߳�

    // �����������ӣ�ʹ�� mesh ��һ���� [-1, 1]
    float scale = 2.0f / max_side;  // ���� 2 ��ƥ�� [-1,1]

    // ��һ����ƽ�Ƶ�ԭ�㲢����
    for (auto v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it) {
        OpenMesh::Vec3f p = mesh.point(*v_it);
        p = (p - center) * scale;  // ��һ���� [-1, 1]
        mesh.set_point(*v_it, p);
    }
}
void MainViewerWidget::load_mesh(const QString& fileName)
{
    mesh.clear();

    if (!OpenMesh::IO::read_mesh(mesh, fileName.toStdString())) {
        std::cerr << "Error: Cannot read mesh from " << fileName.toStdString() << std::endl;
        LoadMeshSuccess = false;
    }
    else
    {
        normalizeMesh(mesh);
        LoadMeshSuccess = true;
        mesh.request_face_normals();
        mesh.update_normals();
    }
    //updateGL();  // ���� OpenGL ���»���
}
void MainViewerWidget::save_mesh(const QString& fileName)
{
    OpenMesh::IO::write_mesh(mesh, fileName.toStdString());
}


void MainViewerWidget::readDepthBuffer()
{
    int width = this->width();
    int height = this->height();

    // **��ȡ OpenGL ��Ȼ�����**
    std::vector<float> depthBuffer(width * height);
    glReadPixels(0, 0, width, height, GL_DEPTH_COMPONENT, GL_FLOAT, depthBuffer.data());

    //// **�洢��һ��������ֵ**
    //depthImage.resize(width * height);

    //// **��һ�����ֵ**
    //for (size_t i = 0; i < depthBuffer.size(); ++i) {
    //    depthImage[i] = (depthBuffer[i] - minDepth) / (maxDepth - minDepth);
    //}
    depthImage = depthBuffer;
    float minDepth = 1.0f, maxDepth = 0.0f;
    for (float d : depthBuffer) {
        if (d < minDepth) minDepth = d;
        if (d > maxDepth) maxDepth = d;
    }
}
bool MainViewerWidget::isTriangleVisible(OpenMesh::FaceHandle fh)
{
    std::vector<Mesh::Point> points;
    Mesh::Point centroid(0, 0, 0);
    for (const auto& vh : mesh.fv_range(fh)) {
        centroid += mesh.point(vh);
		points.push_back(mesh.point(vh));
    }
    centroid /= 3.0f; // ����������
	points.push_back(centroid);

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    GLint viewport[4] = { 0.0,0.0,this->width(),this->height() };
    bool flag = false;
	for (auto& point : points)
	{
        GLdouble winX, winY, winZ;
        gluProject(point[0], point[1], point[2], modelview, projection, viewport, &winX, &winY, &winZ);
        // **ת��Ϊ OpenGL �ӿ�����**
        int pixelX = static_cast<int>(winX);
        int pixelY = static_cast<int>(winY);
        if (pixelX < 0 || pixelX >= viewport[2] || pixelY < 0 || pixelY >= viewport[3])
            continue; // ������Ļ��Χ������

        // **��ȡ�����ص����**
        float depthAtPixel = depthImage[pixelY * viewport[2] + pixelX];

        // **�ж��������Ƿ����ڵ�**
        if (winZ <= depthAtPixel + 1e-5f)
        {
            flag = true;
            break;
        } 
	}
    return flag;

}




















void MainViewerWidget::mousePressEvent(QMouseEvent* _event)
{
    //assert(mouse_mode_ < N_MOUSE_MODES);
    last_point_2D_ = _event->pos();
    last_point_ok_ = map_to_sphere(last_point_2D_, last_point_3D_);
    mouse_mode_ = _event->button();
}

void MainViewerWidget::mouseMoveEvent(QMouseEvent* _event)
{
    //assert(mouse_mode_ < N_MOUSE_MODES);

    QPoint newPoint2D = _event->pos();

    // enable GL context
    makeCurrent();

    if (last_point_ok_)
    {
        switch (mouse_mode_)
        {
        case Qt::LeftButton:
            rotation(newPoint2D);
            break;
        case Qt::RightButton:
            //translation(newPoint2D);
            break;
        default:
            break;
        }
    } // end of if

    // remember this point
    last_point_2D_ = newPoint2D;
    last_point_ok_ = map_to_sphere(last_point_2D_, last_point_3D_);

    // trigger redraw
    updateGL();
}

void MainViewerWidget::mouseReleaseEvent(QMouseEvent* /* _event */)
{
    //assert(mouse_mode_ < N_MOUSE_MODES);
    mouse_mode_ = Qt::NoButton;
    last_point_ok_ = false;
}


void MainViewerWidget::rotation(QPoint p)
{
    OpenMesh::Vec3d  newPoint3D;
    bool newPoint_hitSphere = map_to_sphere(p, newPoint3D);
    double phi1 = std::acos(newPoint3D.normalize()[2]);
	double phi2 = std::acos(last_point_3D_.normalize()[2]);
	double theta1 = std::atan2(newPoint3D.normalize()[1], newPoint3D.normalize()[0]);
	double theta2 = std::atan2(last_point_3D_.normalize()[1], last_point_3D_.normalize()[0]);
	double delta_phi = phi1 - phi2;
    if (delta_phi >= M_PI)
    {
		delta_phi = 2 * M_PI - delta_phi;
    }
    if (delta_phi <= -M_PI)
    {
        delta_phi += 2 * M_PI;
    }
	double delta_theta = theta1 - theta2;
    if (delta_theta >= M_PI)
    {
        delta_theta = 2 * M_PI - delta_theta;
    }
    if (delta_theta <= -M_PI)
    {
        delta_theta += 2 * M_PI;
    }
    if (newPoint_hitSphere)
    {
        theta -= delta_theta * 180 / M_PI;
        phi -= delta_phi * 180 / M_PI;
    }
}


bool MainViewerWidget::map_to_sphere(const QPoint& _v2D, OpenMesh::Vec3d& _v3D)
{
    // This is actually doing the Sphere/Hyperbolic sheet hybrid thing,
    // based on Ken Shoemake's ArcBall in Graphics Gems IV, 1993.
    double x = (2.0 * _v2D.x() - width()) / width();
    double y = -(2.0 * _v2D.y() - height()) / height();
    double xval = x;
    double yval = y;
    double x2y2 = xval * xval + yval * yval;

    const double rsqr = TRACKBALL_RADIUS * TRACKBALL_RADIUS;
    _v3D[0] = xval;
    _v3D[1] = yval;
    if (x2y2 < 0.5 * rsqr)
    {
        _v3D[2] = sqrt(rsqr - x2y2);
    }
    else
    {
        _v3D[2] = 0.5 * rsqr / sqrt(x2y2);
    }

    return true;
}
