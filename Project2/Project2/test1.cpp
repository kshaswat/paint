
#include "Objects.h"
#include "canvas.h"
GLfloat qaAmbientLight[]    = {0.5, 0, 0.3, 1.0};
GLfloat qaDiffuseLight[]    = {0.8, 0.8, 0.8, 1.0};
GLfloat qaSpecularLight[]    = {1.0, 1.0, 1.0, 1.0};
GLfloat emitLight[] = {0.9, 0.9, 0.9, 0.01};
GLfloat Noemit[] = {0.0, 0.0, 0.0, 1.0};
GLfloat qaLightPosition[]    = {4.0, 5.0, 1.0, 0};

GLfloat qaBlack[] = {0.0, 0.0, 0.0, 1.0}; //Black Color
GLfloat qaGreen[] = {0.0, 1.0, 0.0, 1.0}; //Green Color
GLfloat qaWhite[] = {1.0, 1.0, 1.0, 1.0}; //White Color
GLfloat qaRed[] = {1.0, 0.0, 0.0, 1.0}; //White Color
GLfloat ry=0.0f;

canvas *can1=new canvas();

typedef struct points
{
	GLfloat x,y;
	points(){}
	points(GLfloat a,GLfloat b):x(a),y(b){}
}points;





void newWindow(int,char**,char*);
void display();
void reshape(GLsizei,GLsizei);
void initializer();
void myTriangles(points,points,points);
void timer(int);
void drawRectangularPrism();
void keyboard(unsigned char key,int x,int y);
GLfloat sq(int);
void passiveMouse(GLint ,GLint);
void mouse(GLint ,GLint,GLint ,GLint);
void normalize(GLdouble ,GLdouble ,GLdouble ,GLdouble &,GLdouble &,GLdouble &);
GLdouble dot(GLdouble ,GLdouble ,GLdouble ,GLdouble ,GLdouble ,GLdouble );
GLdouble max(GLdouble, GLdouble);

int main(int argc,char ** argv)
{

	newWindow(argc,argv,"tester");

	return 0;
}

void newWindow(int argc,char **argv,char *str)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowPosition(50,50);
	glutInitWindowSize(200,200);
	glutCreateWindow(str);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	//glutTimerFunc(0, timer, 0);
	glutKeyboardFunc(keyboard);
	//glutPassiveMotionFunc(passiveMouse);
	glutMouseFunc(mouse);
	initializer();
	glutMainLoop();
}

void initializer()
{
	std::cout<<"In intializer function\n";
	glClearColor(0.5f,0.5f,0.5f,1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);

	 //glLightfv(GL_LIGHT0, GL_AMBIENT, qaAmbientLight);
     //glLightfv(GL_LIGHT0, GL_DIFFUSE, qaDiffuseLight);
     //glLightfv(GL_LIGHT0, GL_SPECULAR, qaSpecularLight);

	 //glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);


}

void keyboard(unsigned char key,int x,int y)
{
	if(key==GLUT_UP)
		ry+=10;
}
GLdouble posX, posY, posZ;
GLdouble posX2, posY2, posZ2;

void passiveMouse(GLint x,GLint y)
{
	GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    
 
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );
 
    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
 
	//cout<<winZ<<"\n";

    gluUnProject( winX, winY, 1.0, modelview, projection, viewport, &posX, &posY, &posZ);
	 gluUnProject( winX, winY, 0.0, modelview, projection, viewport, &posX2, &posY2, &posZ2);

	 //cout<<winX<<"  "<<y<<"  "<<winY<<endl;
	cout<<posX2<<"  "<<posY2<<"  "<<posZ2<<endl;
	
	display();
}
void normalize(GLdouble x,GLdouble y,GLdouble z,GLdouble &nx,GLdouble &ny,GLdouble &nz)
{
	GLdouble d=sqrt(x*x + y*y + z*z);
	nx=x/d;ny=y/d;nz=z/d;
}

GLdouble max(GLdouble a, GLdouble b)
{
	if (a > b)return a;
	return b;
}

