#pragma once

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "canvas.h"
#include "Objects.h"
#include <vector>
//#include "test1.cpp"
const float PI = 3.14159265358979323846f;

using namespace System::Windows::Forms;

namespace OpenGLForm
{
	float radius = 4;
	void OnMouseClick(int button, int state, int x, int y)
	{
	//	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	//	{
	//		//store the x,y value where the click happened
	//		radius = 8;
	//	}

	}

	template <typename TYPE>
	class GeometryVector
	{


	public:

		GeometryVector(const TYPE x_ = TYPE(), const TYPE y_ = TYPE(), const TYPE z_ = TYPE());

		const TYPE x;
		const TYPE y;
		const TYPE z;

	};
#pragma pack()

	template <typename TYPE>
	GeometryVector<TYPE>::GeometryVector(const TYPE x_, const TYPE y_, const TYPE z_)
		:
		x(x_),
		y(y_),
		z(z_)
	{
	}

	class StackedSphere
	{

	public:

		StackedSphere(const float radius = 1.0f, const unsigned int stacks = 8, const unsigned int slices = 16);
		void render() const;

	private:

		std::vector<GeometryVector<float> > geometryData_;
		std::vector<unsigned short> indexData_;

	};

	StackedSphere::StackedSphere(const float radius, const unsigned int stacks, const unsigned int slices)
	{
		for (unsigned int stackNumber = 0; stackNumber <= stacks; ++stackNumber)
		{
			for (unsigned int sliceNumber = 0; sliceNumber < slices; ++sliceNumber)
			{
				float theta = stackNumber * PI / stacks;
				float phi = sliceNumber * 2 * PI / slices;
				float sinTheta = std::sin(theta);
				float sinPhi = std::sin(phi);
				float cosTheta = std::cos(theta);
				float cosPhi = std::cos(phi);
				geometryData_.push_back(GeometryVector<float>(radius * cosPhi * sinTheta, radius * sinPhi * sinTheta, radius * cosTheta));
			}
		}
		for (unsigned int stackNumber = 0; stackNumber < stacks; ++stackNumber)
		{
			for (unsigned int sliceNumber = 0; sliceNumber <= slices; ++sliceNumber)
			{
				indexData_.push_back((stackNumber * slices) + (sliceNumber % slices));
				indexData_.push_back(((stackNumber + 1) * slices) + (sliceNumber % slices));
			}
		}
	}

	void StackedSphere::render() const
	{
		glVertexPointer(3, GL_FLOAT, 0, &geometryData_[0]);
		glEnableClientState(GL_VERTEX_ARRAY);
		glDrawElements(GL_TRIANGLE_STRIP, indexData_.size(), GL_UNSIGNED_SHORT, &indexData_[0]);
	}

	class SpiralSphere
	{

	public:

		SpiralSphere(const float radius = 1.0f, const unsigned int loops = 8, const unsigned int segmentsPerLoop = 16);
		void render() const;

	private:

		std::vector<GeometryVector<float> > geometryData_;
		std::vector<unsigned short> indexData_;

	};

	SpiralSphere::SpiralSphere(const float radius, const unsigned int loops, const unsigned int segmentsPerLoop)
	{
		for (unsigned int loopSegmentNumber = 0; loopSegmentNumber < segmentsPerLoop; ++loopSegmentNumber)
		{
			float theta = 0;
			float phi = loopSegmentNumber * 2 * PI / segmentsPerLoop;
			float sinTheta = std::sin(theta);
			float sinPhi = std::sin(phi);
			float cosTheta = std::cos(theta);
			float cosPhi = std::cos(phi);
			geometryData_.push_back(GeometryVector<float>(radius * cosPhi * sinTheta, radius * sinPhi * sinTheta, radius * cosTheta));

		}
		for (unsigned int loopNumber = 0; loopNumber <= loops; ++loopNumber)
		{
			for (unsigned int loopSegmentNumber = 0; loopSegmentNumber < segmentsPerLoop; ++loopSegmentNumber)
			{
				float theta = (loopNumber * PI / loops) + ((PI * loopSegmentNumber) / (segmentsPerLoop * loops));
				if (loopNumber == loops)
				{
					theta = PI;
				}
				float phi = loopSegmentNumber * 2 * PI / segmentsPerLoop;
				float sinTheta = std::sin(theta);
				float sinPhi = std::sin(phi);
				float cosTheta = std::cos(theta);
				float cosPhi = std::cos(phi);
				geometryData_.push_back(GeometryVector<float>(radius * cosPhi * sinTheta, radius * sinPhi * sinTheta, radius * cosTheta));

			}
		}
		for (unsigned int loopSegmentNumber = 0; loopSegmentNumber < segmentsPerLoop; ++loopSegmentNumber)
		{
			indexData_.push_back(loopSegmentNumber);
			indexData_.push_back(segmentsPerLoop + loopSegmentNumber);
		}
		for (unsigned int loopNumber = 0; loopNumber < loops; ++loopNumber)
		{
			for (unsigned int loopSegmentNumber = 0; loopSegmentNumber < segmentsPerLoop; ++loopSegmentNumber)
			{
				indexData_.push_back(((loopNumber + 1) * segmentsPerLoop) + loopSegmentNumber);
				indexData_.push_back(((loopNumber + 2) * segmentsPerLoop) + loopSegmentNumber);
			}
		}
	}

