#include "Gameplay/Components/JumpBehaviour.h"
#include <GLFW/glfw3.h>
#include "Gameplay/GameObject.h"
#include "Gameplay/Scene.h"
#include "Utils/ImGuiHelper.h"
#include "Gameplay/InputEngine.h"
#include "Application/Application.h"
#include "Gameplay/Components/GUI/GuiPanel.h";
#include "Application/Layers/PostProcessingLayer.h"
#include "Application/Layers/PostProcessing/ColorCorrectionEffect.h"
#include "Gameplay/Components/ParticleSystem.h"
#include "Application/Layers/PostProcessing/OutlineEffect.h"

void JumpBehaviour::Awake()
{
	_body = GetComponent<Gameplay::Physics::RigidBody>();
	if (_body == nullptr) {
		IsEnabled = false;
	}
	pulse = true;
}

void JumpBehaviour::RenderImGui() {
	LABEL_LEFT(ImGui::DragFloat, "Impulse", &_impulse, 1.0f);
}

nlohmann::json JumpBehaviour::ToJson() const {
	return {
		{ "impulse", _impulse }
	};
}

JumpBehaviour::JumpBehaviour() :
	IComponent(),
	_impulse(10.0f)
{ }

JumpBehaviour::~JumpBehaviour() = default;

JumpBehaviour::Sptr JumpBehaviour::FromJson(const nlohmann::json& blob) {
	JumpBehaviour::Sptr result = std::make_shared<JumpBehaviour>();
	result->_impulse = blob["impulse"];
	return result;
}

float JumpBehaviour::Lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

void JumpBehaviour::Update(float deltaTime) {

	//Main Character Movement Code
	if (InputEngine::GetKeyState(GLFW_KEY_SPACE) == ButtonState::Pressed && GetGameObject()->GetPosition().z <= 1.0f) {
		_body->ApplyImpulse(glm::vec3(0.0f, 0.0f, _impulse));
		Gameplay::IComponent::Sptr ptr = Panel.lock();
		if (ptr != nullptr) {
			ptr->IsEnabled = !ptr->IsEnabled;
		}
	}

	glm::vec3 input = glm::vec3(0.0f);
	if (InputEngine::IsKeyDown(GLFW_KEY_W)) {
		input.x += _moveSpeeds.x;
	}
	if (InputEngine::IsKeyDown(GLFW_KEY_S)) {
		input.x -= _moveSpeeds.x;
	}
	if (InputEngine::IsKeyDown(GLFW_KEY_A)) {
		input.y += _moveSpeeds.y;
	}
	if (InputEngine::IsKeyDown(GLFW_KEY_D)) {
		input.y -= _moveSpeeds.y;
	}
	input *= deltaTime;
	glm::vec3 worldMovement = glm::vec4(input, 1.0f);
	GetGameObject()->SetPostion(GetGameObject()->GetPosition() + worldMovement);
	//Code for player particle trail
	if (input == glm::vec3(0.0f) || GetGameObject()->GetPosition().z >= 1.0f) //foot trail
	{
		ParticleSystem::Sptr test = GetGameObject()->GetChildren()[0]->Get<ParticleSystem>();
		test->IsEnabled = false;
	}
	else
	{
		ParticleSystem::Sptr test = GetGameObject()->GetChildren()[0]->Get<ParticleSystem>();
		test->IsEnabled = true;
	}

	//Lighting Toggles Code
	if (InputEngine::GetKeyState(GLFW_KEY_1) == ButtonState::Pressed) //Ambient
	{
		if (ambientOn == true)
		{
			GetGameObject()->GetScene()->SetAmbientLight(glm::vec3(0.f));
			ambientOn = false;
		}
		else
		{
			GetGameObject()->GetScene()->SetAmbientLight(glm::vec3(1.f));
			ambientOn = true;
		}
	}
	if (InputEngine::GetKeyState(GLFW_KEY_2) == ButtonState::Pressed) //Diffuse
	{
		if (diffuseOn == true)
		{
			imgTest = ResourceManager::CreateAsset<Texture2D>("textures/green.png");
			imgTest->Bind(11);
			diffuseOn = false;
		}
		else
		{
			imgTest = ResourceManager::CreateAsset<Texture2D>("textures/black.png");
			imgTest->Bind(11);
			diffuseOn = true;
		}

	}
	if (InputEngine::GetKeyState(GLFW_KEY_3) == ButtonState::Pressed) //Specular
	{
		if (specularOn == true)
		{
			imgTest = ResourceManager::CreateAsset<Texture2D>("textures/blue.png");
			imgTest->Bind(11);
			specularOn = false;
		}
		else
		{
			imgTest = ResourceManager::CreateAsset<Texture2D>("textures/black.png");
			imgTest->Bind(11);
			specularOn = true;
		}
	}

	//Win/Lose Condition Code
	if (InputEngine::GetKeyState(GLFW_KEY_O) == ButtonState::Pressed)
	{
		gameWin = true;
		//isHurt = true;
	}
	if (InputEngine::GetKeyState(GLFW_KEY_P) == ButtonState::Pressed)
	{
		gameLose = true;
	}
	if (gameWin == true)
	{
		Application& app = Application::Get();
		app.CurrentScene()->FindObjectByName("Win Screen")->Get<GuiPanel>()->IsEnabled = true;
		app.CurrentScene()->MainCamera->FocalDepth = 0.1f;
	}
	if (gameLose == true)
	{
		Application& app = Application::Get();
		app.CurrentScene()->FindObjectByName("Lose Screen")->Get<GuiPanel>()->IsEnabled = true;
		app.CurrentScene()->MainCamera->FocalDepth = 0.1f;
	}

	//PostProcessing Effect on the Walls
	if (pulse == true)
	{
		Application& app = Application::Get();
		PostProcessingLayer::Sptr& postProc = app.GetLayer<PostProcessingLayer>();


		if (pulseStartTime <= pulseEndTime)
		{
			pulseStartTime += deltaTime;
			//return a + t * (b - a);
			float t = pulseStartTime / pulseEndTime;
			float r = startColor + t * (endColor - startColor);
			postProc->GetEffect<OutlineEffect>()->_outlineColor = glm::vec4(r, 0.f, 0.f, 1.f);
		}
		else
		{
			float tempVar = startColor;
			startColor = endColor;
			endColor = tempVar;
			pulseStartTime = 0.f;
		}
	}

	//PostProcessing for Damage Taken
	if (isHurt == true)
	{
		Application& app = Application::Get();
		PostProcessingLayer::Sptr& postProc = app.GetLayer<PostProcessingLayer>();
		ColorCorrectionEffect::Sptr& colCor = postProc->GetEffect<ColorCorrectionEffect>();
		colCor->ChangeChoice(1.0f);

		if (startTime <= endTime)
		{
			startTime += deltaTime;
		}
		else
		{
			colCor->ChangeChoice(0.0f);
			isHurt = false;
			startTime = 0.0f;
		}
	}
}

