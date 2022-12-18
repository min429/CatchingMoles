#include <stdlib.h>
#include <GL/glut.h>
#include <gl/GLAUX.H>
#include<iostream>
#include<time.h>

#pragma comment(lib, "glaux.lib")
#pragma comment (lib,"legacy_stdio_definitions.lib")

using namespace std;

// ��ü ���� �� ����
const GLfloat brown[] = { 0.4, 0.1, 0.1, 1.0 };
const GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };
const GLfloat orange[] = { 0.9, 0.6, 0.3, 1.0 };
const GLfloat polished[] = { 100 };

float width = 800, height = 600; // ó�� ������ ũ��
float mx=0, my=0; // ���콺 x, y��ǥ
int m1=0, m2=0, m3=0, m4=0, m5=0, m6=0, m7=0, m8=0, m9=0; // �δ��� y�� �̵�
bool move1 = false, move2 = false, move3 = false, move4 = false, move5 = false, move6 = false, move7 = false, move8 = false, move9 = false;
bool fail; // �δ��� ��� ���� ����
bool timer_random; // �����Լ� Ÿ�̸Ӱ� �۵��� �� �ִ��� ����
double time_init; // ���ؽð�
int p, q; // �δ��� ��ǥ

unsigned int MyTextureObject[2]; // �ؽ��� ��ü
AUX_RGBImageRec* pTextureImage[2]; // �ؽ��� ���� ������ ����Ű�� ������

