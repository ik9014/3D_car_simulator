#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdio.h>

const double PI = 3.14159265;

static float angle = 0.0f, ratio;
static float x = -10.0f, y = 0.09f, z = 0.0f;   // ī�޶� ��ġ
static float lx = 0.0f, ly = 0.0f, lz = 0.0f;   // ī�޶� �ü� ����

static int leftKey = 0;                 // ���� ����Ű�� ������ �ִ���
static int rightKey = 0;                // ������ ����Ű�� ������ �ִ���
static float angleDegree = 1;           // ȸ�� �ΰ���
float carAngle = 0;                     // �ڵ��� ����

static int groundSize = 10000;          // �ٴ� ũ��
static int buildingRow = 5;
static int buildingCol = 500;
static int buildingHeight = 4; 
static float roadLineWidth = 0.1;                  // ���μ� ��
static float roadLineLength = 10000;               // ���μ� ����


static int isMoving = 0;                // �ڵ��� �̵� ����
static int isPressing = 0;              // Ű�� ������ �ִ��� ����
static int movingDirection = 1;         // �̵� ���� (1: ����, -1: ����)
static float speed = 0.1f;              // �̵� �ӵ�
static float maxSpeed = 1.3f;           // �ִ� �ӵ�
static float acceleration = 0.0005f;    // ���ӵ�

GLuint groundList, roadList, buildingList;  // ���÷��� ����Ʈ


// ���� �ʹ� ���ؼ� ����� �Լ���
// ���÷��� ����Ʈ�� ��ü�Ͽ���
/*
// �ٴ��� �׸��� �Լ�
// �ٴ� ����� �Է¹޴´�
void drawGround(float groundSize) {
    glColor3f(0.1f, 0.1f, 0.1f);    // ȸ��
    glBegin(GL_QUADS);
    glVertex3f(-groundSize, 0.0, -groundSize);  // ���� �ϴ�
    glVertex3f(groundSize, 0.0, -groundSize);   // ���� �ϴ�
    glVertex3f(groundSize, 0.0, groundSize);    // ���� ���
    glVertex3f(-groundSize, 0.0, groundSize);   // ���� ���
    glEnd();
}


// ���θ� �׸��� �Լ�
// ���� ���� �Է¹޴´�
void drawRoad(float x, float z, float width) {
    glColor3f(1.0f, 0.8f, 0.0f);    
  
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(x, 0.01, -z);  // ���� �ϴ�
    glVertex3f(x + width, 0.01, -z);   // ���� �ϴ�
    glVertex3f(x + width, 0.01, z);    // ���� ���
    glVertex3f(x, 0.01, z);   // ���� ���
    glEnd();
    glPopMatrix();
}


// �ǹ��� �׸��� �Լ�
// x�� z��ǥ, ũ��, ���̸� �Է¹޴´�
void drawBuilding(float x, float z, float width, float height) {

    // �����ϰ� �ǹ� ���� ���� (��ǥ ���)
    float rg = fmod(fabs(sin(x * 0.05 + z * 0.05)), 0.1f);          // 0.0 ~ 0.1
    float b = 0.2f + fmod(fabs(cos(x * 0.05 + z * 0.05)), 0.05f);   // 0.2 ~ 0.25
    glColor3f(rg, rg, b);

    // �����ϰ� �ǹ� ���̸� ���� (��ǥ ���)
    srand((int)(x + z));                    // �� ��ǥ�� �׻� ������ ���� ����
    float randomHeight = (rand() % 5 + 1); // 1~5
    float newHeight = height + randomHeight;

    // �ǹ� ����
    glPushMatrix();
    glTranslatef(x, newHeight / 2, z);
    glScalef(width, newHeight, width);
    glutSolidCube(8.0);
    glPopMatrix();
}
*/


