#pragma once

#include <windows.h>
#include <set>
#include <GL/glew.h>
#include <GL/wglew.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "SystemBase.h"
#include "MainWindow.h"
#include "Program.h"
#include "Shaders.h"
#include "Texture.h"
#include "BitmapFont.h"
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")


enum ScreenResolutionMode { R640x480, R800x600, R1024x768, R1280x720, R1280x960, R1920x1080 };

const int								DefaultWindowPosX = 100;
const int								DefaultWindowPosY = 100;
const int								DefaultResolutionX = 1024;
const int								DefaultResolutionY = 768;
const int								DefaultWindowWidth = 1024;
const int								DefaultWindowHeight = 768;
const DWORD								DefaultWindowStyle = WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION;
const ScreenResolutionMode				DefaultScreenResolutionMode = R1024x768;


template<class TList>
class RenderSystem : public SystemBase
{
	bool								FXAAFlag;
	bool								FullscreenFlag;
	long long							RenderableSignature;
	long long							CameraSignature;
	std::set<int>						CameraEntities;
	std::set<int>						RenderableEntities;
	std::set<int>						RenderableTexturedEntities;

	GLuint 								BaseMVPMatrix;
	GLuint								BasePosition;
    GLuint								BaseColor; 

	GLuint								TexturedPosition;
	GLuint								TexturedMVPMatrix;
	GLuint								TexturedUV;
	GLuint								TexturedTextureSampler;
	GLuint								TexturedColor;
	GLuint								TexturedColorMixingCoeff;
	GLuint								TexturedTexCoordOffset;
	GLuint								TexturedTexCoordScale;

	GLuint								PostPosition;
	GLuint								PostTextureSampler;
	GLuint								PostAlpha;
		
	GLuint								FXAAPosition;
	GLuint								FXAATextureSampler;
	GLuint								FXAAAlpha;
	GLuint								FXAAWidth;
	GLuint								FXAAHeight;
		
	GLuint								FullResFBO;
	GLuint								FullResFBO_Texture;
	GLuint								FullResFBO_DepthrenderBuffer;

	GLuint								RenderBox_VAO;
	GLuint								RenderBox_VBO;

	GLuint								LetterBox_VAO;
	GLuint								LetterBox_VBO;
		

	ScreenResolutionMode				CurrentScreenResolutionMode;
	int									ResolutionX;
	int									ResolutionY;
	int									WindowPosX;
	int									WindowPosY;
	int									WindowWidth;
	int									WindowHeight;
	DWORD								WindowStyle;
	HGLRC								hRC;   
	HDC									hDC; 
	HWND								hWND;
	MainWindow							Window;

	std::vector<std::pair<std::string,BitmapFont*>>	BitmapFonts;
	int									CurrentFontIndex;
	
	Program								ShaderBase;
	Program								ShaderTextured;
	Program								ShaderPostprocess;
	Program								ShaderFXAA;
	
	float								ProjectionWidth;
	float								ProjectionHeight;
	float								ProjectionDepth;
	Vector2								CameraPosition;
	float								AspectRatio;

	bool								InitFrameBuffer();
	bool								ClearFrameBuffer();
	bool								SetupPixelFormat();
	bool								CreateOGLContext();
	bool								InitializeOGL();
	
	bool								Initialize();
	bool								Update();
	bool								Register(const Entity*);
	bool								Unregister(const Entity*);
	bool								Close();

public:

	bool								ToggleAntiAliasing(const bool&);
	bool								ToggleVerticalSynchronization(const bool&);
	bool								ToggleFullscreen(const bool&);
	
	//bool								LoadFont(const std::string&, const std::string&);
	bool								LoadFont(BitmapFont*);
	bool								SetCurrentFont(const std::string&);
	inline const BitmapFont*			GetCurrentFont() const;
	bool								ClearFont(const std::string&);
	bool								ClearAllFonts();


	bool								SetScreenResolutionMode(const ScreenResolutionMode&);
	inline const ScreenResolutionMode&	GetScreenResolutionMode() const;
	inline void							SetWindowPositionX(const int&);
	inline void							SetWindowPositionY(const int&);
	inline const int&					GetWindowPositionX() const;
	inline const int&					GetWindowPositionY() const;
	inline const int&					GetWindowWidth() const;
	inline const int&					GetWindowHeight() const;


};


