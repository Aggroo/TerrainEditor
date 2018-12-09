#pragma once
#include "array.h"
#include "foundation/math/vec3.h"
#include "foundation/math/mat4.h"
#include "string.h"
#include <minwindef.h>

namespace Util
{
class Variance
{
public:

	enum Type
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

	Variance();
	~Variance();

	Variance(const Type& type, const Util::String& value);
	Variance(int value);
	Variance(uint value);
	Variance(float value);
	Variance(bool value);
	Variance(const Math::vec4& value);
	Variance(const Math::mat4& value);
	Variance(const Util::String& value);
	Variance(void* value);
	Variance(const Util::Array<int>& value);
	Variance(const Util::Array<float>& value);
	Variance(const Util::Array<bool>& value);
	Variance(const Util::Array<Math::vec4>& value);
	Variance(const Util::Array<Math::mat4>& value);
	Variance(const Util::Array<Util::String>& value);
	Variance(const Variance& value);

	static Variance::Type StringToType(const Util::String& str);

	//Set the type of attribute
	void SetType(Type t);
	//Get type
	Type GetType() const;

	//Clear content, sets to void type / clears the value
	void Clear();

	//Operator overloads
	void operator=(const Variance& rhs);
	void operator=(int val);
	void operator=(uint val);
	void operator=(float val);
	void operator=(bool val);
	void operator=(const Math::vec4& val);
	void operator=(const Math::mat4& val);
	void operator=(const Util::String& val);
	void operator=(void* ptr);
	void operator=(const Util::Array<int>& rhs);
	void operator=(const Util::Array<float>& rhs);
	void operator=(const Util::Array<bool>& rhs);
	void operator=(const Util::Array<Math::vec4>& rhs);
	void operator=(const Util::Array<Math::mat4>& rhs);
	void operator=(const Util::Array<Util::String>& rhs);

	bool operator==(const Variance& rhs) const;
	bool operator==(int val) const;
	bool operator==(uint val) const;
	bool operator==(float val) const;
	bool operator==(bool val) const;
	bool operator==(const Math::vec4& val) ;
	bool operator==(const Math::mat4& val) const;
	bool operator==(const Util::String& val) const;
	bool operator==(void* ptr) const;

	bool operator!=(const Variance& rhs) const;
	bool operator!=(int val) const;
	bool operator!=(uint val) const;
	bool operator!=(float val) const;
	bool operator!=(bool val) const;
	bool operator!=(const Math::vec4& val);
	bool operator!=(const Math::mat4& val) const;
	bool operator!=(const Util::String& val) const;
	bool operator!=(void* ptr) const;

	bool operator<(const Variance& rhs);
	bool operator>(const Variance& rhs);
	bool operator<=(const Variance& rhs);
	bool operator>=(const Variance& rhs);

private:

	void Delete();

	void Copy(const Variance& rhs);

	Type type;

