//------------------------------------------------------------------------------
// main.cc
// (C) 2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "Application.h"


int
main(int argc, const char** argv)
{
	Example::Application app;

	glfwWindowHint(GLFW_SAMPLES, 4);
	if (app.Open())
	{
		app.Run();
		app.Close();
	}
	app.Exit();
	
}