template<class TList> bool RenderSystem<TList>::Initialize()
{	
	if(!InitializationStatus)
	{
		auto PositionComponentIndex = static_cast<long long>(1) << IndexOf<TList, PositionComponent>::value;
		auto RenderComponentIndex   = static_cast<long long>(1) << IndexOf<TList, RenderComponent>::value;
		
		RenderableSignature = PositionComponentIndex | RenderComponentIndex;
		CameraSignature		= static_cast<long long>(1) << IndexOf<TList, CameraComponent>::value;

		FullscreenFlag = false;
		FXAAFlag = false;
		WindowStyle = DefaultWindowStyle;
		
		ResolutionX = DefaultResolutionX;
		ResolutionY = DefaultResolutionY;

		WindowPosX = DefaultWindowPosX;
		WindowPosY = DefaultWindowPosY;

		WindowWidth = DefaultWindowWidth;
		WindowHeight = DefaultWindowHeight;
		
		CurrentScreenResolutionMode = DefaultScreenResolutionMode;
		
		RECT Rect = {0, 0, ResolutionX, ResolutionY};  
		AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, FALSE);
		WindowWidth  = Rect.right;
		WindowHeight = Rect.bottom;

		AspectRatio = static_cast<float>(WindowWidth) / static_cast<float>(WindowHeight);

		if (!Window.Create(L"Render System beta", WindowStyle, 0, WindowPosX, WindowPosY, WindowWidth, WindowHeight))
		{
			std::cout << "Render System:	Unabe to create application window." << std::endl;
			std::cout << "Render System:	Initialization failed!" << std::endl;
			return false;
		}
		else
		{
			hWND = Window.Window();
			hDC	 = GetDC(hWND);
		}
				
		if( SetupPixelFormat() == false )
		{
			std::cout << "Render System:	SetupPixelFormat Failed." << std::endl;
			std::cout << "Render System:	Initialization failed!" << std::endl;
			return false;
		}

		if( CreateOGLContext() == false ) 
		{
			std::cout << "Render System:	Unable to set OpenGl context." << std::endl;
			std::cout << "Render System:	Initialization failed!" << std::endl;
			return false;
		}

		if(InitializeOGL() == false )
		{		
			std::cout << "Render System:	Unable to prepare OpenGl scene." << std::endl;
			std::cout << "Render System:	Initialization failed!" << std::endl;
			return false;
		}

		if(InitFrameBuffer() == false)
		{
			std::cout << "Render System:	Unable to initialize frame buffers." << std::endl;
			std::cout << "Render System:	Initialization failed!" << std::endl;
			return false;
		}
	
		ShowWindow(hWND, SW_SHOW); 
		SetFocus(hWND);
	
		glEnable(GL_BLEND); 
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glCullFace(GL_FRONT_AND_BACK);
		glClearColor( 101.0f/256.0f, 180.0f/256.0f, 221.0f/256.0f, 0.01f);  
		
		ToggleVerticalSynchronization(false);
		ToggleAntiAliasing(false);
		ToggleFullscreen(false);

		ProjectionWidth	 = 80.0f;
		ProjectionHeight = ProjectionWidth / AspectRatio;
		ProjectionDepth	 = 100.0f;
		CameraPosition   = ZeroVector;
		
		SystemStatus = InitializationStatus = true;
		std::cout << "Render System:	Initialization successfull!" << std::endl;
		return true;
	}
	else
	{
		std::cout << "Render System:	system already initialized!!" << std::endl;
		return false;
	}
};


