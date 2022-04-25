#include "Gameplay/Components/TriggerVolumeEnterBehaviour.h"
#include "Gameplay/Components/ComponentManager.h"
#include "Gameplay/GameObject.h"

#include "Gameplay/Scene.h"//testing
#include "Gameplay/Components/GUI/GuiPanel.h";
#include "Gameplay/Components/JumpBehaviour.h"
#include "Gameplay/Components/ParticleSystem.h"
#include "Application/Layers/PostProcessing/DepthOfField.h"
#include "Application/Application.h"

TriggerVolumeEnterBehaviour::TriggerVolumeEnterBehaviour() :
	IComponent()
{ }
TriggerVolumeEnterBehaviour::~TriggerVolumeEnterBehaviour() = default;

void TriggerVolumeEnterBehaviour::OnTriggerVolumeEntered(const std::shared_ptr<Gameplay::Physics::RigidBody>& body)
{
	LOG_INFO("Body has entered {} trigger volume: {}", GetGameObject()->Name, body->GetGameObject()->Name);
	_playerInTrigger = true;
	Application& app = Application::Get();
	bool yOverlap = (app.CurrentScene()->FindObjectByName("Enemy")->GetPosition().y < (app.CurrentScene()->FindObjectByName("Projectile")->GetPosition().y + 0.4) && app.CurrentScene()->FindObjectByName("Enemy")->GetPosition().y >= (app.CurrentScene()->FindObjectByName("Projectile")->GetPosition().y - 0.4));
	if (body->GetGameObject()->Name == "Enemy")
	{
		std::cout << "Boom!";
		GetGameObject()->SetPostion(glm::vec3(0.f, 0.f, 0.5f));
		GetGameObject()->Get<JumpBehaviour>()->isHurt = true;
		lives-= 1.0f;
		if (lives == 0.0f) //lose
		{
			GetGameObject()->Get<JumpBehaviour>()->lose = true; //please work
			GetGameObject()->GetParent()->GetChildren()[5]->Get<GuiPanel>()->IsEnabled = true;

			//blur background after game over
			Application::Get().CurrentScene()->MainCamera->FocalDepth = 0.1f;
		}
	}

	if (yOverlap) //win
	{
		//GetGameObject()->GetParent()->GetChildren()[4]->Get<GuiPanel>()->IsEnabled = true;
		GetGameObject()->Get<JumpBehaviour>()->win = true;
	}
	
}

void TriggerVolumeEnterBehaviour::OnTriggerVolumeLeaving(const std::shared_ptr<Gameplay::Physics::RigidBody>& body) {
	LOG_INFO("Body has left {} trigger volume: {}", GetGameObject()->Name, body->GetGameObject()->Name);
	_playerInTrigger = false;
}

void TriggerVolumeEnterBehaviour::RenderImGui() { }

nlohmann::json TriggerVolumeEnterBehaviour::ToJson() const {
	return { };
}

TriggerVolumeEnterBehaviour::Sptr TriggerVolumeEnterBehaviour::FromJson(const nlohmann::json& blob) {
	TriggerVolumeEnterBehaviour::Sptr result = std::make_shared<TriggerVolumeEnterBehaviour>();
	return result;
}
