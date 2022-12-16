/***********************************************************************/
/*                           HW#8 : 종합                               */
/*  작성자 : 이승민                              날짜 : 2022년 12월 16일 */
/*                                                                     */
/* 문제 정의 : 두더지잡기 게임                                           */
/*                - - - - - - -                                        */
/* 내용 : 지금까지 배운 기술 종합
- 지금까지 수행한 실습과제의 내용을 종합하여 객체를 모델링하고, 콜백프로그래밍,
모델변환, 시점변환, 투영변환, 뷰폿변환, 가시성검사, 조명, 텍스춰 등 수업시간에
배웠던 기술이 모두 쓰일 수 있도록 프로그램을 만들어보시오. 
- 그동안 모델링해봤던 객체를 종합적으로 사용하여도 좋고, 추가로 만들고 싶은 
객체를 모델링해도 좋고, 제공해주는 모델을 사용해도 좋음.
- 모델링 된 객체가 하는 기능은 각자 스스로 정의하도록 함.
예) 달리기를 한다. 팔을 뻗는다. 달이 자전한다. 눈이 내린다. 등             */
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
// 앞면, 오른쪽면, 아래면(CW), 윗면, 뒷면(CW), 왼쪽면(CW)

void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT); // 지정한 색으로 버퍼를 비움

    glFrontFace(GL_CCW); // Vertex 배열을 반시계방향으로 연결하겠다 : 표면
    glEnable(GL_CULL_FACE); // 카메라로부터 보이지 않는 면을 숨기겠다
    glEnableClientState(GL_COLOR_ARRAY); // VertexArray 기능 활성화 : 정점 색
    glEnableClientState(GL_VERTEX_ARRAY); // VertexArray 기능 활성화 : 정점 좌표
    glColorPointer(3, GL_FLOAT, 0, MyColors); // Vertex의 색상을 저장한 변수명 MyColors 제시
    glVertexPointer(3, GL_FLOAT, 0, MyVertices); // Vertex의 좌표를 저장한 변수명 MyVertices 제시

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 60, -80, 0, 0, 0, 0, 1, -1);
    for (GLint i = 0; i < 6; i++)
        glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, &MyVertexList[4 * i]);
    // 두더지가 숨는 땅
    glPushMatrix();




    glutSwapBuffers(); // 백버퍼 <-> 프론트버퍼 스왑(화면 출력)
}
void MyReshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)w / (GLfloat)h, 1, 100); // 원근투영 // 왜곡 방지
}
void MyKeyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'S':
        break;
    default:
        break;
    }
    glutPostRedisplay(); // MyDisplay() 강제호출
}
void MyTimer(int value) {
    

    glutPostRedisplay(); // MyDisplay() 강제호출
    glutTimerFunc(20, MyTimer, 1); // 1 -> 타이머의 용도가 하나뿐이라면 아무값이나 주어도 상관은 없다.
    // 100msec 후에 호출
}
void MyInit(void) {
    glClearColor(0.0, 0.0, 0.0, 1.0); // 배경: 검정색
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // 더블 버퍼, RGB
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