template<class TList> bool RenderSystem<TList>::Update()
{	
	if(SystemStatus==true)
	{		
		glEnable(GL_DEPTH_TEST);

		glBindFramebuffer(GL_FRAMEBUFFER, FullResFBO);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glViewport(0,0,WindowWidth,WindowHeight);
				
		if(EntityManager != nullptr)
		{
			ComponentContainer<PositionComponent>& PositionContainer	= *EntityManager->GetContainer<PositionComponent>(); 
			ComponentContainer<RenderComponent>&   RenderContainer	= *EntityManager->GetContainer<RenderComponent>();
			ComponentContainer<CameraComponent>&   CameraContainer	= *EntityManager->GetContainer<CameraComponent>();

			PositionComponent*  PositionComponent;
			RenderComponent*    RenderComponent;
			CameraComponent*	CameraComponent;
			

			float ScreenHW = 1.0f;
			float ScreenHH = 1.0f / AspectRatio;

			float WorldHW = 0.0f;
			float WorldHH = 0.0f;

			glm::mat4 ScreenProjectionMatrix = glm::ortho( -ScreenHW, ScreenHW, -ScreenHH, ScreenHH, -ProjectionDepth	, ProjectionDepth);
			glm::mat4 ScreenViewMatrix = glm::lookAt(glm::vec3( 0.0f, 0.0f, 1.0f), glm::vec3( 0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f ,0.0f) );
			glm::mat4 ScreenPVMatrix =  ScreenProjectionMatrix * ScreenViewMatrix;
						
			Vector2			Position;
			unsigned int	VertexArrayObject;
			glm::mat4		ModelMatrix;
			glm::mat4		MVPMatrix;
			

			if(!CameraEntities.empty())
			{
				for each(auto Index in CameraEntities)
				{
					CameraComponent = CameraContainer[Index];

					if(CameraComponent->GetActivationFlag())
					{						
						ProjectionWidth	 = CameraComponent->GetProjectionWidth();
						ProjectionHeight = ProjectionWidth / AspectRatio;
						ProjectionDepth	 = CameraComponent->GetProjectionDepth();
						CameraPosition   =CameraComponent->GetPosition() + CameraComponent->GetOffset();
						break;
					}
				}
			}
						
			WorldHW = ProjectionWidth  * 0.5f;
			WorldHH = ProjectionHeight * 0.5f;

			glm::mat4 WorldProjectionMatrix = glm::ortho( -WorldHW, WorldHW, -WorldHH, WorldHH, -ProjectionDepth, ProjectionDepth);;
			glm::mat4 WorldViewMatrix		= glm::lookAt(glm::vec3( CameraPosition.x, CameraPosition.y, 1), glm::vec3( CameraPosition.x, CameraPosition.y , -1), glm::vec3(0,1,0) );
			glm::mat4 WorldPVMatrix			= WorldProjectionMatrix * WorldViewMatrix;
			
			ShaderBase.Use();
			if(!RenderableEntities.empty() /*&& PositionCom != nullptr && RenderCom != nullptr*/)
			{					
				for each(auto Index in RenderableEntities)
				{					
					RenderComponent   = RenderContainer[Index];
					PositionComponent = PositionContainer[Index];

					VertexArrayObject =  RenderComponent->GetVAO();
					if( VertexArrayObject > 0 )
					{
						Position = PositionComponent->GetPosition();
											
						if(RenderComponent->GetModelCoordinatesMode() == World)
						{
							ModelMatrix = glm::translate( glm::mat4(1.0f), glm::vec3(Position.x, Position.y, RenderComponent->GetLayer() ));  
							ModelMatrix = glm::rotate(ModelMatrix, PositionComponent->GetAngle() , glm::vec3(0.0f, 0.0, 1.0f)); 
							ModelMatrix = glm::scale(ModelMatrix, RenderComponent->GetScale() ); 
							MVPMatrix = WorldPVMatrix * ModelMatrix;
						}
						else
						{
							ModelMatrix = glm::translate( glm::mat4(1.0f), glm::vec3(Position.x, Position.y*ScreenHH, RenderComponent->GetLayer() ));  
							ModelMatrix = glm::rotate(ModelMatrix, PositionComponent->GetAngle() , glm::vec3(0.0f, 0.0, 1.0f)); 
							ModelMatrix = glm::scale(ModelMatrix, RenderComponent->GetScale() ); 
							MVPMatrix = ScreenPVMatrix * ModelMatrix; 
						}
						
						glUniformMatrix4fv(BaseMVPMatrix, 1, GL_FALSE, glm::value_ptr(MVPMatrix));
						glVertexAttrib4fv(BaseColor, glm::value_ptr( RenderComponent->GetVertexColor() ));
						glBindVertexArray ( VertexArrayObject );
						glDrawArrays(GL_TRIANGLES, 0, RenderComponent->GetVertexNumber() );
						glBindVertexArray(0);
					}
				}
			}

			ShaderTextured.Use();
			glEnable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0);

			if(!RenderableTexturedEntities.empty() /*&& PositionCom != nullptr && RenderCom != nullptr*/)
			{	
				glUniform1i(TexturedTextureSampler, GL_TEXTURE0);			
				for each(auto Index in RenderableTexturedEntities)
				{			
					RenderComponent   = RenderContainer[Index];
					PositionComponent = PositionContainer[Index];

					VertexArrayObject =  RenderComponent->GetVAO();
					if( VertexArrayObject > 0 )
					{
						Position = PositionComponent->GetPosition();
										
						if(RenderComponent->GetModelCoordinatesMode() == World)
						{
							ModelMatrix = glm::translate( glm::mat4(1.0f), glm::vec3(Position.x, Position.y, RenderComponent->GetLayer() ));  
							ModelMatrix = glm::rotate(ModelMatrix, PositionComponent->GetAngle() , glm::vec3(0.0f, 0.0, 1.0f)); 
							ModelMatrix = glm::scale(ModelMatrix, RenderComponent->GetScale() ); 
							MVPMatrix = WorldPVMatrix * ModelMatrix;
						}
						else
						{
							ModelMatrix = glm::translate( glm::mat4(1.0f), glm::vec3(Position.x, Position.y*ScreenHH, RenderComponent->GetLayer() ));  
							ModelMatrix = glm::rotate(ModelMatrix, PositionComponent->GetAngle() , glm::vec3(0.0f, 0.0, 1.0f)); 
							ModelMatrix = glm::scale(ModelMatrix, RenderComponent->GetScale() ); 
							MVPMatrix = ScreenPVMatrix * ModelMatrix; 
						}
											
						glBindTexture(GL_TEXTURE_2D, RenderComponent->GetTextureId());
					
						glUniformMatrix4fv(TexturedMVPMatrix, 1, GL_FALSE, glm::value_ptr(MVPMatrix));
						glVertexAttrib4fv(TexturedColor, glm::value_ptr( RenderComponent->GetVertexColor() ));
						glUniform1f(TexturedColorMixingCoeff, RenderComponent->GetColorMixingCoeff() );

						glVertexAttrib2fv(TexturedTexCoordScale,  glm::value_ptr(RenderComponent->GetTexCoordScale()) );
						glVertexAttrib2fv(TexturedTexCoordOffset, glm::value_ptr(RenderComponent->GetTexCoordOffset()) );
					
						glBindVertexArray (RenderComponent->GetVAO());
						glDrawArrays(GL_TRIANGLES, 0, RenderComponent->GetVertexNumber() );
						glBindVertexArray(0);
					}
				}
			}
			
			glDisable(GL_DEPTH_TEST);
			if(!RenderableEntities.empty() /*&& PositionCom != nullptr && RenderCom != nullptr*/)
			{	
				auto Font = GetCurrentFont();
				if(Font != nullptr)
				{
					glBindTexture(GL_TEXTURE_2D, Font->GetFontTextureId());
										
					glUniform1f(TexturedColorMixingCoeff, 0.0f );

					for each(auto Index in RenderableEntities)
					{				
						RenderComponent   = RenderContainer[Index];
						PositionComponent = PositionContainer[Index];

						if(RenderComponent->HasText())
						{
							auto Text		= RenderComponent->GetText();
							float TextSize	= RenderComponent->GetTextSize();
							Vector2 Offset	= ZeroVector;
							
							float Angle	= PositionComponent->GetAngle() + RenderComponent->GetTextAngle() ;

							Position = PositionComponent->GetPosition() + RenderComponent->GetTextOffset();

							Position.x += 0.5f*TextSize;
							Position.y += 0.5f*TextSize;
					
							auto FontScale = Font->GetScaleFactor();
							glVertexAttrib2f(TexturedTexCoordScale, FontScale.x, FontScale.y);

							glVertexAttrib4fv(TexturedColor, glm::value_ptr( RenderComponent->GetTextColor() ));

							for(unsigned ii=0; ii < Text.length(); ++ii)
							{						
								auto Coordinates = Font->GetCharCoordinates( Text[ii] ); 
								glVertexAttrib2f(TexturedTexCoordOffset, Coordinates.x, Coordinates.y);

								if(RenderComponent->GetModelCoordinatesMode() == World)
								{
									ModelMatrix = glm::translate( glm::mat4(1.0f), glm::vec3(Position.x, Position.y, RenderComponent->GetLayer() ));  
									ModelMatrix = glm::rotate(ModelMatrix, Angle , glm::vec3(0.0f, 0.0, 1.0f)); 
									ModelMatrix = glm::scale(ModelMatrix,  glm::vec3(TextSize,TextSize,1.0f) ); 
									MVPMatrix = WorldPVMatrix * ModelMatrix;
								}
								else
								{
									
									ModelMatrix = glm::translate( glm::mat4(1.0f), glm::vec3(Position.x, Position.y*ScreenHH, RenderComponent->GetLayer() ));  
									ModelMatrix = glm::rotate( ModelMatrix, Angle , glm::vec3(0.0f, 0.0, 1.0f)); 
									ModelMatrix = glm::translate( ModelMatrix, glm::vec3(Offset.x, Offset.y*ScreenHH, RenderComponent->GetLayer() ));  
									ModelMatrix = glm::scale(ModelMatrix,  glm::vec3(TextSize,TextSize,1.0f) ); 
									MVPMatrix = ScreenPVMatrix * ModelMatrix; 
								}

								glUniformMatrix4fv(TexturedMVPMatrix, 1, GL_FALSE, glm::value_ptr(MVPMatrix));

								glBindVertexArray (LetterBox_VAO);
								glDrawArrays(GL_TRIANGLES, 0, 6);
								glBindVertexArray(0);

								Offset.x += TextSize * Font->GetCharSpacingFactor();
							}
						}
					}
				}
			}
		}


		glEnable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER,  0);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				
	
		//glEnable(GL_TEXTURE_2D);
		//glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, FullResFBO_Texture);
		
		if(FXAAFlag)
		{
			ShaderFXAA.Use();
			glUniform1f(FXAAAlpha, 1.0f);
			glUniform1i(FXAATextureSampler, GL_TEXTURE0);
			glUniform1f(FXAAWidth, static_cast<float>(WindowWidth));
			glUniform1f(FXAAHeight, static_cast<float>(WindowHeight));
		}
		else
		{
			ShaderPostprocess.Use();
			glUniform1f(PostAlpha, 1.0f);
			glUniform1i(PostTextureSampler, GL_TEXTURE0);
		}
		
		glBindVertexArray (RenderBox_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6); 
		glBindVertexArray(0);

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	
		SwapBuffers(hDC);
		return true;
	}
	return false;
};


