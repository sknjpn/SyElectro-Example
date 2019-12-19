#include "Command_SetWire.h"
#include "World.h"
#include "Wire.h"

void Command_SetWire::action()
{
	g_world->addWire(MakeShared<Wire>(g_world->getNode(m_nodeFrID), g_world->getNode(m_nodeToID)));
}