GLdouble dot(GLdouble x,GLdouble y,GLdouble z,GLdouble nx,GLdouble ny,GLdouble nz)
{
	return x*nx + y*ny + z*nz ;
}
void mouse(GLint button,GLint state,GLint x,GLint y)
{
	if(button==GLUT_RIGHT_BUTTON)
	{
		ry=30;
		display();
	}
	else if(state==GLUT_DOWN){
	GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    
 
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );
 
    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
 
	//cout<<winZ<<"\n";

    gluUnProject( winX, winY, 1.0, modelview, projection, viewport, &posX, &posY, &posZ);
	 gluUnProject( winX, winY, 0.0, modelview, projection, viewport, &posX2, &posY2, &posZ2);

	 //cout<<winX<<"  "<<y<<"  "<<winY<<endl;
	 
	//cout<<posX2<<"  "<<posY2<<"  "<<posZ2<<endl;
	 GLdouble dirX, dirY, dirZ; //posZ2 -= 10;
	normalize(posX-posX2,posY-posY2,posZ-posZ2,dirX,dirY,dirZ);

	float px, py, pz; 
	px = modelview[12] - posX2; py = modelview[13] - posY2; pz = modelview[14] - posZ2;

	GLdouble e = dot(px, py, pz, modelview[0], modelview[1], modelview[2]);
	GLdouble f = dot(dirX, dirY, dirZ, modelview[0], modelview[1], modelview[2]);
	GLdouble tmin,tmax;

	tmin=std::numeric_limits<double>::min();
	tmax=std::numeric_limits<double>::max();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cout << modelview[i * 4 + j] << "  ";
		}
		cout << endl;
	}
	GLboolean flag=GL_TRUE;
	//cout<<dirX<<"  "<<dirY<<"  "<<dirZ<<endl;
	//cout<<posX2+25*dirX<<" "<<posY2+25*dirY<<" "<<posZ2+25*dirZ<<endl;
	//cout<<e<<"  :: "<<f<<endl;
	if(abs(f)>0.00000000000001)
	{
		GLdouble t1,t2;
		t1=(e-1)/f;
		t2=(e+1)/f;
		cout<<t1<<" "<<t2<<endl;
		if(t1>t2){GLdouble temp=t1;t1=t2;t2=temp;}
		if(t1>tmin)tmin=t1;
		if(t2<tmax)tmax=t2;
		
		if(tmin>tmax)flag=false;
		else if(tmax<0)flag=GL_FALSE;
	}
	if(flag)
		cout<<"INTERSECTION\n";
	else
		cout<<"MISSED\n";
	e = dot(px, py, pz, modelview[4], modelview[5], modelview[6]);
	f = dot(dirX, dirY, dirZ, modelview[4], modelview[5], modelview[6]);
	cout<<dirX<<"  "<<dirY<<"  "<<dirZ<<endl;
	cout<<posX2+25*dirX<<" "<<posY2+25*dirY<<" "<<posZ2+25*dirZ<<endl;
	cout<<e<<"  :: "<<f<<endl;
	if(abs(f)>0.00000000000001)
	{
		GLdouble t1,t2;
		t1=(e-1)/f;
		t2=(e+1)/f;
		cout<<t1<<" "<<t2<<endl;
		if(t1>t2){GLdouble temp=t1;t1=t2;t2=temp;}
		if(t1>tmin)tmin=t1;
		if(t2<tmax)tmax=t2;
		
		if(tmin>tmax)flag=false;
		else if(tmax<0)flag=GL_FALSE;
	}
	if(flag)
		cout<<"INTERSECTION\n";
	else
		cout<<"MISSED\n";
	e = dot(px, py, pz, modelview[8], modelview[9], modelview[10]);
	f = dot(dirX, dirY, dirZ, modelview[8], modelview[9], modelview[10]);
	cout << e << "  :: " << f << endl;
	//cout<<dirX<<"  "<<dirY<<"  "<<dirZ<<endl;
	//cout<<posX2+25*dirX<<" "<<posY2+25*dirY<<" "<<posZ2+25*dirZ<<endl;
	cout<<posZ2<<"  :: "<<posZ<<endl;
	if(abs(f)>0.00000000000001)
	{
		GLdouble t1,t2;
		t1=(e-1)/f;
		t2=(e+1)/f;
		cout<<t1<<" "<<t2<<endl;
		if(t1>t2){GLdouble temp=t1;t1=t2;t2=temp;}
		if(t1>tmin)tmin=t1;
		if(t2<tmax)tmax=t2;
		
		if(tmin>tmax){cout<<tmin<<"HERE"<<tmax<<"\n";flag=false;}
		else if(tmax<0)flag=GL_FALSE;
	}
	//cout<<tmin<<" HERE again "<<tmax<<"\n";
	if(flag)
		cout<<"INTERSECTION\n";
	else
		cout<<"MISSED\n";


	float a, b, c, D;
	a = dirX*dirX + dirY*dirY + dirZ*dirZ;
	b = 2.0f*dirX*px + 2.0f*dirY*py + 2.0f*dirZ*pz;
	c = modelview[12] * modelview[12] + modelview[13] * modelview[13] + modelview[14] * modelview[14];
	c += (posX2*posX2 + posY2*posY2 + posZ2*posZ2);
	c -= 2.0f*(posX2*modelview[12] + posY2*modelview[13] + posZ2*modelview[14]);
	c -= 3 * max(max(modelview[0], modelview[5]), modelview[8])*2;

	D = b*b - 4.0f*a*c;
	if (D < 0)
		cout << "CIRCLE miss";
	else
		cout << "CIRCLE hit";
	display();
	}
}


