#include "Gameplay/Components/RotatingBehaviour.h"

#include "Gameplay/GameObject.h"

#include "Utils/ImGuiHelper.h"
#include "Utils/JsonGlmHelpers.h"

#include "Gameplay/InputEngine.h"

#include "Gameplay/Components/ShipMoveBehaviour.h"

void RotatingBehaviour::Update(float deltaTime) {
	//GetGameObject()->SetRotation(GetGameObject()->GetRotationEuler() + RotationSpeed * deltaTime);
	if (gameOver == false)
	{
		if (startTime <= endTime)
		{
			startTime += deltaTime;
			glm::vec3 newPos = GetGameObject()->GetPosition();
			float t = startTime / endTime;
			float newZ = Lerp(a, b, t);
			newPos.z = newZ;
			//GetGameObject()->SetPostion(newPos);
			glm::vec3 newCenter = GetComponent<ShipMoveBehaviour>()->Center;
			newCenter.z = newZ;
			GetComponent<ShipMoveBehaviour>()->Center = newCenter;
		}
		else
		{
			float tempVar = a;
			a = b;
			b = tempVar;
			startTime = 0.f;
		}
	}
	if (InputEngine::GetKeyState(GLFW_KEY_ENTER) == ButtonState::Pressed)
	{
		gameOver = true;
	}
}

float RotatingBehaviour::Lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

void RotatingBehaviour::RenderImGui() {
	LABEL_LEFT(ImGui::DragFloat3, "Speed", &RotationSpeed.x);
}

nlohmann::json RotatingBehaviour::ToJson() const {
	return {
		{ "speed", RotationSpeed }
	};
}

RotatingBehaviour::Sptr RotatingBehaviour::FromJson(const nlohmann::json& data) {
	RotatingBehaviour::Sptr result = std::make_shared<RotatingBehaviour>();
	result->RotationSpeed = JsonGet(data, "speed", result->RotationSpeed);
	return result;
}
