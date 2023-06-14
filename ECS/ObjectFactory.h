#pragma once
#include "Entity.h"
#include "Texture.h"
#include "PhysicsSystem.h"
#include "RenderSystem.h"
#include "CameraComponent.h"
#include "RenderComponent.h"
#include "MotionComponent.h"

#include "EntityManagerBase.h"
#include "ResourceManagerBase.h"

class ObjectFactory
{
	
protected:

	static EntityManagerBase*	EntityManager;
	static ResourceManagerBase*	ResourceManager;

	static long long			StaticBoxTemplateId;
	static bool					InitializeStaticBox();

	static long long			DynamicBoxTemplateId;
	static bool					InitializeDynamicBox();

public:

	static bool					RegisterEntityManager(EntityManagerBase*);
	static bool					RegisterResourceManager(ResourceManagerBase*);
	static bool					Initialize();
	static Entity*				CreateStaticBox(const float&, const float&, const float&);
	static Entity*				CreateDynamicBox(const float&, const float&, const float&);

	static void					Close();
	

};


