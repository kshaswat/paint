#include "Objects.h"



Cuboid::Cuboid(GLfloat x,GLfloat y,GLfloat z,GLfloat l,GLfloat b,GLfloat h,color c[]):Objects(x,y,z)
{
	this->faces.resize(6);
	faces[0]=face(point(x-b/2,y+l/2,z+h/2),point(x+b/2,y+l/2,z+h/2),point(x+b/2,y-l/2,z+h/2),point(x-b/2,y-l/2,z+h/2),c[0]);
	faces[1]=face(point(x+b/2,y+l/2,z+h/2),point(x+b/2,y+l/2,z-h/2),point(x+b/2,y-l/2,z-h/2),point(x+b/2,y-l/2,z+h/2),c[1]);
	faces[2]=face(point(x-b/2,y+l/2,z-h/2),point(x+b/2,y+l/2,z-h/2),point(x+b/2,y-l/2,z-h/2),point(x-b/2,y-l/2,z-h/2),c[2]);
	faces[3]=face(point(x-b/2,y+l/2,z-h/2),point(x-b/2,y+l/2,z+h/2),point(x-b/2,y-l/2,z+h/2),point(x-b/2,y-l/2,z-h/2),c[3]);
	faces[4]=face(point(x-b/2,y+l/2,z-h/2),point(x+b/2,y+l/2,z-h/2),point(x+b/2,y+l/2,z+h/2),point(x-b/2,y+l/2,z+h/2),c[4]);
	faces[5]=face(point(x-b/2,y-l/2,z-h/2),point(x+b/2,y-l/2,z-h/2),point(x+b/2,y-l/2,z+h/2),point(x-b/2,y-l/2,z+h/2),c[5]);
}

void Cuboid::draw(GLfloat m[])
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(m);
	glGetFloatv(GL_MODELVIEW_MATRIX,this->modelview);
	//const GLfloat m[16]={1,0,0,0,0,1,0.5,0,0,0,1,0.5,1,2,-10,1};
	//glMultMatrixf(m);
	glBegin(GL_QUADS);
	for(int i=0;i<6;i++)
	{
		//if(!(i==0 || i==3))continue;
		glColor3f(faces[i].c.r,faces[i].c.g,faces[i].c.b);
		glVertex3f(faces[i].tl.x,faces[i].tl.y,faces[i].tl.z);
		glVertex3f(faces[i].tr.x,faces[i].tr.y,faces[i].tr.z);
		glVertex3f(faces[i].br.x,faces[i].br.y,faces[i].br.z);
		glVertex3f(faces[i].bl.x,faces[i].bl.y,faces[i].bl.z);
	}
	glEnd();
}

void Cuboid::printDetails()
{
}

sphere::sphere(GLfloat x, GLfloat y, GLfloat z, GLfloat r, color c) :Objects(x, y, z)
{
	this->radius = r;
	this->c = c;
}

void sphere::draw(GLfloat m[])
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(m);
	glColor3f(this->c.r, this->c.g, this->c.b);
	glutSolidSphere(this->radius, 100, 100);
}

void sphere::printDetails()
{
}

torus::torus(GLfloat x, GLfloat y, GLfloat z, GLfloat r1, GLfloat r2, color c) :Objects(x, y, z)
{
	this->r1 = r1;
	this->r2 = r2;
	this->c = c;
}
void sphere::draw(GLfloat m[])
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(m);
	glColor3f(this->c.r, this->c.g, this->c.b);
	glutSolidSphere(this->radius, 100, 100);
}

void sphere::printDetails()
{
}
