
# include <Siv3D.hpp> // OpenSiv3D v0.4.2


#include "ARVirtualScreen.hpp"

#include "Utilty.hpp"

//#include "WinScreenCapture.hpp"
//#include "CustomCursor.hpp"
//#include "SensorApiManager.hpp"



void Main()
{
	Window::SetStyle(WindowStyle::Sizable);
	WindowResizeOption::ResizeSceneSize;
	Scene::SetTextureFilter(TextureFilter::Nearest);
	
	ARWorkspace::ARVirtualScreen	ar_screen;
	ar_screen.LoadUserSetting();

	// 大きさ 60 のフォントを用意
	const Font font(60);
	double value;

	ar_screen.initialize();
	
	while (System::Update())
	{
		{

			//test.
			auto size = s3d::Window::ClientSize();
			//s3d::Scene::Resize(size);

		}

		//ar_screen.Update();
		ar_screen.Draw();

		font(Profiler::FPS(), U"fps").draw(0.0, 0.0, Palette::Blue);
		
		//double x = s3d::Scene::Size().x - 300;
		//double y = 0;
		//double h = 30;
		//
		//SimpleGUI::Slider(U"R {:.2f}"_fmt(value), value, 0, 600, Vec2(x, y     ), 100, 200);
		//SimpleGUI::Slider(U"R {:.2f}"_fmt(value), value, 0, 600, Vec2(x, y += h), 100, 200);
		//SimpleGUI::Slider(U"R {:.2f}"_fmt(value), value, 0, 600, Vec2(x, y += h), 100, 200);
		//SimpleGUI::Slider(U"R {:.2f}"_fmt(value), value, 0, 600, Vec2(x, y += h), 100, 200);
	
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