void display()
{
	//std::cout<<"In display function\n";
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	/*glBegin(GL_POINT);
		glColor3f(255,255,255);
		glVertex2f(0.0f,0.0f);
	glEnd();*/
	//drawRectangularPrism();


	//static GLfloat rot=1;


		//static GLfloat	x=0.1;
		//static GLfloat  c1=-5;
		//static GLfloat  c2=5;
		//glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();
		//glTranslatef(0.0f,0.0f,-15.0f);
		//glColor3f (0.8, 0.2, 0.1);              // Red ball displaced to left.
	 //   glPushMatrix ();
	 //      glTranslatef    (5, 0.0, 0.0);
	 //      glRotatef       (60.0, 1,0,0);
	 //      glRotatef       (rot, 0,0,1);   // Red ball rotates at twice the rate of blue ball.
		//   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, qaGreen);
		//   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, qaGreen);
		//   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, qaWhite);
	 //      glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20);
	 //      glutSolidSphere (1.0, 20, 50);
	 //   glPopMatrix ();
		//glColor3f (0.2, 0.8, 0.1);              // Red ball displaced to left.
	 //   glPushMatrix ();
	 //      //glTranslatef    (-5, 0.0, 0.0);
	 //      glRotatef       (rot, 0,1,0);
	 //      glRotatef       (30.0, 0,0,1);   // Red ball rotates at twice the rate of blue ball.
		//   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, qaGreen);
		//   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, qaRed);
		//   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, qaWhite);
	 //      glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20);
		//   glutSolidTorus(1,2,100,100);
	 //   glPopMatrix ();

		//
		//c1+=x;
		//c2-=x;

		//std::cout<<c1<<std::endl;
		//if(abs((abs(c1)-6))<=1)
		//	x=0.01;
		//	if(abs(c1-c2)<=2)
		//{
		//	x=-0.01;
		//}

		//rot+=1;


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glRotatef(ry,1,0,0);
	/*GLfloat m1[16];
	glGetFloatv(GL_MODELVIEW_MATRIX,m1);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cout<<m1[4*j+i]<<"  ";
		}
		cout<<"\n";
	}

	color c[]={color(0,0,0),color(255,255,255),color(1,0,0),color(1,0,1),color(0,0,1),color(0,1,1)};	
	Objects * obj=new Cuboid(2,-2,-2,6,1,1,c);
	
	obj->draw(m1);

	delete obj;
	Objects * obj2=new Cuboid(2,-2,-2,1,1,6,c);
	obj2->draw(m1);
	delete obj2;
	char ch='y';
	GLint x,y,z,l,b,h;
	color c1;*/
	//Objects *p;
	/*while(ch=='y')
	{
		cin>>x>>y>>z>>l>>b>>h>>c1.r>>c1.g>>c1.b;
		p=new Cuboid(x,y,z,l,b,h,c);
		can1->regObj(p);
		cout<<"Continue ? : ";
		ch='n';
		cin>>ch;
	}
	can1->clearCanvas();*/

	/*glBegin(GL_QUADS);
	glColor3f(1.0,1.0,0.0);
	glVertex3d(-5,-5,-20);
	glVertex3d(-5,5,-20);
	glVertex3d(5,5,-20);
	glVertex3d(5,-5,-20);
	glEnd();*/
	//glTranslatef(1, 1, -50);
	//glBegin(GL_QUADS);
	//glColor3f(1.0, 1.0, 1.0);
	//glVertex3f(-20.0f, 0.0f, 0.0f);
	//glVertex3f(-20.0f, 0.0f, -20.0f);  
	//glVertex3f(20.0f, 0.0f, -20.0f); 
	//glVertex3f(20.0f, 0.0f, 0.0f);
	//glEnd();
	//glBegin(GL_QUADS);
	//glColor3f(1.0, 0.0, 0.0);
	//glVertex3f(0.0f, 0.0f, 0.0f);
	//glVertex3f(0.0f, 20.0f, 0.0f);
	//glVertex3f(0.0f, 20.0f,-20.0f);
	//glVertex3f(0.0f, 0.0f, -20.0f);
	//
	//glBegin(GL_QUADS);
	//glColor3f(1.0, 0.0, 0.0);
	//glVertex3d(posX2 - 0.1, posY2 - 0.1, posZ2-10.1);
	//glVertex3d(posX2 - 0.1, posY2 + 0.1, posZ2-10.1);
	//glVertex3d(posX2 + 0.1, posY2 + 0.1, posZ2-10.1);
	//glVertex3d(posX2 + 0.1, posY2 - 0.1, posZ2-10.1);
	//glEnd(); 
	////glColor3f(1.0f, 1.0f, 1.0f);    // Color Red    
	////glVertex3f(1.0f, 1.0f, -8.9f);    // Top Right Of The Quad (Front)
	////glVertex3f(1.0f, 3.0f, -8.9f);    // Top Left Of The Quad (Front)
	////glVertex3f(3.0f, 3.0f, -8.9f);    // Bottom Left Of The Quad (Front)
	////glVertex3f(3.0f, 1.0f, -8.9f);
	//glEnd();
	//glTranslatef(0, 0, -50);
	//glScaled(3.0, 3.0, 3.0);
	//glRotated(30, 0, 0, 1);
	//glBegin(GL_QUADS);        // Draw The Cube Using quads
	//glColor3f(0.0f, 1.0f, 0.0f);    // Color Blue
	//glVertex3f(1.0f, 1.0f, -1.0f);    // Top Right Of The Quad (Top)
	//glVertex3f(-1.0f, 1.0f, -1.0f);    // Top Left Of The Quad (Top)
	//glVertex3f(-1.0f, 1.0f, 1.0f);    // Bottom Left Of The Quad (Top)
	//glVertex3f(1.0f, 1.0f, 1.0f);    // Bottom Right Of The Quad (Top)
	//glColor3f(1.0f, 0.5f, 0.0f);    // Color Orange
	//glVertex3f(1.0f, -1.0f, 1.0f);    // Top Right Of The Quad (Bottom)
	//glVertex3f(-1.0f, -1.0f, 1.0f);    // Top Left Of The Quad (Bottom)
	//glVertex3f(-1.0f, -1.0f, -1.0f);    // Bottom Left Of The Quad (Bottom)
	//glVertex3f(1.0f, -1.0f, -1.0f);    // Bottom Right Of The Quad (Bottom)
	//glColor3f(1.0f, 0.0f, 0.0f);    // Color Red    
	//glVertex3f(1.0f, 1.0f, 1.0f);    // Top Right Of The Quad (Front)
	//glVertex3f(-1.0f, 1.0f, 1.0f);    // Top Left Of The Quad (Front)
	//glVertex3f(-1.0f, -1.0f, 1.0f);    // Bottom Left Of The Quad (Front)
	//glVertex3f(1.0f, -1.0f, 1.0f);    // Bottom Right Of The Quad (Front)
	//glColor3f(1.0f, 1.0f, 0.0f);    // Color Yellow
	//glVertex3f(1.0f, -1.0f, -1.0f);    // Top Right Of The Quad (Back)
	//glVertex3f(-1.0f, -1.0f, -1.0f);    // Top Left Of The Quad (Back)
	//glVertex3f(-1.0f, 1.0f, -1.0f);    // Bottom Left Of The Quad (Back)
	//glVertex3f(1.0f, 1.0f, -1.0f);    // Bottom Right Of The Quad (Back)
	//glColor3f(0.0f, 0.0f, 1.0f);    // Color Blue
	//glVertex3f(-1.0f, 1.0f, 1.0f);    // Top Right Of The Quad (Left)
	//glVertex3f(-1.0f, 1.0f, -1.0f);    // Top Left Of The Quad (Left)
	//glVertex3f(-1.0f, -1.0f, -1.0f);    // Bottom Left Of The Quad (Left)
	//glVertex3f(-1.0f, -1.0f, 1.0f);    // Bottom Right Of The Quad (Left)
	//glColor3f(1.0f, 0.0f, 1.0f);    // Color Violet
	//glVertex3f(1.0f, 1.0f, -1.0f);    // Top Right Of The Quad (Right)
	//glVertex3f(1.0f, 1.0f, 1.0f);    // Top Left Of The Quad (Right)
	//glVertex3f(1.0f, -1.0f, 1.0f);    // Bottom Left Of The Quad (Right)
	//glVertex3f(1.0f, -1.0f, -1.0f);    // Bottom Right Of The Quad (Right)
	//glEnd();   // -See more at : http ://www.codemiles.com/c-opengl-examples/draw-3d-cube-using-opengl-t9018.html#sthash.L3gWuACU.dpuf
	////glTranslatef(0, 0, 10);
	////cout<<"here\n";
	///*glBegin(GL_QUADS);
	//glColor3f(1.0,1.0,1.0);
	//glVertex3d(posX-5,posY-5,posZ);
	//glVertex3d(posX-5,posY+5,posZ);
	//glVertex3d(posX+5,posY+5,posZ);
	//glVertex3d(posX+5,posY-5,posZ);
	//glEnd();
	//
	///*glColor3f(0.0,1.0,0.0);
	//glTranslated(posX,posY,posZ);
	//glutSolidSphere(1,10,10);*/
	///*glBegin(GL_LINES);
	//glColor3f(0.0,1.0,0.0);
	//glVertex3d(-3,-3,-5);
	//glVertex3d(posX,posY,posZ);*/
	////glColor3f(0.0,1.0,0.0);
	////glVertex3d(0,0,-5);
	////glVertex3d(posX,posY,posZ);
	////glEnd();
glTranslatef(0, 0, -20);
color c[] = { color(0, 0, 0), color(255, 255, 255), color(1, 0, 0), color(1, 0, 1), color(0, 0, 1), color(0, 1, 1) };
Objects *p1 = new Cuboid(0, 0, 0, 2, 2, 2, c);
glTranslatef(-5, 2, 0);
GLfloat m1[16];
glGetFloatv(GL_MODELVIEW_MATRIX, m1);
p1->draw(m1);
glTranslatef(5, -2, 0);
glTranslatef(5, 0, 0);
glGetFloatv(GL_MODELVIEW_MATRIX, m1);
Objects *p2 = new sphere(0, 0, 0, 1,color(1.0,0,0));
p2->draw(m1);
delete p1;
delete p2;

	glutSwapBuffers();
}
GLfloat sq(int x)
{
	GLfloat y=(GLfloat)x/(GLfloat)100;
	return y*y;
}

