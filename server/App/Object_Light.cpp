#include "Object_Light.h"
#include "Node.h"

Object_Light::Object_Light()
{
	m_nodes.emplace_back(MakeShared<Node>(Vec2(4, 10)));
	m_nodes.emplace_back(MakeShared<Node>(Vec2(24, 10)));
}

void Object_Light::draw()
{
	bool isBlight = (m_nodes[0]->m_preState == Node::State::High && m_nodes[1]->m_preState == Node::State::Low) ||
		(m_nodes[0]->m_preState == Node::State::Low && m_nodes[1]->m_preState == Node::State::High);

	TextureAsset::Register(U"light1", U"assets/light1.png");
	TextureAsset::Register(U"light2", U"assets/light2.png");

	if (isBlight) TextureAsset(U"light1").draw(m_position);
	else TextureAsset(U"light2").draw(m_position);
}