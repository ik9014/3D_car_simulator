#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdio.h>

const double PI = 3.14159265;

static float angle = 0.0f, ratio;
static float x = -10.0f, y = 0.09f, z = 0.0f;   // 카메라 위치
static float lx = 0.0f, ly = 0.0f, lz = 0.0f;   // 카메라 시선 방향

static int leftKey = 0;                 // 왼쪽 방향키를 누르고 있는지
static int rightKey = 0;                // 오른쪽 방향키를 누르고 있는지
static float angleDegree = 1;           // 회전 민감도
float carAngle = 0;                     // 자동차 방향

static int groundSize = 10000;          // 바닥 크기
static int buildingRow = 5;
static int buildingCol = 500;
static int buildingHeight = 4; 
static float roadLineWidth = 0.1;                  // 도로선 폭
static float roadLineLength = 10000;               // 도로선 길이


static int isMoving = 0;                // 자동차 이동 여부
static int isPressing = 0;              // 키를 누르고 있는지 여부
static int movingDirection = 1;         // 이동 방향 (1: 전진, -1: 후진)
static float speed = 0.1f;              // 이동 속도
static float maxSpeed = 1.3f;           // 최대 속도
static float acceleration = 0.0005f;    // 가속도

GLuint groundList, roadList, buildingList;  // 디스플레이 리스트


// 렉이 너무 심해서 폐기한 함수들
// 디스플레이 리스트로 대체하였음
/*
// 바닥을 그리는 함수
// 바닥 사이즈를 입력받는다
void drawGround(float groundSize) {
    glColor3f(0.1f, 0.1f, 0.1f);    // 회색
    glBegin(GL_QUADS);
    glVertex3f(-groundSize, 0.0, -groundSize);  // 좌측 하단
    glVertex3f(groundSize, 0.0, -groundSize);   // 우측 하단
    glVertex3f(groundSize, 0.0, groundSize);    // 우측 상단
    glVertex3f(-groundSize, 0.0, groundSize);   // 좌측 상단
    glEnd();
}


// 도로를 그리는 함수
// 도로 폭을 입력받는다
void drawRoad(float x, float z, float width) {
    glColor3f(1.0f, 0.8f, 0.0f);    
  
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(x, 0.01, -z);  // 좌측 하단
    glVertex3f(x + width, 0.01, -z);   // 우측 하단
    glVertex3f(x + width, 0.01, z);    // 우측 상단
    glVertex3f(x, 0.01, z);   // 좌측 상단
    glEnd();
    glPopMatrix();
}


// 건물을 그리는 함수
// x와 z좌표, 크기, 높이를 입력받는다
void drawBuilding(float x, float z, float width, float height) {

    // 랜덤하게 건물 색을 지정 (좌표 기반)
    float rg = fmod(fabs(sin(x * 0.05 + z * 0.05)), 0.1f);          // 0.0 ~ 0.1
    float b = 0.2f + fmod(fabs(cos(x * 0.05 + z * 0.05)), 0.05f);   // 0.2 ~ 0.25
    glColor3f(rg, rg, b);

    // 랜덤하게 건물 높이를 지정 (좌표 기반)
    srand((int)(x + z));                    // 각 좌표서 항상 동일한 난수 생성
    float randomHeight = (rand() % 5 + 1); // 1~5
    float newHeight = height + randomHeight;

    // 건물 생성
    glPushMatrix();
    glTranslatef(x, newHeight / 2, z);
    glScalef(width, newHeight, width);
    glutSolidCube(8.0);
    glPopMatrix();
}
*/