	void SpiralSphere::render() const
	{
		glVertexPointer(3, GL_FLOAT, 0, &geometryData_[0]);
		glEnableClientState(GL_VERTEX_ARRAY);
		glDrawElements(GL_TRIANGLE_STRIP, indexData_.size(), GL_UNSIGNED_SHORT, &indexData_[0]);
	}
	public ref class COpenGL : public System::Windows::Forms::NativeWindow
	{
	public:
		COpenGL(System::Windows::Forms::Form ^ parentForm, GLsizei iWidth, GLsizei iHeight)
		{
			CreateParams^ cp = gcnew CreateParams;

			// Set the position on the form
			cp->X = 40;
			cp->Y = 60;
			cp->Height = iHeight;
			cp->Width = iWidth;

			// Specify the form as the parent.
			cp->Parent = parentForm->Handle;

			// Create as a child of the specified parent and make OpenGL compliant (no clipping)
			cp->Style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
			
			// Create the actual window
			this->CreateHandle(cp);

			m_hDC = GetDC((HWND)this->Handle.ToPointer());

			if (m_hDC)
			{
				MySetPixelFormat(m_hDC);
				ReSizeGLScene(iWidth, iHeight);
				//glutMouseFunc(OnMouseClick);
				InitGL();
			}

			rtri = 0.0f;
			rquad = 0.0f;
		}

		System::Void Render(System::Void)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear screen and depth buffer
			//draw1();
			//drawCone(0, 0, 0, 1, 1);
			//circle(0, 0, 0, 10);
			//display();
			//cylinder(0, 0, 0, 10, 10);
			draw_cylinder(50, 0, 2, 4);
		}
		void draw_cylinder(int n, int arg, float mult, float v) {
			/*
			Function drw_polygon:
			Arguments:
			n - number of sides
			arg - starting angle (not so important at all)
			mult - multiplying sides to incrase their length
			v - cylinder height
			*/

			// DumbProof Double Check :)
			if (arg < 0)
				arg = 0;
			float M_PI = 3;
			//for (float M_PI = 0; M_PI < 4 * n - 4;)
			// Cylinder Bottom
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			glTranslated(0, 0, -30);
			glRotatef(60, 1.0f, 0.0f, 0.0f);
			glBegin(GL_POLYGON);
			glColor4f(1.0, 0.0, 0.0, 1.0);
			for (int i = arg; i <= (360 + arg); i += (360 / n)) {
				float a = i * M_PI / 180; // degrees to radians
				glVertex3f(mult * cos(a), mult * sin(a), 0.0);
			}
			glEnd();

			// Cylinder Top
			glBegin(GL_POLYGON);
			glColor4f(0.0, 0.0, 1.0, 1.0);
			for (int i = arg; i <= (360 + arg); i += (360 / n)) {
				float a = i * M_PI / 180; // degrees to radians
				glVertex3f(mult * cos(a), mult * sin(a), v);
			}
			glEnd();

			// Cylinder "Cover"
			glBegin(GL_QUAD_STRIP);
			glColor4f(1.0, 1.0, 0.0, 1.0);
			for (int i = arg; i < 480; i += (360 / n)) {
				float a = i * M_PI / 180; // degrees to radians
				glVertex3f(mult * cos(a), mult * sin(a), 0.0);
				glVertex3f(mult * cos(a), mult * sin(a), v);
			}
			glEnd();

		}
		void cylinder(float a, float b, float c, float r, float h){
			float p, k;
			float x, y;
			x = 0;
			y = r * 10;
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			glTranslated(0, 0, -30);
			glRotatef(15, 1.0f, 0.0f, 0.0f);
			glBegin(GL_TRIANGLE_STRIP);
			glColor3f(1.0, 1.0, 1.0);
			
			p = 1 - r * 10;
			k = 0;
			while (x <= y){
				float xs = x / 10;
				float ys = y / 10;
				float h2 = h / 2;
				glVertex3f(xs + a, b+h2, ys + c);
				glVertex3f(xs + a, b - h2, ys + c);
				glVertex3f(xs + a, b + h2, -ys + c);
				glVertex3f(xs + a, b - h2, -ys + c);
				glVertex3f(-xs + a, b + h2, ys + c);
				glVertex3f(-xs + a, b - h2, ys + c);
				glVertex3f(-xs + a, b + h2, -ys + c);
				glVertex3f(-xs + a, b - h2, -ys + c);
				glVertex3f(ys + a, b + h2, xs + c);
				glVertex3f(ys + a, b - h2, xs + c);
				glVertex3f(-ys + a, b + h2, xs + c);
				glVertex3f(-ys + a, b - h2, xs + c);
				glVertex3f(ys + a, b + h2, -xs + c);
				glVertex3f(ys + a, b - h2, -xs + c);
				glVertex3f(-ys + a, b + h2, -xs + c);
				glVertex3f(-ys + a, b - h2, -xs + c);
				if (p<0){
					x = x + 1;
					p = (p + 2 * x + 1);
				}
				else{
					x = x + 1;
					y = y - 1;
					p = (p + 2 * x + 1 - 2 * y);
				}
				k++;
			}

			glEnd();
			x = 0;
			y = r * 10;
			glBegin(GL_TRIANGLE_STRIP);
			glColor3f(1.0, 0.0, 0.0);
			glVertex3f(a, b, c);
			p = 1 - r * 10;
			k = 0;
			while (x <= y){
				float xs = x / 10;
				float ys = y / 10;
				float h2 = h / 2;
				glVertex3f(xs + a, b+h2, ys + c);
				glVertex3f(xs + a, b + h2, -ys + c);
				glVertex3f(-xs + a, b + h2, ys + c);
				glVertex3f(-xs + a, b + h2, -ys + c);
				glVertex3f(ys + a, b + h2, xs + c);
				glVertex3f(-ys + a, b + h2, xs + c);
				glVertex3f(ys + a, b + h2, -xs + c);
				glVertex3f(-ys + a, b + h2, -xs + c);
				if (p<0){
					x = x + 1;
					p = (p + 2 * x + 1);
				}
				else{
					x = x + 1;
					y = y - 1;
					p = (p + 2 * x + 1 - 2 * y);
				}
				k++;
			}

			glEnd();
			x = 0;
			y = r * 10;
			glBegin(GL_TRIANGLE_STRIP);
			glColor3f(1.0, 0.0, 0.0);
			glVertex3f(a, b, c);
			p = 1 - r * 10;
			k = 0;
			while (x <= y){
				float xs = x / 10;
				float ys = y / 10;
				float h2 = -h / 2;
				glVertex3f(xs + a, b + h2, ys + c);
				glVertex3f(xs + a, b + h2, -ys + c);
				glVertex3f(-xs + a, b + h2, ys + c);
				glVertex3f(-xs + a, b + h2, -ys + c);
				glVertex3f(ys + a, b + h2, xs + c);
				glVertex3f(-ys + a, b + h2, xs + c);
				glVertex3f(ys + a, b + h2, -xs + c);
				glVertex3f(-ys + a, b + h2, -xs + c);
				if (p<0){
					x = x + 1;
					p = (p + 2 * x + 1);
				}
				else{
					x = x + 1;
					y = y - 1;
					p = (p + 2 * x + 1 - 2 * y);
				}
				k++;
			}

			glEnd();
		}