// ���÷��� ����Ʈ �ʱ�ȭ
void initDisplayLists() {
    // �ٴ� ���÷��� ����Ʈ ����
    groundList = glGenLists(1);
    glNewList(groundList, GL_COMPILE);
    glColor3f(0.1f, 0.1f, 0.1f);    // ȸ��
    glBegin(GL_QUADS);
    glVertex3f(-groundSize, 0.0, -groundSize);  // ���� �ϴ�
    glVertex3f(groundSize, 0.0, -groundSize);   // ���� �ϴ�
    glVertex3f(groundSize, 0.0, groundSize);    // ���� ���
    glVertex3f(-groundSize, 0.0, groundSize);   // ���� ���
    glEnd();
    glEndList();

    // ���� ���÷��� ����Ʈ ����
    roadList = glGenLists(1);
    glNewList(roadList, GL_COMPILE);
    float x = -15.0;
    glColor3f(1.0f, 0.8f, 0.0f);
    glBegin(GL_QUADS);
    glVertex3f(x, 0.01f, -roadLineLength);  // ���� �ϴ�
    glVertex3f(x + roadLineWidth, 0.01f, -roadLineLength);  // ���� �ϴ�
    glVertex3f(x + roadLineWidth, 0.01f, roadLineLength);   // ���� ���
    glVertex3f(x, 0.01f, roadLineLength);   // ���� ���
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);

    float whiteLineLoc = 2.0;
    float whiteLineGap = 4.0;
    float x2 = -15.0;

    for (int i = -4; i <= 4; i++) {
        if (i == 0) continue;
        for (float z = -roadLineLength; z < roadLineLength; z += whiteLineGap) { // ���� ���� �ݺ�
            glBegin(GL_QUADS);
            glVertex3f(i * whiteLineLoc - 15.0f, 0.02f, z);  // ���� �ϴ�
            glVertex3f(i * whiteLineLoc - 15.0f + roadLineWidth, 0.02f, z);  // ���� �ϴ�
            glVertex3f(i * whiteLineLoc - 15.0f + roadLineWidth, 0.02f, z + 2.0f);   // ���� ���
            glVertex3f(i * whiteLineLoc - 15.0f, 0.02f, z + 2.0f);   // ���� ���
            glEnd();
        }
    }
    glEndList();

    

    // �ǹ� ���÷��� ����Ʈ ����
    buildingList = glGenLists(1);
    glNewList(buildingList, GL_COMPILE);
    for (int i = -buildingRow; i <= buildingRow; ++i) {
        for (int j = -buildingCol; j <= buildingCol; ++j) {
            float x = i * 30;
            float z = j * 20;

            // ���� ���� ����
            float rg = fmod(fabs(sin(x * 0.05 + z * 0.05)), 0.1f);          // 0.0 ~ 0.1
            float b = 0.2f + fmod(fabs(cos(x * 0.05 + z * 0.05)), 0.05f);   // 0.2 ~ 0.25
            glColor3f(rg, rg, b);

            srand((int)(x + z));                  
            float randomHeight = (rand() % 5 + 1); // 1~5
            float newHeight = buildingHeight + randomHeight;

            glPushMatrix();
            glTranslatef(x, newHeight / 2, z);
            glScalef(1, newHeight, 1);
            glutSolidCube(8.0);
            glPopMatrix();
        }
    }
    glEndList();
}


// �ڵ����� �׸��� �Լ�
void drawCar() {

    float carX = x + lx;
    float carY = y + ly;
    float carZ = z + lz;

    glPushMatrix();
        glTranslatef(carX, carY, carZ);
        glRotatef(carAngle, 0.0f, 1.0f, 0.0f);

        // ��ü ����
        glColor3f(1.0f, 0.8f, 0.0f);    // �����
        glPushMatrix();
            glScalef(0.8f, 1.0f, 1.2f);
            glutSolidCube(1.0);
        glPopMatrix();
                 
        // â�� ���� (��)
        glColor3f(0.3f, 0.7f, 1.0f);    // �ϴû�
        glPushMatrix();
            glTranslatef(-0.18f, 0.32f, -0.555f);
            glScalef(0.31f, 0.25f, 0.1f);
            glutSolidCube(1.0);
            glTranslatef(1.18f, 0.0f, 0.0f);
            glutSolidCube(1.0);
        glPopMatrix();

        // â�� ���� (��)
        glColor3f(0.3f, 0.7f, 1.0f);    // �ϴû�
        glPushMatrix();
            glTranslatef(0.0f, 0.32f, 0.555f);
            glScalef(0.6f, 0.25f, 0.1f);
            glutSolidCube(1.0);
        glPopMatrix();

        // â�� ���� (���� �Ĺ�)
        glPushMatrix();
            glTranslatef(0.0f, 0.32f, 0.25f);
            glScalef(0.805f, 0.25f, 0.5f);
            glutSolidCube(1.0);
        glPopMatrix();
   
        // â�� ���� (���� ����)
        glPushMatrix();
            glTranslatef(0.0f, 0.32f, -0.3f);
            glScalef(0.805f, 0.25f, 0.3f);
            glutSolidCube(1.0);
        glPopMatrix();
    glPopMatrix();
}


