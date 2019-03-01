#include "TransformComponent.h"

#include "Logger.h"

sf::Packet &operator<<(sf::Packet &p_Packet, TransformComponent &p_TransformComponent) {
	p_Packet << sf::Int32(p_TransformComponent.m_CircleTransforms.size());
	Log(Type::INFO) << "Number of circles in the transform component: " << p_TransformComponent.m_CircleTransforms.size();
	for (auto &transform : p_TransformComponent.m_CircleTransforms) {
		p_Packet << transform.m_Position.X();
		p_Packet << transform.m_Position.Y();
		p_Packet << transform.m_Radius;
		p_Packet << transform.m_Rotation;
		Log(Type::INFO) << "Transform data: XPos: " << transform.m_Position.X() << "\tYPos: " << transform.m_Position.Y() <<
			"\tRadius: " << transform.m_Radius << "\tRotation:" << transform.m_Rotation;
	}

	return p_Packet;
}

sf::Packet &operator>>(sf::Packet &p_Packet, TransformComponent &p_TransformComponent) {
	sf::Int32 numberOfCircles = 0;
	p_Packet >> numberOfCircles;
	for (int i = 0; i < numberOfCircles; i++) {
		float xPosition;
		p_Packet >> xPosition;

		float yPosition;
		p_Packet >> yPosition;

		float radius;
		p_Packet >> radius;

		float rotation;
		p_Packet >> rotation;

		Log(Type::INFO) << "XPos: " << xPosition << "\tYPos: " << yPosition << "\tRadius: " << radius << "\tRotation: " << rotation;
		p_TransformComponent.m_CircleTransforms.emplace_back(Vector2D<float>(xPosition, yPosition), radius, rotation);
	}

	return p_Packet;
}