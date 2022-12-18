#include <stdlib.h>
#include <GL/glut.h>
#include <gl/GLAUX.H>
#include<iostream>
#include<time.h>

#pragma comment(lib, "glaux.lib")
#pragma comment (lib,"legacy_stdio_definitions.lib")

using namespace std;

// 물체 색깔 및 윤기
const GLfloat brown[] = { 0.4, 0.1, 0.1, 1.0 };
const GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };
const GLfloat orange[] = { 0.9, 0.6, 0.3, 1.0 };
const GLfloat polished[] = { 100 };

float width = 800, height = 600; // 처음 윈도우 크기
float mx=0, my=0; // 마우스 x, y좌표
int m1=0, m2=0, m3=0, m4=0, m5=0, m6=0, m7=0, m8=0, m9=0; // 두더지 y축 이동
bool move1 = false, move2 = false, move3 = false, move4 = false, move5 = false, move6 = false, move7 = false, move8 = false, move9 = false;
bool fail; // 두더지 잡기 실패 여부
bool timer_random; // 랜덤함수 타이머가 작동할 수 있는지 여부
double time_init; // 기준시간
int p, q; // 두더지 좌표

unsigned int MyTextureObject[2]; // 텍스쳐 객체
AUX_RGBImageRec* pTextureImage[2]; // 텍스쳐 저장 공간을 가리키는 포인터

