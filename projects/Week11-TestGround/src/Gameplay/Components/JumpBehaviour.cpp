#include "Gameplay/Components/JumpBehaviour.h"
#include <GLFW/glfw3.h>
#include "Gameplay/GameObject.h"
#include "Gameplay/Scene.h"
#include "Utils/ImGuiHelper.h"
#include "Gameplay/InputEngine.h"

#include "Application/Layers/PostProcessingLayer.h"

#include "Application/Application.h"
#include "Application/Layers/RenderLayer.h"

#include "Application/Layers/PostProcessing/ColorCorrectionEffect.h"

#include "Gameplay/Components/ParticleSystem.h"

#include "Gameplay/Components/GUI/GuiPanel.h";

#include "Gameplay/Components/TriggerVolumeEnterBehaviour.h"



void JumpBehaviour::Awake()
{
	_body = GetComponent<Gameplay::Physics::RigidBody>();
	if (_body == nullptr) {
		IsEnabled = false;
	}
	imgTest = ResourceManager::CreateAsset<Texture2D>("textures/black.png");
	imgTest->Bind(11);
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
	_impulse(6.0f)
{ }

JumpBehaviour::~JumpBehaviour() = default;

JumpBehaviour::Sptr JumpBehaviour::FromJson(const nlohmann::json& blob) {
	JumpBehaviour::Sptr result = std::make_shared<JumpBehaviour>();
	result->_impulse = blob["impulse"];
	return result;
}

void JumpBehaviour::Update(float deltaTime) {
	if (InputEngine::GetKeyState(GLFW_KEY_SPACE) == ButtonState::Pressed && GetGameObject()->GetPosition().z <= 1.0f) {
		_body->ApplyImpulse(glm::vec3(0.0f, 0.0f, _impulse));
		Gameplay::IComponent::Sptr ptr = Panel.lock();
		if (ptr != nullptr) {
			ptr->IsEnabled = !ptr->IsEnabled;
		}
	}
	if (InputEngine::GetKeyState(GLFW_KEY_T) == ButtonState::Pressed)
	{
		Application& app = Application::Get();

		// Grab the render layer from the app, get it's output and the G-Buffer
		PostProcessingLayer::Sptr& postProc = app.GetLayer<PostProcessingLayer>();
		if (ColorCorrectOn == true)
		{
			postProc->GetEffect<ColorCorrectionEffect>()->Enabled = false;
			ColorCorrectOn = false;
		}
		else
		{
			postProc->GetEffect<ColorCorrectionEffect>()->Enabled = true;
			ColorCorrectOn = true;
		}
		
	}
	if (InputEngine::GetKeyState(GLFW_KEY_Q) == ButtonState::Pressed)
	{
		Application& app = Application::Get();

		// Grab the render layer from the app, get it's output and the G-Buffer
		PostProcessingLayer::Sptr& postProc = app.GetLayer<PostProcessingLayer>();
		ColorCorrectionEffect::Sptr& colCor = postProc->GetEffect<ColorCorrectionEffect>();
		postProc->GetEffect<ColorCorrectionEffect>()->ChangeStrength(0.2f);

	}
	if (InputEngine::GetKeyState(GLFW_KEY_Y) == ButtonState::Pressed)
	{
		Application& app = Application::Get();
		const auto& cam = Application::Get().CurrentScene()->MainCamera;
		cam->FocalDepth = 0.1f;
	}
	if (InputEngine::GetKeyState(GLFW_KEY_U) == ButtonState::Pressed)
	{
		//isHurt = true;
		if (win == true)
		{
			win = false;
		}
		else
		{
			win = true;
		}
		
	}
	if (InputEngine::GetKeyState(GLFW_KEY_I) == ButtonState::Pressed)
	{
		ParticleSystem::Sptr test = GetGameObject()->GetChildren()[0]->Get<ParticleSystem>();
		test->IsEnabled = false;
		
	}

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

	if (InputEngine::GetKeyState(GLFW_KEY_L) == ButtonState::Pressed)
	{
		GetGameObject()->GetParent()->GetChildren()[3]->Get<GuiPanel>()->IsEnabled = true;
	}

	if (InputEngine::GetKeyState(GLFW_KEY_P) == ButtonState::Pressed)
	{
		
		if (lose == true)
		{
			GetGameObject()->Get<TriggerVolumeEnterBehaviour>()->lives = 3;
			Application::Get().CurrentScene()->MainCamera->FocalDepth = 0.1f;
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
	if (win == true)
	{
		ParticleSystem::Sptr test = GetGameObject()->GetParent()->GetChildren()[2]->GetChildren()[0]->Get<ParticleSystem>();
		test->IsEnabled = true;
	}
}

