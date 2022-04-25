#pragma once
#include "IComponent.h"

/// <summary>
/// Showcases a very simple behaviour that rotates the parent gameobject at a fixed rate over time
/// </summary>
class RotatingBehaviour : public Gameplay::IComponent {
public:
	typedef std::shared_ptr<RotatingBehaviour> Sptr;

	RotatingBehaviour() = default;
	glm::vec3 RotationSpeed;

	virtual void Update(float deltaTime) override;

	virtual void RenderImGui() override;

	virtual nlohmann::json ToJson() const override;
	static RotatingBehaviour::Sptr FromJson(const nlohmann::json& data);

	MAKE_TYPENAME(RotatingBehaviour);

	float enemyStartTime = 0.f;
	float enemyEndTime = 4.f;
	glm::vec3 enemyStartPos = glm::vec3(3.f, 3.f, 0.f);
	glm::vec3 enemyEndPos = glm::vec3(6.f, -6.f, 3.f);
	bool jumped = false;
};

