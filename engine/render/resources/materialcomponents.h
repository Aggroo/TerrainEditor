#pragma once
#include <string>
#include "json.hpp"

using nlohmann::json;

namespace Components
{

//------------------------------------------------------------------------------
/**
*/
struct Pass
{
	std::string name;
	std::string shader;
};

inline void to_json(json& j, const Pass& p) {
	j = json{ {"name", p.name}, {"shader", p.shader} };
}

inline void from_json(const json& j, Pass& p) {
	j.at("name").get_to(p.name);
	j.at("shader").get_to(p.shader);
};

//------------------------------------------------------------------------------
/**
*/
struct Parameter
{
	std::string name;
	std::string value;
};

inline void to_json(json& j, const Parameter& p) {
	j = json{ {"name", p.name}, {"value", p.value} };
}

inline void from_json(const json& j, Parameter& p) {
	j.at("name").get_to(p.name);
	j.at("value").get_to(p.value);
};

//------------------------------------------------------------------------------
/**
*/
struct Textures
{
	std::string name;
	std::string sampler;
	std::string value;
};

inline void to_json(json& j, const Textures& p) {
	j = json{ {"name", p.name}, {"sampler", p.sampler}, {"value", p.value} };
}

inline void from_json(const json& j, Textures& p) {
	j.at("name").get_to(p.name);
	j.at("sampler").get_to(p.sampler);
	j.at("value").get_to(p.value);
};

//------------------------------------------------------------------------------
/**
*/
struct Materials
{
	std::string name;
	std::string desc;
	std::vector<Pass> pass;
	std::vector<Textures> textures;
	std::vector<Parameter> parameter;
};

inline void to_json(json& j, const Materials& p) {
	j = json{ {"name", p.name}, {"desc", p.desc}, {"Pass", p.pass}, {"Textures", p.textures}, {"Parameters", p.parameter} };
}

inline void from_json(const json& j, Materials& m) {
	j.at("name").get_to(m.name);
	j.at("desc").get_to(m.desc);
	j.at("Pass").get_to(m.pass);
	j.at("Textures").get_to(m.textures);
	j.at("Parameters").get_to(m.parameter);
};

//------------------------------------------------------------------------------
/**
*/
struct Surface
{
	std::string material;
	std::vector<Textures> textures;
	std::vector<Parameter> param;
};

inline void to_json(json& j, const Surface& p) {
	j = json{ {"material", p.material}, {"Textures", p.textures}, {"Parameters", p.param} };
}

inline void from_json(const json& j, Surface& p) {
	j.at("material").get_to(p.material);
	j.at("Textures").get_to(p.textures);
	j.at("Parameters").get_to(p.param);
};

//------------------------------------------------------------------------------
/**
*/
struct Shader 
{
	std::string name;
	std::string desc;
	std::string vertexShader;
	std::string fragmentShader;
	std::string renderState;
};

inline void to_json(json& j, const Shader& s) {
	j = json{ 
		{"name", s.name}, 
		{"desc", s.desc}, 
		{"VertexShader", s.vertexShader},
		{"FragmentShader", s.fragmentShader},
		{"RenderState", s.renderState}
	};
};

inline void from_json(const json& j, Shader& s) {
	j.at("name").get_to(s.name);
	j.at("desc").get_to(s.desc);
	j.at("VertexShader").get_to(s.vertexShader);
	j.at("FragmentShader").get_to(s.fragmentShader);
	j.at("RenderState").get_to(s.renderState);
};

//------------------------------------------------------------------------------
/**
*/
struct ComputeShader
{
	std::string name;
	std::string desc;
	std::string shader;
};

inline void to_json(json& j, const ComputeShader& s) {
	j = json{
		{"name", s.name},
		{"desc", s.desc},
		{"shader", s.shader}
	};
};

inline void from_json(const json& j, ComputeShader& s) {
	j.at("name").get_to(s.name);
	j.at("desc").get_to(s.desc);
	j.at("shader").get_to(s.shader);
};

//------------------------------------------------------------------------------
/**
*/
struct Primitives
{
	int node;
	std::string surface;
};

inline void to_json(json& j, const Primitives& p) {
	j = json{ {"node", p.node}, {"surface", p.surface} };
}

inline void from_json(const json& j, Primitives& p) {
	j.at("node").get_to(p.node);
	j.at("surface").get_to(p.surface);
};

//------------------------------------------------------------------------------
/**
*/
struct Model
{
	std::string mesh;
	std::vector<Primitives> primitives;
};

inline void to_json(json& j, const Model& p) {
	j = json{ {"mesh", p.mesh}, {"primitives", p.primitives} };
}

inline void from_json(const json& j, Model& p) {
	j.at("mesh").get_to(p.mesh);
	j.at("primitives").get_to(p.primitives);
};


}