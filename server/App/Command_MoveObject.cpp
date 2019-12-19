#include "Command_MoveObject.h"
#include "World.h"
#include "Object.h"

void Command_MoveObject::action()
{
	g_world->getObject(m_objectID)->m_position = m_position;
}