AUX_RGBImageRec* LoadBMP(char* szFilename) { // BMP���� �ε�

    FILE* pFile = NULL;
    if (!szFilename) {
        return NULL;
    }
    fopen_s(&pFile, szFilename, "r");

    // seFilename�� �����ڵ� ��Ʈ������ �ٲٱ�
    int size = strlen(szFilename);
    wchar_t* w_szFilename = new wchar_t[size];

    for (int i = 0; i < size; ++i)  w_szFilename[i] = szFilename[i];
    w_szFilename[size] = NULL;

    if (pFile) {
        fclose(pFile);
        return auxDIBImageLoad(w_szFilename); // ���Ϸκ��� �޸𸮷�
        // auxDIBImageLoad() : ����Ÿ�� => LPCWSTR : const �� 16 - bit UNICODE character �� ������
        // LPSTR - Long Point String�� ���ڷ�, ���� C��� �����δ� char * 
        // LPCWSTR - Long Point Contant Wide String�� ���ڷ�, ���� C��� ������ wchar_t const * 	
    }
    return NULL;

}
int LoadGLTextures(char* szFilePath, char* szFilePath2) { // ������ �ε��ϰ� �ؽ��ķ� ��ȯ
    int Status = FALSE;
    glClearColor(0.0, 0.0, 0.0, 0.5); // �ʱ�ȭ�� �� ����
    memset(pTextureImage, 0, sizeof(void*) * 1); // ������ ���

    if (pTextureImage[0] = LoadBMP(szFilePath)) { // ��Ʈ���� �ε��ϰ� ����Ȯ��
        Status = TRUE; // ���������� ���� True
        glGenTextures(1, &MyTextureObject[0]); // �ؽ��� ����
        glBindTexture(GL_TEXTURE_2D, MyTextureObject[0]); // �ؽ��� ��ü ����
        // �ؽ��ĸ޸𸮸� �ؽ��� ��⿡�� ����� �� �ִ� ���·� ��ȯ�Ͽ� �ؽ��� �޸𸮿� �÷���
        glTexImage2D(GL_TEXTURE_2D, 0, 3,
            pTextureImage[0]->sizeX, pTextureImage[0]->sizeY,
            0, GL_RGB, GL_UNSIGNED_BYTE, pTextureImage[0]->data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // ����� �� �帮�� ó��
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // Ȯ���� �� �����ϰ� ó��
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL); // �ؽ��� ��ĥ��
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
    if (pTextureImage[0] && pTextureImage[1]) { // �ؽ��İ� �����ϸ�
        if (pTextureImage[0]->data) { // �ؽ��� ������ �����ϸ�
            free(pTextureImage[0]->data); // �ؽ��� ������� �ݳ�
        }
        free(pTextureImage[0]); // �ؽ��� �ݳ�
        if (pTextureImage[1]->data) { // �ؽ��� ������ �����ϸ�
            free(pTextureImage[1]->data); // �ؽ��� ������� �ݳ�
        }
        free(pTextureImage[1]); // �ؽ��� �ݳ�
    }

    return Status;
}
void RandomMove() { // �ö�� �δ��� ����
    double time_current = (double)clock() / 1000; // ����ð� �ʷ� ��ȯ
    if ((time_current - time_init) >= 2) { // 2�� ������
        srand(time(NULL)); // seed������ ����ð� �ο�
        int move = (rand() % 9) + 1; // 1~9
        switch (move) { // � �δ����� �������� ����
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
        fail = true; // �δ����� �������� �ٷ� �������� �̸� �ʱ�ȭ
    }
}
void DrawMole(float x, float y, float z) { // �δ��� �׸���
    GLUquadric* qobj = gluNewQuadric(); // gluQuadric��ü�� ����ϱ� ����

    glPushMatrix(); // ctm1
    glMaterialfv(GL_FRONT, GL_DIFFUSE, brown); // ���ݻ� -> ����
    glMaterialfv(GL_FRONT, GL_SHININESS, polished); // ���⳪��
    gluQuadricDrawStyle(qobj, GLU_FILL); // �� ä��
    glTranslatef(x, y, z); // �δ��� ��ġ, ������
    glRotatef(90, 1, 0, 0); // ���� ���� ����
    gluCylinder(qobj, 8, 8, 8, 60, 20); // ����
    gluSphere(qobj, 8, 100, 100); // �Ӹ�

    glPushMatrix(); // ctm2
    glTranslatef(-3, 7, -4);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
    glMaterialfv(GL_FRONT, GL_SHININESS, polished);
    gluSphere(qobj, 1, 50, 50); // ���� ��
    glPopMatrix(); // ctm1

    glPushMatrix(); // ctm2
    glTranslatef(3, 7, -4);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
    glMaterialfv(GL_FRONT, GL_SHININESS, polished);
    gluSphere(qobj, 1, 50, 50); // ������ ��
    glPopMatrix(); // ctm1

    glPushMatrix(); // ctm2
    glTranslatef(0, 8, 0);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, orange);
    glMaterialfv(GL_FRONT, GL_SHININESS, polished);
    gluSphere(qobj, 1, 50, 50); // ��
    glPopMatrix(); // ctm1
    glPopMatrix(); // ctm0
}
void DrawLand() {
    // �δ����� ���� ��1
    glBindTexture(GL_TEXTURE_2D, MyTextureObject[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-30, 0, 30); //�ո�
    glTexCoord2f(1.0f, 0.0f); glVertex3f(30, 0, 30);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(30, 6, 30);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-30, 6, 30);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-30, 0, -30); //�޸�
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-30, 6, -30);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(30, 6, -30);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(30, 0, -30);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-30, 6, -30); //����
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-30, 6, 30);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(30, 6, 30);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(30, 6, -30);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-30, 0, -30); //�Ʒ���
    glTexCoord2f(0.0f, 1.0f); glVertex3f(30, 0, -30);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(30, 0, 30);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-30, 0, 30);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(30, 0, -30); //������
    glTexCoord2f(1.0f, 1.0f); glVertex3f(30, 6, -30);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(30, 6, 30);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(30, 0, 30);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-30, 0, -30); //������
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-30, 0, 30);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-30, 6, 30);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-30, 6, -30);
    glEnd();

    // �δ����� ���� ��2
    glBindTexture(GL_TEXTURE_2D, MyTextureObject[1]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-30, -6, 30); //�ո�
    glTexCoord2f(1.0f, 0.0f); glVertex3f(30, -6, 30);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(30, 0, 30);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-30, 0, 30);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-30, -6, -30); //�޸�
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-30, 0, -30);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(30, 0, -30);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(30, -6, -30);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-30, 0, -30); //����
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-30, 0, 30);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(30, 0, 30);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(30, 0, -30);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-30, -6, -30); //�Ʒ���
    glTexCoord2f(0.0f, 1.0f); glVertex3f(30, -6, -30);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(30, -6, 30);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-30, -6, 30);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(30, -6, -30); //������
    glTexCoord2f(1.0f, 1.0f); glVertex3f(30, 0, -30);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(30, 0, 30);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(30, -6, 30);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-30, -6, -30); //������
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-30, -6, 30);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-30, 0, 30);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-30, 0, -30);
    glEnd();
}
void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ��, ���� ���۸� ���

    glMatrixMode(GL_MODELVIEW); // �𵨺� ��� ���
    glLoadIdentity(); // ctm0 // ���(ctm) �ʱ�ȭ
    gluLookAt(0, 40, 80, 0, 0, 0, 0, 1, -1); // ���� ����

    glEnable(GL_TEXTURE_2D); // �ؽ��ı�� Ȱ��ȭ

    DrawLand(); // �� �׸�

    glDisable(GL_TEXTURE_2D); // �ؽ��� ���� ��Ȱ��ȭ

    // �δ��� �׸�
    DrawMole(-19, m1, -19);
    DrawMole(0, m2, -19); // ���
    DrawMole(19, m3, -19);
    DrawMole(0, m5, 0); // �����
    DrawMole(-19,m4,0);
    DrawMole(19, m6, 0);
    DrawMole(-19, m7, 19);
    DrawMole(0, m8, 19); // ���
    DrawMole(19, m9, 19);

    glPopMatrix(); // ctm0
    glutSwapBuffers(); // ����� <-> ����Ʈ���� ����(ȭ�� ���)

    if (!move1 && !move2 && !move3 && !move4 && !move5 && !move6 && !move7 && !move8 && !move9)
        RandomMove(); // Ƣ��� �δ��� ��������
    else { // �δ����� ������ �� ���ؽð� ����
        if (timer_random) { // RandomMove Ÿ�̸Ӱ� �۵��� �� �ִٸ�
            time_init = (double)clock() / 1000; // �� ������ ��ȯ
        }
    }
}
void MyReshape(int w, int h) {
    width = w, height = h; // �ٲ� �ʺ�, ���� ����
    glViewport(0, 0, (GLsizei)w, (GLsizei)h); // ���� ����
    glMatrixMode(GL_PROJECTION); // ������� ���
    glLoadIdentity(); // ��� �ʱ�ȭ
    // ȭ�� ������ �� ���� ����
    gluPerspective(60, (GLfloat)w / (GLfloat)h, 1, 200); // �������� // �ְ� ����
}
void MyMouse(int button, int state, int x, int y) {
    if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) { // ���� ���콺 Ŭ��
        double time_current = (double)clock() / 1000; // ����ð� �ʷ� ��ȯ
        if (move1) { // 1�� �δ����� �ö�� ��
            // �ش� �δ����� Ŭ���ϸ�
            if (x >= 260 * (width / 800) && x <= 345 * (width / 800) && y >= 180 * (height / 600) && y <= 245 * (height / 600)){
                fail = false; // �δ��� ��� ����
                cout << "�����ӵ�: " << (time_current - time_init) << "��\n";
            }
        }
        else if (move2) {
            if (x >= 360 * (width / 800) && x <= 440 * (width / 800) && y >= 180 * (height / 600) && y <= 245 * (height / 600)) {
                fail = false;
                cout << "�����ӵ�: " << (time_current - time_init) << "��\n";
            }
        }
        else if (move3) {
            if (x >= 455 * (width / 800) && x <= 540 * (width / 800) && y >= 180 * (height / 600) && y <= 245 * (height / 600)) {
                fail = false;
                cout << "�����ӵ�: " << (time_current - time_init) << "��\n";
            }
        }
        else if (move4) {
            if (x >= 230 * (width / 800) && x <= 330 * (width / 800) && y >= 210 * (height / 600) && y <= 275 * (height / 600)) {
                fail = false;
                cout << "�����ӵ�: " << (time_current - time_init) << "��\n";
            }
        }
        else if (move5) {
            if (x >= 350 * (width / 800) && x <= 445 * (width / 800) && y >= 210 * (height / 600) && y <= 285 * (height / 600)) {
                fail = false;
                cout << "�����ӵ�: " << (time_current - time_init) << "��\n";
            }
        }
        else if (move6) {
            if (x >= 465 * (width / 800) && x <= 565 * (width / 800) && y >= 210 * (height / 600) && y <= 285 * (height / 600)) {
                fail = false;
                cout << "�����ӵ�: " << (time_current - time_init) << "��\n";
            }
        }
        else if (move7) {
            if (x >= 190 * (width / 800) && x <= 315 * (width / 800) && y >= 250 * (height / 600) && y <= 355 * (height / 600)) {
                fail = false;
                cout << "�����ӵ�: " << (time_current - time_init) << "��\n";
            }
        }
        else if (move8) {
            if (x >= 340 * (width / 800) && x <= 460 * (width / 800) && y >= 250 * (height / 600) && y <= 355 * (height / 600)) {
                fail = false;
                cout << "�����ӵ�: " << (time_current - time_init) << "��\n";
            }
        }
        else if (move9) {
            if (x >= 480 * (width / 800) && x <= 605 * (width / 800) && y >= 250 * (height / 600) && y <= 355 * (height / 600)) {
                fail = false;
                cout << "�����ӵ�: " << (time_current - time_init) << "��\n";
            }   
        }
    }
}
void CatchTimer(double time_init) {
    timer_random = false; // RandomMove()���� Ÿ�̸Ӱ� �۵����� ���ϵ�����
    double time_current = (double)clock() / 1000; // ����ð� �ʷ� ��ȯ
    if ((time_current - time_init) >= 2) { // 2�� ������
        timer_random = true; // RandomMove()���� Ÿ�̸Ӱ� �۵��� �� �ֵ�����
        if (fail) { // ������ ���
            cout << "fail";
            exit(0); // ���α׷� ����
        }
    }
}
void MyTimer(int value) {
    if (move1) {
        if (fail) { // �δ����� ���� ���� ���� ���
            if (m1 <= 8) { // �δ����� ���� �ö���� ��
                if (m1 == 0){ // �δ����� ���ӿ� ���� ��
                    time_init = (double)clock() / 1000; // ���ؽð� ����
                }
                m1++; // �δ��� ���� �ö��
            }
            else {
                CatchTimer(time_init); // �� 2���Ŀ� �δ��� ��Ҵ��� üũ
            }
        }
        else { // �δ����� ���� ���
            if (m1 > 0) { // �δ����� �������� ��
                m1--; // �δ��� ������
            }
            else { // �� ����������
                move1 = false; // ������ ���Ŀ��� �������� ����
            }
        }
    }
    else if (move2) {
        if (fail) { // �δ����� ���� ���� ���� ���
            if (m2 <= 8) {
                if (m2 == 0) {
                    time_init = (double)clock() / 1000;
                }
                m2++;
            }
            else {
                CatchTimer(time_init); // �� 2���Ŀ� �δ��� ��Ҵ��� üũ
            }
        }
        else { // �δ����� ���� ���
            if (m2 > 0) { // �δ��� ������
                m2--;
            }
            else { // ������ ���Ŀ��� �������� ����
                move2 = false;
            }
        }
    }
    else if (move3) {
        if (fail) { // �δ����� ���� ���� ���� ���
            if (m3 <= 8) {
                if (m3 == 0) {
                    time_init = (double)clock() / 1000;
                }
                m3++;
            }
            else {
                CatchTimer(time_init); // �� 2���Ŀ� �δ��� ��Ҵ��� üũ
            }
        }
        else { // �δ����� ���� ���
            if (m3 > 0) { // �δ��� ������
                m3--;
            }
            else { // ������ ���Ŀ��� �������� ����
                move3 = false;
            }
        }
    }
    else if (move4) {
        if (fail) { // �δ����� ���� ���� ���� ���
            if (m4 <= 8) {
                if (m4 == 0) {
                    time_init = (double)clock() / 1000;
                }
                m4++;
            }
            else {
                CatchTimer(time_init); // �� 2���Ŀ� �δ��� ��Ҵ��� üũ
            }
        }
        else { // �δ����� ���� ���
            if (m4 > 0) { // �δ��� ������
                m4--;
            }
            else { // ������ ���Ŀ��� �������� ����
                move4 = false;
            }
        }
    }
    else if (move5) {
        if (fail) { // �δ����� ���� ���� ���� ���
            if (m5 <= 8) {
                if (m5 == 0) {
                    time_init = (double)clock() / 1000;
                }
                m5++;
            }
            else {
                CatchTimer(time_init); // �� 2���Ŀ� �δ��� ��Ҵ��� üũ
            }
        }
        else { // �δ����� ���� ���
            if (m5 > 0) { // �δ��� ������
                m5--;
            }
            else { // ������ ���Ŀ��� �������� ����
                move5 = false;
            }
        }
    }
    else if (move6) {
        if (fail) { // �δ����� ���� ���� ���� ���
            if (m6 <= 8) {
                if (m6 == 0) {
                    time_init = (double)clock() / 1000;
                }
                m6++;
            }
            else {
                CatchTimer(time_init); // �� 2���Ŀ� �δ��� ��Ҵ��� üũ
            }
        }
        else { // �δ����� ���� ���
            if (m6 > 0) { // �δ��� ������
                m6--;
            }
            else { // ������ ���Ŀ��� �������� ����
                move6 = false;
            }
        }
    }
    else if (move7) {
        if (fail) { // �δ����� ���� ���� ���� ���
            if (m7 <= 8) {
                if (m7 == 0) {
                    time_init = (double)clock() / 1000;
                }
                m7++;
            }
            else {
                CatchTimer(time_init); // �� 2���Ŀ� �δ��� ��Ҵ��� üũ
            }
        }
        else { // �δ����� ���� ���
            if (m7 > 0) { // �δ��� ������
                m7--;
            }
            else { // ������ ���Ŀ��� �������� ����
                move7 = false;
            }
        }
    }
    else if (move8) {
        if (fail) { // �δ����� ���� ���� ���� ���
            if (m8 <= 8) {
                if (m8 == 0) {
                    time_init = (double)clock() / 1000;
                }
                m8++;
            }
            else {
                CatchTimer(time_init); // �� 2���Ŀ� �δ��� ��Ҵ��� üũ
            }
        }
        else { // �δ����� ���� ���
            if (m8 > 0) { // �δ��� ������
                m8--;
            }
            else { // ������ ���Ŀ��� �������� ����
                move8 = false;
            }
        }
    }
    else if (move9) {
        if (fail) { // �δ����� ���� ���� ���� ���
            if (m9 <= 8) {
                if (m9 == 0) {
                    time_init = (double)clock() / 1000;
                }
                m9++;
            }
            else {
                CatchTimer(time_init); // �� 2���Ŀ� �δ��� ��Ҵ��� üũ
            }
        }
        else { // �δ����� ���� ���
            if (m9 > 0) { // �δ��� ������
                m9--;
            }
            else { // ������ ���Ŀ��� �������� ����
                move9 = false;
            }
        }
    }

    glutPostRedisplay(); // MyDisplay() ����ȣ��
    glutTimerFunc(20, MyTimer, 1); // 1 -> Ÿ�̸��� �뵵�� �ϳ����̶�� �ƹ����̳� �־ ����� ����.
    // 20msec �Ŀ� ȣ��
}
void InitLight() {
    // 0�� ���� Ư��
    GLfloat light0_ambient[] = { 1.0, 1.0, 1.0, 1.0 }; // �ֺ��ݻ�
    GLfloat light0_diffuse[] = { 1,1,1, 1.0 }; // ���ݻ�
    GLfloat light0_position[] = { 0.0, 100.0, 0, 1.0 }; // 0�� ������ġ
    GLfloat light0_direction[] = { 0.0, -1.0, 0.0 }; // 0�� ���� ����
    GLfloat light0_cutoff[] = { 100 }; // 0�� ���� ����

    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);	// ����Ʈ����Ʈ
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light0_direction); // ����
    glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF, light0_cutoff); // ����
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 1.0);	 // �ڻ��� �¼�

    glShadeModel(GL_SMOOTH); // ���� ���̵�
    glEnable(GL_DEPTH_TEST); // ���� ���� Ȱ��ȭ
    glEnable(GL_LIGHTING); // ���� Ȱ��ȭ
    glEnable(GL_LIGHT0); // 0�� ���� Ȱ��ȭ
    
    // 0�� ���� Ư���Ҵ�
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient); // �ֺ��ݻ�
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse); // ���ݻ�
}
void MyInit(void) {
    glClearColor(0.0, 0.0, 0.0, 1.0); // ���: ������
    time_init = (double)clock() / 1000; // �� ������ ��ȯ

    // �ĸ� �� �������� ����
    glEnable(GL_CULL_FACE); // �ĸ����� Ȱ��ȭ
    glFrontFace(GL_CCW); // �ð�ݴ������ ����
    glCullFace(GL_BACK); // �ĸ� ����
    glEnable(GL_DEPTH_TEST); // ���̹��� Ȱ��ȭ -> ��������
}
int main(int argc, char** argv) {
    const char* szFilename = "grass.bmp";
    const char* szFilename2 = "soil.bmp";

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // ���� ����, RGB ����, ���̹���
    glutInitWindowSize(width, height); // ������ ũ��
    glutInitWindowPosition(500, 200); // ������ ��ġ
    glutCreateWindow("Catching Moles");
    glutDisplayFunc(MyDisplay); // �ݹ��Լ�
    glutReshapeFunc(MyReshape); // �ݹ��Լ�
    glutMouseFunc(MyMouse); // �ݹ��Լ�
    glutTimerFunc(20, MyTimer, 1); // �ݹ��Լ�

    if (LoadGLTextures((char*)szFilename, (char*)szFilename2)) { // �ؽ��� �ε� �����ϸ�
        MyInit(); // �ʱ�ȭ
        InitLight(); // ���� �ʱ�ȭ
        glutMainLoop(); // ���� ����
    }
    return 0;
}