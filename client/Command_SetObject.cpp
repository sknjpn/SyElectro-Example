#include "Command_SetObject.h"
#include "World.h"
#include "Object.h"

void Command_SetObject::action()
{
	auto object = Object::MakeObject(m_objType);
	object->m_position = m_position;

	g_world->addObject(object);
}
