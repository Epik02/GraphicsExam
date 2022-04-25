#pragma once
#include "Application/Layers/PostProcessingLayer.h"
#include "Graphics/ShaderProgram.h"
#include "Graphics/Textures/Texture3D.h"

class ColorCorrectionEffect : public PostProcessingLayer::Effect {
public:
	MAKE_PTRS(ColorCorrectionEffect);
	Texture3D::Sptr Lut;
	Texture3D::Sptr Lut2;

	ColorCorrectionEffect();
	ColorCorrectionEffect(bool defaultLut);
	virtual ~ColorCorrectionEffect();

	virtual void Apply(const Framebuffer::Sptr& gBuffer) override;
	virtual void RenderImGui() override;
	//Test
	virtual void ChangeStrength(float _newStrength);
	virtual void ChangeChoice(float _newChoice);

	// Inherited from IResource

	ColorCorrectionEffect::Sptr FromJson(const nlohmann::json& data);
	virtual nlohmann::json ToJson() const override;

	

protected:
	ShaderProgram::Sptr _shader;
	float _strength;
	float _choice;
};

