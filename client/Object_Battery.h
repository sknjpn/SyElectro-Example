#pragma once

#include "Object.h"

class Object_Battery
	: public Object
{
public:
	Object_Battery();

	uint8 type() override { return 0; }

	void update() override;

	void draw() override
	{
		TextureAsset(U"battery").draw(m_position);
	}

	bool mouseover() const override
	{
		return RectF(m_position, 48, 16).mouseOver();
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
