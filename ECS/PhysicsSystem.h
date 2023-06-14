#pragma once
#include <set>

#include "PositionComponent.h"
#include "MotionComponent.h"
#include "BodyComponent.h"
#include "CollisionComponent.h"


#include "WorldProperties.h"
#include "SystemBase.h"
#include "DynamicGrid.h"
#include "CollisionData.h"


const int	DefaultIntegrationSubstepNumber = 3;
const int	DefaultSolverIterations			= 10;//10
const int	DefaultCollisionDataSize		= 350;
const float	DefaultPenetrationTolerance		= 0.005f;//0.01
const float	DefaultBiasCoeff				= 4.5f; //2


template<class TList> class PhysicsSystem : public SystemBase
{		
		float								BiasCoeff;
		float								PenetrationTolerance;
		int									SolverIterationsNumber;
		int									IntegrationSubstepNumber;
		unsigned							PhysicalCollisionNumber;
		unsigned							LogicalCollisionNumber;
		unsigned							SleepingModeFlag;
		long long							MovableSignature;
		long long							CollidableSignature;
		WorldProperties						WrdProperties;
		Rectangle2							SimulationBox;
		DynamicGrid							DynamicGrid;
		std::set<int>						MovableEntities;
		std::set<int>						CollidingEntities;
		std::set<int>						CollidableEntities;
		std::vector<CollisionPair>			CollisionPairs;
		std::vector<CollisionData>			LogicalCollisionData;
		std::vector<CollisionData>			PhysicalCollisionData;
		std::vector<std::vector<int>>		CollisionGroups;
							
		void								RunConstraintsSolver();
		void								UpdateConstraintsSolver();
		void								InitializeConstraintsSolver();
		
		
		bool								IsPointInside(const Vector2&, const Polygon2*);
		bool								IsPointInside2(const Vector2&, const Polygon2*);
		void								Integrate(ComponentContainer<PositionComponent>&, ComponentContainer<MotionComponent>&);
		void								PrecalculateIntervals(std::set<int>&, ComponentContainer<BodyComponent>&, ComponentContainer<MotionComponent>&);
		void								UpdateCollisionImpulse(ComponentContainer<MotionComponent>&);

		void								GetCollisionImpulseData(ComponentContainer<PositionComponent>&, ComponentContainer<MotionComponent>&);
		void								GetCollisionPointsData(ComponentContainer<BodyComponent>&, ComponentContainer<MotionComponent>&);
				
		void								GetCollisionGroups(ComponentContainer<MotionComponent>&, ComponentContainer<CollisionComponent>&);

		void								CalculateInterval(const Vector2&, const Polygon2*, float&, float&);
		bool								ArePolygonsSeparated(const Polygon2*, const Polygon2*, Vector2&); 
		bool								IsAxisSeparating(const Polygon2*, const Polygon2*, const int&, const int&, Vector2&);
		void								RunSolvers(ComponentContainer<PositionComponent>&, ComponentContainer<MotionComponent>&, ComponentContainer<BodyComponent>&, ComponentContainer<CollisionComponent>&);
		void								ApplyCollisionImpulse(ComponentContainer<PositionComponent>&, ComponentContainer<MotionComponent>&);
		void								ApplyPositionCorrection(ComponentContainer<PositionComponent>&, ComponentContainer<MotionComponent>&, ComponentContainer<BodyComponent>&, ComponentContainer<CollisionComponent>&);
		void								InitializeCollisionsSolver(ComponentContainer<PositionComponent>&, ComponentContainer<MotionComponent>&, ComponentContainer<BodyComponent>&, ComponentContainer<CollisionComponent>&);

		bool								Close();
		bool								Update();
		bool								Initialize();
		bool								Register(const Entity*);
		bool								Unregister(const Entity*);
public:
											PhysicsSystem();
		
		inline const int&					GetIntegrationSubstepNumber() const;
		inline const int&					GetSolverIterationsNumber() const;
		inline const float&					GetPenetrationTolerance() const;
		inline const float&					GetBiasCoefficient() const;
		inline const WorldProperties&		GetWorldProperties() const;
		inline const Rectangle2&			GetSimulationBox() const;
				
		void								SetIntegrationSubstepNumber(const int&);
		void								SetSolverIterationsNumber(const int&);
		void								SetPenetrationTolerance(const float&);
		void								SetBiasCoefficient(const float&);
		void								SetWorldProperties(const WorldProperties&);
		inline void							SetSimulationBox(const Vector2&, const Vector2&);
		inline void							SetSimulationBox(const Rectangle2&);

		inline void							ToggleSleepingMode(const bool&);
};




template<class TList>	PhysicsSystem<TList>::PhysicsSystem()
{
	IntegrationSubstepNumber	= 0;
	SolverIterationsNumber		= 0; 
	MovableSignature			= 0;
	CollidableSignature			= 0;
	PenetrationTolerance		= 0.0f;
	BiasCoeff					= 0.0f;
	SleepingModeFlag			= 0;
	EntityManager				= nullptr;

	CollisionPairs.reserve(300);
};

template<class TList> void PhysicsSystem<TList>::ToggleSleepingMode(const bool& Value)
{
	if(Value)
	{
		SleepingModeFlag = 1;
	}
	else
	{
		SleepingModeFlag = 0;
	}
};

template<class TList> bool PhysicsSystem<TList>::Initialize()
{	
	if(!InitializationStatus)
	{
		auto PositionComponentMask	= static_cast<long long>(1) << IndexOf<TList, PositionComponent>::value;
		auto BodyComponentMask		= static_cast<long long>(1) << IndexOf<TList, BodyComponent>::value;
		auto MotionComponentMask	= static_cast<long long>(1) << IndexOf<TList, MotionComponent>::value;
		auto CollisionComponentMask = static_cast<long long>(1) << IndexOf<TList, CollisionComponent>::value;

		MovableSignature	= PositionComponentMask | MotionComponentMask;
		CollidableSignature = PositionComponentMask | MotionComponentMask | BodyComponentMask;



		DynamicGrid.Initialize();

		PhysicalCollisionData.resize(DefaultCollisionDataSize);
		LogicalCollisionData.resize(DefaultCollisionDataSize);
	
		PhysicalCollisionNumber = 0;
		LogicalCollisionNumber  = 0;

		PenetrationTolerance		= DefaultPenetrationTolerance;
		BiasCoeff					= DefaultBiasCoeff;
		IntegrationSubstepNumber	= DefaultIntegrationSubstepNumber;
		SolverIterationsNumber		= DefaultSolverIterations; 

		std::cout << "Physics System:	Initialization successfull!" << std::endl;
		SystemStatus = InitializationStatus = true;
		return true;
	}
	else
	{
		std::cout << "Physics System:	System already initialized!!" << std::endl;
		return false;
	}
};


