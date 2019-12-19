#include "Object.h"
#include "Node.h"

#include "Object_Battery.h"
#include "Object_Light.h"
#include "Object_Relay.h"
#include "Object_Switch.h"

shared_ptr<Object> Object::MakeObject(uint8 objType)
{
	shared_ptr<Object> object;

	if (objType == 0) object = MakeShared<Object_Battery>();
	if (objType == 1) object = MakeShared<Object_Light>();
	if (objType == 2) object = MakeShared<Object_Relay>();
	if (objType == 3) object = MakeShared<Object_Switch>();

	for (const auto& node : object->m_nodes)
		node->m_object = object;

	if (!object) throw Error(U"存在しないオブジェクト" + objType);

	return object;
}

shared_ptr<Object> Object::MakeObject(Deserializer<ByteArray>& reader)
{
	uint8 objType;
	reader >> objType;

	shared_ptr<Object> object;

	if (objType == 0) object = MakeShared<Object_Battery>();
	if (objType == 1) object = MakeShared<Object_Light>();
	if (objType == 2) object = MakeShared<Object_Relay>();
	if (objType == 3) object = MakeShared<Object_Switch>();

	for (const auto& node : object->m_nodes)
		node->m_object = object;

	if (!object) throw Error(U"存在しないオブジェクト" + objType);

	object->load(reader);

	return object;
}