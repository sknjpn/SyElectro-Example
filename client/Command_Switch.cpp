#include "Command_Switch.h"
#include "World.h"
#include "Object_Switch.h"

void Command_Switch::action()
{
	dynamic_pointer_cast<Object_Switch>(g_world->getObject(m_objectID))->switching();
}