AUX_RGBImageRec* LoadBMP(char* szFilename) { // BMP파일 로드

    FILE* pFile = NULL;
    if (!szFilename) {
        return NULL;
    }
    fopen_s(&pFile, szFilename, "r");

    // seFilename을 유니코드 스트링으로 바꾸기
    int size = strlen(szFilename);
    wchar_t* w_szFilename = new wchar_t[size];

    for (int i = 0; i < size; ++i)  w_szFilename[i] = szFilename[i];
    w_szFilename[size] = NULL;

    if (pFile) {
        fclose(pFile);
        return auxDIBImageLoad(w_szFilename); // 파일로부터 메모리로
        // auxDIBImageLoad() : 인자타입 => LPCWSTR : const 인 16 - bit UNICODE character 의 포인터
        // LPSTR - Long Point String의 약자로, 실제 C언어 상으로는 char * 
        // LPCWSTR - Long Point Contant Wide String의 약자로, 실제 C언어 상으로 wchar_t const * 	
    }
    return NULL;

}
int LoadGLTextures(char* szFilePath, char* szFilePath2) { // 파일을 로드하고 텍스쳐로 변환
    int Status = FALSE;
    glClearColor(0.0, 0.0, 0.0, 0.5); // 초기화할 색 설정
    memset(pTextureImage, 0, sizeof(void*) * 1); // 포인터 비움

    if (pTextureImage[0] = LoadBMP(szFilePath)) { // 비트맵을 로드하고 오류확인
        Status = TRUE; // 오류없으면 상태 True
        glGenTextures(1, &MyTextureObject[0]); // 텍스쳐 생성
        glBindTexture(GL_TEXTURE_2D, MyTextureObject[0]); // 텍스쳐 객체 생성
        // 텍스쳐메모리를 텍스쳐 모듈에서 사용할 수 있는 형태로 변환하여 텍스춰 메모리에 올려줌
        glTexImage2D(GL_TEXTURE_2D, 0, 3,
            pTextureImage[0]->sizeX, pTextureImage[0]->sizeY,
            0, GL_RGB, GL_UNSIGNED_BYTE, pTextureImage[0]->data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 축소할 때 흐리게 처리
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // 확대할 때 선명하게 처리
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL); // 텍스쳐 덧칠함
    }

    if (pTextureImage[1] = LoadBMP(szFilePath2)) {
        Status = TRUE;
        glGenTextures(1, &MyTextureObject[1]);
        glBindTexture(GL_TEXTURE_2D, MyTextureObject[1]);
        glTexImage2D(GL_TEXTURE_2D, 0, 3,
            pTextureImage[1]->sizeX, pTextureImage[1]->sizeY,
            0, GL_RGB, GL_UNSIGNED_BYTE, pTextureImage[1]->data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    }
    if (pTextureImage[0] && pTextureImage[1]) { // 텍스쳐가 존재하면
        if (pTextureImage[0]->data) { // 텍스쳐 영상이 존재하면
            free(pTextureImage[0]->data); // 텍스쳐 영상공간 반납
        }
        free(pTextureImage[0]); // 텍스쳐 반납
        if (pTextureImage[1]->data) { // 텍스쳐 영상이 존재하면
            free(pTextureImage[1]->data); // 텍스쳐 영상공간 반납
        }
        free(pTextureImage[1]); // 텍스쳐 반납
    }

    return Status;
}
void RandomMove() { // 올라올 두더지 정함
    double time_current = (double)clock() / 1000; // 현재시간 초로 변환
    if ((time_current - time_init) >= 2) { // 2초 지나면
        srand(time(NULL)); // seed값으로 현재시간 부여
        int move = (rand() % 9) + 1; // 1~9
        switch (move) { // 어떤 두더지를 움직일지 정함
        case 1: {
            move1 = true;
            break;
        }case 2: {
            move2 = true;
            break;
        }case 3: {
            move3 = true;
            break;
        }case 4: {
            move4 = true;
            break;
        }case 5: {
            move5 = true;
            break;
        }case 6: {
            move6 = true;
            break;
        }case 7: {
            move7 = true;
            break;
        }case 8: {
            move8 = true;
            break;
        }case 9: {
            move9 = true;
            break;
        }
        }
        fail = true; // 두더지를 못잡으면 바로 끝내도록 미리 초기화
    }
}
void DrawMole(float x, float y, float z) { // 두더지 그리기
    GLUquadric* qobj = gluNewQuadric(); // gluQuadric객체를 사용하기 위해

    glPushMatrix(); // ctm1
    glMaterialfv(GL_FRONT, GL_DIFFUSE, brown); // 난반사 -> 갈색
    glMaterialfv(GL_FRONT, GL_SHININESS, polished); // 윤기나게
    gluQuadricDrawStyle(qobj, GLU_FILL); // 색 채움
    glTranslatef(x, y, z); // 두더지 위치, 움직임
    glRotatef(90, 1, 0, 0); // 몸통 방향 설정
    gluCylinder(qobj, 8, 8, 8, 60, 20); // 몸통
    gluSphere(qobj, 8, 100, 100); // 머리

    glPushMatrix(); // ctm2
    glTranslatef(-3, 7, -4);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
    glMaterialfv(GL_FRONT, GL_SHININESS, polished);
    gluSphere(qobj, 1, 50, 50); // 왼쪽 눈
    glPopMatrix(); // ctm1

    glPushMatrix(); // ctm2
    glTranslatef(3, 7, -4);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
    glMaterialfv(GL_FRONT, GL_SHININESS, polished);
    gluSphere(qobj, 1, 50, 50); // 오른쪽 눈
    glPopMatrix(); // ctm1

    glPushMatrix(); // ctm2
    glTranslatef(0, 8, 0);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, orange);
    glMaterialfv(GL_FRONT, GL_SHININESS, polished);
    gluSphere(qobj, 1, 50, 50); // 코
    glPopMatrix(); // ctm1
    glPopMatrix(); // ctm0
}
void DrawLand() {
    // 두더지가 숨는 땅1
    glBindTexture(GL_TEXTURE_2D, MyTextureObject[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-30, 0, 30); //앞면
    glTexCoord2f(1.0f, 0.0f); glVertex3f(30, 0, 30);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(30, 6, 30);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-30, 6, 30);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-30, 0, -30); //뒷면
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-30, 6, -30);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(30, 6, -30);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(30, 0, -30);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-30, 6, -30); //윗면
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-30, 6, 30);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(30, 6, 30);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(30, 6, -30);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-30, 0, -30); //아랫면
    glTexCoord2f(0.0f, 1.0f); glVertex3f(30, 0, -30);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(30, 0, 30);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-30, 0, 30);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(30, 0, -30); //우측면
    glTexCoord2f(1.0f, 1.0f); glVertex3f(30, 6, -30);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(30, 6, 30);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(30, 0, 30);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-30, 0, -30); //좌측면
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-30, 0, 30);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-30, 6, 30);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-30, 6, -30);
    glEnd();

    // 두더지가 숨는 땅2
    glBindTexture(GL_TEXTURE_2D, MyTextureObject[1]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-30, -6, 30); //앞면
    glTexCoord2f(1.0f, 0.0f); glVertex3f(30, -6, 30);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(30, 0, 30);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-30, 0, 30);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-30, -6, -30); //뒷면
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-30, 0, -30);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(30, 0, -30);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(30, -6, -30);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-30, 0, -30); //윗면
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-30, 0, 30);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(30, 0, 30);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(30, 0, -30);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-30, -6, -30); //아랫면
    glTexCoord2f(0.0f, 1.0f); glVertex3f(30, -6, -30);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(30, -6, 30);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-30, -6, 30);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(30, -6, -30); //우측면
    glTexCoord2f(1.0f, 1.0f); glVertex3f(30, 0, -30);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(30, 0, 30);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(30, -6, 30);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-30, -6, -30); //좌측면
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-30, -6, 30);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-30, 0, 30);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-30, 0, -30);
    glEnd();
}
void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 색, 깊이 버퍼를 비움

    glMatrixMode(GL_MODELVIEW); // 모델뷰 행렬 사용
    glLoadIdentity(); // ctm0 // 행렬(ctm) 초기화
    gluLookAt(0, 40, 80, 0, 0, 0, 0, 1, -1); // 시점 설정

    glEnable(GL_TEXTURE_2D); // 텍스쳐기능 활성화

    DrawLand(); // 땅 그림

    glDisable(GL_TEXTURE_2D); // 텍스쳐 매핑 비활성화

    // 두더지 그림
    DrawMole(-19, m1, -19);
    DrawMole(0, m2, -19); // 가운데
    DrawMole(19, m3, -19);
    DrawMole(0, m5, 0); // 정가운데
    DrawMole(-19,m4,0);
    DrawMole(19, m6, 0);
    DrawMole(-19, m7, 19);
    DrawMole(0, m8, 19); // 가운데
    DrawMole(19, m9, 19);

    glPopMatrix(); // ctm0
    glutSwapBuffers(); // 백버퍼 <-> 프론트버퍼 스왑(화면 출력)

    if (!move1 && !move2 && !move3 && !move4 && !move5 && !move6 && !move7 && !move8 && !move9)
        RandomMove(); // 튀어나올 두더지 랜덤설정
    else { // 두더지가 움직일 때 기준시간 설정
        if (timer_random) { // RandomMove 타이머가 작동할 수 있다면
            time_init = (double)clock() / 1000; // 초 단위로 변환
        }
    }
}
void MyReshape(int w, int h) {
    width = w, height = h; // 바뀐 너비, 높이 설정
    glViewport(0, 0, (GLsizei)w, (GLsizei)h); // 뷰폿 설정
    glMatrixMode(GL_PROJECTION); // 투영행렬 사용
    glLoadIdentity(); // 행렬 초기화
    // 화면 조정할 때 비율 고정
    gluPerspective(60, (GLfloat)w / (GLfloat)h, 1, 200); // 원근투영 // 왜곡 방지
}
void MyMouse(int button, int state, int x, int y) {
    if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) { // 왼쪽 마우스 클릭
        double time_current = (double)clock() / 1000; // 현재시간 초로 변환
        if (move1) { // 1번 두더지가 올라올 때
            // 해당 두더지를 클릭하면
            if (x >= 260 * (width / 800) && x <= 345 * (width / 800) && y >= 180 * (height / 600) && y <= 245 * (height / 600)){
                fail = false; // 두더지 잡기 성공
                cout << "반응속도: " << (time_current - time_init) << "초\n";
            }
        }
        else if (move2) {
            if (x >= 360 * (width / 800) && x <= 440 * (width / 800) && y >= 180 * (height / 600) && y <= 245 * (height / 600)) {
                fail = false;
                cout << "반응속도: " << (time_current - time_init) << "초\n";
            }
        }
        else if (move3) {
            if (x >= 455 * (width / 800) && x <= 540 * (width / 800) && y >= 180 * (height / 600) && y <= 245 * (height / 600)) {
                fail = false;
                cout << "반응속도: " << (time_current - time_init) << "초\n";
            }
        }
        else if (move4) {
            if (x >= 230 * (width / 800) && x <= 330 * (width / 800) && y >= 210 * (height / 600) && y <= 275 * (height / 600)) {
                fail = false;
                cout << "반응속도: " << (time_current - time_init) << "초\n";
            }
        }
        else if (move5) {
            if (x >= 350 * (width / 800) && x <= 445 * (width / 800) && y >= 210 * (height / 600) && y <= 285 * (height / 600)) {
                fail = false;
                cout << "반응속도: " << (time_current - time_init) << "초\n";
            }
        }
        else if (move6) {
            if (x >= 465 * (width / 800) && x <= 565 * (width / 800) && y >= 210 * (height / 600) && y <= 285 * (height / 600)) {
                fail = false;
                cout << "반응속도: " << (time_current - time_init) << "초\n";
            }
        }
        else if (move7) {
            if (x >= 190 * (width / 800) && x <= 315 * (width / 800) && y >= 250 * (height / 600) && y <= 355 * (height / 600)) {
                fail = false;
                cout << "반응속도: " << (time_current - time_init) << "초\n";
            }
        }
        else if (move8) {
            if (x >= 340 * (width / 800) && x <= 460 * (width / 800) && y >= 250 * (height / 600) && y <= 355 * (height / 600)) {
                fail = false;
                cout << "반응속도: " << (time_current - time_init) << "초\n";
            }
        }
        else if (move9) {
            if (x >= 480 * (width / 800) && x <= 605 * (width / 800) && y >= 250 * (height / 600) && y <= 355 * (height / 600)) {
                fail = false;
                cout << "반응속도: " << (time_current - time_init) << "초\n";
            }   
        }
    }
}
void CatchTimer(double time_init) {
    timer_random = false; // RandomMove()에서 타이머가 작동하지 못하도록함
    double time_current = (double)clock() / 1000; // 현재시간 초로 변환
    if ((time_current - time_init) >= 2) { // 2초 지나면
        timer_random = true; // RandomMove()에서 타이머가 작동할 수 있도록함
        if (fail) { // 못잡은 경우
            cout << "fail";
            exit(0); // 프로그램 종료
        }
    }
}
void MyTimer(int value) {
    if (move1) {
        if (fail) { // 두더지를 아직 잡지 않은 경우
            if (m1 <= 8) { // 두더지가 위로 올라오는 중
                if (m1 == 0){ // 두더지가 땅속에 있을 때
                    time_init = (double)clock() / 1000; // 기준시간 구함
                }
                m1++; // 두더지 위로 올라옴
            }
            else {
                CatchTimer(time_init); // 약 2초후에 두더지 잡았는지 체크
            }
        }
        else { // 두더지를 잡은 경우
            if (m1 > 0) { // 두더지가 내려가는 중
                m1--; // 두더지 내려감
            }
            else { // 다 내려갔으면
                move1 = false; // 내려간 이후에는 움직이지 않음
            }
        }
    }
    else if (move2) {
        if (fail) { // 두더지를 아직 잡지 않은 경우
            if (m2 <= 8) {
                if (m2 == 0) {
                    time_init = (double)clock() / 1000;
                }
                m2++;
            }
            else {
                CatchTimer(time_init); // 약 2초후에 두더지 잡았는지 체크
            }
        }
        else { // 두더지를 잡은 경우
            if (m2 > 0) { // 두더지 내려감
                m2--;
            }
            else { // 내려간 이후에는 움직이지 않음
                move2 = false;
            }
        }
    }
    else if (move3) {
        if (fail) { // 두더지를 아직 잡지 않은 경우
            if (m3 <= 8) {
                if (m3 == 0) {
                    time_init = (double)clock() / 1000;
                }
                m3++;
            }
            else {
                CatchTimer(time_init); // 약 2초후에 두더지 잡았는지 체크
            }
        }
        else { // 두더지를 잡은 경우
            if (m3 > 0) { // 두더지 내려감
                m3--;
            }
            else { // 내려간 이후에는 움직이지 않음
                move3 = false;
            }
        }
    }
    else if (move4) {
        if (fail) { // 두더지를 아직 잡지 않은 경우
            if (m4 <= 8) {
                if (m4 == 0) {
                    time_init = (double)clock() / 1000;
                }
                m4++;
            }
            else {
                CatchTimer(time_init); // 약 2초후에 두더지 잡았는지 체크
            }
        }
        else { // 두더지를 잡은 경우
            if (m4 > 0) { // 두더지 내려감
                m4--;
            }
            else { // 내려간 이후에는 움직이지 않음
                move4 = false;
            }
        }
    }
    else if (move5) {
        if (fail) { // 두더지를 아직 잡지 않은 경우
            if (m5 <= 8) {
                if (m5 == 0) {
                    time_init = (double)clock() / 1000;
                }
                m5++;
            }
            else {
                CatchTimer(time_init); // 약 2초후에 두더지 잡았는지 체크
            }
        }
        else { // 두더지를 잡은 경우
            if (m5 > 0) { // 두더지 내려감
                m5--;
            }
            else { // 내려간 이후에는 움직이지 않음
                move5 = false;
            }
        }
    }
    else if (move6) {
        if (fail) { // 두더지를 아직 잡지 않은 경우
            if (m6 <= 8) {
                if (m6 == 0) {
                    time_init = (double)clock() / 1000;
                }
                m6++;
            }
            else {
                CatchTimer(time_init); // 약 2초후에 두더지 잡았는지 체크
            }
        }
        else { // 두더지를 잡은 경우
            if (m6 > 0) { // 두더지 내려감
                m6--;
            }
            else { // 내려간 이후에는 움직이지 않음
                move6 = false;
            }
        }
    }
    else if (move7) {
        if (fail) { // 두더지를 아직 잡지 않은 경우
            if (m7 <= 8) {
                if (m7 == 0) {
                    time_init = (double)clock() / 1000;
                }
                m7++;
            }
            else {
                CatchTimer(time_init); // 약 2초후에 두더지 잡았는지 체크
            }
        }
        else { // 두더지를 잡은 경우
            if (m7 > 0) { // 두더지 내려감
                m7--;
            }
            else { // 내려간 이후에는 움직이지 않음
                move7 = false;
            }
        }
    }
    else if (move8) {
        if (fail) { // 두더지를 아직 잡지 않은 경우
            if (m8 <= 8) {
                if (m8 == 0) {
                    time_init = (double)clock() / 1000;
                }
                m8++;
            }
            else {
                CatchTimer(time_init); // 약 2초후에 두더지 잡았는지 체크
            }
        }
        else { // 두더지를 잡은 경우
            if (m8 > 0) { // 두더지 내려감
                m8--;
            }
            else { // 내려간 이후에는 움직이지 않음
                move8 = false;
            }
        }
    }
    else if (move9) {
        if (fail) { // 두더지를 아직 잡지 않은 경우
            if (m9 <= 8) {
                if (m9 == 0) {
                    time_init = (double)clock() / 1000;
                }
                m9++;
            }
            else {
                CatchTimer(time_init); // 약 2초후에 두더지 잡았는지 체크
            }
        }
        else { // 두더지를 잡은 경우
            if (m9 > 0) { // 두더지 내려감
                m9--;
            }
            else { // 내려간 이후에는 움직이지 않음
                move9 = false;
            }
        }
    }

    glutPostRedisplay(); // MyDisplay() 강제호출
    glutTimerFunc(20, MyTimer, 1); // 1 -> 타이머의 용도가 하나뿐이라면 아무값이나 주어도 상관은 없다.
    // 20msec 후에 호출
}
void InitLight() {
    // 0번 광원 특성
    GLfloat light0_ambient[] = { 1.0, 1.0, 1.0, 1.0 }; // 주변반사
    GLfloat light0_diffuse[] = { 1,1,1, 1.0 }; // 난반사
    GLfloat light0_position[] = { 0.0, 100.0, 0, 1.0 }; // 0번 광원위치
    GLfloat light0_direction[] = { 0.0, -1.0, 0.0 }; // 0번 광원 방향
    GLfloat light0_cutoff[] = { 100 }; // 0번 광원 각도

    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);	// 스포트라이트
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light0_direction); // 방향
    glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF, light0_cutoff); // 각도
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 1.0);	 // 코사인 승수

    glShadeModel(GL_SMOOTH); // 고러드 셰이딩
    glEnable(GL_DEPTH_TEST); // 깊이 버퍼 활성화
    glEnable(GL_LIGHTING); // 조명 활성화
    glEnable(GL_LIGHT0); // 0번 광원 활성화
    
    // 0번 광원 특성할당
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient); // 주변반사
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse); // 난반사
}
void MyInit(void) {
    glClearColor(0.0, 0.0, 0.0, 1.0); // 배경: 검정색
    time_init = (double)clock() / 1000; // 초 단위로 변환

    // 후면 및 은면제거 설정
    glEnable(GL_CULL_FACE); // 후면제거 활성화
    glFrontFace(GL_CCW); // 시계반대방향이 전면
    glCullFace(GL_BACK); // 후면 제거
    glEnable(GL_DEPTH_TEST); // 깊이버퍼 활성화 -> 은면제거
}
int main(int argc, char** argv) {
    const char* szFilename = "grass.bmp";
    const char* szFilename2 = "soil.bmp";

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // 더블 버퍼, RGB 버퍼, 깊이버퍼
    glutInitWindowSize(width, height); // 윈도우 크기
    glutInitWindowPosition(500, 200); // 윈도우 위치
    glutCreateWindow("Catching Moles");
    glutDisplayFunc(MyDisplay); // 콜백함수
    glutReshapeFunc(MyReshape); // 콜백함수
    glutMouseFunc(MyMouse); // 콜백함수
    glutTimerFunc(20, MyTimer, 1); // 콜백함수

    if (LoadGLTextures((char*)szFilename, (char*)szFilename2)) { // 텍스쳐 로드 성공하면
        MyInit(); // 초기화
        InitLight(); // 광원 초기화
        glutMainLoop(); // 루프 시작
    }
    return 0;
}