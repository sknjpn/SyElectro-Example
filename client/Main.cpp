#include "TitleViewer.h"
#include "World.h"

unique_ptr<TCPClient> g_client;

void Main()
{
	TextureAsset::Register(U"battery", U"assets/battery.png");
	TextureAsset::Register(U"light1", U"assets/light1.png");
	TextureAsset::Register(U"light2", U"assets/light2.png");
	TextureAsset::Register(U"relay1", U"assets/relay1.png");
	TextureAsset::Register(U"relay2", U"assets/relay2.png");
	TextureAsset::Register(U"switch1", U"assets/switch1.png");
	TextureAsset::Register(U"switch2", U"assets/switch2.png");

	auto items = FileSystem::DirectoryContents(U"assets/");
	for (auto item : items)
	{
		Image img(item);
		for (auto& p : img) if (p == Palette::White) p = Color(0, 0);

		img.save(item);
	}

	Window::SetTitle(U"SyElectro Client");

	g_client = MakeUnique<TCPClient>();
	g_world = MakeUnique<World>();


	EasyViewer::GetRootViewer()->addChildViewer<TitleViewer>();
	EasyViewer::Run();

	// サーバーのアドレス解決

	// サーバーへの接続

	// データ同期

	// 更新

	//  コマンド取得

	//  コマンド送信

	g_client.reset();
	g_world.reset();
}
