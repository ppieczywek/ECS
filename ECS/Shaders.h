#pragma once
#include <string>


static std::string BasicVertexShader =
" #version 330 \n"
" layout(location = 0) in vec3 Position; \n"
" layout(location = 2) in vec4 Color; \n"
" uniform mat4 MVPMatrix; \n"
" out vec4 FragmentColor; \n"
" void main(void) \n"
" { \n"
"     gl_Position = MVPMatrix * vec4(Position, 1.0); \n"
"     FragmentColor = Color; \n"
" } \n";



static std::string BasicFragmentShader =
" #version 330 \n"
" in  vec4 FragmentColor; \n"
" out vec4 out_Color; \n"
" void main(void) \n"
" { \n"
"	out_Color = FragmentColor; \n"
" } \n";



static std::string TexturedVertexShader =
" #version 330 \n"
" layout(location = 0) in vec3 Position; \n"
" layout(location = 1) in vec2 TexCoord; \n"
" layout(location = 2) in vec4 Color; \n"
" layout(location = 4) in vec2 TexCoordScale; \n"
" layout(location = 5) in vec2 TexCoordOffset; \n"
" out vec4 VertexColor; \n"
" out vec2 UV; \n"
" uniform mat4 MVPMatrix; \n"
" void main(void) \n"
" { \n"
" gl_Position = MVPMatrix * vec4(Position, 1.0); \n"
" UV = (TexCoord * TexCoordScale) + TexCoordOffset; \n"
" VertexColor = Color; \n"
" } \n";



static std::string TexturedFragmentShader =
" #version 330 \n"
" in vec2 UV; \n"
" in  vec4 VertexColor; \n"
" uniform sampler2D Texture; \n"
" uniform float ColorMixingCoeff; \n"
" out vec4 Color; \n"
" vec4 TextureColor; \n"
" void main() \n"
" { \n"
"	TextureColor = texture(Texture, UV); \n"
"	Color = ColorMixingCoeff * TextureColor + (1.0f-ColorMixingCoeff) * vec4( VertexColor.xyz, TextureColor.a); \n"
" } \n";

//vec4( texture2D( renderedTexture, UV).xyz, Alpha)
//" Color = vec4( texture2D(Texture, UV).xyz, Alpha); \n"

//"	Color = ColorMixingCoeff * texture2D(Texture, UV) + (1.0f-ColorMixingCoeff) * VertexColor; \n"

static std::string PostprocessVertexShader =
" #version 330 \n"
" layout(location = 0) in vec3 Position; \n"
" out vec2 UV; \n"
" void main() \n"
" { \n"
"	gl_Position =  vec4(Position.xy,-1,1); \n"
"	UV = (Position.xy+vec2(1,1))/2.0; \n"
" } \n";



static std::string PostprocessFragmentShader =
" #version 330 \n"
" in vec2 UV; \n"
" out vec4 Color; \n"
" uniform sampler2D Texture; \n"
" uniform float Alpha; \n"
" void main() \n"
" { \n"
"	Color = vec4( texture( Texture, UV).xyz, Alpha); \n"
" } \n";



static std::string FXAAVertexShader =
" #version 330 \n"
" layout(location = 0) in vec3 Position; \n"
" out vec2 UV; \n"
" void main() \n"
" { \n"
" gl_Position =  vec4(Position.xy,-1,1); \n"
" UV = (Position.xy+vec2(1,1))/2.0; \n"
" } \n";



static std::string FXAAFragmentShader =
"#version 330  \n"
" uniform sampler2D Texture; \n"
" vec2 texcoordOffset; \n"
" uniform float Alpha; \n"
" uniform float Width; \n"
" uniform float Height; \n"
" out vec4 Color; \n"
" in vec2 UV; \n"
" void main() { \n"
" texcoordOffset.x = 1.0/Width; \n"
" texcoordOffset.y = 1.0/Height; \n"
" float FXAA_SPAN_MAX = 8.0; \n"
" float FXAA_REDUCE_MUL = 1.0/8.0; \n"
" float FXAA_REDUCE_MIN = (1.0/128.0); \n"
" vec3 rgbNW = texture(Texture, UV.xy + (vec2(-1.0, -1.0) * texcoordOffset)).xyz; \n"
" vec3 rgbNE = texture(Texture, UV.xy + (vec2(+1.0, -1.0) * texcoordOffset)).xyz; \n"
" vec3 rgbSW = texture(Texture, UV.xy + (vec2(-1.0, +1.0) * texcoordOffset)).xyz; \n"
" vec3 rgbSE = texture(Texture, UV.xy + (vec2(+1.0, +1.0) * texcoordOffset)).xyz; \n"
" vec3 rgbM  = texture(Texture, UV.xy).xyz; \n"
" vec3 luma = vec3(0.299, 0.587, 0.114); \n"
" float lumaNW = dot(rgbNW, luma); \n"
" float lumaNE = dot(rgbNE, luma); \n"
" float lumaSW = dot(rgbSW, luma); \n"
" float lumaSE = dot(rgbSE, luma); \n"
" float lumaM  = dot( rgbM, luma); \n"
" float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE))); \n"
" float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE))); \n"
" vec2 dir; \n"
" dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE)); \n"
" dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE)); \n"
" float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL), FXAA_REDUCE_MIN); \n"
" float rcpDirMin = 1.0/(min(abs(dir.x), abs(dir.y)) + dirReduce); \n"
" dir = min(vec2(FXAA_SPAN_MAX,  FXAA_SPAN_MAX),  \n"
" max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX), dir * rcpDirMin)) * texcoordOffset; \n"	
" vec3 rgbA = (1.0/2.0) * ( \n"
" texture(Texture, UV.xy + dir * (1.0/3.0 - 0.5)).xyz + \n"
" texture(Texture, UV.xy + dir * (2.0/3.0 - 0.5)).xyz); \n"
" vec3 rgbB = rgbA * (1.0/2.0) + (1.0/4.0) * ( \n"
" texture(Texture, UV.xy + dir * (0.0/3.0 - 0.5)).xyz + \n"
" texture(Texture, UV.xy + dir * (3.0/3.0 - 0.5)).xyz); \n"
" float lumaB = dot(rgbB, luma); \n"
" if((lumaB < lumaMin) || (lumaB > lumaMax)){ \n"
" Color.xyz=rgbA; \n"
" } else { \n"
" Color.xyz=rgbB; \n"
" } \n"
" Color.a = Alpha; \n"
" } \n";