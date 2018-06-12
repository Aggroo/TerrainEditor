#pragma once
#include <any>

namespace Render
{
enum class ParameterTypes
{
	Void,
	Int,
	UInt,
	Float,
	Bool,
	Vector2,
	Vector3,
	Vector4,
	String,
	Matrix4,
	VoidPtr,
	IntArray,
	FloatArray,
	BoolArray,
	Vector2Array,
	Vector3Array,
	Vector4Array,
	StringArray,
	Matrix4Array,
};

//TODO: Set variable and save which type it is. This is used in a map later to get parameters. Also create functions to return the variable value based on the type
class MaterialParameter
{
public:
	MaterialParameter();
	~MaterialParameter();

	ParameterTypes GetType();
private:

	ParameterTypes type;
	std::any variable;
};
}
