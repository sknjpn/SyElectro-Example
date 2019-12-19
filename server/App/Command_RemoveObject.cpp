#include "Command_RemoveObject.h"
#include "World.h"

void Command_RemoveObject::action()
{
	g_world->removeObject(m_objectID);
}