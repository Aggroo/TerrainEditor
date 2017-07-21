//------------------------------------------------------------------------------
// main.cc
// (C) 2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"


int
main(int argc, const char** argv)
{
	Example::ExampleApp app;
	if (app.Open())
	{
		app.Run();
		app.Close();
	}
	app.Exit();
	
}