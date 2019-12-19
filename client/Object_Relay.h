#pragma once

#include "Object.h"

class Object_Relay
	: public Object
{
public:
	Object_Relay();

	uint8 type() override { return 2; }

	void update() override;

	void draw() override;

	bool mouseover() const override
	{
		return RectF(m_position, 48, 36).mouseOver();
	}

	void save(Serializer<MemoryWriter>& writer) override
	{
		Object::save(writer);
	}

	void load(Deserializer<ByteArray>& reader) override
	{
		Object::load(reader);
	}
};