template<class TList> bool RenderSystem<TList>::Register(const Entity* Entity)
{
	bool result = false;

	if(Entity != nullptr)
	{
		if( (RenderableSignature & Entity->GetComponentMask()) == RenderableSignature )
		{
			auto Render = EntityManager->GetComponent<RenderComponent>(Entity);
			if(Render->GetTextureFlag())
			{
				RenderableTexturedEntities.insert( Entity->GetIndex());
				result = true;
			}
			else
			{
				RenderableEntities.insert( Entity->GetIndex());
				result = true;
			}
		}

		if( (CameraSignature & Entity->GetComponentMask()) == CameraSignature )
		{
			CameraEntities.insert(Entity->GetIndex());
			result = true;
		}
	}
	return result;
};


template<class TList> bool RenderSystem<TList>::Unregister(const Entity* Entity)
{
	bool result = false;

	if(Entity != nullptr)
	{
		if( (RenderableSignature & Entity->GetComponentMask()) == RenderableSignature )
		{
			RenderableEntities.erase( Entity->GetIndex());
			RenderableTexturedEntities.erase( Entity->GetIndex());
			result = true;
		}

		if( (CameraSignature & Entity->GetComponentMask()) == CameraSignature )
		{
			CameraEntities.erase(Entity->GetIndex());
			result = true;
		}
	}
	return result;
};


