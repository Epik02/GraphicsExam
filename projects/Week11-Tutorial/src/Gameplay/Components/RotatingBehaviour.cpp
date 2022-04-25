#include "Gameplay/Components/RotatingBehaviour.h"
#include "Gameplay/Components/JumpBehaviour.h"

#include "Gameplay/GameObject.h"

#include "Utils/ImGuiHelper.h"
#include "Utils/JsonGlmHelpers.h"

void RotatingBehaviour::Update(float deltaTime) {
	//GetGameObject()->SetRotation(GetGameObject()->GetRotationEuler() + RotationSpeed * deltaTime);
	if (enemyStartTime <= enemyEndTime)
	{
		enemyStartTime += deltaTime;
		float t = enemyStartTime / enemyEndTime;
		float x = enemyStartPos.x + t * (enemyEndPos.x - enemyStartPos.x);
		float y = enemyStartPos.y + t * (enemyEndPos.y - enemyStartPos.y);
		float z = enemyStartPos.z + t * (enemyEndPos.z - enemyStartPos.z);
		GetGameObject()->SetPostion(glm::vec3(x, y, z));
			//return a + t * (b - a);
	}
	else
	{
		enemyStartTime = 0.f;
		jumped = false;
	}
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