template<class TList> bool PhysicsSystem<TList>::Update()
{	
	if(EntityManager != nullptr && SystemStatus==true)
	{
		ComponentContainer<PositionComponent>*  PositionCom		= EntityManager->GetContainer<PositionComponent>(); 
		ComponentContainer<MotionComponent>*    MotionCom		= EntityManager->GetContainer<MotionComponent>();
		ComponentContainer<BodyComponent>*     BodyCom			= EntityManager->GetContainer<BodyComponent>();
		ComponentContainer<CollisionComponent>* CollisionCom	= EntityManager->GetContainer<CollisionComponent>();
		
		if(PositionCom != nullptr && MotionCom != nullptr && BodyCom != nullptr && CollisionCom != nullptr)
		{
			for (int ii = 0; ii < IntegrationSubstepNumber; ++ii)
			{
				RunSolvers(*PositionCom, *MotionCom, *BodyCom, *CollisionCom);
				Integrate(*PositionCom, *MotionCom);
			}
			return true;
		}
	}
	return false;
};


template<class TList> bool PhysicsSystem<TList>::Register(const Entity* Entity)
{
	bool result = false;
	if(Entity != nullptr)
	{
		if( (MovableSignature & Entity->GetComponentMask()) == MovableSignature )
		{
			MovableEntities.insert( Entity->GetIndex() );
			result = true;
		}

		if( (CollidableSignature & Entity->GetComponentMask()) == CollidableSignature )
		{
			CollidableEntities.insert( Entity->GetIndex() );
			result = true;
		}
	}
	return result;
};


template<class TList> bool PhysicsSystem<TList>::Unregister(const Entity* Entity)
{
	bool result = false;
	if(Entity != nullptr)
	{
		if( (MovableSignature & Entity->GetComponentMask()) > 0 )
		{
			if( MovableEntities.erase(Entity->GetIndex()) > 0 ) result = true;
		}

		if( (CollidableSignature & Entity->GetComponentMask()) > 0 )
		{			
			if( CollidableEntities.erase(Entity->GetIndex()) > 0 ) result = true;
		}
	}
	return result;
};


template<class TList> bool PhysicsSystem<TList>::Close()
{
	if(InitializationStatus != false)
	{
		CollisionPairs.clear();
		MovableEntities.clear();
		
		CollidableEntities.clear();
		LogicalCollisionData.clear();
		PhysicalCollisionData.clear();

		CollidingEntities.clear();
		CollisionGroups.clear();

		SystemStatus = InitializationStatus = false;
		std::cout << "Physics System:	Closed!" << std::endl;
		return true;
	}
	else
	{
		std::cout << "Physics System:	System already closed!" << std::endl;
		return false;
	}
};


template<class TList> void PhysicsSystem<TList>::Integrate(ComponentContainer<PositionComponent>& PositionContainer, ComponentContainer<MotionComponent>& MotionContainer)
{
	if(!MovableEntities.empty())
	{		
		float TimeStep = WrdProperties.GetTimeStep() / IntegrationSubstepNumber;
		float SquaredTimeStep = TimeStep * TimeStep;
		float InversedTimeStep = 1.0f / TimeStep;
		
		PositionComponent*	PositionComponent;
		MotionComponent*	MotionComponent;
		
		//for(int ii=0; ii < IntegrationSubstepNumber; ++ii)
		//{
			if(WrdProperties.GetGravityState()) 
			{
				auto GravityMagnitude = WrdProperties.GetGravityMagnitude();
				if(SleepingModeFlag == 1)
				{
					for each(auto Index in MovableEntities)
					{
						MotionComponent = MotionContainer[Index];

						if(!MotionComponent->GetSleep())
						{
							MotionComponent->ApplyForce(   GravityMagnitude * MotionComponent->GetMass(),   ZeroVector);
						}
					}
				}
				else
				{
					for each(auto Index in MovableEntities)
					{
						MotionComponent = MotionContainer[Index];
						MotionComponent->ApplyForce(   GravityMagnitude * MotionComponent->GetMass(),   ZeroVector);
					}
				}
			}
	
			if(WrdProperties.GetDragState())
			{
				if(SleepingModeFlag == 1)
				{
					for each(auto Index in MovableEntities)
					{
						MotionComponent = MotionContainer[Index];
						if(!MotionComponent->GetSleep())
						{
							MotionComponent->ApplyForce( -1.0f * MotionComponent->GetLinearVelocity() * MotionComponent->GetLinearDamping() * MotionComponent->GetMass(), ZeroVector);
							MotionComponent->ApplyTorque( -1.0f * MotionComponent->GetAngularVelocity() * MotionComponent->GetAngularDamping() * MotionComponent->GetInertia());
						}
					}
				}
				else
				{
					for each(auto Index in MovableEntities)
					{
						MotionComponent = MotionContainer[Index];
						MotionComponent->ApplyForce( -1.0f * MotionComponent->GetLinearVelocity() * MotionComponent->GetLinearDamping() * MotionComponent->GetMass(), ZeroVector);
						MotionComponent->ApplyTorque( -1.0f * MotionComponent->GetAngularVelocity() * MotionComponent->GetAngularDamping() * MotionComponent->GetInertia());
					}
				}
			}
			
	
				
			if(SleepingModeFlag == 1)
			{
				for each(auto Index in MovableEntities)
				{	 
					PositionComponent = PositionContainer[Index];
					MotionComponent = MotionContainer[Index];

					if(!MotionComponent->GetSleep())
					{
						if(MotionComponent->GetAngularMotion()) 
						{ 
							PositionComponent->AddAngle( MotionComponent->GetAngularVelocity() * TimeStep +  0.5f*MotionComponent->GetAngularAcceleration() * SquaredTimeStep );
							MotionComponent->SetAngularVelocity( MotionComponent->GetAngularVelocity() +  MotionComponent->GetAngularAcceleration() * TimeStep );
							MotionComponent->SetAngularAcceleration(0.0f);				
						}
						PositionComponent->AddPosition( MotionComponent->GetLinearVelocity() * TimeStep  +  0.5f*MotionComponent->GetLinearAcceleration() * SquaredTimeStep);
						MotionComponent->SetLinearVelocity( MotionComponent->GetLinearVelocity() +  MotionComponent->GetLinearAcceleration() * TimeStep );
						MotionComponent->SetLinearAcceleration(ZeroVector);
					}
				}
			}
			else
			{
				for each(auto Index in MovableEntities)
				{ 
					PositionComponent = PositionContainer[Index];
					MotionComponent = MotionContainer[Index];

					if(MotionComponent->GetAngularMotion()) 
					{ 
						PositionComponent->AddAngle( MotionComponent->GetAngularVelocity() * TimeStep +  0.5f*MotionComponent->GetAngularAcceleration() * SquaredTimeStep );
						MotionComponent->SetAngularVelocity( MotionComponent->GetAngularVelocity() +  MotionComponent->GetAngularAcceleration() * TimeStep );
						MotionComponent->SetAngularAcceleration(0.0f);				
					}
					PositionComponent->AddPosition( MotionComponent->GetLinearVelocity() * TimeStep  +  0.5f*MotionComponent->GetLinearAcceleration() * SquaredTimeStep);
					MotionComponent->SetLinearVelocity( MotionComponent->GetLinearVelocity() +  MotionComponent->GetLinearAcceleration() * TimeStep );
					MotionComponent->SetLinearAcceleration(ZeroVector);
				}
			}
		//}
	
	}
};