template<class TList> bool RenderSystem<TList>::Close()
{
	if(InitializationStatus != false)
	{
		CameraEntities.clear();
		RenderableEntities.clear();
		RenderableTexturedEntities.clear();

		ClearFrameBuffer();
		ClearAllFonts();
		ShaderBase.Delete();
		ShaderTextured.Delete();
		ShaderPostprocess.Delete();
		ShaderFXAA.Delete();
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hRC);
		ReleaseDC(hWND, hDC);
		DestroyWindow(hWND);
		hWND = NULL;
		SystemStatus = InitializationStatus = false;
		std::cout << "Render System:	Closed!" << std::endl;
		return true;
	}
	else
	{
		std::cout << "Render System:	System already closed!" << std::endl;
		return false;
	}
};


template<class TList> bool RenderSystem<TList>::SetupPixelFormat()
{
	PIXELFORMATDESCRIPTOR pfd; 
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR)); 
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR); 
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32; 
	pfd.cDepthBits = 24; 
	pfd.iLayerType = PFD_MAIN_PLANE; 
	
	int nPixelFormat = ChoosePixelFormat(hDC, &pfd); 
	if (nPixelFormat == 0) 
			return false;

	auto bResult = SetPixelFormat(hDC, nPixelFormat, &pfd); 

	return bResult > 0;
};


template<class TList> bool RenderSystem<TList>::CreateOGLContext()
{
	HGLRC tempOpenGLContext = wglCreateContext(hDC); 
	wglMakeCurrent(hDC, tempOpenGLContext); 

	GLenum error = glewInit(); 
	if (error != GLEW_OK) 
		return false;

	int attributes[] = 
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3, 
		WGL_CONTEXT_MINOR_VERSION_ARB, 3, 
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, 
		0
	};

	if (wglewIsSupported("WGL_ARB_create_context") == 1)
	{
		hRC = wglCreateContextAttribsARB(hDC, NULL, attributes); 
		wglMakeCurrent(NULL, NULL); 
		wglDeleteContext(tempOpenGLContext); 
		wglMakeCurrent(hDC, hRC); 
	}
	else 
	{
		hRC = tempOpenGLContext; 
	}

	int glVersion[2] = {-1, -1}; 
	glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]); 
	glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]); 
		
	return true;
};


