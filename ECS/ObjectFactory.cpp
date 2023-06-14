#pragma once
#include "stdafx.h"
#include "ObjectFactory.h"


EntityManagerBase*	ObjectFactory::EntityManager = nullptr;
ResourceManagerBase*	ObjectFactory::ResourceManager = nullptr;

long long			ObjectFactory::StaticBoxTemplateId = -1;
long long			ObjectFactory::DynamicBoxTemplateId = -1;

bool ObjectFactory::RegisterEntityManager(EntityManagerBase* EntityManagerBase)
{
	if(EntityManagerBase != nullptr)
	{
		EntityManager = EntityManagerBase;
		return true;
	}
	return false;
};


bool ObjectFactory::RegisterResourceManager(ResourceManagerBase* ResourceManagerBase)
{
	if(ResourceManagerBase != nullptr)
	{
		ResourceManager = ResourceManagerBase;
		return true;
	}
	return false;
};


bool ObjectFactory::Initialize()
{
	if(EntityManager != nullptr && ResourceManager != nullptr)
	{
		bool Result;
		Result = InitializeStaticBox();
		if(!Result)
		{
			return false;
		}

		Result = InitializeDynamicBox();
		if(!Result)
		{
			return false;
		}

		return true;
	}
	return false;
};

bool ObjectFactory::InitializeStaticBox()
{
	const int size= 6;
	Vector2 Vertices[4];
	Vertices[0] = Vector2( -0.5f,  0.5f );
	Vertices[1] = Vector2(  0.5f,  0.5f );
	Vertices[2] = Vector2(  0.5f, -0.5f );
	Vertices[3] = Vector2( -0.5f, -0.5f );

	Polygon2 StaticBoxBody = CreatePolygon(Vertices, 4);

	Vertex BoxVertex[size];

	BoxVertex[0].Position[0] = Vertices[0].x;
	BoxVertex[0].Position[1] = Vertices[0].y;
	BoxVertex[0].Position[2] = 0.0f;
	BoxVertex[0].TexCoord[0] = 0.0f;
	BoxVertex[0].TexCoord[1] = 0.0f;

	BoxVertex[1].Position[0] = Vertices[1].x;
	BoxVertex[1].Position[1] = Vertices[1].y;
	BoxVertex[1].Position[2] = 0.0f;
	BoxVertex[1].TexCoord[0] = 1.0f;
	BoxVertex[1].TexCoord[1] = 0.0f;

	BoxVertex[2].Position[0] = Vertices[2].x;
	BoxVertex[2].Position[1] = Vertices[2].y;
	BoxVertex[2].Position[2] = 0.0f;
	BoxVertex[2].TexCoord[0] = 1.0f;
	BoxVertex[2].TexCoord[1] = 1.0f;
	
	BoxVertex[3].Position[0] = Vertices[2].x;
	BoxVertex[3].Position[1] = Vertices[2].y;
	BoxVertex[3].Position[2] = 0.0f;
	BoxVertex[3].TexCoord[0] = 1.0f;
	BoxVertex[3].TexCoord[1] = 1.0f;

	BoxVertex[4].Position[0] = Vertices[3].x;
	BoxVertex[4].Position[1] = Vertices[3].y;
	BoxVertex[4].Position[2] = 0.0f;
	BoxVertex[4].TexCoord[0] = 0.0f;
	BoxVertex[4].TexCoord[1] = 1.0f;
	
	BoxVertex[5].Position[0] = Vertices[0].x;
	BoxVertex[5].Position[1] = Vertices[0].y;
	BoxVertex[5].Position[2] = 0.0f;
	BoxVertex[5].TexCoord[0] = 0.0f;
	BoxVertex[5].TexCoord[1] = 0.0f;


	ResourceManager->AddResource(std::make_shared<ModelData>("StaticBoxModeData", BoxVertex, size ));

	//StaticBoxModelData.Create(BoxVertex, size);

	
	long long StaticBoxBitMask = EntityManager->GetComponentMask<PositionComponent>()  |
								 EntityManager->GetComponentMask<MotionComponent>()	   |
								 EntityManager->GetComponentMask<BodyComponent>()      |
								 EntityManager->GetComponentMask<CollisionComponent>() |
								 EntityManager->GetComponentMask<RenderComponent>() ;


	auto Box = EntityManager->CreateTemplate( StaticBoxBitMask );
	
	auto BoxMotionComponent = EntityManager->GetComponent<MotionComponent>(Box);
	BoxMotionComponent->SetTypeOfBody(Static);
	BoxMotionComponent->SetDensity(1.0f);
	BoxMotionComponent->SetMass(1.0f);
	BoxMotionComponent->SetInertia(1.0f/12.0f);
	BoxMotionComponent->SetLinearDamping(0.02f);
	BoxMotionComponent->SetAngularDamping(0.02f);

	
	auto BoxBodyComponent = EntityManager->GetComponent<BodyComponent>(Box);
	BoxBodyComponent->AddPolygon(StaticBoxBody);


	auto BoxCollisionComponent = EntityManager->GetComponent<CollisionComponent>(Box);
	BoxCollisionComponent->SetCollisionType(Both);
	BoxCollisionComponent->SetStaticFrictionCoeff(0.5f);
	BoxCollisionComponent->SetDynamicFrictionCoeff(0.25f);
	BoxCollisionComponent->SetRestitutionCoeff(1.15f);
	

	auto BoxRenderComponent = EntityManager->GetComponent<RenderComponent>(Box);
	BoxRenderComponent->SetModelData(ResourceManager->GetResource<ModelData>("StaticBoxModeData"));
	BoxRenderComponent->SetVisibleFlag(true);
	

	StaticBoxTemplateId = Box->GetIndex() ;

	return true;
};


