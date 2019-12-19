#include "Node.h"
#include "Object.h"

Vec2 Node::getPosition() const
{
	return m_object.lock()->m_position.movedBy(m_localPosition);
}

void Node::draw()
{
	Color color = Palette::White;
	if (m_nowState == State::High) color = Palette::Red;
	if (m_nowState == State::Low) color = Palette::Blue;

	Circle(getPosition(), 4).draw(color).drawFrame(2.0, Palette::Black);
}

void Node::flowToConnected()
{
	if (m_nowState == State::None) return;

	for (const auto& connected : m_connecteds)
	{
		if (connected.lock()->m_nowState == State::None)
		{
			connected.lock()->m_nowState = m_nowState;
			connected.lock()->flowToConnected();
		}
	}
}

bool Node::isConnected(const shared_ptr<Node>& to) const
{
	for (const auto& node : m_connecteds)
		if (node.lock() == to) return true;

	return false;
}
