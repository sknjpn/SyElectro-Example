#include "ConnectingViewer.h"
#include "WorldViewer.h"

void ConnectingViewer::init()
{
	//g_client->connect(IPv4::Localhost(), 31400);

	// xの部分をサーバーのIPアドレスに置き換えてください
	g_client->connect(IPv4(U"xxx.xx.xx.xxx"), 31400);
}

void ConnectingViewer::update()
{
	{
		static Font font(16);

		font(U"接続中...").draw();
	}

	if (g_client->isConnected())
	{
		getParentViewer()->addChildViewer<WorldViewer>();

		destroy();
	}
}
