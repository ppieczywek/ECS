// ECS.cpp : Defines the entry point for the console application.
//
#pragma once
#include "stdafx.h"
#include <iostream>
#include "EntityManagerBase.h"

#include "PositionComponent.h"
#include "MotionComponent.h"
#include "BodyComponent.h"
#include "CollisionComponent.h"
#include "CameraComponent.h"
#include "RenderComponent.h"


#include "PhysicsSystem.h"
#include "RenderSystem.h"
#include "CameraSystem.h"

#include "ObjectFactory.h"

#include "HighResolutionTimer.h"
#include "WorldManagerBase.h"
#include "BitmapFont.h"


int _tmain(int argc, _TCHAR* argv[])
{
	typedef TYPELIST_6(PositionComponent, MotionComponent, BodyComponent, CollisionComponent, CameraComponent, RenderComponent) MyList;

	
	typedef	PhysicsSystem<MyList>  PhysicsSystem; 
	typedef	RenderSystem<MyList>   RenderSystem; 
	typedef	CameraSystem<MyList>   CameraSystem; 

	typedef TYPELIST_3(PhysicsSystem, CameraSystem, RenderSystem ) MySystemList;
	
	auto CameraSignature = static_cast<long long>(1) << IndexOf<MyList, CameraComponent>::value;
	auto StaticTextSignature = (static_cast<long long>(1) << IndexOf<MyList, RenderComponent>::value ) | (static_cast<long long>(1) << IndexOf<MyList, PositionComponent>::value );

	std::vector<int> DynamicEntityList;

	WorldManagerBase WorldManager;

	if( WorldManager.Initialize<MyList, MySystemList>() )
	{
		auto PhysSys = WorldManager.GetSystem<PhysicsSystem>();
		if(PhysSys != nullptr)
		{
			PhysSys->SetSimulationBox( Vector2( 0.0f,  0.0f), Vector2(200.0f, 220.0f));
			PhysSys->ToggleSleepingMode(true);
		}

		auto RenderSys = WorldManager.GetSystem<RenderSystem>();
		if(RenderSys != nullptr)
		{
			auto resManager = WorldManager.GetResourceManager();
			if(resManager != nullptr) 
			{
				resManager->AddResource(std::make_shared<BitmapFont>("ArcadeFont", "ArcadeFont.tga" ));
				RenderSys->LoadFont( resManager->GetResource<BitmapFont>("ArcadeFont"));
				RenderSys->SetCurrentFont("ArcadeFont");
			}
			RenderSys->SetScreenResolutionMode(R1024x768);
			RenderSys->ToggleFullscreen(false);
			RenderSys->ToggleVerticalSynchronization(true);
		}
			
		ObjectFactory::RegisterEntityManager( WorldManager.GetEntityManager() );
		ObjectFactory::RegisterResourceManager( WorldManager.GetResourceManager() );

		if( ObjectFactory::Initialize() )
		{
			
			auto Box = ObjectFactory::CreateStaticBox(70.0f, 5.0f, 3.0f);

			auto BoxPos = WorldManager.GetEntityManager()->GetComponent<PositionComponent>(Box);
			BoxPos->SetPosition(Vector2(150.0f, 70.0f));
			BoxPos->SetAngle(0.0f);
			
			auto BoxMot = WorldManager.GetEntityManager()->GetComponent<MotionComponent>(Box);
			BoxMot->SetTypeOfBody(BodyType::Kinematic);
			BoxMot->SetMass(0.0f);
			BoxMot->SetInertia(0.0f);
			BoxMot->SetAngularVelocity(2.5f);
								   
			Box = ObjectFactory::CreateStaticBox(70.0f, 5.0f, 3.0f);

			BoxPos = WorldManager.GetEntityManager()->GetComponent<PositionComponent>(Box);
			BoxPos->SetPosition(Vector2(50.0f, 70.0f));
			BoxPos->SetAngle(0.0f);

			BoxMot = WorldManager.GetEntityManager()->GetComponent<MotionComponent>(Box);
			BoxMot->SetTypeOfBody(BodyType::Kinematic);
			BoxMot->SetMass(0.0f);
			BoxMot->SetInertia(0.0f);
			BoxMot->SetAngularVelocity(-2.5f);


			Box = ObjectFactory::CreateStaticBox(3.0f, 60.0f, 3.0f);
			BoxPos = WorldManager.GetEntityManager()->GetComponent<PositionComponent>(Box);
			BoxPos->SetPosition(Vector2(20.0f, 30.0f));
			BoxPos->SetAngle(0.7f);

			Box = ObjectFactory::CreateStaticBox(3.0f, 100.0f, 3.0f);
			BoxPos = WorldManager.GetEntityManager()->GetComponent<PositionComponent>(Box);
			BoxPos->SetPosition(Vector2(2.0f, 105.0f));


			Box = ObjectFactory::CreateStaticBox(3.0f, 100.0f, 3.0f);
			BoxPos = WorldManager.GetEntityManager()->GetComponent<PositionComponent>(Box);
			BoxPos->SetPosition(Vector2(198.0f, 105.0f));


			Box = ObjectFactory::CreateStaticBox(40.0f, 5.0f, 3.0f);
			BoxPos = WorldManager.GetEntityManager()->GetComponent<PositionComponent>(Box);
			BoxPos->SetPosition(Vector2(100.0f, 5.0f));

			Box = ObjectFactory::CreateStaticBox(3.0f, 60.0f, 3.0f);
			BoxPos = WorldManager.GetEntityManager()->GetComponent<PositionComponent>(Box);
			BoxPos->SetPosition(Vector2(178.0f, 30.0f));
			BoxPos->SetAngle(-0.7f);


			for(int ii=0; ii<20; ++ii)
			{
				for(int jj=0; jj<19; ++jj)
				{
					Box = ObjectFactory::CreateDynamicBox(5.5f, 5.5f, 2.0f);
					BoxPos = WorldManager.GetEntityManager()->GetComponent<PositionComponent>(Box);
					BoxPos->SetPosition(Vector2(30.0f + 7.5f*ii, 100.0f + jj*6.0f));
					BoxPos->SetAngle(0.01f);

					DynamicEntityList.push_back(Box->GetIndex());
					
				}
			}
					
			auto Camera = WorldManager.GetEntityManager()->Create(CameraSignature);
			auto CameraCom = WorldManager.GetEntityManager()->GetComponent<CameraComponent>(Camera);
			CameraCom->SetActivationFlag(true);
			CameraCom->SetPosition(Vector2(250.0f, 100.0f));
			CameraCom->SetProjectionWidth(600.0f);
						
			auto Text = WorldManager.GetEntityManager()->Create(StaticTextSignature);
			auto RenderCom = WorldManager.GetEntityManager()->GetComponent<RenderComponent>(Text);
			RenderCom->SetTextColor(glm::vec4(0.7f,0.2f,0.0f,1.0f));
			RenderCom->SetTextSize(0.05f);
			RenderCom->SetModelCoordinatesMode(Screen);
			BoxPos = WorldManager.GetEntityManager()->GetComponent<PositionComponent>(Text);
			BoxPos->SetPosition(Vector2(-0.98f, 0.85f));
			
			
			MSG WindowMessage;
			
			const int FRAMES_PER_SECOND = static_cast<int>(60);
			const int SKIP_TICKS = 1000 / FRAMES_PER_SECOND;
			DWORD next_game_tick = GetTickCount();
	

			HighResolutionTimer timer;
	
			double fps = 0;

			while(TRUE)
			{
			
				timer.Start();

				while(PeekMessage(&WindowMessage, NULL, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&WindowMessage);
					DispatchMessage(&WindowMessage);	
				}

				if(WindowMessage.message == WM_QUIT)
				break;

				WorldManager.Update();
				auto BodyPos = WorldManager.GetEntityManager()->GetContainer<PositionComponent>();
				
				if (PhysSys != nullptr)
				{
					auto SimulationBox = PhysSys->GetSimulationBox();
					for (size_t ii = 0; ii < DynamicEntityList.size(); ii++)
					{
						Entity* MyEntity = WorldManager.GetEntityManager()->GetEntity(DynamicEntityList[ii]);
						auto BoxPos = WorldManager.GetEntityManager()->GetComponent<PositionComponent>(MyEntity);
						auto Position = BoxPos->GetPosition();

						if (Position.x < SimulationBox.min.x)
						{
							Position.x = SimulationBox.max.x;
						}
						else if (Position.x > SimulationBox.max.x)
						{
							Position.x = SimulationBox.min.x;
						}
						else
						{

						}

						if (Position.y < SimulationBox.min.y)
						{
							Position.y = SimulationBox.max.y;
						}
						else if (Position.y > SimulationBox.max.y)
						{
							Position.y = SimulationBox.min.y;
						}
						else
						{

						}
						BoxPos->SetPosition(Position);
					}
				}
				
				if( GetAsyncKeyState(VK_ESCAPE) != 0 ) 
				{
					PostQuitMessage(WM_QUIT);
				}
				fps = 0.6 * (1.0 / timer.Stop()) + (1.0 - 0.6)*fps;
				RenderCom = WorldManager.GetEntityManager()->GetComponent<RenderComponent>(Text);
				if(RenderCom!=nullptr) RenderCom->SetText("FPS: " + std::to_string( fps ) );
			}

		}
	}	
	
	
	ObjectFactory::Close();
	WorldManager.Close();



	system("PAUSE");
	return 0;
}

