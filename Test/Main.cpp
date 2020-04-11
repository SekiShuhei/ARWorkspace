
# include <Siv3D.hpp> // OpenSiv3D v0.4.2


#include "ARVirtualScreen.hpp"
#include "GuiMenu.hpp"
#include "KeyCommand.hpp"

void Main()
{
	s3d::Window::SetStyle(WindowStyle::Sizable);
	s3d::WindowResizeOption::ResizeSceneSize;
	s3d::Scene::SetScaleMode(ScaleMode::ResizeFill);
	s3d::Scene::SetTextureFilter(TextureFilter::Linear);
	
	auto p_ar_screen = std::make_shared<ARWorkspace::ARVirtualScreen>();
	p_ar_screen->ReadConfigFile();

	ARWorkspace::GuiMenu		gui_capture_menu(p_ar_screen);
	ARWorkspace::KeyCommand		key_command(p_ar_screen);

	// 大きさ 60 のフォントを用意
	const Font font(60);
	double value;

	p_ar_screen->Initialize();
	p_ar_screen->SetAutoResizeMode(true);
	
	while (System::Update())
	{
		

		key_command.Update();
		p_ar_screen->Draw();
		gui_capture_menu.Draw();

		font(Profiler::FPS(), U"fps").draw(0.0, 0.0, Palette::Blue);
		
	
	}

	p_ar_screen->WriteConfigFile();
}