#include<GL\glut.h>
#include<cmath>
#include<time.h>
#include<iostream>
#define FUNC 2

clock_t begin, end;
int solution = 0;
int xleft, yleft, xright, yright;

int Round(float x)//四舍五入
{
	if (x - (int)x >= 0.5){
		return (int)x + 1;
	}
	else{
		return (int)x;
	}
}

void LineDDA(int xleft, int yleft, int xright, int yright)//数值微分法
{
	int steps;
	float xIncrement, yIncrement, x = xleft, y = yleft;
	int dy = yright - yleft;
	int dx = xright - xleft;
	if (abs(dx) > abs(dy)){
		steps = abs(dx);
	}
	else {
		steps = abs(dy);
	}
	xIncrement = float(dx) / float(steps);
	yIncrement = float(dy) / float(steps);
	for (int k = 0; k <= steps; k++) {
		glBegin(GL_POINTS);
		glVertex3f(Round(x), Round(y), 0);
		glEnd();
		x += xIncrement; y += yIncrement;
	}
}

void MidpointLine(int xleft, int yleft, int xright, int yright)//中点画线法
{
	int dx = abs(xright - xleft), dy = abs(yright - yleft);
	if (dx >= dy){
		int p = 2 * dy - dx;
		int pE = 2 * dy, pNE = 2 * (dy - dx);
		int x = xleft, y = yleft;
		do {
			glBegin(GL_POINTS);
			glVertex3f(x, y, 0);
			glEnd();
			if (p <= 0) { // Current d
				p += pE; // Next d at E
				x++;
			}
			else {
				p += pNE; // Next d at NE
				x++;
				if (yright >= yleft)
					y++;
				else
					y--;
			}
		} while (x <= xright);
	}
	else{
		int p = 2 * dx - dy;
		int pE = 2 * dx, pNE = 2 * (dx - dy);
		int x = xleft, y = yleft;
		do {
			glBegin(GL_POINTS);
			glVertex3f(x, y, 0);
			glEnd();
			if (p <= 0) { // Current d
				p += pE; // Next d at E
				if (yright >= yleft)
					y++;
				else
					y--;
			}
			else {
				p += pNE; // Next d at NE
				x++;
				if (yright >= yleft)
					y++;
				else
					y--;
			}
		} while (y <= yright);
	}
}

void display()
{
	//设置摄像机位置和二维平面位置
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);
	gluLookAt(0, 0, 0, 0, 0, -1, 0, 1, 0);
	glTranslatef(0, 0, -5);//二维平面为z=-5
	//选择画线方法
	switch (solution){
	case 0://数值微分法
		begin = clock();
		for (int i = 0; i < 10000; i++){
			LineDDA(xleft, yleft, xright, yright);
		}
		end = clock();
		std::cout << "LineDDA: " << (end - begin)/10 << " us\n";
		break;
	case 1://中点画线法
		begin = clock();
		for (int i = 0; i < 10000; i++){
			MidpointLine(xleft, yleft, xright, yright);
		}
		end = clock();
		std::cout << "MidpointLine: " << (end - begin)/10 << " us\n";
		break;
	default:
		break;
	}
	glFlush();
}

void reshape(int w, int h)
{
	if (0 == h)
		h = 1;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,100,0,100,1,100);//二维平面范围是0<=x<=100,0<=y<=100
	glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key,int x,int y)
{
	switch (key)
	{
	case 'q':
		solution = (solution + 1) % FUNC;
		glutPostRedisplay();
		break;
	case 'w':
		std::cout << "change point: x0 y0 x1 y1\n";
		std::cin.sync();
		std::cin >> xleft;
		std::cin >> yleft;
		std::cin >> xright;
		std::cin >> yright;
		glutPostRedisplay();
		break;
	case 'e':
		int left, right, up, down;
		std::cout << "change scale: left right down up\n";
		std::cin.sync();
		std::cin >> left;
		std::cin >> right;
		std::cin >> down;
		std::cin >> up;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(left, right, down, up, 1, 100);//二维平面范围是left<=x<=right,down<=y<=up
		glMatrixMode(GL_MODELVIEW);
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

int main(int argc, char** argv)
{
	std::cout << "Input point: x0 y0 x1 y1\n";
	std::cin.sync();
	std::cin >> xleft;
	std::cin >> yleft; 
	std::cin >> xright; 
	std::cin >> yright;
	std::cout << "Use Q to change method, W to change line, E to change scale.\n";
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("CG1");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}