Entity*	 ObjectFactory::CreateStaticBox(const float& Width, const float& Height, const float& Mass)
{
	auto Box = EntityManager->CloneEntity(StaticBoxTemplateId);

	float Area = Width * Height;
	float Density = Area / Mass;
	float Inertia  = Mass * (Width*Width + Height*Height) / 12.0f;

	auto BoxMotionComponent = EntityManager->GetComponent<MotionComponent>(Box);
	BoxMotionComponent->SetTypeOfBody(Static);
	BoxMotionComponent->SetDensity(Density);
	BoxMotionComponent->SetMass(Mass);
	BoxMotionComponent->SetInertia(Inertia);
	BoxMotionComponent->SetLinearDamping(0.5f);
	BoxMotionComponent->SetAngularDamping(0.5f);


	auto BoxBodyComponent = EntityManager->GetComponent<BodyComponent>(Box);
	auto BoxPolygon = BoxBodyComponent->GetPolygon(0);

	float HW = Width/2.0f;
	float HH = Height/2.0f;

	BoxPolygon->LocalVertex[0].x = -HW;
	BoxPolygon->LocalVertex[0].y =  HH;
	BoxPolygon->LocalVertex[1].x =  HW;
	BoxPolygon->LocalVertex[1].y =  HH;
	BoxPolygon->LocalVertex[2].x =  HW;
	BoxPolygon->LocalVertex[2].y = -HH;

	BoxPolygon->LocalVertex[3].x = -HW;
	BoxPolygon->LocalVertex[3].y = -HH;


	auto BoxCollisionComponent = EntityManager->GetComponent<CollisionComponent>(Box);
	BoxCollisionComponent->SetCollisionType(Both);
	BoxCollisionComponent->SetStaticFrictionCoeff(0.5f);
	BoxCollisionComponent->SetDynamicFrictionCoeff(0.25f);
	BoxCollisionComponent->SetRestitutionCoeff(1.15f);

		
	auto BoxRenderComponent = EntityManager->GetComponent<RenderComponent>(Box);
	
	BoxRenderComponent->SetVisibleFlag(true);
	BoxRenderComponent->SetScale( glm::vec3(Width, Height, 1.0f) );
	//BoxRenderComponent->SetAlpha(1.0f);
	BoxRenderComponent->SetVertexColor(glm::vec4(1.0f));
	BoxRenderComponent->SetLayer(0.0f);

	return Box;
};



