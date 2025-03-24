#include "MainViewerWidget.h"

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

    // 计算正交投影边界
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
    qDebug() << "paintGL() called!";  // 确保被触发
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // **球坐标转笛卡尔坐标**
    float radTheta = theta * M_PI / 180.0f;  // 角度转弧度
    float radPhi = phi * M_PI / 180.0f;
    float camX = radius * sin(radPhi) * sin(radTheta);
    float camY = radius * cos(radPhi);
    float camZ = radius * sin(radPhi) * cos(radTheta);

    // **相机视角**
    gluLookAt(camX, camY, camZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // **绘制三角网格**
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
    glLineWidth(1.0f);  // 设定线宽
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // 线框模式
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

    // 计算 mesh 的 bounding box
    for (auto v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it) {
        OpenMesh::Vec3f p = mesh.point(*v_it);
        min_corner.minimize(p);
        max_corner.maximize(p);
    }

    // 计算中心点 & 最大边长
    OpenMesh::Vec3f center = (min_corner + max_corner) * 0.5f;
    float max_side = (max_corner - min_corner).max(); // 找到最长的边长

    // 计算缩放因子，使得 mesh 归一化到 [-1, 1]
    float scale = 2.0f / max_side;  // 乘以 2 以匹配 [-1,1]

    // 归一化：平移到原点并缩放
    for (auto v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it) {
        OpenMesh::Vec3f p = mesh.point(*v_it);
        p = (p - center) * scale;  // 归一化到 [-1, 1]
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
    //updateGL();  // 触发 OpenGL 重新绘制
}
void MainViewerWidget::save_mesh(const QString& fileName)
{
    OpenMesh::IO::write_mesh(mesh, fileName.toStdString());
}


void MainViewerWidget::readDepthBuffer()
{
    int width = this->width();
    int height = this->height();

    // **读取 OpenGL 深度缓冲区**
    std::vector<float> depthBuffer(width * height);
    glReadPixels(0, 0, width, height, GL_DEPTH_COMPONENT, GL_FLOAT, depthBuffer.data());

    //// **存储归一化后的深度值**
    //depthImage.resize(width * height);

    //// **归一化深度值**
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
    // **计算三角形质心的深度**
    OpenMesh::Vec3f centroid(0, 0, 0);
    for (const auto& vh : mesh.fv_range(fh)) {
        centroid += mesh.point(vh);
    }
    centroid /= 3.0f; // 三角形质心

    // **将质心投影到屏幕坐标**
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    //GLdouble modelview[16] = { 1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0 };
    GLint viewport[4] = { 0.0,0.0,this->width(),this->height() };

    GLdouble winX, winY, winZ;
    gluProject(centroid[0], centroid[1], centroid[2], modelview, projection, viewport, &winX, &winY, &winZ);

    // **转换为 OpenGL 视口坐标**
    int pixelX = static_cast<int>(winX);
    int pixelY = static_cast<int>(winY);
    if (pixelX < 0 || pixelX >= viewport[2] || pixelY < 0 || pixelY >= viewport[3])
        return false; // 超出屏幕范围，跳过

    // **获取该像素的深度**
    float depthAtPixel = depthImage[pixelY * viewport[2] + pixelX];

    // **判断三角形是否无遮挡**
    return (winZ <= depthAtPixel + 1e-5f);
}