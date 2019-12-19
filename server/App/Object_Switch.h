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
		TextureAsset::Register(U"switch1", U"assets/switch1.png");
		TextureAsset::Register(U"switch2", U"assets/switch2.png");

		if (m_isOn) TextureAsset(U"switch1").draw(m_position);
		else TextureAsset(U"switch2").draw(m_position);
	}

	bool mouseover() const override
	{
		return RectF(32, 32).setCenter(m_position).mouseOver();
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
