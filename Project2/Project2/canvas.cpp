#include "canvas.h"


canvas::canvas(void)
{
	this->count=0;
}


canvas::~canvas(void)
{
	cout<<"destroy canvas\n";
	this->clearCanvas();
}

void canvas::regObj(Objects *o)
{
	this->objects.push_back(make_pair(++count,o));
	this->refresh();
}

void canvas::refresh()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	for(int i=0;i<count;i++)
	{
		//objects[i].second->draw();
	}
	glutSwapBuffers();
}

void canvas::clearCanvas()
{
	count=0;
	for(int i=0;i<objects.size();i++)
		delete objects[i].second;
}