template<class TList> void	PhysicsSystem<TList>::RunSolvers(ComponentContainer<PositionComponent>& PositionComponent, ComponentContainer<MotionComponent>& MotionComponent, ComponentContainer<BodyComponent>& BodyComponent, ComponentContainer<CollisionComponent>& CollisionComponent)
{	
	InitializeCollisionsSolver(PositionComponent, MotionComponent, BodyComponent, CollisionComponent);
	for(int ii=0; ii < SolverIterationsNumber; ++ii)
	{
		UpdateCollisionImpulse(MotionComponent);
		ApplyCollisionImpulse(PositionComponent, MotionComponent);	
	}
	ApplyPositionCorrection(PositionComponent, MotionComponent, BodyComponent, CollisionComponent);
};


template<class TList> void	PhysicsSystem<TList>::InitializeConstraintsSolver()
{

};


template<class TList> void	PhysicsSystem<TList>::RunConstraintsSolver()
{

};


template<class TList> void	PhysicsSystem<TList>::UpdateConstraintsSolver()
{

};


template<class TList> void	PhysicsSystem<TList>::InitializeCollisionsSolver(ComponentContainer<PositionComponent>& PositionContainer, ComponentContainer<MotionComponent>& MotionContainer, ComponentContainer<BodyComponent>& BodyContainer, ComponentContainer<CollisionComponent>& CollisionContainer)
{
	PhysicalCollisionNumber = 0;
	LogicalCollisionNumber  = 0;
	static int cnt  = 0;
	if(!CollidableEntities.empty())
	{
		PositionComponent* PositionComponent1;
		PositionComponent* PositionComponent2;
		
		MotionComponent* MotionComponent1;
		MotionComponent* MotionComponent2;

		BodyComponent* BodyComponent1;
		BodyComponent* BodyComponent2;
		
		CollisionComponent* CollisionComponent1;
		CollisionComponent* CollisionComponent2;

		if(SleepingModeFlag == 1)
		{
			for each(auto Index in CollidableEntities)
			{
				CollisionComponent1 = CollisionContainer[Index]; 
				PositionComponent1  = PositionContainer[Index]; 
				MotionComponent1    = MotionContainer[Index]; 
				BodyComponent1      = BodyContainer[Index]; 
				
				CollisionComponent1->ClearBodyContact();
				CollisionComponent1->ClearSensorContact();
				CollisionComponent1->SetInGroupFlag(false);

				if(!MotionComponent1->GetSleep())
				{
					BodyComponent1->SetTransform(PositionComponent1->GetPosition(), PositionComponent1->GetAngle());
					BodyComponent1->UpdateBoundingBox();
				}
			}
		}
		else
		{
			for each(auto Index in CollidableEntities)
			{
				CollisionComponent1 = CollisionContainer[Index]; 
				PositionComponent1  = PositionContainer[Index]; 
				MotionComponent1    = MotionContainer[Index]; 
				BodyComponent1      = BodyContainer[Index]; 

				CollisionComponent1->ClearBodyContact();
				CollisionComponent1->ClearSensorContact();
				BodyComponent1->SetTransform(PositionComponent1->GetPosition(), PositionComponent1->GetAngle());
				BodyComponent1->UpdateBoundingBox();

			}
		}


		if(cnt==0)
		{
			DynamicGrid.SetGridBox(SimulationBox);
			DynamicGrid.Update(BodyContainer, CollisionContainer, CollidableEntities, CollisionPairs/*, CollisionPairsNumber*/);
		}
		

		if(cnt==1)
		{
			cnt = 0; 
		}
		else
		{
			cnt++; 
		}

		if(CollisionPairs.size() >= PhysicalCollisionData.size())
		{
			auto NewSize = 2 * PhysicalCollisionData.size(); 
			PhysicalCollisionData.resize(NewSize);
			LogicalCollisionData.resize(NewSize);
		}
				
		if(!CollisionPairs.empty())
		{
			CollisionData*	CollisionDataPtr;
			Vector2			MinimumTranslationVector;
			Vector2			tempMinimumTranslationVector;
			int				Face1;
			int				Face2;
			int				BodyOverlap;
			
			float			CurrentLength = 0.0f;

			PrecalculateIntervals(CollidableEntities, BodyContainer, MotionContainer);

			CollidingEntities.clear();
			
			for(auto it = CollisionPairs.begin(); it != CollisionPairs.end() ;++it)
			{		
				unsigned B1 = it->GetBody1();
				unsigned B2 = it->GetBody2();

				CollisionComponent1 = CollisionContainer[B1]; 
				PositionComponent1  = PositionContainer[B1]; 
				MotionComponent1    = MotionContainer[B1]; 
				BodyComponent1      = BodyContainer[B1]; 

				CollisionComponent2 = CollisionContainer[B2]; 
				PositionComponent2  = PositionContainer[B2]; 
				MotionComponent2    = MotionContainer[B2]; 
				BodyComponent2      = BodyContainer[B2]; 

				BodyOverlap = 0;

				MinimumTranslationVector.y = MinimumTranslationVector.x = 0.0f;
				tempMinimumTranslationVector = MinimumTranslationVector;
							
				CurrentLength = 0.0f;
				
				for(unsigned qq=0; qq < BodyComponent1->GetPolygonNumber(); ++qq) 
				{
					Polygon2* P1 = BodyComponent1->GetPolygon(qq);
					for(unsigned pp=0; pp < BodyComponent2->GetPolygonNumber(); ++pp) 
					{
						Polygon2* P2 = BodyComponent2->GetPolygon(pp);
						if(!ArePolygonsSeparated( P1, P2, tempMinimumTranslationVector)) 
						{		
							auto Ln = tempMinimumTranslationVector.x * tempMinimumTranslationVector.x +
									  tempMinimumTranslationVector.y * tempMinimumTranslationVector.y;
							BodyOverlap = 1;
							if( Ln > CurrentLength )
							{
								Face1 = qq;
								Face2 = pp;
								CurrentLength = Ln;
								MinimumTranslationVector = tempMinimumTranslationVector;
							}
						}
					}
				}
				
				for(unsigned qq=0; qq < BodyComponent1->GetSensorNumber(); ++qq) 
				{
					Sensor* S1 = BodyComponent1->GetSensor(qq);
					for(unsigned pp=0; pp < BodyComponent2->GetPolygonNumber(); ++pp) 
					{
						Polygon2* P2 = BodyComponent2->GetPolygon(pp);
						if(!ArePolygonsSeparated( S1->GetBody(), P2, tempMinimumTranslationVector)) 
						{		
							CollisionComponent1->AddSensorContact(qq, B2);
						}
					}
				}

				for(unsigned qq=0; qq < BodyComponent2->GetSensorNumber(); ++qq) 
				{
					Sensor* S2 = BodyComponent2->GetSensor(qq);
					for(unsigned pp=0; pp < BodyComponent1->GetPolygonNumber(); ++pp) 
					{
						Polygon2* P1 = BodyComponent1->GetPolygon(pp);
						if(!ArePolygonsSeparated( S2->GetBody(), P1, tempMinimumTranslationVector)) 
						{		
							CollisionComponent2->AddSensorContact(qq, B1);
						}
					}
				}


				if(BodyOverlap == 1) 
				{											
					if( (CollisionComponent1->GetCollisionType() == Logical || CollisionComponent2->GetCollisionType() == Logical) || 
						 (MotionComponent1->GetTypeOfBody() != Dynamic &&  MotionComponent2->GetTypeOfBody() != Dynamic)) 
					{
						CollisionDataPtr = &LogicalCollisionData[LogicalCollisionNumber];					
						CollisionDataPtr->Clear();
						CollisionDataPtr->cType = Logical;
					} 
					else 
					{
						CollisionDataPtr = &PhysicalCollisionData[PhysicalCollisionNumber];					
						CollisionDataPtr->Clear();
						CollisionDataPtr->cType = Physical;
					}
						
					CollisionComponent1->AddBodyContact(B2,CollisionDataPtr);
					CollisionComponent2->AddBodyContact(B1,CollisionDataPtr);

  					CollisionDataPtr->Body1 = B1;
					CollisionDataPtr->Body2 = B2;
					CollisionDataPtr->Overlap = BodyOverlap;
					CollisionDataPtr->MinimumTranslationVector = MinimumTranslationVector;
					CollisionDataPtr->Face1 = Face1;
					CollisionDataPtr->Face2 = Face2;

					CollisionDataPtr->StaticFrictionCoeff = 0.5f *  (CollisionComponent1->GetStaticFrictionCoeff() + CollisionComponent2->GetStaticFrictionCoeff());
					CollisionDataPtr->DynamicFrictionCoeff = 0.5f * (CollisionComponent1->GetDynamicFrictionCoeff() + CollisionComponent2->GetDynamicFrictionCoeff());
					
					Polygon2* P1 = BodyComponent1->GetPolygon(Face1);
					Polygon2* P2 = BodyComponent2->GetPolygon(Face2); 

					if ( (P2->GlobalCentroid - P1->GlobalCentroid) * CollisionDataPtr->MinimumTranslationVector < 0.0f) 
					{
						CollisionDataPtr->MinimumTranslationVector *= -1;
					}

					CollisionDataPtr->Penetration		= -0.5f*MinimumTranslationVector.length();//by³o 0.5
					CollisionDataPtr->CollisionNormal	= CollisionDataPtr->MinimumTranslationVector.unit();
					CollisionDataPtr->Restitution		= CollisionComponent1->GetRestitutionCoeff() > CollisionComponent2->GetRestitutionCoeff() ? CollisionComponent1->GetRestitutionCoeff() : CollisionComponent2->GetRestitutionCoeff();

					
					if(CollisionDataPtr->cType != Logical)
					{
						if(MotionComponent1->GetTypeOfBody() == Dynamic &&  MotionComponent2->GetTypeOfBody() == Dynamic)
						{
							CollisionComponent1->AccumulateTranslation( -0.5f * CollisionDataPtr->MinimumTranslationVector);
							CollisionComponent2->AccumulateTranslation(  0.5f * CollisionDataPtr->MinimumTranslationVector);
							CollidingEntities.insert(B1);
							CollidingEntities.insert(B2);
						}
						else if(MotionComponent1->GetTypeOfBody() != Dynamic)
						{
							CollisionComponent2->AccumulateTranslation( 1.98f * CollisionDataPtr->MinimumTranslationVector);
							CollidingEntities.insert(B2);
						}
						else
						{
							CollisionComponent1->AccumulateTranslation( -1.98f * CollisionDataPtr->MinimumTranslationVector);
							CollidingEntities.insert(B1);
						}
					}	

					CollisionDataPtr->NormalImpulseMagnitude = 100000.0f;
					if(CollisionDataPtr->cType == Logical)
					{
						++LogicalCollisionNumber;
					}
					else
					{
						++PhysicalCollisionNumber;
					}
				}
			}
					
			if(SleepingModeFlag == 1) GetCollisionGroups(MotionContainer, CollisionContainer);

			GetCollisionPointsData(BodyContainer, MotionContainer);
			GetCollisionImpulseData(PositionContainer, MotionContainer);

		}
	}
};


