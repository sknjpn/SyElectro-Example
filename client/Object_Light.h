#pragma once

#include "Object.h"

class Object_Light
	: public Object
{
public:
	Object_Light();

	uint8 type() override { return 1; }

	void update() override
	{

	}

	void draw() override;

	bool mouseover() const override
	{
		return RectF(m_position, 24, 24).mouseOver();
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
