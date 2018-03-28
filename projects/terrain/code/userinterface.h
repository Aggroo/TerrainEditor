#pragma once
#include <memory>

namespace Example{ class CGLab; }

class UserInterface
{
public:
	UserInterface(Example::CGLab* app);
	~UserInterface();

	void Run();
	void ShowFileMenu();
	void RenderDocks();

private:
	Example::CGLab* application;
};