template<class TList> void	PhysicsSystem<TList>::ApplyCollisionImpulse(ComponentContainer<PositionComponent>& PositionContainer, ComponentContainer<MotionComponent>& MotionContainer)
{
	if(PhysicalCollisionNumber != 0)
	{	
		for(unsigned ii = 0; ii < PhysicalCollisionNumber; ++ii) 
		{					
			CollisionData* CollisionDataPtr = &PhysicalCollisionData[ ii ];

			PositionComponent* PositionComponent1 = PositionContainer[CollisionDataPtr->Body1];
			PositionComponent* PositionComponent2 = PositionContainer[CollisionDataPtr->Body2];
			
			MotionComponent*   MotionComponent1 = MotionContainer[CollisionDataPtr->Body1];
			MotionComponent*   MotionComponent2 = MotionContainer[CollisionDataPtr->Body2];

			if( !MotionComponent1->GetSleep() || !MotionComponent2->GetSleep() )
			{
				if(CollisionDataPtr->Overlap == 1) 
				{
					if(CollisionDataPtr->Impulse == 1) 
					{
						MotionComponent1->ApplyImpulse( -CollisionDataPtr->NormalImpulse, CollisionDataPtr->CollisionPoint - PositionComponent1->GetPosition() );
						MotionComponent2->ApplyImpulse(  CollisionDataPtr->NormalImpulse, CollisionDataPtr->CollisionPoint - PositionComponent2->GetPosition() );

						if(WrdProperties.GetFrictionState()) 
						{
							MotionComponent1->ApplyImpulse( -CollisionDataPtr->TangentImpulse, CollisionDataPtr->CollisionPoint - PositionComponent1->GetPosition() );
							MotionComponent2->ApplyImpulse(  CollisionDataPtr->TangentImpulse, CollisionDataPtr->CollisionPoint - PositionComponent2->GetPosition() );
						}
					}
				}	
			}
		}
	}
};


