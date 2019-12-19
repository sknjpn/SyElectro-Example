#include "TitleViewer.h"
#include "ConnectingViewer.h"

void TitleViewer::init()
{
}

void TitleViewer::update()
{
	{
		static Font font(32);

		font(U"スペースキーを押してください").drawAt(getViewerSize() / 2);
	}

	if (KeySpace.down())
	{
		getParentViewer()->addChildViewer<ConnectingViewer>();

		destroy();
	}
}
