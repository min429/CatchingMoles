/***********************************************************************/
/*                           HW#8 : ����                               */
/*  �ۼ��� : �̽¹�                              ��¥ : 2022�� 12�� 16�� */
/*                                                                     */
/* ���� ���� : �δ������ ����                                           */
/*                - - - - - - -                                        */
/* ���� : ���ݱ��� ��� ��� ����
- ���ݱ��� ������ �ǽ������� ������ �����Ͽ� ��ü�� �𵨸��ϰ�, �ݹ����α׷���,
�𵨺�ȯ, ������ȯ, ������ȯ, ������ȯ, ���ü��˻�, ����, �ؽ��� �� �����ð���
����� ����� ��� ���� �� �ֵ��� ���α׷��� �����ÿ�. 
- �׵��� �𵨸��غô� ��ü�� ���������� ����Ͽ��� ����, �߰��� ����� ���� 
��ü�� �𵨸��ص� ����, �������ִ� ���� ����ص� ����.
- �𵨸� �� ��ü�� �ϴ� ����� ���� ������ �����ϵ��� ��.
��) �޸��⸦ �Ѵ�. ���� ���´�. ���� �����Ѵ�. ���� ������. ��             */
/*                - - - - - - -                                        */
/***********************************************************************/
#include <stdlib.h>
#include <GL/glut.h>

GLfloat MyVertices[8][3] = { 
    {-30,-5,30}, {-30,5,30}, {30,5,30},
    {30,-5,30}, {-30,-5,-30}, {-30,5,-30},
    {30,5,-30}, {30,-5,-30}
};
GLfloat MyColors[8][3] = { {0.2,0.2,0.2}, {1.0,0.0,0.0}, {1.0, 1.0, 0.0}, {0.0,1.0,0.0},
{0.0,0.0,1.0}, {1.0,0.0,1.0}, {1.0,1.0,1.0}, {0.0,1.0,1.0} };
GLubyte MyVertexList[24] = { 0,3,2,1, 3,7,6,2, 0,4,7,3, 1,2,6,5, 4,5,6,7, 0,1,5,4 };
// �ո�, �����ʸ�, �Ʒ���(CW), ����, �޸�(CW), ���ʸ�(CW)

void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ���۸� ���

    glFrontFace(GL_CCW); // Vertex �迭�� �ݽð�������� �����ϰڴ� : ǥ��
    glEnable(GL_CULL_FACE); // ī�޶�κ��� ������ �ʴ� ���� ����ڴ�
    glEnableClientState(GL_COLOR_ARRAY); // VertexArray ��� Ȱ��ȭ : ���� ��
    glEnableClientState(GL_VERTEX_ARRAY); // VertexArray ��� Ȱ��ȭ : ���� ��ǥ
    glColorPointer(3, GL_FLOAT, 0, MyColors); // Vertex�� ������ ������ ������ MyColors ����
    glVertexPointer(3, GL_FLOAT, 0, MyVertices); // Vertex�� ��ǥ�� ������ ������ MyVertices ����

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 60, -80, 0, 0, 0, 0, 1, -1);
    for (GLint i = 0; i < 6; i++)
        glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, &MyVertexList[4 * i]);
    // �δ����� ���� ��
    glPushMatrix();




    glutSwapBuffers(); // ����� <-> ����Ʈ���� ����(ȭ�� ���)
}
void MyReshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)w / (GLfloat)h, 1, 100); // �������� // �ְ� ����
}
void MyKeyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'S':
        break;
    default:
        break;
    }
    glutPostRedisplay(); // MyDisplay() ����ȣ��
}
void MyTimer(int value) {
    

    glutPostRedisplay(); // MyDisplay() ����ȣ��
    glutTimerFunc(20, MyTimer, 1); // 1 -> Ÿ�̸��� �뵵�� �ϳ����̶�� �ƹ����̳� �־ ����� ����.
    // 100msec �Ŀ� ȣ��
}
void MyInit(void) {
    glClearColor(0.0, 0.0, 0.0, 1.0); // ���: ������
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // ���� ����, RGB
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Robot");
    MyInit();
    glutDisplayFunc(MyDisplay);
    glutReshapeFunc(MyReshape);
    glutKeyboardFunc(MyKeyboard);
    glutTimerFunc(20, MyTimer, 1);
    glutMainLoop();
    return 0;
}