#pragma once
#include "Objects.h"
class canvas
{
	vector<pair<GLint ,Objects *> > objects;
	GLint count;
public:
	canvas(void);
	~canvas(void);
	void regObj(Objects *);
	void refresh();
	void eraseObj(GLint);
	void clearCanvas(void);

};