		void display()
		{
			//std::cout<<"In display function\n";
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
			glTranslatef(0, 0, -50);
			//glScaled(3.0, 3.0, 3.0);
			//glRotated(30, 0, 0, 1);
			glBegin(GL_QUADS);        // Draw The Cube Using quads
			glColor3f(0.0f, 1.0f, 0.0f);    // Color Blue
			glVertex3f(1.0f, 1.0f, -1.0f);    // Top Right Of The Quad (Top)
			glVertex3f(-1.0f, 1.0f, -1.0f);    // Top Left Of The Quad (Top)
			glVertex3f(-1.0f, 1.0f, 1.0f);    // Bottom Left Of The Quad (Top)
			glVertex3f(1.0f, 1.0f, 1.0f);    // Bottom Right Of The Quad (Top)
			glColor3f(1.0f, 0.5f, 0.0f);    // Color Orange
			glVertex3f(1.0f, -1.0f, 1.0f);    // Top Right Of The Quad (Bottom)
			glVertex3f(-1.0f, -1.0f, 1.0f);    // Top Left Of The Quad (Bottom)
			glVertex3f(-1.0f, -1.0f, -1.0f);    // Bottom Left Of The Quad (Bottom)
			glVertex3f(1.0f, -1.0f, -1.0f);    // Bottom Right Of The Quad (Bottom)
			glColor3f(1.0f, 0.0f, 0.0f);    // Color Red    
			glVertex3f(1.0f, 1.0f, 1.0f);    // Top Right Of The Quad (Front)
			glVertex3f(-1.0f, 1.0f, 1.0f);    // Top Left Of The Quad (Front)
			glVertex3f(-1.0f, -1.0f, 1.0f);    // Bottom Left Of The Quad (Front)
			glVertex3f(1.0f, -1.0f, 1.0f);    // Bottom Right Of The Quad (Front)
			glColor3f(1.0f, 1.0f, 0.0f);    // Color Yellow
			glVertex3f(1.0f, -1.0f, -1.0f);    // Top Right Of The Quad (Back)
			glVertex3f(-1.0f, -1.0f, -1.0f);    // Top Left Of The Quad (Back)
			glVertex3f(-1.0f, 1.0f, -1.0f);    // Bottom Left Of The Quad (Back)
			glVertex3f(1.0f, 1.0f, -1.0f);    // Bottom Right Of The Quad (Back)
			glColor3f(0.0f, 0.0f, 1.0f);    // Color Blue
			glVertex3f(-1.0f, 1.0f, 1.0f);    // Top Right Of The Quad (Left)
			glVertex3f(-1.0f, 1.0f, -1.0f);    // Top Left Of The Quad (Left)
			glVertex3f(-1.0f, -1.0f, -1.0f);    // Bottom Left Of The Quad (Left)
			glVertex3f(-1.0f, -1.0f, 1.0f);    // Bottom Right Of The Quad (Left)
			glColor3f(1.0f, 0.0f, 1.0f);    // Color Violet
			glVertex3f(1.0f, 1.0f, -1.0f);    // Top Right Of The Quad (Right)
			glVertex3f(1.0f, 1.0f, 1.0f);    // Top Left Of The Quad (Right)
			glVertex3f(1.0f, -1.0f, 1.0f);    // Bottom Left Of The Quad (Right)
			glVertex3f(1.0f, -1.0f, -1.0f);    // Bottom Right Of The Quad (Right)
			glEnd();   // -See more at : http ://www.codemiles.com/c-opengl-examples/draw-3d-cube-using-opengl-t9018.html#sthash.L3gWuACU.dpuf
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
			//glTranslatef(-5, 0, 0);
			color c[] = { color(0, 0, 0), color(255, 255, 255), color(1, 0, 0), color(1, 0, 1), color(0, 0, 1), color(0, 1, 1) };
			Objects *p1 = new Cuboid(0, 0, 0, 2, 2, 2, c);
			glTranslatef(-5, 2, 0);
			GLfloat m1[16];
			glGetFloatv(GL_MODELVIEW_MATRIX, m1);
			p1->draw(m1);
			glTranslatef(5, -2, 0);
			glTranslatef(5, 0, 0);
			StackedSphere sphere1(radius, 20, 40);
			sphere1.render();
			//glutSolidSphere(3, 100, 100);
			glGetFloatv(GL_MODELVIEW_MATRIX, m1);
			//Objects *p2 = new sphere(0, 0, 0, 1, color(1.0, 0, 0));
			//p2->draw(m1);
			delete p1;
			//delete p2;
		}

		
		void drawCone(float a, float b, float c, float r, float h){
			float p, k;
			float x, y;
			x = 0;
			y = r * 10;
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			glTranslated(0, 0, -30);
			glRotatef(-15, 1.0f, 0.0f, 0.0f);
			glBegin(GL_TRIANGLE_FAN);
			glColor3f(1.0, 1.0, 1.0);
			glVertex3f(a, b+h, c);
			p = 1 - r * 10;
			k = 0;
			while (x <= y){
				float xs = x / 10;
				float ys = y / 10;
				glVertex3f(xs + a, b, ys + c);
				glVertex3f(xs + a, b, -ys + c);
				glVertex3f(-xs + a, b, ys + c);
				glVertex3f(-xs + a, b, -ys + c);
				glVertex3f(ys + a, b, xs + c);
				glVertex3f(-ys + a, b, xs + c);
				glVertex3f(ys + a, b, -xs + c);
				glVertex3f(-ys + a, b, -xs + c);
				if (p<0){
					x = x + 1;
					p = (p + 2 * x + 1);
				}
				else{
					x = x + 1;
					y = y - 1;
					p = (p + 2 * x + 1 - 2 * y);
				}
				k++;
			}

			glEnd();
			
			x = 0;
			y = r * 10;
			glBegin(GL_TRIANGLE_STRIP);
			glColor3f(1.0, 0.0, 0.0);
			glVertex3f(a, b, c);
			p = 1 - r * 10;
			k = 0;
			while (x <= y){
				float xs = x / 10;
				float ys = y / 10;
				glVertex3f(xs + a, b, ys + c);
				glVertex3f(xs + a, b, -ys + c);
				glVertex3f(-xs + a, b, ys + c);
				glVertex3f(-xs + a, b, -ys + c);
				glVertex3f(ys + a, b, xs + c);
				glVertex3f(-ys + a, b, xs + c);
				glVertex3f(ys + a, b, -xs + c);
				glVertex3f(-ys + a, b, -xs + c);
				if (p<0){
					x = x + 1;
					p = (p + 2 * x + 1);
				}
				else{
					x = x + 1;
					y = y - 1;
					p = (p + 2 * x + 1 - 2 * y);
				}
				k++;
			}

			glEnd();
		}

