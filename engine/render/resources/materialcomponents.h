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

void to_json(json& j, const Pass& p) {
	j = json{
		{"name", p.name},
		{"shader", p.shader}
	};
};

void from_json(const json& j, Pass& p) {
	j.at("name").get_to(p.name);
	j.at("shader").get_to(p.shader);
};

//------------------------------------------------------------------------------
/**
*/
struct Parameter
{
	std::string name;
	std::string defaultValue;
};

void to_json(json& j, const Parameter& p) {
	j = json{
		{"name", p.name},
		{"defaultValue", p.defaultValue}
	};
};

void from_json(const json& j, Parameter& p) {
	j.at("name").get_to(p.name);
	j.at("defaultValue").get_to(p.defaultValue);
};

//------------------------------------------------------------------------------
/**
*/
struct Material
{
	std::string name;
	std::string desc;
	std::vector<Pass> pass;
	std::vector<Parameter> parameter;
};

void to_json(json& j, const Material& m) {
	j = json{
		{"name", m.name},
		{"desc", m.desc},
		{"Pass", m.pass},
		{"Parameters", m.parameter}
	};
};

void from_json(const json& j, Material& m) {
	j.at("name").get_to(m.name);
	j.at("desc").get_to(m.desc);
	j.at("Pass").get_to(m.pass);
	j.at("Parameters").get_to(m.parameter);
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

void to_json(json& j, const Shader& s) {
	j = json{ 
		{"name", s.name}, 
		{"desc", s.desc}, 
		{"VertexShader", s.vertexShader},
		{"FragmentShader", s.fragmentShader},
		{"RenderState", s.renderState}
	};
};

void from_json(const json& j, Shader& s) {
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

void to_json(json& j, const ComputeShader& s) {
	j = json{
		{"name", s.name},
		{"desc", s.desc},
		{"shader", s.shader}
	};
};

void from_json(const json& j, ComputeShader& s) {
	j.at("name").get_to(s.name);
	j.at("desc").get_to(s.desc);
	j.at("shader").get_to(s.shader);
};

}