	union
	{
		int i;
		uint ui;
		bool b;
		float f[4];
		Math::mat4* m;
		Util::String* string;
		void* voidPtr;
		Util::Array<int>* intArray;
		Util::Array<float>* floatArray;
		Util::Array<bool>* boolArray;
		Util::Array<Math::vec4>* vector4Array;
		Util::Array<Math::mat4>* matrix4Array;
		Util::Array<Util::String>* stringArray;

	};
};

inline Variance::Variance() : type(Void), string(0)
{
}

inline Variance::~Variance()
{
	this->Delete();
}

inline Variance::Variance(const Type& type, const Util::String& value) : type(type)
{
	switch (type)
	{
	case String: this->string = new Util::String(value); return;
	case Int:
		this->i = value.AsInt();
		return;
	case Float:
		this->f[0] = value.AsFloat();
		return;
	case Bool:
		this->b = value.AsBool();
		return;
	case Matrix4: 
		this->m = new Math::mat4(value.AsMat4());
		return;
	case Vector2: 
		float vec2[2];
		sscanf(value.AsCharPtr(), "%f %f", &vec2[0], &vec2[1]);
		this->f[0] = vec2[0]; this->f[1] = vec2[1];
		return;
	case Vector3:
		float vec3[3];
		sscanf(value.AsCharPtr(), "%f %f %f", &vec3[0], &vec3[1], &vec3[2]);
		this->f[0] = vec3[0]; this->f[1] = vec3[1]; this->f[2] = vec3[2];
		return;
	case Vector4:
		float vec4[3];
		sscanf(value.AsCharPtr(), "%f %f %f %f", &vec4[0], &vec4[1], &vec4[2], &vec4[3]);
		this->f[0] = vec4[0]; this->f[1] = vec4[1]; this->f[2] = vec4[2]; this->f[3] = vec4[3];
		return;
	default:
		printf("ERROR: Variable constructor not implemented\n");
		assert(false);
		return;
	}
}

inline Variance::Variance(int value) : type(Int), i(value)
{
}

inline Variance::Variance(uint value) : type(UInt), ui(value)
{
}

inline Variance::Variance(float value) : type(Float)
{
	this->f[0] = value;
}

inline Variance::Variance(bool value) : type(Bool), b(value)
{
}

inline Variance::Variance(const Math::vec4& value) : type(Vector3)
{
	this->f[0] = value[0];
	this->f[1] = value[1];
	this->f[2] = value[2];
	this->f[3] = value[3];
}

inline Variance::Variance(const Math::mat4& value) : type(Matrix4)
{
	if (this->m != nullptr)
		delete this->m;

	this->m = new Math::mat4(value);
}

inline Variance::Variance(const Util::String& value) : type(String)
{
	if (this->string != nullptr)
		delete this->string;

	this->string = new Util::String(value);
}

inline Variance::Variance(void* value) : type(VoidPtr)
{
	this->voidPtr = value;
}

inline Variance::Variance(const Util::Array<int>& value) : type(IntArray)
{
	if (this->intArray != nullptr)
		delete this->intArray;

	this->intArray = new Util::Array<int>(value);
}

inline Variance::Variance(const Util::Array<float>& value) : type(FloatArray)
{
	if (this->floatArray != nullptr)
		delete this->floatArray;

	this->floatArray = new Util::Array<float>(value);
}

inline Variance::Variance(const Util::Array<bool>& value) : type(BoolArray)
{
	if (this->boolArray != nullptr)
		delete this->boolArray;

	this->boolArray = new Util::Array<bool>(value);
}

inline Variance::Variance(const Util::Array<Math::vec4>& value) : type(Vector4Array)
{
	if (this->vector4Array != nullptr)
		delete this->vector4Array;

	this->vector4Array = new Util::Array<Math::vec4>(value);
}

inline Variance::Variance(const Util::Array<Math::mat4>& value) : type(Matrix4Array)
{
	if (this->matrix4Array != nullptr)
		delete this->m;

	this->matrix4Array = new Util::Array<Math::mat4>(value);
}

inline Variance::Variance(const Util::Array<Util::String>& value) : type(StringArray)
{
	if (this->stringArray!= nullptr)
		delete this->stringArray;

	this->stringArray = new Util::Array<Util::String>(value);

}

inline Variance::Type Variance::StringToType(const Util::String& str)
{
	if (str == "string")
	{
		return String;
	}
	else if (str == "void")
	{
		return Void;
	}
	else if (str == "int")
	{
		return Int;
	}
	else if (str == "uint")
	{
		return UInt;
	}
	else if (str == "float")
	{
		return Float;
	}
	else if (str == "bool")
	{
		return Bool;
	}
	else if (str == "vector2")
	{
		return Vector2;
	}
	else if (str == "vector3")
	{
		return Vector3;
	}
	else if (str == "vector4")
	{
		return Vector4;
	}
	else if (str == "matrix4")
	{
		return Matrix4;
	}
	else if (str == "voidPtr")
	{
		return VoidPtr;
	}
	else if (str == "intArray")
	{
		return IntArray;
	}
	else if (str == "floatArray")
	{
		return FloatArray;
	}
	else if (str == "boolArray")
	{
		return BoolArray;
	}
	else if (str == "vector2Array")
	{
		return Vector2Array;
	}
	else if (str == "vector3Array")
	{
		return Vector3Array;
	}
	else if (str == "vector4Array")
	{
		return Vector4Array;
	}
	else if (str == "stringArray")
	{
		return StringArray;
	}
	else if (str == "matrix4Array")
	{
		return Matrix4Array;
	}
	else
	{
		printf("ERROR: No such variable!");
		assert(false);
	}

	return Void;
}

inline void Variance::SetType(Type t)
{
	this->Delete();
	this->type = t;
	switch (t)
	{
	case String:
		this->string = new Util::String();
		break;
	case Matrix4:
		this->m = new Math::mat4();
		break;
	case VoidPtr:
		this->voidPtr = 0;
		break;
	case IntArray:
		this->intArray = new Util::Array<int>();
		break;
	case FloatArray:
		this->floatArray = new Util::Array<float>();
		break;
	case BoolArray:
		this->boolArray = new Util::Array<bool>();
		break;
	case Vector4Array:
		this->vector4Array = new Util::Array<Math::vec4>();
		break;
	case Matrix4Array:
		this->matrix4Array = new Util::Array<Math::mat4>();
		break;
	case StringArray:
		this->stringArray = new Util::Array<Util::String>();
		break;
	default:
		break;
	}
}

inline Variance::Type Variance::GetType() const
{
	return this->type;
}

inline void Variance::Clear()
{
	this->Delete();
}

inline void Variance::Delete()
{
	switch (this->type)
	{
	case String: 
		delete this->string; 
		this->string = 0; 
		break;
	case Matrix4: 
		delete this->m; 
		this->m = 0; 
		break;
	case VoidPtr: 
		delete this->voidPtr; 
		this->voidPtr = 0; 
		break;
	case IntArray: 
		delete this->intArray; 
		this->intArray = 0; 
		break;
	case FloatArray: 
		delete this->floatArray; 
		this->floatArray = 0; 
		break;
	case BoolArray: 
		delete this->boolArray; 
		this->boolArray = 0; 
		break;
	case Vector4Array: 
		delete this->vector4Array; 
		this->vector4Array = 0; 
		break;
	case Matrix4Array: 
		delete this->matrix4Array; 
		this->matrix4Array = 0; 
		break;
	case StringArray: 
		delete this->stringArray; 
		this->stringArray = 0; 
		break;
	case Void: break;
	default: break;
	}
	this->type = Void;
}

inline void Variance::Copy(const Variance& rhs)
{
	assert(Void == this->type);
	this->type = rhs.type;
	switch (rhs.type)
	{
	case Void:
		break;
	case Int:
		this->i = rhs.i;
		break;
	case UInt:
		this->ui = rhs.ui;
		break;
	case Float:
		this->f[0] = rhs.f[0];
		break;
	case Bool:
		this->b = rhs.b;
		break;
	case Vector2:
		this->f[0] = rhs.f[0];
		this->f[1] = rhs.f[1];
	case Vector3:
		this->f[0] = rhs.f[0];
		this->f[1] = rhs.f[1];
		this->f[2] = rhs.f[2];
		break;
	case Vector4:
		this->f[0] = rhs.f[0];
		this->f[1] = rhs.f[1];
		this->f[2] = rhs.f[2];
		this->f[3] = rhs.f[3];
		break;
	case String:
		this->Delete();
		this->string = new Util::String(*rhs.string);
		break;
	case Matrix4:
		this->Delete();
		this->m = new Math::mat4(*rhs.m);
		break;
	case VoidPtr:
		this->voidPtr = rhs.voidPtr;
		break;
	case IntArray:
		this->Delete();
		this->intArray = new Util::Array<int>(*rhs.intArray);
		break;
	case FloatArray:
		this->Delete();
		this->floatArray = new Util::Array<float>(*rhs.floatArray);
		break;
	case BoolArray:
		this->Delete();
		this->boolArray = new Util::Array<bool>(*rhs.boolArray);
		break;
	case Vector4Array:
		this->Delete();
		this->vector4Array = new Util::Array<Math::vec4>(*rhs.vector4Array);
		break;
	case Matrix4Array:
		this->Delete();
		this->matrix4Array = new Util::Array<Math::mat4>(*rhs.matrix4Array);
		break;
	case StringArray:
		this->Delete();
		this->stringArray = new Util::Array<Util::String>(*rhs.stringArray);
		break;
	default:
		//invalid type!
		assert(false);
		break;
	}
}

inline void Variance::operator=(const Variance& rhs)
{
	this->Delete();
	this->Copy(rhs);
}

inline void Variance::operator=(int val)
{
	this->Delete();
	this->type = Int;
	this->i = val;
}

inline void Variance::operator=(uint val)
{
	this->Delete();
	this->type = UInt;
	this->ui = val;
}

inline void Variance::operator=(float val)
{
	this->Delete();
	this->type = Float;
	this->f[0] = val;
}

inline void Variance::operator=(bool val)
{
	this->Delete();
	this->type = Bool;
	this->b = val;
}

inline void Variance::operator=(const Math::vec4& val)
{
	this->Delete();
	this->type = Vector4;
	this->f[0] = val[0];
	this->f[1] = val[1];
	this->f[2] = val[2];
	this->f[3] = val[3];
}

inline void Variance::operator=(const Math::mat4& val)
{
	if(this->type == Matrix4)
	{
		*this->m = val;
	}
	else
	{
		this->Delete();		
		this->m = new Math::mat4(val);
	}
	this->type = Matrix4;
}

inline void Variance::operator=(const Util::String& val)
{
	if (this->type == String)
	{
		*this->string = val;
	}
	else
	{
		this->Delete();
		this->string = new Util::String(val);
	}
	this->type = String;
}

inline void Variance::operator=(void* ptr)
{
	this->Delete();
	this->type = VoidPtr;
	this->voidPtr = ptr;
}

inline void Variance::operator=(const Util::Array<int>& rhs)
{
	if (this->type == IntArray)
	{
		*this->intArray = rhs;
	}
	else
	{
		this->Delete();
		this->intArray = new Util::Array<int>(rhs);
	}
	this->type = IntArray;
}

inline void Variance::operator=(const Util::Array<float>& rhs)
{
	if (this->type == FloatArray)
	{
		*this->floatArray = rhs;
	}
	else
	{
		this->Delete();
		this->floatArray = new Util::Array<float>(rhs);
	}
	this->type = FloatArray;
}

inline void Variance::operator=(const Util::Array<bool>& rhs)
{
	if (this->type == BoolArray)
	{
		*this->boolArray = rhs;
	}
	else
	{
		this->Delete();
		this->boolArray = new Util::Array<bool>(rhs);
	}
	this->type = BoolArray;
}

inline void Variance::operator=(const Util::Array<Math::vec4>& rhs)
{
	if (this->type == Vector4Array)
	{
		*this->vector4Array = rhs;
	}
	else
	{
		this->Delete();
		this->vector4Array = new Util::Array<Math::vec4>(rhs);
	}
	this->type = Vector4Array;
}

inline void Variance::operator=(const Util::Array<Math::mat4>& rhs)
{
	if (this->type == Matrix4Array)
	{
		*this->matrix4Array = rhs;
	}
	else
	{
		this->Delete();
		this->matrix4Array = new Util::Array<Math::mat4>(rhs);
	}
	this->type = Matrix4Array;
}

inline void Variance::operator=(const Util::Array<Util::String>& rhs)
{
	if (this->type == StringArray)
	{
		*this->stringArray = rhs;
	}
	else
	{
		this->Delete();
		this->stringArray = new Util::Array<Util::String>(rhs);
	}
	this->type = StringArray;
}

inline bool Variance::operator==(const Variance& rhs) const
{
	if (rhs.type == this->type)
	{
		switch (rhs.type)
		{
		case Void:
			return true;
		case Int:
			return (this->i == rhs.i);
		case UInt:
			return (this->ui == rhs.ui);
		case Bool:
			return (this->b == rhs.b);
		case Float:
			return (this->f[0] == rhs.f[0]);
		case String:
			return ((*this->string) == (*rhs.string));
		case Vector2:
			return ((this->f[0] == rhs.f[0]) && (this->f[1] == rhs.f[1]));
		case Vector3:
			return ((this->f[0] == rhs.f[0]) && (this->f[1] == rhs.f[1]) && (this->f[2] == rhs.f[2]));
		case Vector4:
			return ((this->f[0] == rhs.f[0]) &&
				(this->f[1] == rhs.f[1]) &&
				(this->f[2] == rhs.f[2]) &&
				(this->f[3] == rhs.f[3]));
		case VoidPtr:
			return (this->voidPtr == rhs.voidPtr);
		case Matrix4:
			return ((*this->m) == (*rhs.m));
		default:
			//invalid variant type!
			assert(false);

			return false;
		}
	}
	return false;
}

inline bool Variance::operator==(int val) const
{
	assert(Int == this->type);
	return (this->i == val);
}

inline bool Variance::operator==(uint val) const
{
	assert(UInt == this->type);
	return (this->ui == val);
}

inline bool Variance::operator==(float val) const
{
	assert(Float == this->type);
	return (this->f[0] == val);
}

inline bool Variance::operator==(bool val) const
{
	assert(Bool == this->type);
	return (this->b == val);
}

inline bool Variance::operator==(const Math::vec4& val)
{
	assert(Vector4 == this->type);
	return (this->f[0] == val[0] && this->f[1] == val[1] && this->f[2] == val[2] && this->f[3] == val[3]);
}

inline bool Variance::operator==(const Math::mat4& val) const
{
	assert(Matrix4 == this->type);
	return (*this->m == val);
}

inline bool Variance::operator==(const Util::String& val) const
{
	assert(String == this->type);
	return (*this->string == val);
}

inline bool Variance::operator==(void* ptr) const
{
	assert(VoidPtr == this->type);
	return (this->voidPtr == ptr);
}

inline bool Variance::operator!=(const Variance& rhs) const
{
	return !(*this == rhs);
}

inline bool Variance::operator!=(int val) const
{
	assert(Int == this->type);
	return (this->i != val);
}

inline bool Variance::operator!=(uint val) const
{
	assert(UInt == this->type);
	return (this->ui != val);
}

inline bool Variance::operator!=(float val) const
{
	assert(Float == this->type);
	return (this->f[0] != val);
}

inline bool Variance::operator!=(bool val) const
{
	assert(Bool == this->type);
	return (this->b != val);
}

inline bool Variance::operator!=(const Math::vec4& val)
{
	assert(Vector4 == this->type);
	return ((this->f[0] != val[0]) ||
			(this->f[1] != val[1]) ||
			(this->f[2] != val[2]) ||
			(this->f[3] != val[3]));
}

inline bool Variance::operator!=(const Math::mat4& val) const
{
	assert(Matrix4 == this->type);
	return (*this->m != val);
}

inline bool Variance::operator!=(const Util::String& val) const
{
	assert(String == this->type);
	return (*this->string) != val;
}

inline bool Variance::operator!=(void* ptr) const
{
	assert(VoidPtr == this->type);
	return this->voidPtr != ptr;
}

inline bool Variance::operator<(const Variance& rhs)
{
	if (rhs.type == this->type)
	{
		switch (rhs.type)
		{
		case Void:
			return true;
		case Int:
			return (this->i < rhs.i);
		case UInt:
			return (this->ui < rhs.ui);
		case Bool:
			return (this->b < rhs.b);
		case Float:
			return (this->f[0] < rhs.f[0]);
		case String:
			return ((*this->string) < (*rhs.string));
		case Vector2:
			return ((this->f[0] < rhs.f[0]) && (this->f[1] < rhs.f[1]));
		case Vector3:
			return ((this->f[0] < rhs.f[0]) && (this->f[1] < rhs.f[1]) && (this->f[2] < rhs.f[2]));
		case Vector4:
			return ((this->f[0] < rhs.f[0]) &&
				(this->f[1] < rhs.f[1]) &&
				(this->f[2] < rhs.f[2]) &&
				(this->f[3] < rhs.f[3]));
		case VoidPtr:
			return (this->voidPtr < rhs.voidPtr);
		default:
			assert(false);
			return false;
		}
	}
	return false;
}

inline bool Variance::operator>(const Variance& rhs)
{
	if (rhs.type == this->type)
	{
		switch (rhs.type)
		{
		case Void:
			return true;
		case Int:
			return (this->i > rhs.i);
		case UInt:
			return (this->ui > rhs.ui);
		case Bool:
			return (this->b > rhs.b);
		case Float:
			return (this->f[0] > rhs.f[0]);
		case String:
			return ((*this->string) > (*rhs.string));
		case Vector2:
			return ((this->f[0] > rhs.f[0]) && (this->f[1] > rhs.f[1]));
		case Vector3:
			return ((this->f[0] > rhs.f[0]) && (this->f[1] > rhs.f[1]) && (this->f[2] > rhs.f[2]));
		case Vector4:
			return ((this->f[0] > rhs.f[0]) &&
				(this->f[1] > rhs.f[1]) &&
				(this->f[2] > rhs.f[2]) &&
				(this->f[3] > rhs.f[3]));
		case VoidPtr:
			return (this->voidPtr > rhs.voidPtr);
		default:
			assert(false);
			return false;
		}
	}
	return false;
}

inline bool Variance::operator<=(const Variance& rhs)
{
	if (rhs.type == this->type)
	{
		switch (rhs.type)
		{
		case Void:
			return true;
		case Int:
			return (this->i <= rhs.i);
		case UInt:
			return (this->ui <= rhs.ui);
		case Bool:
			return (this->b <= rhs.b);
		case Float:
			return (this->f[0] <= rhs.f[0]);
		case String:
			return ((*this->string) <= (*rhs.string));
		case Vector2:
			return ((this->f[0] <= rhs.f[0]) && (this->f[1] <= rhs.f[1]));
		case Vector3:
			return ((this->f[0] <= rhs.f[0]) && (this->f[1] <= rhs.f[1]) && (this->f[2] <= rhs.f[2]));
		case Vector4:
			return ((this->f[0] <= rhs.f[0]) &&
				(this->f[1] <= rhs.f[1]) &&
				(this->f[2] <= rhs.f[2]) &&
				(this->f[3] <= rhs.f[3]));
		case VoidPtr:
			return (this->voidPtr <= rhs.voidPtr);
		default:
			assert(false);
			return false;
		}
	}
	return false;
}

inline bool Variance::operator>=(const Variance& rhs)
{
	if (rhs.type == this->type)
	{
		switch (rhs.type)
		{
		case Void:
			return true;
		case Int:
			return (this->i >= rhs.i);
		case UInt:
			return (this->ui >= rhs.ui);
		case Bool:
			return (this->b >= rhs.b);
		case Float:
			return (this->f[0] >= rhs.f[0]);
		case String:
			return ((*this->string) >= (*rhs.string));
		case Vector2:
			return ((this->f[0] >= rhs.f[0]) && (this->f[1] >= rhs.f[1]));
		case Vector3:
			return ((this->f[0] >= rhs.f[0]) && (this->f[1] >= rhs.f[1]) && (this->f[2] >= rhs.f[2]));
		case Vector4:
			return ((this->f[0] >= rhs.f[0]) &&
				(this->f[1] >= rhs.f[1]) &&
				(this->f[2] >= rhs.f[2]) &&
				(this->f[3] >= rhs.f[3]));
		case VoidPtr:
			return (this->voidPtr >= rhs.voidPtr);
		default:
			assert(false);
			return false;
		}
	}
	return false;
}
}
