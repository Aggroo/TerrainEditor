//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"
namespace Example
{
class ExampleApp : public Core::App
{
public:
	/// constructor
	ExampleApp();
	/// destructor
	~ExampleApp();

	/// open app
	bool Open();
	/// run app
	void Run();
private:

	GLuint program;
	GLuint vertexShader;
	GLuint pixelShader;
	GLuint triangle;
	GLuint triangle2;
	Display::Window* window;
	
};
} // namespace Example