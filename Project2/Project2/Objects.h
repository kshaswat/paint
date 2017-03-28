#pragma once
#include "Header.h"

struct point
{
	GLfloat x,y,z;
	point(GLfloat a,GLfloat b,GLfloat c):x(a),y(b),z(c){}
	point(point &p):x(p.x),y(p.y),z(p.z){}
};

struct color
{
	GLfloat r,g,b;
	color(GLfloat x,GLfloat y,GLfloat z):r(x),g(y),b(z){}
	color():r(255.0f),g(255.0f),b(255.0f){}
};

struct face
{
	point tl,tr,bl,br;
	color c;
	face(point p1,point p2,point p3,point p4,color clr):tl(p1),tr(p2),br(p3),bl(p4),c(clr){}
	face():tl(point(0,0,0)),tr(point(0,0,0)),bl(point(0,0,0)),br(point(0,0,0)){}
	face(face &f):tl(f.tl),tr(f.tr),bl(f.bl),br(f.br),c(f.c){}

};

class Objects
{
	GLfloat comx,comy,comz;
protected:
	GLfloat modelview[16];
	char *name;
public:
	Objects(GLfloat x,GLfloat y,GLfloat z):comx(x),comy(y),comz(z){}
	~Objects(void){cout<<"destroy objects\n";}
	virtual void draw(GLfloat m[])=0;
	virtual void printDetails(void)=0;
};

class Cuboid:public Objects
{
	vector<face> faces;
	public:
		Cuboid(GLfloat x,GLfloat y,GLfloat z,GLfloat l,GLfloat b,GLfloat h,color c[]);
		~Cuboid();
		void draw(GLfloat m[]);
		void printDetails(void);

};

class sphere :public Objects
{
public:
	sphere(GLfloat x, GLfloat y, GLfloat z, GLfloat r, color c);
	~sphere();
	void draw(GLfloat m[]);
	void printDetails(void);
private:
	GLfloat radius;
	color c;
	
};

class cone :public Objects
{
public:
	cone(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat h, color c[]);
	~cone();
	void draw(GLfloat m[]);
	void printDetails(void);
private:
	GLfloat r, h;
	color c[2];
};

class cylinder :public Objects
{
public:
	cylinder(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat h, color c[]);
	~cylinder();
	void draw(GLfloat m[]);
	void printDetails(void);

private:
	GLfloat r, h;
	color c[3];
};

class tetrahedron :public Objects
{
public:
	tetrahedron(GLfloat x, GLfloat y, GLfloat z,color c[]);
	~tetrahedron();
	void draw(GLfloat m[]);
	void printDetails(void);

private:
	color c[5];
	point p[5];
};

class torus :public Objects
{
public:
	torus(GLfloat x, GLfloat y, GLfloat z, GLfloat r1, GLfloat r2, color c);
	~torus();

private:
	GLfloat r1, r2;
	color c;
};