// 디스플레이 리스트 초기화
void initDisplayLists() {
    // 바닥 디스플레이 리스트 생성
    groundList = glGenLists(1);
    glNewList(groundList, GL_COMPILE);
    glColor3f(0.1f, 0.1f, 0.1f);    // 회색
    glBegin(GL_QUADS);
    glVertex3f(-groundSize, 0.0, -groundSize);  // 좌측 하단
    glVertex3f(groundSize, 0.0, -groundSize);   // 우측 하단
    glVertex3f(groundSize, 0.0, groundSize);    // 우측 상단
    glVertex3f(-groundSize, 0.0, groundSize);   // 좌측 상단
    glEnd();
    glEndList();

    // 도로 디스플레이 리스트 생성
    roadList = glGenLists(1);
    glNewList(roadList, GL_COMPILE);
    float x = -15.0;
    glColor3f(1.0f, 0.8f, 0.0f);
    glBegin(GL_QUADS);
    glVertex3f(x, 0.01f, -roadLineLength);  // 좌측 하단
    glVertex3f(x + roadLineWidth, 0.01f, -roadLineLength);  // 우측 하단
    glVertex3f(x + roadLineWidth, 0.01f, roadLineLength);   // 우측 상단
    glVertex3f(x, 0.01f, roadLineLength);   // 좌측 상단
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);

    float whiteLineLoc = 2.0;
    float whiteLineGap = 4.0;
    float x2 = -15.0;

    for (int i = -4; i <= 4; i++) {
        if (i == 0) continue;
        for (float z = -roadLineLength; z < roadLineLength; z += whiteLineGap) { // 간격 포함 반복
            glBegin(GL_QUADS);
            glVertex3f(i * whiteLineLoc - 15.0f, 0.02f, z);  // 좌측 하단
            glVertex3f(i * whiteLineLoc - 15.0f + roadLineWidth, 0.02f, z);  // 우측 하단
            glVertex3f(i * whiteLineLoc - 15.0f + roadLineWidth, 0.02f, z + 2.0f);   // 우측 상단
            glVertex3f(i * whiteLineLoc - 15.0f, 0.02f, z + 2.0f);   // 좌측 상단
            glEnd();
        }
    }
    glEndList();

    

    // 건물 디스플레이 리스트 생성
    buildingList = glGenLists(1);
    glNewList(buildingList, GL_COMPILE);
    for (int i = -buildingRow; i <= buildingRow; ++i) {
        for (int j = -buildingCol; j <= buildingCol; ++j) {
            float x = i * 30;
            float z = j * 20;

            // 랜덤 색상 지정
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


// 자동차를 그리는 함수
void drawCar() {

    float carX = x + lx;
    float carY = y + ly;
    float carZ = z + lz;

    glPushMatrix();
        glTranslatef(carX, carY, carZ);
        glRotatef(carAngle, 0.0f, 1.0f, 0.0f);

        // 차체 생성
        glColor3f(1.0f, 0.8f, 0.0f);    // 노란색
        glPushMatrix();
            glScalef(0.8f, 1.0f, 1.2f);
            glutSolidCube(1.0);
        glPopMatrix();
                 
        // 창문 생성 (앞)
        glColor3f(0.3f, 0.7f, 1.0f);    // 하늘색
        glPushMatrix();
            glTranslatef(-0.18f, 0.32f, -0.555f);
            glScalef(0.31f, 0.25f, 0.1f);
            glutSolidCube(1.0);
            glTranslatef(1.18f, 0.0f, 0.0f);
            glutSolidCube(1.0);
        glPopMatrix();

        // 창문 생성 (뒤)
        glColor3f(0.3f, 0.7f, 1.0f);    // 하늘색
        glPushMatrix();
            glTranslatef(0.0f, 0.32f, 0.555f);
            glScalef(0.6f, 0.25f, 0.1f);
            glutSolidCube(1.0);
        glPopMatrix();

        // 창문 생성 (측면 후방)
        glPushMatrix();
            glTranslatef(0.0f, 0.32f, 0.25f);
            glScalef(0.805f, 0.25f, 0.5f);
            glutSolidCube(1.0);
        glPopMatrix();
   
        // 창문 생성 (측면 전방)
        glPushMatrix();
            glTranslatef(0.0f, 0.32f, -0.3f);
            glScalef(0.805f, 0.25f, 0.3f);
            glutSolidCube(1.0);
        glPopMatrix();
    glPopMatrix();
}


// 카메라를 갱신하는 함수다
void updateCamera(int cameraView[]) {

    glLoadIdentity();           // 현재 모델-뷰 행렬을 초기화

    // 자동차의 방향을 기준으로 카메라 위치를 계산
    float offsetX = x + cameraView[2] * lx + cameraView[0] * (-lz); // 좌표 기준으로 카메라 위치 조정
    float offsetY = y + cameraView[1];
    float offsetZ = z + cameraView[2] * lz + cameraView[0] * lx;    // 좌표 기준으로 카메라 위치 조정
                    

    gluLookAt(
        offsetX, offsetY, offsetZ,                // 카메라 위치
        x + lx, y + ly, z + lz, // 카메라 시선 방향
        0.0f, 1.0f, 0.0f);
}


void drawMap() {
    /*
    drawGround(groundSize); // 바닥 렌더링

    int buildingRow = 2;
    int buildingCol = 300;
    for (int i = -buildingRow; i <= buildingRow; ++i) { // 건물 렌더링
        for (int j = -buildingCol; j <= buildingCol; ++j) {
            drawBuilding(i * 30, j * 20, 1, 4);
            drawRoad(-15.0, 100.0, 0.1);     // 도로 렌더링
        }
    }
    */

    glCallList(groundList);   // 바닥
    glCallList(roadList);     // 도로
    glCallList(buildingList); // 건물
}


// 원근 투영을 설정하는 함수
void MyReshape(int x, int y, int width, int height, int fov, int cameraView[]) {
    glViewport(x, y, width, height); // 전체 창 크기
    ratio = 1.0f * width / height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, ratio, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
    updateCamera(cameraView);
}



// OpenGL 디스플레이 콜백 함수
void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 버퍼 초기화

    // 창 크기: 1280x720

    int cameraView[5][3] = {
        {0,1,-10},     // 후방샷 
        {0,1,10},     // 전방샷
        {5,1,1},     // 좌측측면샷
        {-5,1,1},     // 우측측면샷
        {0,20,0} };   // 상단샷

    // 하단부 (후방샷)
    MyReshape(250, 0, 780, 150, 30, cameraView[0]);
    drawMap();
    drawCar();

    // 상단부 (전방샷)
    MyReshape(250, 570, 780, 150, 30, cameraView[1]);
    drawMap();
    drawCar();

    // 측면부 (좌측측면샷)
    MyReshape(0, 0, 250, 720, 80, cameraView[2]);
    drawMap();
    drawCar();

    // 측면부 (우측측면샷)
    MyReshape(1030, 0, 250, 720, 80, cameraView[3]);
    drawMap();
    drawCar();

    // 중앙부 (상단샷)
    MyReshape(250, 150, 780, 420, 80, cameraView[4]);
    drawMap();
    drawCar();

    glutSwapBuffers();  // 버퍼 교환
}


// 카메라 시야 방향 업데이트 함수
// 각도를 입력받는다
void setTurningAngle(float ang) {
    // 방향 업데이트
    lx = sin(ang);
    lz = -cos(ang);

    carAngle = -atan2(lx, -lz) * (180.0f / PI);
}


// 자동차의 방향 전환 처리 함수
void setDirection() {
    float angleDeg = 0.004f;
    // 좌회전 키가 눌린 경우
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

        // 우회전 키가 눌린 경우
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


// 자동차의 속도를 증가시키는 함수
void speedUp() {
    if (speed < maxSpeed)
        speed += acceleration;
    printf("speed= %.2f\n", speed);
}


// 감속을 처리하는 함수
// 감속 계수를 입력받는다
void slowDown(int degree) {
    if (speed > 0.01f) {
        speed -= acceleration * degree * 0.5f;
        if (speed < 0.01f) { // 최소 속도 유지
            speed = 0.01f;
        }
    }
    printf("speed= %.2f\n", speed);
}


// 자동차의 이동 로직을 처리하는 함수
void moving() {
    // 방향 설정
    setDirection();

    // 가속 또는 감속 처리
    if (isPressing) {
        if (isMoving == movingDirection) {
            speedUp();
        }
        else {
            slowDown(6); // 브레이크 효과
            if (speed <= 0.1f) {
                isMoving = movingDirection; // 방향 전환
            }
        }
    }
    else {
        slowDown(2); // 감속
    }

    // 위치 업데이트
    if (speed > 0.0) {
        x += lx * speed * isMoving;
        z += lz * speed * isMoving;
    }
    else {
        isMoving = 0;
    }

    glutPostRedisplay();
}


// 특수 키 입력을 처리하는 함수 (방향키)
void inputKey(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:       // 전진
        isPressing = 1;     // 사용자가 키를 누르고 있는 상태
        movingDirection = 1;// 자동차가 전진하고 있다
        break;

    case GLUT_KEY_DOWN:     // 후진
        isPressing = 1;
        movingDirection = -1;
        break;

    case GLUT_KEY_LEFT:     // 좌회전
        leftKey = 1;
        break;

    case GLUT_KEY_RIGHT:    // 우회전
        rightKey = 1;
        break;
    }
}


// 특수 키 해제를 처리하는 함수 (방향키)
void releaseKey(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:        // 전후진 키 해제 시 비활성화
    case GLUT_KEY_DOWN:
        isPressing = 0;
        break;

    case GLUT_KEY_LEFT:     // 좌회전 키 해제 시 회전 상태 초기화
        leftKey = 0;
        break;

    case GLUT_KEY_RIGHT:    // 우회전 키 해제 시 회전 상태 초기화
        rightKey = 0;
        break;
    }
}