template<class TList> bool RenderSystem<TList>::InitializeOGL()
{
	Shader	basicVertexShader;
	Shader	basicFragmentShader;
		
	if(!basicVertexShader.Create(GL_VERTEX_SHADER, BasicVertexShader)) return false;
	if(!basicFragmentShader.Create(GL_FRAGMENT_SHADER, BasicFragmentShader)) return false;

	if(!ShaderBase.Create(basicVertexShader, basicFragmentShader)) return false;
		
	BasePosition = glGetAttribLocation(ShaderBase.GetProgramID(), "Position");
    BaseColor = glGetAttribLocation(ShaderBase.GetProgramID(), "Color"); 
	BaseMVPMatrix = glGetUniformLocation(ShaderBase.GetProgramID(), "MVPMatrix");
			
	basicVertexShader.Delete();
	basicFragmentShader.Delete();
	


	Shader	postVertexShader;
	Shader	postFragmentShader;
	
	if(!postVertexShader.Create(GL_VERTEX_SHADER, PostprocessVertexShader)) return false;
	if(!postFragmentShader.Create(GL_FRAGMENT_SHADER, PostprocessFragmentShader)) return false;

	if(!ShaderPostprocess.Create(postVertexShader, postFragmentShader)) return false;

	PostPosition = glGetAttribLocation(ShaderPostprocess.GetProgramID(), "Position"); 
	PostTextureSampler = glGetUniformLocation(ShaderPostprocess.GetProgramID(), "Texture"); 
	PostAlpha = glGetUniformLocation(ShaderPostprocess.GetProgramID(), "Alpha"); 
		
	postVertexShader.Delete();
	postFragmentShader.Delete();
		

	
	Shader	texturedVertexShader;
	Shader	texturedFragmentShader;

	if(!texturedVertexShader.Create(GL_VERTEX_SHADER, TexturedVertexShader)) return false;
	if(!texturedFragmentShader.Create(GL_FRAGMENT_SHADER, TexturedFragmentShader)) return false;

	if(!ShaderTextured.Create(texturedVertexShader, texturedFragmentShader)) return false;

	TexturedPosition			= glGetAttribLocation(ShaderTextured.GetProgramID(), "Position"); 
	TexturedMVPMatrix			= glGetUniformLocation(ShaderTextured.GetProgramID(), "MVPMatrix"); 
	TexturedUV					= glGetAttribLocation(ShaderTextured.GetProgramID(), "TexCoord"); 
	TexturedTextureSampler		= glGetUniformLocation(ShaderTextured.GetProgramID(), "Texture"); 
	TexturedColorMixingCoeff	= glGetUniformLocation(ShaderTextured.GetProgramID(), "ColorMixingCoeff"); 
	TexturedColor				= glGetAttribLocation(ShaderTextured.GetProgramID(), "Color"); 
	TexturedTexCoordOffset		= glGetAttribLocation(ShaderTextured.GetProgramID(), "TexCoordOffset"); 
	TexturedTexCoordScale		= glGetAttribLocation(ShaderTextured.GetProgramID(), "TexCoordScale"); 

	texturedVertexShader.Delete();
	texturedFragmentShader.Delete();


	
	Shader	fxaaVertexShader;
	Shader	fxaaFragmentShader;

	if(!fxaaVertexShader.Create(GL_VERTEX_SHADER, FXAAVertexShader)) return false;
	if(!fxaaFragmentShader.Create(GL_FRAGMENT_SHADER, FXAAFragmentShader)) return false;
	
	if(!ShaderFXAA.Create(fxaaVertexShader, fxaaFragmentShader)) return false;
	
	FXAAPosition = glGetAttribLocation(ShaderFXAA.GetProgramID(), "Position");
	FXAATextureSampler = glGetUniformLocation(ShaderFXAA.GetProgramID(), "Texture");
	FXAAAlpha = glGetUniformLocation(ShaderFXAA.GetProgramID(), "Alpha");
	FXAAWidth = glGetUniformLocation(ShaderFXAA.GetProgramID(), "Width");
	FXAAHeight = glGetUniformLocation(ShaderFXAA.GetProgramID(), "Height");;
	
	fxaaVertexShader.Delete();
	fxaaFragmentShader.Delete();


		
	return true;
};

/*
template<class TList> void RenderSystem<TList>::SetProjectionMatrix(glm::mat4& Matrix)
{
	ProjectionMatrix = Matrix;
	UpdateMVPMatrix();
};


template<class TList> void RenderSystem<TList>::SetViewMatrix(glm::mat4& Matrix)
{
	ViewMatrix = Matrix;
	UpdateMVPMatrix();
};


template<class TList> void RenderSystem<TList>::SetModelMatrix(glm::mat4& Matrix)
{
	ModelMatrix = Matrix;
	UpdateMVPMatrix();
};


template<class TList> void RenderSystem<TList>::UpdateMVPMatrix()
{
	MVPMatrix = ProjectionMatrix * ViewMatrix * ModelMatrix;
///	glUniformMatrix4fv(iMVPLoc, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
};*/



template<class TList> bool RenderSystem<TList>::ToggleVerticalSynchronization(const bool& bEnabled)
{
	if(!wglSwapIntervalEXT)return false;
	if(bEnabled)wglSwapIntervalEXT(1);
	else wglSwapIntervalEXT(0);
	return true;
};



