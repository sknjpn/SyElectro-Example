#include "Object_Relay.h"
#include "Node.h"

Object_Relay::Object_Relay()
{
	// Coil
	m_nodes.emplace_back(MakeShared<Node>(Vec2(36, 0)));
	m_nodes.emplace_back(MakeShared<Node>(Vec2(36, 28)));

	// Switch
	m_nodes.emplace_back(MakeShared<Node>(Vec2(8, 14)));
	m_nodes.emplace_back(MakeShared<Node>(Vec2(20, 0)));
	m_nodes.emplace_back(MakeShared<Node>(Vec2(20, 28)));
}

void Object_Relay::update()
{
	bool isFlowing = (m_nodes[0]->m_preState == Node::State::High && m_nodes[1]->m_preState == Node::State::Low) ||
		(m_nodes[0]->m_preState == Node::State::Low && m_nodes[1]->m_preState == Node::State::High);

	// 配線切り替え
	if (isFlowing)
	{
		if (!m_nodes[2]->isConnected(m_nodes[4]))
		{
			m_nodes[2]->disconnect(m_nodes[3]);
			m_nodes[3]->disconnect(m_nodes[2]);
			m_nodes[2]->connect(m_nodes[4]);
			m_nodes[4]->connect(m_nodes[2]);
		}
	}
	else
	{
		if (!m_nodes[2]->isConnected(m_nodes[3]))
		{
			m_nodes[2]->disconnect(m_nodes[4]);
			m_nodes[4]->disconnect(m_nodes[2]);
			m_nodes[2]->connect(m_nodes[3]);
			m_nodes[3]->connect(m_nodes[2]);
		}
	}
}

void Object_Relay::draw()
{
	bool isBlight = (m_nodes[0]->m_preState == Node::State::High && m_nodes[1]->m_preState == Node::State::Low) ||
		(m_nodes[0]->m_preState == Node::State::Low && m_nodes[1]->m_preState == Node::State::High);

	TextureAsset::Register(U"relay1", U"assets/relay1.png");
	TextureAsset::Register(U"relay2", U"assets/relay2.png");

	if (isBlight) TextureAsset(U"relay1").draw(m_position);
	else TextureAsset(U"relay2").draw(m_position);
}