// 자동차의 회전 민감도를 조작하는 함수
void angleDeg(unsigned char key, int x, int y) {
    switch (key) {
    case 'w':
        angleDegree = 1.5;
        break;
    case 's':
        angleDegree = 0.2;
        break;
    case 32: // Space (브레이크 역할)
        if (speed < 0.05)
            isMoving = 0;
        break;
    }
}


// 자동차의 회전 민감도를 초기화하는 함수
void releaseAngleDeg(unsigned char key, int x, int y) {
    switch (key) {
    case 'w':
        angleDegree = 1;
        break;
    case 's':
        angleDegree = 1;
    }
}


// 메인 함수
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);          // 윈도우 창 크기
    glutInitWindowPosition(730, 200);
    glutCreateWindow("3D 자동차 게임");     // 윈도우 창 제목

    glClearColor(0.5f, 0.7f, 1.0f, 1.0f);   // 하늘색
    glEnable(GL_DEPTH_TEST);

    // 콜백 함수 등록
    glutDisplayFunc(MyDisplay);

    initDisplayLists(); // 디스플레이 리스트 초기화

    // 키 입력 처리
    glutSpecialFunc(inputKey);
    glutSpecialUpFunc(releaseKey);
    glutKeyboardFunc(angleDeg);
    glutKeyboardUpFunc(releaseAngleDeg);

    glutPostRedisplay(); // 초기 화면 강제 렌더링 요청

    // 자동차 이동 및 화면 갱신 처리
    glutIdleFunc(moving);

    glutMainLoop();
    return 0;
}