// ī�޶� �����ϴ� �Լ���
void updateCamera(int cameraView[]) {

    glLoadIdentity();           // ���� ��-�� ����� �ʱ�ȭ

    // �ڵ����� ������ �������� ī�޶� ��ġ�� ���
    float offsetX = x + cameraView[2] * lx + cameraView[0] * (-lz); // ��ǥ �������� ī�޶� ��ġ ����
    float offsetY = y + cameraView[1];
    float offsetZ = z + cameraView[2] * lz + cameraView[0] * lx;    // ��ǥ �������� ī�޶� ��ġ ����
                    

    gluLookAt(
        offsetX, offsetY, offsetZ,                // ī�޶� ��ġ
        x + lx, y + ly, z + lz, // ī�޶� �ü� ����
        0.0f, 1.0f, 0.0f);
}


void drawMap() {
    /*
    drawGround(groundSize); // �ٴ� ������

    int buildingRow = 2;
    int buildingCol = 300;
    for (int i = -buildingRow; i <= buildingRow; ++i) { // �ǹ� ������
        for (int j = -buildingCol; j <= buildingCol; ++j) {
            drawBuilding(i * 30, j * 20, 1, 4);
            drawRoad(-15.0, 100.0, 0.1);     // ���� ������
        }
    }
    */

    glCallList(groundList);   // �ٴ�
    glCallList(roadList);     // ����
    glCallList(buildingList); // �ǹ�
}


// ���� ������ �����ϴ� �Լ�
void MyReshape(int x, int y, int width, int height, int fov, int cameraView[]) {
    glViewport(x, y, width, height); // ��ü â ũ��
    ratio = 1.0f * width / height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, ratio, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
    updateCamera(cameraView);
}



// OpenGL ���÷��� �ݹ� �Լ�
void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ���� �ʱ�ȭ

    // â ũ��: 1280x720

    int cameraView[5][3] = {
        {0,1,-10},     // �Ĺ漦 
        {0,1,10},     // ���漦
        {5,1,1},     // �������鼦
        {-5,1,1},     // �������鼦
        {0,20,0} };   // ��ܼ�

    // �ϴܺ� (�Ĺ漦)
    MyReshape(250, 0, 780, 150, 30, cameraView[0]);
    drawMap();
    drawCar();

    // ��ܺ� (���漦)
    MyReshape(250, 570, 780, 150, 30, cameraView[1]);
    drawMap();
    drawCar();

    // ����� (�������鼦)
    MyReshape(0, 0, 250, 720, 80, cameraView[2]);
    drawMap();
    drawCar();

    // ����� (�������鼦)
    MyReshape(1030, 0, 250, 720, 80, cameraView[3]);
    drawMap();
    drawCar();

    // �߾Ӻ� (��ܼ�)
    MyReshape(250, 150, 780, 420, 80, cameraView[4]);
    drawMap();
    drawCar();

    glutSwapBuffers();  // ���� ��ȯ
}


// ī�޶� �þ� ���� ������Ʈ �Լ�
// ������ �Է¹޴´�
void setTurningAngle(float ang) {
    // ���� ������Ʈ
    lx = sin(ang);
    lz = -cos(ang);

    carAngle = -atan2(lx, -lz) * (180.0f / PI);
}


// �ڵ����� ���� ��ȯ ó�� �Լ�
void setDirection() {
    float angleDeg = 0.004f;
    // ��ȸ�� Ű�� ���� ���
    if (isMoving) {
        if (leftKey) {
            if (speed > 0.0f) {
                if (isMoving == 1)
                    angle -= angleDeg * angleDegree;
                else
                    angle += angleDeg * angleDegree;

                setTurningAngle(angle);
            }
        }

        // ��ȸ�� Ű�� ���� ���
        if (rightKey) {
            if (speed > 0.0f) {
                if (isMoving == 1)
                    angle += angleDeg * angleDegree;
                else
                    angle -= angleDeg * angleDegree;

                setTurningAngle(angle);
            }
        }
    }
   
}