template<class TList> bool RenderSystem<TList>::InitFrameBuffer()
{		
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &FullResFBO_Texture);
 	glBindTexture(GL_TEXTURE_2D, FullResFBO_Texture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, WindowWidth, WindowHeight, 0,GL_RGBA, GL_UNSIGNED_BYTE, 0);
 	glBindTexture(GL_TEXTURE_2D, 0);
	
	glGenRenderbuffers(1, &FullResFBO_DepthrenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, FullResFBO_DepthrenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WindowWidth, WindowHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glGenFramebuffers(1, &FullResFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FullResFBO);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, FullResFBO_DepthrenderBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D, FullResFBO_Texture, 0);
	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers); 
	GLenum Result;
	if( (Result = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
		return false;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	static const GLfloat RenderBox_Vertex[] = { 
		 1.0f, -1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f
	};
	
	glGenVertexArrays(1, &RenderBox_VAO);
	glGenBuffers(1, &RenderBox_VBO);
	
	glBindVertexArray(RenderBox_VAO); 
	glBindBuffer(GL_ARRAY_BUFFER,RenderBox_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(RenderBox_Vertex),RenderBox_Vertex, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray (0);
		
	static const GLfloat LetterBox_Vertex[] = { 
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};

	glGenVertexArrays(1, &LetterBox_VAO);
	glBindVertexArray(LetterBox_VAO); 

	glGenBuffers(1, &LetterBox_VBO); 
	glBindBuffer(GL_ARRAY_BUFFER, LetterBox_VBO); 
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(LetterBox_Vertex), LetterBox_Vertex, GL_STATIC_DRAW); 
			
	glEnableVertexAttribArray(0); 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), NULL ); 
	glEnableVertexAttribArray(1); 
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), ((char*)NULL)+3*sizeof(GLfloat) ); 
		
	glBindVertexArray(0); 
    glBindBuffer(GL_ARRAY_BUFFER, 0); 


	return true;
};


template<class TList> bool RenderSystem<TList>::ClearFrameBuffer()
{
	glDeleteFramebuffers(1,&FullResFBO);
	glDeleteTextures(1, &FullResFBO_Texture);
	glDeleteRenderbuffers(1, &FullResFBO_DepthrenderBuffer);

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray (0);

	glDeleteBuffers(1, &RenderBox_VBO);
	glDeleteVertexArrays(1, &RenderBox_VAO);
	
	glDeleteBuffers(1, &LetterBox_VBO);
	glDeleteVertexArrays(1, &LetterBox_VAO);
		
	return true;
};


template<class TList> bool RenderSystem<TList>::ToggleAntiAliasing(const bool& Value)
{
	return FXAAFlag = Value;
};


template<class TList> bool RenderSystem<TList>::ToggleFullscreen(const bool& Value)
{	
	if(Value)
	{			
		DEVMODE NewScreenSettings;                   
		memset(&NewScreenSettings,0,sizeof(NewScreenSettings)); 

		DEVMODE PreviousScreenSettings;                   
		memset(&PreviousScreenSettings,0,sizeof(PreviousScreenSettings)); 

		EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &PreviousScreenSettings);
						
		NewScreenSettings.dmSize=sizeof(NewScreenSettings);  
		NewScreenSettings.dmPelsWidth    = ResolutionX;    
		NewScreenSettings.dmPelsHeight   = ResolutionY;   
		NewScreenSettings.dmBitsPerPel   = 32;             
		NewScreenSettings.dmFields		 = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
	
		ShowWindow(hWND, SW_HIDE);

		long Result = ChangeDisplaySettings(&NewScreenSettings, CDS_FULLSCREEN);
		  
		if ( Result != DISP_CHANGE_SUCCESSFUL )
		{			
			if(FullscreenFlag)
			{
				ChangeDisplaySettings(&PreviousScreenSettings, CDS_FULLSCREEN);
 				FullscreenFlag = true;
			}
			else
			{
				FullscreenFlag = false;
			}
			ShowWindow(hWND, SW_SHOW);
			SetForegroundWindow(hWND);
			return FullscreenFlag;
		}
		else 
		{			
			ClearFrameBuffer();
			
			WindowWidth  = ResolutionX;
			WindowHeight = ResolutionY;

			SetWindowLongPtr(hWND, GWL_STYLE,  WS_POPUPWINDOW | WS_VISIBLE );
			MoveWindow(hWND, 0, 0, WindowWidth, WindowHeight, TRUE);

			AspectRatio = static_cast<float>(WindowWidth) / static_cast<float>(WindowHeight);

			InitFrameBuffer();
			ShowWindow(hWND, SW_SHOW);
			SetForegroundWindow(hWND);

			FullscreenFlag = true;

			return FullscreenFlag;
		}
	}
	else
	{
		ClearFrameBuffer();
		ChangeDisplaySettings(0, 0);

		RECT Rect = {0, 0, ResolutionX, ResolutionY};  
		AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, FALSE);
		WindowWidth  = Rect.right;
		WindowHeight = Rect.bottom;
		SetWindowLongPtr(hWND, GWL_STYLE,  WindowStyle);
		MoveWindow(hWND, WindowPosX, WindowPosY, WindowWidth, WindowHeight, TRUE);

		AspectRatio = static_cast<float>(WindowWidth) / static_cast<float>(WindowHeight);
				
		InitFrameBuffer();
		ShowWindow(hWND, SW_SHOW);
		SetForegroundWindow(hWND);
		FullscreenFlag = false;

		return FullscreenFlag;
	}
};
	

