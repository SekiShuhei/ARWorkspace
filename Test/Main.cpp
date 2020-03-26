
# include <Siv3D.hpp> // OpenSiv3D v0.4.2


#include "ARVirtualScreen.hpp"
#include "GuiMenu.hpp"


void Main()
{
	Window::SetStyle(WindowStyle::Sizable);
	WindowResizeOption::ResizeSceneSize;
	Scene::SetTextureFilter(TextureFilter::Linear);
	
	ARWorkspace::ARVirtualScreen	ar_screen;
	ar_screen.LoadUserSetting();

	ARWorkspace::GuiMenu		gui_capture_menu(ar_screen);

	// 大きさ 60 のフォントを用意
	const Font font(60);
	double value;

	ar_screen.Initialize();
	ar_screen.SetAutoResizeMode(true);
	
	while (System::Update())
	{
		{

			//test.
			auto size = s3d::Window::ClientSize();
			s3d::Scene::Resize(size);

		}

		ar_screen.Draw();
		gui_capture_menu.Draw();

		font(Profiler::FPS(), U"fps").draw(0.0, 0.0, Palette::Blue);
		
	
	}
}




// OpenSiv3D リファレンス
// https://siv3d.github.io/ja-jp/
//
// チュートリアル
// https://siv3d.github.io/ja-jp/tutorial/basic/
//
// よくある間違い
// https://siv3d.github.io/ja-jp/articles/mistakes/
//
// サポートについて
// https://siv3d.github.io/ja-jp/support/support/
//
// Siv3D Slack (ユーザコミュニティ) への参加
// https://siv3d.github.io/ja-jp/community/community/
//
// 新機能の提案やバグの報告
// https://github.com/Siv3D/OpenSiv3D/issues
//