// �ڵ����� �ӵ��� ������Ű�� �Լ�
void speedUp() {
    if (speed < maxSpeed)
        speed += acceleration;
    printf("speed= %.2f\n", speed);
}


// ������ ó���ϴ� �Լ�
// ���� ����� �Է¹޴´�
void slowDown(int degree) {
    if (speed > 0.01f) {
        speed -= acceleration * degree * 0.5f;
        if (speed < 0.01f) { // �ּ� �ӵ� ����
            speed = 0.01f;
        }
    }
    printf("speed= %.2f\n", speed);
}


// �ڵ����� �̵� ������ ó���ϴ� �Լ�
void moving() {
    // ���� ����
    setDirection();

    // ���� �Ǵ� ���� ó��
    if (isPressing) {
        if (isMoving == movingDirection) {
            speedUp();
        }
        else {
            slowDown(6); // �극��ũ ȿ��
            if (speed <= 0.1f) {
                isMoving = movingDirection; // ���� ��ȯ
            }
        }
    }
    else {
        slowDown(2); // ����
    }

    // ��ġ ������Ʈ
    if (speed > 0.0) {
        x += lx * speed * isMoving;
        z += lz * speed * isMoving;
    }
    else {
        isMoving = 0;
    }

    glutPostRedisplay();
}


// Ư�� Ű �Է��� ó���ϴ� �Լ� (����Ű)
void inputKey(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:       // ����
        isPressing = 1;     // ����ڰ� Ű�� ������ �ִ� ����
        movingDirection = 1;// �ڵ����� �����ϰ� �ִ�
        break;

    case GLUT_KEY_DOWN:     // ����
        isPressing = 1;
        movingDirection = -1;
        break;

    case GLUT_KEY_LEFT:     // ��ȸ��
        leftKey = 1;
        break;

    case GLUT_KEY_RIGHT:    // ��ȸ��
        rightKey = 1;
        break;
    }
}


// Ư�� Ű ������ ó���ϴ� �Լ� (����Ű)
void releaseKey(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:        // ������ Ű ���� �� ��Ȱ��ȭ
    case GLUT_KEY_DOWN:
        isPressing = 0;
        break;

    case GLUT_KEY_LEFT:     // ��ȸ�� Ű ���� �� ȸ�� ���� �ʱ�ȭ
        leftKey = 0;
        break;

    case GLUT_KEY_RIGHT:    // ��ȸ�� Ű ���� �� ȸ�� ���� �ʱ�ȭ
        rightKey = 0;
        break;
    }
}


// �ڵ����� ȸ�� �ΰ����� �����ϴ� �Լ�
void angleDeg(unsigned char key, int x, int y) {
    switch (key) {
    case 'w':
        angleDegree = 1.5;
        break;
    case 's':
        angleDegree = 0.2;
        break;
    case 32: // Space (�극��ũ ����)
        if (speed < 0.05)
            isMoving = 0;
        break;
    }
}


// �ڵ����� ȸ�� �ΰ����� �ʱ�ȭ�ϴ� �Լ�
void releaseAngleDeg(unsigned char key, int x, int y) {
    switch (key) {
    case 'w':
        angleDegree = 1;
        break;
    case 's':
        angleDegree = 1;
    }
}


// ���� �Լ�
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);          // ������ â ũ��
    glutInitWindowPosition(730, 200);
    glutCreateWindow("3D �ڵ��� ����");     // ������ â ����

    glClearColor(0.5f, 0.7f, 1.0f, 1.0f);   // �ϴû�
    glEnable(GL_DEPTH_TEST);

    // �ݹ� �Լ� ���
    glutDisplayFunc(MyDisplay);

    initDisplayLists(); // ���÷��� ����Ʈ �ʱ�ȭ

    // Ű �Է� ó��
    glutSpecialFunc(inputKey);
    glutSpecialUpFunc(releaseKey);
    glutKeyboardFunc(angleDeg);
    glutKeyboardUpFunc(releaseAngleDeg);

    glutPostRedisplay(); // �ʱ� ȭ�� ���� ������ ��û

    // �ڵ��� �̵� �� ȭ�� ���� ó��
    glutIdleFunc(moving);

    glutMainLoop();
    return 0;
}