bool ObjectFactory::InitializeDynamicBox()
{
	const int size= 6;
	Vector2 Vertices[4];
	Vertices[0] = Vector2( -0.5f,  0.5f );
	Vertices[1] = Vector2(  0.5f,  0.5f );
	Vertices[2] = Vector2(  0.5f, -0.5f );
	Vertices[3] = Vector2( -0.5f, -0.5f );

	Polygon2 DynamicBoxBody = CreatePolygon(Vertices, 4);

	Vertex BoxVertex[size];

	BoxVertex[0].Position[0] = Vertices[0].x;
	BoxVertex[0].Position[1] = Vertices[0].y;
	BoxVertex[0].Position[2] = 0.0f;
	BoxVertex[0].TexCoord[0] = 0.0f;
	BoxVertex[0].TexCoord[1] = 1.0f;

	BoxVertex[1].Position[0] = Vertices[1].x;
	BoxVertex[1].Position[1] = Vertices[1].y;
	BoxVertex[1].Position[2] = 0.0f;
	BoxVertex[1].TexCoord[0] = 1.0f;
	BoxVertex[1].TexCoord[1] = 1.0f;

	BoxVertex[2].Position[0] = Vertices[2].x;
	BoxVertex[2].Position[1] = Vertices[2].y;
	BoxVertex[2].Position[2] = 0.0f;
	BoxVertex[2].TexCoord[0] = 1.0f;
	BoxVertex[2].TexCoord[1] = 0.0f;
	
	BoxVertex[3].Position[0] = Vertices[2].x;
	BoxVertex[3].Position[1] = Vertices[2].y;
	BoxVertex[3].Position[2] = 0.0f;
	BoxVertex[3].TexCoord[0] = 1.0f;
	BoxVertex[3].TexCoord[1] = 0.0f;

	BoxVertex[4].Position[0] = Vertices[3].x;
	BoxVertex[4].Position[1] = Vertices[3].y;
	BoxVertex[4].Position[2] = 0.0f;
	BoxVertex[4].TexCoord[0] = 0.0f;
	BoxVertex[4].TexCoord[1] = 0.0f;
	
	BoxVertex[5].Position[0] = Vertices[0].x;
	BoxVertex[5].Position[1] = Vertices[0].y;
	BoxVertex[5].Position[2] = 0.0f;
	BoxVertex[5].TexCoord[0] = 0.0f;
	BoxVertex[5].TexCoord[1] = 1.0f;


	ResourceManager->AddResource(std::make_shared<ModelData>("DynamicBoxModeData", BoxVertex, size ));
	ResourceManager->AddResource(std::make_shared<Texture>("DynamicBoxTexture", "Ghost.tga" ));

	
	long long DynamicBoxBitMask = EntityManager->GetComponentMask<PositionComponent>()  |
								 EntityManager->GetComponentMask<MotionComponent>()	   |
								 EntityManager->GetComponentMask<BodyComponent>()      |
								 EntityManager->GetComponentMask<CollisionComponent>() |
								 EntityManager->GetComponentMask<RenderComponent>() ;


	auto Box = EntityManager->CreateTemplate( DynamicBoxBitMask );
	
	auto BoxMotionComponent = EntityManager->GetComponent<MotionComponent>(Box);
	BoxMotionComponent->SetTypeOfBody(Dynamic);
	BoxMotionComponent->SetDensity(1.0f);
	BoxMotionComponent->SetMass(1.0f);
	BoxMotionComponent->SetInertia(1.0f/12.0f);
	BoxMotionComponent->SetLinearDamping(0.001f);
	BoxMotionComponent->SetAngularDamping(0.001f);

	
	auto BoxBodyComponent = EntityManager->GetComponent<BodyComponent>(Box);
	BoxBodyComponent->AddPolygon(DynamicBoxBody);


	auto BoxCollisionComponent = EntityManager->GetComponent<CollisionComponent>(Box);
	BoxCollisionComponent->SetCollisionType(Both);
	BoxCollisionComponent->SetStaticFrictionCoeff(0.5f);
	BoxCollisionComponent->SetDynamicFrictionCoeff(0.25f);
	BoxCollisionComponent->SetRestitutionCoeff(1.15f);

	auto BoxRenderComponent = EntityManager->GetComponent<RenderComponent>(Box);
	BoxRenderComponent->SetModelData(ResourceManager->GetResource<ModelData>("DynamicBoxModeData"));
	BoxRenderComponent->SetVisibleFlag(true);

	auto texture = ResourceManager->GetResource<Texture>("DynamicBoxTexture");
	BoxRenderComponent->SetTextureId(texture->GetId());

	//DynamicBoxTemplateId = EntityManager->CreateTemplate(Box);

	DynamicBoxTemplateId = Box->GetIndex();
	return true;
};