template<class TList> void	PhysicsSystem<TList>::ApplyPositionCorrection(ComponentContainer<PositionComponent>& PositionComponent, ComponentContainer<MotionComponent>& MotionComponent, ComponentContainer<BodyComponent>& BodyComponent, ComponentContainer<CollisionComponent>& CollisionComponent)
{
	if(PhysicalCollisionNumber != 0)
	{
		for each(auto Index in CollidingEntities)
		{
			if( !MotionComponent[Index]->GetSleep() )
			{
				PositionComponent[Index]->AddPosition( CollisionComponent[Index]->GetTotalTranslation() );
			}
		}
	}
};


template<class TList> bool PhysicsSystem<TList>::IsPointInside2(const Vector2& Point, const Polygon2* Polygon)
{
	//float PointProjection = 0.0f;
	for(unsigned jj=0; jj < Polygon->VertexNumber; ++jj)
	{
		//PointProjection = (Point - Polygon->GlobalVertex[jj]) * Polygon->GlobalNormals[jj] ;
		//if ( ((Point - Polygon->GlobalVertex[jj]) * Polygon->GlobalNormals[jj]) >= 0.009f ) return false;//0.005
		float PointProjection = (Point - Polygon->GlobalVertex[jj]) * Polygon->GlobalNormals[jj];
		if ( PointProjection >= 0.009f ) return false;//0.005
	}
	return true;
};


template<class TList> bool PhysicsSystem<TList>::IsPointInside(const Vector2& Point, const Polygon2* Polygon)
{	
	for(unsigned jj=0; jj < Polygon->ValidEdgeNumber; ++jj)
	{		
		float PointProjection = Point * Polygon->GlobalNormals[ Polygon->ValidEdgeIndex[jj] ];	
		if ( PointProjection > Polygon->Intervals[jj].y || PointProjection < Polygon->Intervals[jj].x ) return false;//0.005
	}
	return true;
};


template<class TList> void	PhysicsSystem<TList>::PrecalculateIntervals(std::set<int>& Entities,ComponentContainer<BodyComponent>& BodyContainer, ComponentContainer<MotionComponent>& MotionContainer)
{
	for each(auto Index in Entities)
	{
		BodyComponent*    BodyComponent    = BodyContainer[Index];
		MotionComponent*  MotionComponent  = MotionContainer[Index];

		if( !MotionComponent->GetSleep() )
		{
			for(unsigned ii=0; ii < BodyComponent->GetPolygonNumber();++ii)
			{
				Polygon2* Polygon =  BodyComponent->GetPolygon(ii);

				for(unsigned jj=0; jj< Polygon->ValidEdgeNumber; ++jj)
				{
					unsigned ValidIndex = Polygon->ValidEdgeIndex[jj];
					Vector2  Normal = Polygon->GlobalNormals[ValidIndex];
					float min = Polygon->GlobalVertex[0] * Normal;
					float max = min;
					float d   = min;

					for(unsigned pp=1; pp < Polygon->VertexNumber; ++pp)
					{
						d = Polygon->GlobalVertex[pp]  * Normal; 
						if (d < min) min = d;	
						if (d > max) max = d; 
					}
					Polygon->Intervals[ValidIndex].x = min;
					Polygon->Intervals[ValidIndex].y = max;
				}
			}

			for(unsigned ii=0; ii < BodyComponent->GetSensorNumber();++ii)
			{
				Polygon2* const Polygon =  BodyComponent->GetSensor(ii)->GetBody();

				for(unsigned jj=0; jj< Polygon->ValidEdgeNumber; ++jj)
				{
					unsigned ValidIndex = Polygon->ValidEdgeIndex[jj];
					Vector2  Normal = Polygon->GlobalNormals[ValidIndex];
					float min = Polygon->GlobalVertex[0] * Normal;
					float max = min;
					float d   = min;

					for(unsigned pp=1; pp < Polygon->VertexNumber; ++pp)
					{
						d = Polygon->GlobalVertex[pp]  * Normal; 
						if (d < min) min = d;	
						if (d > max) max = d; 
					}
					Polygon->Intervals[ValidIndex].x = min;
					Polygon->Intervals[ValidIndex].y = max;
				}
			}

		}
	}
};


