#pragma once
#include "shadersemantics.h"

namespace Render
{
const char shaderHeader[] =
	"#version 430\n"
	"#define TILE_SIZE 16\n"
	"layout (std140, binding = 0) uniform GlobalBlock\n"
	"{\n"
	"mat4 " TERRAIN_SEMANTIC_VIEW ";\n"
	"mat4 " TERRAIN_SEMANTIC_PROJECTION ";\n"
	"mat4 " TERRAIN_SEMANTIC_VIEWPROJECTION ";\n"
	"mat4 " TERRAIN_SEMANTIC_INVVIEW ";\n"
	"mat4 " TERRAIN_SEMANTIC_INVPROJECTION ";\n"
	"mat4 " TERRAIN_SEMANTIC_INVVIEWPROJECTION ";\n"
	"mat4 " TERRAIN_SEMANTIC_VIEWTOTEXTURESPACE ";\n"
	"vec4 " TERRAIN_SEMANTIC_CAMERAPOSITION ";\n"
	"ivec2 " TERRAIN_SEMANTIC_SCREENSIZE ";\n"
	"vec2 " TERRAIN_SEMANTIC_TIMEANDRANDOM ";\n"
	"uvec2 " TERRAIN_SEMANTIC_LIGHTTILEWORKGROUPS ";\n"
	"};\n"
	"\n";

const char vertexHeader[] =
	"uniform mat4 " TERRAIN_SEMANTIC_MODEL ";\n"
	"uniform mat4 " TERRAIN_SEMANTIC_INVMODEL ";\n";

const char fragmentHeader[] =
	"";
}