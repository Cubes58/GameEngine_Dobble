#include "UserInterface.h"

#include "Window.h"
#include "Shader.h"

UserInterface::UserInterface(const Vector2D<float> &p_ScreenSize, const std::string &p_File) {
	m_FontRenderer = std::make_shared<FontRenderer>("resources/fonts/arial.ttf", p_ScreenSize);
	LoadUserInterfaceLayout(p_File);
}

void UserInterface::LoadUserInterfaceLayout(const std::string &p_File) {

}

void UserInterface::Update(float p_DeltaTime) {

}

void UserInterface::Render(Window &p_Window) {

}