template<class TList> void	PhysicsSystem<TList>::CalculateInterval(const Vector2& Normal, const Polygon2* Polygon, float& min, float& max)
{
   float d = Polygon->GlobalVertex[0] * Normal;
   min = max = d; 
   
   for(unsigned ii=1; ii<Polygon->VertexNumber; ++ii)
   {
		d = Polygon->GlobalVertex[ii]  * Normal; 
		if (d < min) min = d;	
		if (d > max) max = d; 
   }
};


template<class TList> bool PhysicsSystem<TList>::IsAxisSeparating(const Polygon2* Polygon1, const Polygon2* Polygon2, const int& PolyIndex, const int& NormalIndex, Vector2 &Normal) //czêœc obliczeñ jest na darmo// powtarzaj¹ siê
{
	float mina, maxa; 
    float minb, maxb; 
	if(PolyIndex == 1)
	{
		mina = Polygon1->Intervals[NormalIndex].x;
		maxa = Polygon1->Intervals[NormalIndex].y;
		CalculateInterval(Normal, Polygon2, minb, maxb); 
	}

	if(PolyIndex == 2)
	{
		minb = Polygon2->Intervals[NormalIndex].x;
		maxb = Polygon2->Intervals[NormalIndex].y;
		CalculateInterval(Normal, Polygon1, mina, maxa); 
	}

    if (mina > maxb || minb > maxa) 
		      return true; 
    // find the interval overlap 
    float d0 = maxa - minb; 
    float d1 = maxb - mina; 
    float depth = (d0 < d1)? d0 : d1; 
	
	if(depth > PenetrationTolerance)
		depth-=PenetrationTolerance;
	
	if (depth > 0.0f) 
	{
		ClampNormal(Normal); //mo¿e to przenieœæ w inne miejsce
		Normal *= depth; 
		return false; 
	} else {
		return true;
	}
};


template<class TList> bool PhysicsSystem<TList>::ArePolygonsSeparated(const Polygon2* Polygon1, const Polygon2* Polygon2, Vector2& PushVector) 
{	
	Vector2 Normal(0.0f,0.0f);
	auto PushVectorLength = 400000.0f; //PushVector.x * PushVector.x + PushVector.y * PushVector.y;//by³o zeo
	auto NormalLength = 0.0f;

	for(unsigned ii=0; ii<Polygon1->ValidEdgeNumber; ++ii) 
	{
		Normal = Polygon1->GlobalNormals[ Polygon1->ValidEdgeIndex[ii] ]; 
		if( IsAxisSeparating(Polygon1, Polygon2, 1, ii, Normal)) return true;

		if(Polygon1->EdgeFlag[ Polygon1->ValidEdgeIndex[ii] ] == true)
		{
			NormalLength = (Normal.x*Normal.x)+(Normal.y*Normal.y);
			if (PushVectorLength > NormalLength) 
			{
				PushVector = Normal;	
				PushVectorLength = NormalLength;
			}
		}
	}
			
	for(unsigned ii=0; ii<Polygon2->ValidEdgeNumber; ++ii) 
	{
		Normal = Polygon2->GlobalNormals[ Polygon2->ValidEdgeIndex[ii] ];
		if( IsAxisSeparating(Polygon1, Polygon2, 2, ii, Normal)) return true;

		if(Polygon2->EdgeFlag[ Polygon2->ValidEdgeIndex[ii] ] == true)
		{
			NormalLength = (Normal.x*Normal.x)+(Normal.y*Normal.y);
			if (PushVectorLength > NormalLength) 
			{
				PushVector = Normal;	
				PushVectorLength = NormalLength;
			}
		}
	}
	return false;
};


template<class TList> void	PhysicsSystem<TList>::GetCollisionPointsData(ComponentContainer<BodyComponent>& BodyContainer, ComponentContainer<MotionComponent>& MotionContainer)
{					
	for(unsigned kk = 0; kk < PhysicalCollisionNumber; ++kk) 
	{
		CollisionData* Solution =  &PhysicalCollisionData[ kk ];

		BodyComponent* BodyComponent1 = BodyContainer[Solution->Body1];
		BodyComponent* BodyComponent2 = BodyContainer[Solution->Body2];
			
		MotionComponent*   MotionComponent1 = MotionContainer[Solution->Body1];
		MotionComponent*   MotionComponent2 = MotionContainer[Solution->Body2];

		if( !MotionComponent1->GetSleep() || !MotionComponent2->GetSleep() )
		{
			if(Solution->Overlap == 1) 
			{
				float	 NumberOfPoints = 0.0f;
				unsigned B1 = Solution->Body1;
				unsigned B2 = Solution->Body2;
				Solution->CollisionPoint = ZeroVector;
			
				Polygon2* P1 = BodyComponent1->GetPolygon(Solution->Face1);
				Polygon2* P2 = BodyComponent2->GetPolygon(Solution->Face2);
		
				const Rectangle2* BBox = &BodyComponent2->GetBoundingBox();
								
				for(unsigned jj=0; jj < P1->VertexNumber; ++jj)
				{
					Vector2 Buffer = P1->GlobalVertex[ jj ];
					if(	Buffer.x >= BBox->min.x &&	Buffer.x <= BBox->max.x && Buffer.y <= BBox->max.y &&	Buffer.y >= BBox->min.y)
					{
						if(IsPointInside(Buffer, P2)) 
						{
							Solution->CollisionPoint += Buffer;
							++NumberOfPoints;
						}
					}
				} 
		
									
				BBox = &BodyComponent1->GetBoundingBox();

				for(unsigned jj=0; jj < P2->VertexNumber; ++jj)
				{
					Vector2 Buffer = P2->GlobalVertex[ jj ];
					if(	Buffer.x >= BBox->min.x &&	Buffer.x <= BBox->max.x && Buffer.y <= BBox->max.y &&	Buffer.y >= BBox->min.y)
					{
						if(IsPointInside(Buffer, P1)) 
						{
							Solution->CollisionPoint += Buffer;
							++NumberOfPoints;
						}
					}
				} 
	
			
				if(NumberOfPoints>0.0f) 
				{
					Solution->CollisionPoint /= NumberOfPoints;
					Solution->Overlap = 1;
				} 
				else 
				{
					Solution->Overlap = 0;
				}
			}
		}
	}
};


