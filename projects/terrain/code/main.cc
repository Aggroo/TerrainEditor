//------------------------------------------------------------------------------
// main.cc
// (C) 2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "application.h"


int
main(int argc, const char** argv)
{
	Application::Application app;

	glfwWindowHint(GLFW_SAMPLES, 4);
	if (app.Open())
	{
		app.Run();
		app.Close();
	}
	app.Exit();
	
}