template<class TList> bool	RenderSystem<TList>::SetScreenResolutionMode(const ScreenResolutionMode& NewResolution)
{
	auto PreviousResolutionX = ResolutionX;
	auto PreviousResolutionY = ResolutionY;

	if(NewResolution == R640x480)
	{
		ResolutionX = 640;
		ResolutionY = 480;
	}
	else if(NewResolution == R800x600)
	{
		ResolutionX = 800;
		ResolutionY = 600;
	}
	else if(NewResolution == R1024x768)
	{
		ResolutionX = 1024;
		ResolutionY = 768;
	}
	else if(NewResolution == R1280x720)
	{
		ResolutionX = 1280;
		ResolutionY = 720;
	}
	else if(NewResolution == R1280x960)
	{
		ResolutionX = 1280;
		ResolutionY = 960;
	}
	else if(NewResolution == R1920x1080)
	{
		ResolutionX = 1920;
		ResolutionY = 1080;
	}

	auto Result = ToggleFullscreen(FullscreenFlag);

	if(!Result)
	{
		ResolutionX = PreviousResolutionX; 
		ResolutionY = PreviousResolutionY;
	}
	else
	{
		CurrentScreenResolutionMode = NewResolution;
	}

	AspectRatio = static_cast<float>(WindowWidth) / static_cast<float>(WindowHeight);

	return Result;
};


template<class TList> inline const ScreenResolutionMode& RenderSystem<TList>::GetScreenResolutionMode() const
{
	return CurrentScreenResolutionMode;
};


template<class TList> inline void RenderSystem<TList>::SetWindowPositionX(const int& Value)
{
	WindowPosX = Value;
	MoveWindow(hWND, WindowPosX, WindowPosY, WindowWidth, WindowHeight, TRUE);
};


template<class TList> inline void RenderSystem<TList>::SetWindowPositionY(const int& Value)
{
	WindowPosY = Value;
	MoveWindow(hWND, WindowPosX, WindowPosY, WindowWidth, WindowHeight, TRUE);
};


template<class TList> inline const int& RenderSystem<TList>::GetWindowPositionX() const
{
	return WindowPosX;
};


template<class TList> inline const int&	RenderSystem<TList>::GetWindowPositionY() const
{
	return WindowPosY;
};


template<class TList> inline const int&	RenderSystem<TList>::GetWindowWidth() const
{
	return WindowWidth;
};


template<class TList> inline const int&	RenderSystem<TList>::GetWindowHeight() const
{
	return WindowHeight;
};



//template<class TList> bool RenderSystem<TList>::LoadFont(const std::string& Tag, const std::string& FileName)
template<class TList> bool RenderSystem<TList>::LoadFont(BitmapFont* Font)
{
	if(Font != nullptr)
	{
		if(Font->GetValid())
		{
			BitmapFonts.push_back(std::make_pair(Font->GetTag(), Font));
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
};


template<class TList> bool RenderSystem<TList>::SetCurrentFont(const std::string& Tag)
{
	if(!BitmapFonts.empty())
	{
		int NumberOfFonts = BitmapFonts.size();
		for(int ii=0; ii < NumberOfFonts; ++ii)
		{
			if( BitmapFonts[ii].first == Tag )
			{
				CurrentFontIndex = ii;
				return true;
			}
		}
	}
	return false;
};


template<class TList> inline const BitmapFont* RenderSystem<TList>::GetCurrentFont() const
{
	if(!BitmapFonts.empty())
	{
		if(CurrentFontIndex >= 0 && CurrentFontIndex < static_cast<int>(BitmapFonts.size()))
		{
			return BitmapFonts[CurrentFontIndex].second;
		}
	}
	return nullptr;
};


template<class TList> bool RenderSystem<TList>::ClearFont(const std::string&)
{
	if(!BitmapFonts.empty())
	{
		int NumberOfFonts = BitmapFonts.size();
		for(int ii=0; ii < NumberOfFonts; ++ii)
		{
			if( BitmapFonts[ii].first == Tag )
			{
				//CurrentFontTexture = BitmapFonts[ii].second.Clear();
				BitmapFonts.erase(BitmapFonts.begin() + ii);
				return true;
			}
		}
	}
	return false;
};


template<class TList> bool RenderSystem<TList>::ClearAllFonts()
{
	if(!BitmapFonts.empty())
	{
		BitmapFonts.clear();
		return true;
	}
	return false;
};