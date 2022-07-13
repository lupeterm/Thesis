#include "Lighting.h"

Lighting::Lighting() {
 setAmbientColor  ((GLfloat)0.2, (GLfloat)0.2,   (GLfloat)0.2); 
 setDifuseColor   ((GLfloat)0.7, (GLfloat)0.7,   (GLfloat)0.7);
 setSpecularColor ((GLfloat)0.5, (GLfloat)0.5,   (GLfloat)0.5);
 setPosition      ((GLfloat)0.0, (GLfloat)500.0, (GLfloat)500.0);
}

void Lighting::setPosition(GLfloat x, GLfloat y, GLfloat z) {
   mPosition[0] = x;
   mPosition[1] = y;
   mPosition[2] = z;
   mPosition[3] = 1.0;
}

void Lighting::setDifuseColor(GLfloat red, GLfloat green, GLfloat blue) {
   mDifuseColor[0] = red;
   mDifuseColor[1] = green;
   mDifuseColor[2] = blue;
}

void Lighting::setSpecularColor(GLfloat red, GLfloat green, GLfloat blue) {
   mSpecularColor[0] = red;
   mSpecularColor[1] = green;
   mSpecularColor[2] = blue;
}

void Lighting::setAmbientColor(GLfloat red, GLfloat green, GLfloat blue) {
   mAmbientColor[0] = red;
   mAmbientColor[1] = green;
   mAmbientColor[2] = blue;
}

void Lighting::enableLighting() {

 	// Especifica que a cor de fundo da janela será preta
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Habilita o modelo de colorização de Gouraud
	glShadeModel(GL_SMOOTH);
	// Define a refletância do material 
	glMaterialfv(GL_FRONT,GL_SPECULAR, mSpecularColor);
	// Define a concentração do brilho
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,60.f);
	// Ativa o uso da luz ambiente 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, mAmbientColor);

	// Define os parâmetros da luz de número 0
	glLightfv(GL_LIGHT0, GL_AMBIENT,  mAmbientColor); 
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  mDifuseColor);
	glLightfv(GL_LIGHT0, GL_SPECULAR, mSpecularColor);
	glLightfv(GL_LIGHT0, GL_POSITION, mPosition);

	// Habilita a definição da cor do material a partir da cor corrente
	glEnable(GL_COLOR_MATERIAL);
	//Habilita o uso de iluminação
	glEnable(GL_LIGHTING);  
	// Habilita a luz de número 0
	glEnable(GL_LIGHT0);
	// Habilita o depth-buffering
	glEnable(GL_DEPTH_TEST);
}
