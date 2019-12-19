#pragma once

#include "EasyViewer.h"

class ConnectingViewer
	: public EasyViewer
{
public:
	void init() override;
	void update() override;
};

