#pragma once
#include <string>
#include "Vector2.h"
#include "Vector3.h"
#include "ComponentBase.h"
#include "ModelData.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

enum CoordinatesMode {Screen, World};

class RenderComponent : public ComponentBase
{
	
		unsigned						VisibleFlag;
		unsigned						TextureFlag;
		unsigned int					VAO;
		unsigned int					VertexNumber;
		unsigned int					TextureId;
		float							Layer;
		float							ColorMixingCoeff;
		float							TextSize;
		float							TextAngle;
		Vector2							TextOffset;
		glm::vec2						TexCoordScale;
		glm::vec2						TexCoordOffset;
		glm::vec3						Scale;
		glm::vec4						VertexColor;
		glm::vec4						TextColor;
		std::string						Text;
		CoordinatesMode					ModelCoordinatesMode;
	
public:
										RenderComponent();
										~RenderComponent();
		virtual void					Reset();
	

		inline const bool				GetVisibleFlag()const;
		inline const bool				GetTextureFlag()const;
		inline const float&				GetAlpha()const;
		inline const float&				GetLayer()const;
		inline const glm::vec4&			GetVertexColor()const;
		inline const glm::vec3&			GetScale()const;
		inline const float&				GetColorMixingCoeff() const;
		inline const glm::vec2&			GetTexCoordScale() const;
		inline const glm::vec2&			GetTexCoordOffset() const;
		inline const CoordinatesMode&	GetModelCoordinatesMode() const;
		inline const std::string&		GetText() const;
		inline const float&				GetTextSize() const;
		inline const glm::vec4&			GetTextColor() const;
		inline const float&				GetTextAlpha() const;
		inline const unsigned int&		GetVAO();
		inline const unsigned int&		GetVertexNumber();
		inline const unsigned int&		GetTextureId();

		inline void						SetModelCoordinatesMode(const CoordinatesMode&);
		inline void						SetVisibleFlag(const bool&);
		inline void						SetAlpha(const float&);
		inline void						SetLayer(const float&);	
		inline void						SetVertexColor(const glm::vec4&);
		inline void						SetScale(const glm::vec3&);
		inline void						SetColorMixingCoeff(const float&);
		inline void						SetTexCoordScale(const glm::vec2&);
		inline void						SetTexCoordOffset(const glm::vec2&);
		inline void						SetTextureId(const unsigned int&);
		inline void						SetText(const std::string&);
		inline void						SetTextSize(const float&);
		inline void			 			SetTextColor(glm::vec4&);
		inline void						SetTextAlpha(const float&);
		void							SetModelData(const ModelData*);
		
		inline const bool				HasText() const;

		inline const Vector2&			GetTextOffset() const;
		inline const float&				GetTextAngle() const;

		inline void						SetTextOffset(const Vector2&);
		inline void						SetTextAngle(const float&);
};


inline const bool RenderComponent::GetVisibleFlag() const
{
	return (VisibleFlag==1);
};


inline const bool RenderComponent::GetTextureFlag() const
{
	return (TextureFlag==1);
};


inline const float&	RenderComponent::GetAlpha() const
{
	return VertexColor.w;
};


inline const float&	RenderComponent::GetLayer() const
{
	return Layer;
};


inline const glm::vec4& RenderComponent::GetVertexColor() const
{
	return VertexColor;
};


inline const glm::vec3& RenderComponent::GetScale() const
{
	return Scale;
};


inline void	RenderComponent::SetVisibleFlag(const bool& Value)
{
	if(Value)
		VisibleFlag = 1;
	else
		VisibleFlag = 0;
};


inline void	RenderComponent::SetAlpha(const float& Value)
{
	VertexColor.w = Value;
};


inline void RenderComponent::SetLayer(const float& Value)
{
	Layer = Value;
};


inline void	RenderComponent::SetVertexColor(const glm::vec4& Value)
{
	VertexColor = Value;
};


inline void	RenderComponent::SetScale(const glm::vec3& Value)
{
	Scale = Value;
};


inline const unsigned int& RenderComponent::GetVAO()
{
	return VAO;
};


inline const unsigned int& RenderComponent::GetVertexNumber()
{
	return VertexNumber;
};


inline const unsigned int& RenderComponent::GetTextureId()
{
	return TextureId;
};

inline void RenderComponent::SetTextureId(const unsigned int& Value)
{
	if(Value > 0)
	{
		TextureId = Value;
		TextureFlag = 1;
	}
	else
	{
		TextureFlag = 0;
		TextureId = 0;
	}
};


inline const float&	RenderComponent::GetColorMixingCoeff() const
{
	return ColorMixingCoeff;
};


inline const glm::vec2& RenderComponent::GetTexCoordScale() const
{
	return TexCoordScale;
};


inline const glm::vec2& RenderComponent::GetTexCoordOffset() const
{
	return TexCoordOffset;
};


inline void	RenderComponent::SetColorMixingCoeff(const float& Value)
{
	ColorMixingCoeff = Value;
};


inline void	RenderComponent::SetTexCoordScale(const glm::vec2& Value)
{
	TexCoordScale = Value;
};


inline void	RenderComponent::SetTexCoordOffset(const glm::vec2& Value)
{
	TexCoordOffset = Value;
};



inline void	RenderComponent::SetText(const std::string& Value)
{
	Text = Value;
};


inline const std::string& RenderComponent::GetText() const
{
	return Text;
};


inline const CoordinatesMode& RenderComponent::GetModelCoordinatesMode() const
{
	return ModelCoordinatesMode;
};


inline void RenderComponent::SetModelCoordinatesMode(const CoordinatesMode& Value)
{
	ModelCoordinatesMode = Value;
};

inline const bool RenderComponent::HasText() const
{
		return !Text.empty();
};


inline const float&	RenderComponent::GetTextSize() const
{
	return TextSize;
};


inline const glm::vec4&	RenderComponent::GetTextColor() const
{
	return TextColor;
};


inline const float&	RenderComponent::GetTextAlpha() const
{
	return TextColor.w;
};


inline void	RenderComponent::SetTextSize(const float& Value)
{
	TextSize = Value;
};


inline void	RenderComponent::SetTextColor(glm::vec4& Value)
{
	TextColor = Value;
};


inline void	RenderComponent::SetTextAlpha(const float& Value)
{
	TextColor.w = Value;
};


inline const Vector2& RenderComponent::GetTextOffset() const
{
	return TextOffset;
};


inline const float& RenderComponent::GetTextAngle() const
{
	return TextAngle;
};


inline void RenderComponent::SetTextOffset(const Vector2& Value)
{
	TextOffset = Value;
};


inline void	RenderComponent::SetTextAngle(const float& Value)
{
	TextAngle = Value;
};



