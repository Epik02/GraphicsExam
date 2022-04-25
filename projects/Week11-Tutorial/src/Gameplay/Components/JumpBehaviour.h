#pragma once
#include "IComponent.h"
#include "Gameplay/Physics/RigidBody.h"
#include "Graphics/Textures/Texture2D.h"

/// <summary>
/// A simple behaviour that applies an impulse along the Z axis to the 
/// rigidbody of the parent when the space key is pressed
/// </summary>
class JumpBehaviour : public Gameplay::IComponent {
public:
	typedef std::shared_ptr<JumpBehaviour> Sptr;

	std::weak_ptr<Gameplay::IComponent> Panel;

	JumpBehaviour();
	virtual ~JumpBehaviour();

	virtual void Awake() override;
	virtual void Update(float deltaTime) override;

public:
	virtual void RenderImGui() override;
	MAKE_TYPENAME(JumpBehaviour);
	virtual nlohmann::json ToJson() const override;
	static JumpBehaviour::Sptr FromJson(const nlohmann::json& blob);

	float Lerp(float a, float b, float t);
	Texture2D::Sptr imgTest;

	bool gameLose = false;
	bool gameWin = false;

	
	

protected:
	float _impulse = 2.f;
	glm::vec3 _moveSpeeds = glm::vec3(2.f, 2.f, 1.0f);
	bool _isPressed = false;
	Gameplay::Physics::RigidBody::Sptr _body;
	bool ColorCorrectOn = true;
	//Test
	float startTime = 0.f;
	float endTime = 0.4f;

	//Lighting
	bool ambientOn = true;
	bool diffuseOn = true;
	bool specularOn = true;

	//Postprocessing effect for the walls
	bool pulse = false;
	float pulseStartTime = 0.f;
	float pulseEndTime = 2.f;
	float startColor = 0.f;
	float endColor = 1.f;
	
	bool isHurt = false;
};