void drawRectangularPrism()
{
	static GLfloat angle=0;
	static GLfloat distance=10;
	static GLfloat sign=1.0;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,-distance);
	glRotatef(angle, 1.0f, 1.0f, 1.0f);
	glTranslatef(0.0f,0.0f,5.0f);

	glBegin(GL_TRIANGLES);
		glColor3f(0,0,0);
		glVertex3f(0.0f,1.0f,0.0f);
		glVertex3f(-1.0f,-1.0f,0.0f);
		glVertex3f(1.0f,-1.0f,0.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
		glColor3f(0,0,0);
		glVertex3f(0.0f,1.0f,-10.0f);
		glVertex3f(-1.0f,-1.0f,-10.0f);
		glVertex3f(1.0f,-1.0f,-10.0f);
	glEnd();

	glBegin(GL_QUADS);
	{
		glColor3f(0,255,255);
		glVertex3f(0.0f,1.0f,0.0f);
		glVertex3f(0.0f,1.0f,-10.0f);

		glVertex3f(-1.0f,-1.0f,-10.0f);
		glVertex3f(-1.0f,-1.0f,0.0f);
	
	}
	glEnd();
	glBegin(GL_QUADS);
	{
		glColor3f(255,0,255);
		glVertex3f(0.0f,1.0f,0.0f);
		glVertex3f(0.0f,1.0f,-10.0f);
		
		glVertex3f(1.0f,-1.0f,-10.0f);
		glVertex3f(1.0f,-1.0f,0.0f);		
	}
	glEnd();

	glBegin(GL_QUADS);
	{
		glColor3f(255,255,0);

		glVertex3f(-1.0f,-1.0f,-10.0f);
		glVertex3f(-1.0f,-1.0f,0.0f);
		
		glVertex3f(1.0f,-1.0f,0.0f);
		glVertex3f(1.0f,-1.0f,-10.0f);		
	}
	glEnd();

	angle+=1.0;
	distance=distance+sign*0.1;
	if(distance>=80)
		sign=-1;
	else if(distance<=10)
		sign=1;

}


