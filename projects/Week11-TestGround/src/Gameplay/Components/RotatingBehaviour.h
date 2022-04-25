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

	virtual float Lerp(float a, float b, float t);

	virtual void RenderImGui() override;

	virtual nlohmann::json ToJson() const override;
	static RotatingBehaviour::Sptr FromJson(const nlohmann::json& data);

	MAKE_TYPENAME(RotatingBehaviour);
protected:
	float startTime = 0.f;
	float endTime = 4.f;
	float offset = 0.f;
	float a = 0.f;
	float b = 3.f;
	bool gameOver = false;

};