		void circle(float a, float b, float c, float r){
			float p, k;
			float x, y;
			x = 0;
			y = r*10;
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			
			glTranslated(0, 0, -30);
			glRotatef(-15, 1.0f, 0.0f, 0.0f);
			glBegin(GL_TRIANGLE_STRIP);
			glColor3f(1.0,1.0,0.0);
			glVertex3f(a,b,c);
			p = 1 - r*10;
			k = 0;
			while (x <= y){
				float xs = x / 10;
				float ys = y / 10;
				glVertex3f(xs + a, b, ys + c);
				glVertex3f(xs + a, b, -ys + c);
				glVertex3f(-xs + a, b, ys + c);
				glVertex3f(-xs + a, b, -ys + c);
				glVertex3f(ys + a, b, xs + c);
				glVertex3f(-ys + a, b, xs + c);
				glVertex3f(ys + a, b, -xs + c);
				glVertex3f(-ys + a, b, -xs + c);
				if (p<0){
					x = x + 1;
					p = (p + 2 * x + 1);
				}
				else{
					x = x + 1;
					y = y - 1;
					p = (p + 2 * x + 1 - 2 * y);
				}
				k++;
			}

			glEnd();
		}

		System::Void draw1(System::Void)
		{
			glLoadIdentity();									// Reset the current modelview matrix
			glTranslatef(-1.5f, 0.0f, -6.0f);						// Move left 1.5 units and into the screen 6.0
			glRotatef(rtri, 0.0f, 1.0f, 0.0f);						// Rotate the triangle on the y axis 
			glBegin(GL_TRIANGLES);								// Start drawing a triangle
			glColor3f(1.0f, 0.0f, 0.0f);						// Red
			glVertex3f(0.0f, 1.0f, 0.0f);					// Top Of triangle (front)
			glColor3f(0.0f, 1.0f, 0.0f);						// Green
			glVertex3f(-1.0f, -1.0f, 1.0f);					// Left of triangle (front)
			glColor3f(0.0f, 0.0f, 1.0f);						// Blue
			glVertex3f(1.0f, -1.0f, 1.0f);					// Right of triangle (front)
			glColor3f(1.0f, 0.0f, 0.0f);						// Red
			glVertex3f(0.0f, 1.0f, 0.0f);					// Top Of triangle (right)
			glColor3f(0.0f, 0.0f, 1.0f);						// Blue
			glVertex3f(1.0f, -1.0f, 1.0f);					// Left of triangle (right)
			glColor3f(0.0f, 1.0f, 0.0f);						// Green
			glVertex3f(1.0f, -1.0f, -1.0f);					// Right of triangle (right)
			glColor3f(1.0f, 0.0f, 0.0f);						// Red
			glVertex3f(0.0f, 1.0f, 0.0f);					// Top Of triangle (back)
			glColor3f(0.0f, 1.0f, 0.0f);						// Green
			glVertex3f(1.0f, -1.0f, -1.0f);					// Left of triangle (back)
			glColor3f(0.0f, 0.0f, 1.0f);						// Blue
			glVertex3f(-1.0f, -1.0f, -1.0f);					// Right of triangle (back)
			glColor3f(1.0f, 0.0f, 0.0f);						// Red
			glVertex3f(0.0f, 1.0f, 0.0f);					// Top Of triangle (left)
			glColor3f(0.0f, 0.0f, 1.0f);						// Blue
			glVertex3f(-1.0f, -1.0f, -1.0f);					// Left of triangle (left)
			glColor3f(0.0f, 1.0f, 0.0f);						// Green
			glVertex3f(-1.0f, -1.0f, 1.0f);					// Right of triangle (left)
			glEnd();											// Done drawing the pyramid

			glLoadIdentity();									// Reset the current modelview matrix
			glTranslatef(1.5f, 0.0f, -7.0f);						// Move right 1.5 units and into the screen 7.0
			glRotatef(rquad, 1.0f, 1.0f, 1.0f);					// Rotate the quad on the x axis 
			glBegin(GL_QUADS);									// Draw a quad
			glColor3f(0.0f, 1.0f, 0.0f);						// Set The color to green
			glVertex3f(1.0f, 1.0f, -1.0f);					// Top Right of the quad (top)
			glVertex3f(-1.0f, 1.0f, -1.0f);					// Top Left of the quad (top)
			glVertex3f(-1.0f, 1.0f, 1.0f);					// Bottom left of the quad (top)
			glVertex3f(1.0f, 1.0f, 1.0f);					// Bottom right of the quad (top)
			glColor3f(1.0f, 0.5f, 0.0f);						// Set The color to orange
			glVertex3f(1.0f, -1.0f, 1.0f);					// Top Right of the quad (bottom)
			glVertex3f(-1.0f, -1.0f, 1.0f);					// Top Left of the quad (bottom)
			glVertex3f(-1.0f, -1.0f, -1.0f);					// Bottom left of the quad (bottom)
			glVertex3f(1.0f, -1.0f, -1.0f);					// Bottom right of the quad (bottom)
			glColor3f(1.0f, 0.0f, 0.0f);						// Set The color to red
			glVertex3f(1.0f, 1.0f, 1.0f);					// Top Right of the quad (front)
			glVertex3f(-1.0f, 1.0f, 1.0f);					// Top Left of the quad (front)
			glVertex3f(-1.0f, -1.0f, 1.0f);					// Bottom left of the quad (front)
			glVertex3f(1.0f, -1.0f, 1.0f);					// Bottom right of the quad (front)
			glColor3f(1.0f, 1.0f, 0.0f);						// Set The color to yellow
			glVertex3f(1.0f, -1.0f, -1.0f);					// Top Right of the quad (back)
			glVertex3f(-1.0f, -1.0f, -1.0f);					// Top Left of the quad (back)
			glVertex3f(-1.0f, 1.0f, -1.0f);					// Bottom left of the quad (back)
			glVertex3f(1.0f, 1.0f, -1.0f);					// Bottom right of the quad (back)
			glColor3f(0.0f, 0.0f, 1.0f);						// Set The color to blue
			glVertex3f(-1.0f, 1.0f, 1.0f);					// Top Right of the quad (left)
			glVertex3f(-1.0f, 1.0f, -1.0f);					// Top Left of the quad (left)
			glVertex3f(-1.0f, -1.0f, -1.0f);					// Bottom left of the quad (left)
			glVertex3f(-1.0f, -1.0f, 1.0f);					// Bottom right of the quad (left)
			glColor3f(1.0f, 0.0f, 1.0f);						// Set The color to violet
			glVertex3f(1.0f, 1.0f, -1.0f);					// Top Right of the quad (right)
			glVertex3f(1.0f, 1.0f, 1.0f);					// Top Left of the quad (right)
			glVertex3f(1.0f, -1.0f, 1.0f);					// Bottom left of the quad (right)
			glVertex3f(1.0f, -1.0f, -1.0f);					// Bottom right of the quad (right)
			glEnd();											// Done drawing the quad

			rtri += valrtri;											// Increase the rotation variable for the triangle
			rquad += valrquad;										// Decrease the rotation variable for the quad
		}