template<class TList> void	PhysicsSystem<TList>::UpdateCollisionImpulse(ComponentContainer<MotionComponent>& MotionContainer)
{
	for(unsigned ii = 0; ii < PhysicalCollisionNumber; ++ii) 
	{
		CollisionData* CollisionDataPtr =  &PhysicalCollisionData[ii];
				
		MotionComponent*   MotionComponent1 = MotionContainer[CollisionDataPtr->Body1];
		MotionComponent*   MotionComponent2 = MotionContainer[CollisionDataPtr->Body2];

		if( !MotionComponent1->GetSleep() || !MotionComponent2->GetSleep() )
		{

			if(CollisionDataPtr->Overlap == 1) 
			{
				unsigned B1 = CollisionDataPtr->Body1;
				unsigned B2 = CollisionDataPtr->Body2;
						
				Vector2 RelativeVelocity  = MotionComponent2->GetLinearVelocityAtLocalPoint(CollisionDataPtr->R2) - MotionComponent1->GetLinearVelocityAtLocalPoint(CollisionDataPtr->R1);
				float	VelocityMagnitude = RelativeVelocity * CollisionDataPtr->CollisionNormal;
	
				if(VelocityMagnitude < VelocityThreshold) 
				{
					VelocityMagnitude += CollisionDataPtr->BiasNormal;

					if(VelocityMagnitude < PlasticCollisionThreshold)
					{
						CollisionDataPtr->NormalImpulseMagnitude = -1.0f*(CollisionDataPtr->Restitution*VelocityMagnitude)*CollisionDataPtr->DenominatorNormal;
					}
					else
					{
						CollisionDataPtr->NormalImpulseMagnitude = -1.0f*VelocityMagnitude*CollisionDataPtr->DenominatorNormal;
					}
					CollisionDataPtr->NormalImpulse = CollisionDataPtr->NormalImpulseMagnitude*CollisionDataPtr->CollisionNormal;
	

					CollisionDataPtr->TangentImpulseMagnitude = -1.0f*((RelativeVelocity+CollisionDataPtr->BiasTangent) * CollisionDataPtr->CollisionTangent) * CollisionDataPtr->DenominatorTangent;
		 		 
					if( CollisionDataPtr->StaticFrictionCoeff*abs(CollisionDataPtr->NormalImpulseMagnitude) < abs(CollisionDataPtr->TangentImpulseMagnitude) ) 
					{
						CollisionDataPtr->TangentImpulse = -CollisionDataPtr->DynamicFrictionCoeff*abs(CollisionDataPtr->NormalImpulseMagnitude) * CollisionDataPtr->CollisionTangent;
					} 
					else
					{
						CollisionDataPtr->TangentImpulse = CollisionDataPtr->TangentImpulseMagnitude*CollisionDataPtr->CollisionTangent;
					}
				
					CollisionDataPtr->Impulse = 1;
				}	
				else 
				{
				CollisionDataPtr->Impulse = 0;
				}
			}
		}
	}
};


template<class TList> void	PhysicsSystem<TList>::GetCollisionImpulseData(ComponentContainer<PositionComponent>& PositionContainer, ComponentContainer<MotionComponent>& MotionContainer)
{
	Vector2			GravityValue = WrdProperties.GetGravityMagnitude();
	float			TimeStep = WrdProperties.GetTimeStep();

	for(unsigned ii = 0; ii < PhysicalCollisionNumber; ++ii) //ContactList.size()
	{
		CollisionData* CollisionDataPtr =  &PhysicalCollisionData[ii];

		MotionComponent*  MotionComponent1  = MotionContainer[CollisionDataPtr->Body1];
		MotionComponent*  MotionComponent2  = MotionContainer[CollisionDataPtr->Body2];

		PositionComponent*  PositionComponent1  = PositionContainer[CollisionDataPtr->Body1];
		PositionComponent*  PositionComponent2  = PositionContainer[CollisionDataPtr->Body2];

		if( !MotionComponent1->GetSleep() || !MotionComponent2->GetSleep() )
		{
			if(CollisionDataPtr->Overlap == 1) 
			{
				//unsigned B1 = CollisionDataPtr->Body1;
				//unsigned B2 = CollisionDataPtr->Body2;

				float CumulativeMass	= MotionComponent1->GetInverseMass() + MotionComponent2->GetInverseMass();

				CollisionDataPtr->R1 = CollisionDataPtr->CollisionPoint - PositionComponent1->GetPosition();
				CollisionDataPtr->R2 = CollisionDataPtr->CollisionPoint - PositionComponent2->GetPosition();
						
				Vector2 RelativeVelocity = MotionComponent2->GetLinearVelocityAtLocalPoint(CollisionDataPtr->R2) - MotionComponent1->GetLinearVelocityAtLocalPoint(CollisionDataPtr->R1);
				float   VelocityMagnitude = RelativeVelocity * CollisionDataPtr->CollisionNormal;

				CollisionDataPtr->BiasNormal =  CollisionDataPtr->Penetration*BiasCoeff;
				
				float RN_1 = VectorProduct(CollisionDataPtr->R1, CollisionDataPtr->CollisionNormal);
				float RN_2 = VectorProduct(CollisionDataPtr->R2, CollisionDataPtr->CollisionNormal);
		
				CollisionDataPtr->DenominatorNormal = CumulativeMass + (RN_1*RN_1)* (MotionComponent1->GetInverseInertia()) + (RN_2*RN_2)*(MotionComponent2->GetInverseInertia());
				CollisionDataPtr->DenominatorNormal = CollisionDataPtr->DenominatorNormal > 0.0f ? 1.0f /  CollisionDataPtr->DenominatorNormal : 0.0f;

				float NormalPart = VectorProduct(RelativeVelocity, CollisionDataPtr->CollisionNormal);
				if(  NormalPart < 0.0f )	
				{
						CollisionDataPtr->CollisionTangent.x = -CollisionDataPtr->CollisionNormal.y;
						CollisionDataPtr->CollisionTangent.y =  CollisionDataPtr->CollisionNormal.x;
				}
				else
				{
					CollisionDataPtr->CollisionTangent.x =  CollisionDataPtr->CollisionNormal.y;
					CollisionDataPtr->CollisionTangent.y = -CollisionDataPtr->CollisionNormal.x;
				}
			
				RN_1 = VectorProduct(CollisionDataPtr->R1,CollisionDataPtr->CollisionTangent);
				RN_2 = VectorProduct(CollisionDataPtr->R2,CollisionDataPtr->CollisionTangent);
				
				CollisionDataPtr->DenominatorTangent = CumulativeMass + ( ((RN_1*RN_1) * MotionComponent1->GetInverseInertia()) + ((RN_2*RN_2) * MotionComponent2->GetInverseInertia()) );
				CollisionDataPtr->DenominatorTangent = CollisionDataPtr->DenominatorTangent > 0.0f ? 1.0f / CollisionDataPtr->DenominatorTangent : 0.0f;
				CollisionDataPtr->BiasTangent = 2.0f*GravityValue * TimeStep * CollisionDataPtr->CollisionTangent * CollisionDataPtr->CollisionTangent; //grawitacja
			}
		}
	}
};