Entity*	 ObjectFactory::CreateDynamicBox(const float& Width, const float& Height, const float& Mass)
{
	//auto Box = EntityManager->CreateFromTemplate(DynamicBoxTemplateId);
	auto Box = EntityManager->CloneEntity(DynamicBoxTemplateId);

	float Area = Width * Height;
	float Density = Area / Mass;
	float Inertia  = Mass * (Width*Width + Height*Height) / 12.0f;

	auto BoxMotionComponent = EntityManager->GetComponent<MotionComponent>(Box);
	BoxMotionComponent->SetTypeOfBody(Dynamic);
	BoxMotionComponent->SetDensity(Density);
	BoxMotionComponent->SetMass(Mass);
	BoxMotionComponent->SetInertia(Inertia);
	BoxMotionComponent->SetLinearDamping(0.5f);
	BoxMotionComponent->SetAngularDamping(0.006f);


	auto BoxBodyComponent = EntityManager->GetComponent<BodyComponent>(Box);
	auto BoxPolygon = BoxBodyComponent->GetPolygon(0);

	float HW = Width/2.0f;
	float HH = Height/2.0f;

	BoxPolygon->LocalVertex[0].x = -HW;
	BoxPolygon->LocalVertex[0].y =  HH;
	BoxPolygon->LocalVertex[1].x =  HW;
	BoxPolygon->LocalVertex[1].y =  HH;
	BoxPolygon->LocalVertex[2].x =  HW;
	BoxPolygon->LocalVertex[2].y = -HH;

	BoxPolygon->LocalVertex[3].x = -HW;
	BoxPolygon->LocalVertex[3].y = -HH;


	auto BoxCollisionComponent = EntityManager->GetComponent<CollisionComponent>(Box);
	BoxCollisionComponent->SetCollisionType(Both);
	BoxCollisionComponent->SetStaticFrictionCoeff(0.5f);
	BoxCollisionComponent->SetDynamicFrictionCoeff(0.25f);
	BoxCollisionComponent->SetRestitutionCoeff(1.15f);

		
	auto BoxRenderComponent = EntityManager->GetComponent<RenderComponent>(Box);
	//Vector3 Scale(Width, Height, 0.0f);
	//BoxRenderComponent->SetTextureId( DynamicBoxTexture.GetId() );
	BoxRenderComponent->SetVisibleFlag(true);
	BoxRenderComponent->SetScale(glm::vec3(Width, Height, 1.0f));
	//BoxRenderComponent->SetAlpha(1.0f);
	BoxRenderComponent->SetVertexColor(glm::vec4(1.0f));
	BoxRenderComponent->SetLayer(0.0f);

	return Box;
};


void	ObjectFactory::Close()
{
//	DynamicBoxTexture.Clear();
};