		System::Void clicked(System::Void)
		{
			valrtri = -valrtri;
			valrquad = -valrquad;
		}

		System::Void SwapOpenGLBuffers(System::Void)
		{
			SwapBuffers(m_hDC);
		}

	private:
		HDC m_hDC;
		HGLRC m_hglrc;
		GLfloat	rtri;				// Angle for the triangle
		GLfloat	rquad;				// Angle for the quad
		GLfloat	valrtri=.2f;
		GLfloat	valrquad=-.15f;

	protected:
		~COpenGL(System::Void)
		{
			this->DestroyHandle();
		}

		GLint MySetPixelFormat(HDC hdc)
		{
			static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
			{
				sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
				1,											// Version Number
				PFD_DRAW_TO_WINDOW |						// Format Must Support Window
				PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
				PFD_DOUBLEBUFFER,							// Must Support Double Buffering
				PFD_TYPE_RGBA,								// Request An RGBA Format
				16,										// Select Our Color Depth
				0, 0, 0, 0, 0, 0,							// Color Bits Ignored
				0,											// No Alpha Buffer
				0,											// Shift Bit Ignored
				0,											// No Accumulation Buffer
				0, 0, 0, 0,									// Accumulation Bits Ignored
				16,											// 16Bit Z-Buffer (Depth Buffer)  
				0,											// No Stencil Buffer
				0,											// No Auxiliary Buffer
				PFD_MAIN_PLANE,								// Main Drawing Layer
				0,											// Reserved
				0, 0, 0										// Layer Masks Ignored
			};

			GLint  iPixelFormat;

			// get the device context's best, available pixel format match 
			if ((iPixelFormat = ChoosePixelFormat(hdc, &pfd)) == 0)
			{
				MessageBox::Show("ChoosePixelFormat Failed");
				return 0;
			}

			// make that match the device context's current pixel format 
			if (SetPixelFormat(hdc, iPixelFormat, &pfd) == FALSE)
			{
				MessageBox::Show("SetPixelFormat Failed");
				return 0;
			}

			if ((m_hglrc = wglCreateContext(m_hDC)) == NULL)
			{
				MessageBox::Show("wglCreateContext Failed");
				return 0;
			}

			if ((wglMakeCurrent(m_hDC, m_hglrc)) == NULL)
			{
				MessageBox::Show("wglMakeCurrent Failed");
				return 0;
			}


			return 1;
		}




		bool InitGL(GLvoid)										// All setup for opengl goes here
		{
			glShadeModel(GL_SMOOTH);							// Enable smooth shading
			glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black background
			glClearDepth(1.0f);									// Depth buffer setup
			glEnable(GL_DEPTH_TEST);							// Enables depth testing
			glDepthFunc(GL_LEQUAL);								// The type of depth testing to do
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really nice perspective calculations
			return TRUE;										// Initialisation went ok
		}
		
		GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize and initialise the gl window
		{
			if (height == 0)										// Prevent A Divide By Zero By
			{
				height = 1;										// Making Height Equal One
			}

			glViewport(0, 0, width, height);						// Reset The Current Viewport
			
			glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
			glLoadIdentity();									// Reset The Projection Matrix

			// Calculate The Aspect Ratio Of The Window
			gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

			glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
			glLoadIdentity();									// Reset The Modelview Matrix
		}
	};
}