template<class TList> inline const int& PhysicsSystem<TList>::GetIntegrationSubstepNumber() const
{
	return IntegrationSubstepNumber;		
};


template<class TList> inline const int& PhysicsSystem<TList>::GetSolverIterationsNumber() const
{
	return SolverIterationsNumber;
};


template<class TList> inline const float& PhysicsSystem<TList>::GetPenetrationTolerance() const
{
	return PenetrationTolerance;
};


template<class TList> inline const float& PhysicsSystem<TList>::GetBiasCoefficient() const
{
	return BiasCoeff;
};


template<class TList> inline const WorldProperties& PhysicsSystem<TList>::GetWorldProperties() const
{
	return WrdProperties;
};


template<class TList> inline const Rectangle2&	PhysicsSystem<TList>::GetSimulationBox() const
{
	return SimulationBox;
};


template<class TList> void	PhysicsSystem<TList>::SetIntegrationSubstepNumber(const int& Value)
{
	if(Value > 0)	
	{
		IntegrationSubstepNumber = Value;
	}
};


template<class TList> void	PhysicsSystem<TList>::SetSolverIterationsNumber(const int& Value)
{
	if(Value > 0)	
	{
		SolverIterationsNumber = Value;
	}
};


template<class TList> void PhysicsSystem<TList>::SetPenetrationTolerance(const float& Value)
{
	PenetrationTolerance = Value;
};


template<class TList> void PhysicsSystem<TList>::SetBiasCoefficient(const float& Value)
{
	BiasCoeff = Value;
};


template<class TList> inline void PhysicsSystem<TList>::SetSimulationBox(const Vector2& Position, const Vector2& Size)
{
	SimulationBox.min.x = Position.x;
	SimulationBox.min.y = Position.y;
	SimulationBox.max.x = Position.x + Size.x;
	SimulationBox.max.y = Position.y + Size.y;
};


template<class TList> inline void PhysicsSystem<TList>::SetSimulationBox(const Rectangle2& Value)
{
	SimulationBox = Value;
};


template<class TList> void PhysicsSystem<TList>::SetWorldProperties(const WorldProperties& Value)
{
	WrdProperties = Value;
};


//std::vector<std::vector<int>>		CollisionGroups;
template<class TList> void  PhysicsSystem<TList>::GetCollisionGroups(ComponentContainer<MotionComponent>& MotionComponent, ComponentContainer<CollisionComponent>& CollisionComponent)
{
	std::stack<int> ToCheck;

	CollisionGroups.clear();

	unsigned CurrentGroup = 0;
	unsigned SubGroupSize = 0;

	for each(auto Index in CollidingEntities)
	{
		
		if( !CollisionComponent[Index]->GetInGroupFlag() )
		{
			CollisionGroups.resize(CurrentGroup + 1);
			CollisionGroups[CurrentGroup].reserve(150);
			ToCheck.push(Index);

			while( !ToCheck.empty() )
			{
				auto Index2 = ToCheck.top();

				if( !CollisionComponent[Index2]->GetInGroupFlag() )
				{
					CollisionGroups[CurrentGroup].push_back(Index2);
					CollisionComponent[Index2]->SetInGroupFlag(true);

					SubGroupSize = CollisionComponent[Index2]->GetBodyContactNumber();	
					if(SubGroupSize > 0)
					{
						auto SubGroup = CollisionComponent[Index2]->GetBodyContact();
						for(unsigned ii=0; ii < SubGroupSize; ++ii)
						{
							if( !CollisionComponent[ SubGroup[ii].first ]->GetInGroupFlag() )
							{
								ToCheck.push( SubGroup[ii].first );
							}
						}
					}
				}
				ToCheck.pop();
			}
			CurrentGroup+=1;
		}
	}


	bool ThresholdExceded = false;

	float Threshold = 10.0f * EnergyThreshold;
	for(unsigned ii=0; ii < CollisionGroups.size(); ++ii)
	{

		ThresholdExceded = false;
		if( CollisionGroups[ii].size() > 1)
		{
			for(unsigned jj=0; jj < CollisionGroups[ii].size(); ++jj)
			{
				auto Energy = MotionComponent[ CollisionGroups[ii][jj] ]->GetEnergy();
				if (Energy > Threshold)
				{
					ThresholdExceded = true;
					break;
				}
				else if(Energy < EnergyThreshold)
				{
					MotionComponent[ CollisionGroups[ii][jj] ]->SetSleep(true);
				}
			}
		}
		else
		{
			MotionComponent[ CollisionGroups[ii][0] ]->SetSleep(false);
		}


		if(ThresholdExceded)
		{
			for(unsigned jj=0; jj < CollisionGroups[ii].size(); ++jj)
			{
				MotionComponent[ CollisionGroups[ii][jj] ]->SetSleep(false);
			}
		}
	}

};