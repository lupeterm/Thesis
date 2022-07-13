#include "OpenGL.h"
#include <GL/gl.h>
#include <GL/glut.h>

Canvas *OpenGL::_canvas;

void OpenGL::specialKeyboard(int key, int x, int y) { _canvas->specialKeyboardDown(key + 100, x, y); }
void OpenGL::specialKeyboardUP(int key, int x, int y) { _canvas->specialKeyboardUp(key + 100, x, y); }
void OpenGL::keyboardDown(unsigned char key, int x, int y) { _canvas->keyboardDown(key, x, y); }
void OpenGL::keyboardUp(unsigned char key, int x, int y) { _canvas->keyboardUp(key, x, y); }
void OpenGL::mouseClick(int button, int state, int x, int y) { _canvas->mouseClick(button, state, x, y); }
void OpenGL::mouseMotion(int x, int y) { _canvas->mouseMotion(x, y); }
void OpenGL::reshape(int w, int h) { _canvas->reshape(w, h); }

void OpenGL::display()
{
	_canvas->update();
	_canvas->render();
	glutSwapBuffers();
}

void OpenGL::init(Canvas *canvas)
{
	_canvas = canvas;
	int argc = 1;
	char *argv[] = {"Application"};

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(_canvas->_width, _canvas->_height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Application");
	glutReshapeFunc(OpenGL::reshape);
	glutDisplayFunc(OpenGL::display);
	glutKeyboardFunc(OpenGL::keyboardDown);
	glutKeyboardUpFunc(OpenGL::keyboardUp);
	glutSpecialUpFunc(OpenGL::specialKeyboardUP);
	glutSpecialFunc(OpenGL::specialKeyboard);
	glutIdleFunc(OpenGL::display);
	glutMouseFunc(OpenGL::mouseClick);
	glutPassiveMotionFunc(OpenGL::mouseMotion);
	glutMotionFunc(OpenGL::mouseMotion);

	glutMainLoop();
}