void drawSphereManual()
{	//GLboolean sphere[100][100][100];

	//for(int i=0;i<100;i++)
	//{
	//	for (int j = 0; j < 100; j++)
	//	{
	//		for (int k = 0; k < 100; k++)
	//		{
	//			sphere[i][j][k]=GL_FALSE;
	//		}
	//	}
	//}

	//for(int i=0;i<100;i++)
	//{
	//	for (int j = 0; j < 100; j++)
	//	{
	//		GLfloat ii,jj;
	//		ii=sq(i);
	//		jj=sq(j);
	//		GLdouble p=(GLfloat)1-ii-jj;
	//		if(p>=0)
	//		{
	//			int k=sqrt(10000.0*p);
	//			//std::cout<<i<<"  "<<j<<"  "<<k<<"\n";
	//			sphere[i][j][k]=GL_TRUE;
	//		}
	//	}
	//}
	//static GLfloat angle=0;
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//glTranslatef(0.0f,0.0f,-5.0f);

	//glRotatef(angle, 0.0f, 1.0f, 0.0f);

	//glBegin(GL_POINTS);
	//glColor3f(255,255,255);
	//std::cout<<"reached here\n";
	//for(int i=0;i<100;i++)
	//{
	//	for (int j = 0; j < 100; j++)
	//	{
	//		for (int k = 0; k < 100; k++)
	//		{
	//			if(sphere[i][j][k])
	//				{
	//					//std::cout<<i<<" "<<j<<" "<<k<<std::endl;
	//					glVertex3f((GLfloat)i/100.0f,(GLfloat)j/100.0f,-(GLfloat)k/100.0f);
	//					glVertex3f((GLfloat)i/100.0f,-(GLfloat)j/100.0f,-(GLfloat)k/100.0f);
	//					glVertex3f(-(GLfloat)i/100.0f,(GLfloat)j/100.0f,-(GLfloat)k/100.0f);
	//					glVertex3f(-(GLfloat)i/100.0f,-(GLfloat)j/100.0f,-(GLfloat)k/100.0f);
	//					glVertex3f((GLfloat)i/100.0f,(GLfloat)j/100.0f,(GLfloat)k/100.0f);
	//					glVertex3f((GLfloat)i/100.0f,-(GLfloat)j/100.0f,(GLfloat)k/100.0f);
	//					glVertex3f(-(GLfloat)i/100.0f,(GLfloat)j/100.0f,(GLfloat)k/100.0f);
	//					glVertex3f(-(GLfloat)i/100.0f,-(GLfloat)j/100.0f,(GLfloat)k/100.0f);
	//				}
	//		}
	//	}
	//}


	//glEnd();
	//angle+=1.0;
}

void myTriangles(points p1,points p2,points p3)
{
	glBegin(GL_TRIANGLES);
		glColor3f(0,0,0);
		glVertex2f(p1.x,p1.y);
		glVertex2f(p2.x,p2.y);
		glVertex2f(p3.x,p3.y);
	glEnd();
}
void reshape(GLsizei width,GLsizei height)
{
	std::cout<<"In rehape function : "<<width<<"  "<<height<<std::endl;

	if(height==0)
		height=1;
	GLfloat aspect=(GLfloat)width/(GLfloat)height;
	glViewport(0,0,width,height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0,aspect,0.1,100.0);		

}

void timer(int value)
{
	glutPostRedisplay();  
	glutTimerFunc(15, timer, 0);
}
