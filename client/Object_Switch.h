#pragma once

#include "Object.h"

class Object_Switch
	: public Object
{
	bool m_isOn = false;

public:
	Object_Switch();

	uint8 type() override { return 3; }

	void update() override;

	void draw() override
	{
		if (m_isOn) TextureAsset(U"switch1").draw(m_position);
		else TextureAsset(U"switch2").draw(m_position);
	}

	bool mouseover() const override
	{
		return RectF(m_position, 20, 36).mouseOver();
	}

	void save(Serializer<MemoryWriter>& writer) override
	{
		Object::save(writer);

		writer << m_isOn;
	}

	void load(Deserializer<ByteArray>& reader) override
	{
		Object::load(reader);

		reader >> m_isOn;
	}

	void switching() { m_isOn = !m_isOn; }
};
