#pragma once
#include <windows.h>
#include <set>
#include "SystemBase.h"
#include "ComponentContainer.h"

template<class TList> class CameraSystem : public SystemBase
{
	
	long long				CameraSignature;
	std::set<int>			CameraEntities;
	bool					MouseDragStatus;

	POINT					PreviousMousePosition;
	POINT					CurrentMousePosition;

	bool					Initialize();
	bool					Update();
	bool					Register(const Entity*);
	bool					Unregister(const Entity*);
	bool					Close();

public:
	
	const Entity*			GetActiveCamera();
	bool					ToggleMouseDrag(const bool&);
};


template<class TList> const Entity* CameraSystem<TList>::GetActiveCamera()
{
	if(!CameraEntities.empty() && EntityManager != nullptr)
	{	
		ComponentContainer<CameraComponent>& CameraCom	= *EntityManager->GetContainer<CameraComponent>();
		for each(auto Index in CameraEntities)
		{
			if( CameraCom[Index]->GetActiVationStatus() )
			{
				return EntityManager->GetEntity(Index);
			}
		}
	}
	return nullptr;
};


template<class TList> bool	CameraSystem<TList>::ToggleMouseDrag(const bool& Value)
{
	return MouseDragStatus = Value;
};


template<class TList> bool CameraSystem<TList>::Initialize()
{
	if(!InitializationStatus)
	{	
		CameraSignature = static_cast<long long>(1) << IndexOf<TList, CameraComponent>::value;
		MouseDragStatus = true;
		SystemStatus = InitializationStatus = true;
		std::cout << "Camera System:	Initialization successfull!" << std::endl;
		return true;
	}
	else
	{
		std::cout << "Camera System:	System already initialized!!" << std::endl;
		return false;
	}
};


template<class TList> bool CameraSystem<TList>::Update()
{		
	if(SystemStatus==true)
	{	
		if(EntityManager != nullptr)
		{
			GetCursorPos(&CurrentMousePosition);

			ComponentContainer<PositionComponent>&  PositionCom		= *EntityManager->GetContainer<PositionComponent>(); 
			ComponentContainer<CameraComponent>&    CameraCom		= *EntityManager->GetContainer<CameraComponent>();
			auto RightButtonPressed = ((GetKeyState(VK_RBUTTON) & 0x100) != 0);
			auto MiddleButtonPressed = ((GetKeyState(VK_MBUTTON) & 0x100) != 0);

			if(!CameraEntities.empty()/* && PositionCom != nullptr && CameraCom != nullptr*/)
			{	
				for each(auto Index in CameraEntities)
				{
					if(CameraCom[Index]->GetActivationFlag())
					{
						if(MouseDragStatus && RightButtonPressed)
						{
							Vector2 CameraShift(-1.f *  static_cast<float>(CurrentMousePosition.x - PreviousMousePosition.x),
												 static_cast<float>(CurrentMousePosition.y - PreviousMousePosition.y));

							auto CurrentCameraPosition = CameraCom[Index]->GetPosition();
							CurrentCameraPosition+= 0.5f*(CameraShift);
							CameraCom[Index]->SetPosition(CurrentCameraPosition);
						}
						else if(MouseDragStatus && MiddleButtonPressed)
						{
							Vector2 CameraShift(-1.f *  static_cast<float>(CurrentMousePosition.x - PreviousMousePosition.x),
												 static_cast<float>(CurrentMousePosition.y - PreviousMousePosition.y));

							auto CurrentProjectionWidth = CameraCom[Index]->GetProjectionWidth();
							CurrentProjectionWidth+= 0.8f*(CameraShift.y);
							CameraCom[Index]->SetProjectionWidth(CurrentProjectionWidth);
						}
						else
						{
							auto Id = CameraCom[Index]->GetTrackedEntityId();
							if( Id != -1 )
							{
								if(CameraCom[Index]->GetDeadZoneFlag())
								{
									auto Direction = PositionCom[Id]->GetPosition() - CameraCom[Index]->GetPosition();
									auto Distance = Direction.length() - CameraCom[Index]->GetDeadZoneRadius();
									if(Distance > 0.0f)
									{
										CameraCom[Index]->SetPosition( CameraCom[Index]->GetPosition() + Direction.unit()*Distance );
									}
								}
								else
								{
									CameraCom[Index]->SetPosition( PositionCom[Id]->GetPosition() );
								}
							}
						}
					}
					else
					{
						auto Id = CameraCom[Index]->GetTrackedEntityId();
						if( Id != -1 )
						{
							if(CameraCom[Index]->GetDeadZoneFlag())
							{
								auto Direction = PositionCom[Id]->GetPosition() - CameraCom[Index]->GetPosition();
								auto Distance = Direction.length() - CameraCom[Index]->GetDeadZoneRadius();
								if(Distance > 0.0f)
								{
									CameraCom[Index]->SetPosition( CameraCom[Index]->GetPosition() + Direction.unit()*Distance );
								}
							}
							else
							{
								CameraCom[Index]->SetPosition( PositionCom[Id]->GetPosition() );
							}
						}
					}
				}
			}
			PreviousMousePosition = CurrentMousePosition;
			return true;
		}
	}
	return false;
};


template<class TList> bool CameraSystem<TList>::Register(const Entity* Entity)
{
	bool result = false;
	if(Entity != nullptr)
	{
		if( (CameraSignature & Entity->GetComponentMask()) == CameraSignature )
		{
			CameraEntities.insert( Entity->GetIndex());
			result = true;
		}
	}
	return result;
};


template<class TList> bool CameraSystem<TList>::Unregister(const Entity* Entity)
{
	bool result = false;
	if(Entity != nullptr)
	{
		if( (CameraSignature & Entity->GetComponentMask()) == CameraSignature )
		{
			CameraEntities.erase( Entity->GetIndex());
			result = true;
		}
	}
	return result;
};


template<class TList> bool CameraSystem<TList>::Close()
{
	if(InitializationStatus != false )
	{
		CameraEntities.clear();
		SystemStatus = InitializationStatus = false;
		std::cout << "Camera System:	Closed!" << std::endl;
		return true;
	}
	else
	{
		std::cout << "Camera System:	System already closed!" << std::endl;
		return false;
	}
};
