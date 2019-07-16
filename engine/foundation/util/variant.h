#pragma once
//------------------------------------------------------------------------------
/**
@class Util::Variant

An "any type" variable.

Since the Variant class has a rich set of assignment and cast operators,
a variant variable can most of the time be used like a normal C++ variable.

(C) 2006 RadonLabs GmbH.
(C) 2013-2018 Individual contributors, see AUTHORS file
*/
#include "array.h"
#include "foundation/math/vec2.h"
#include "foundation/math/mat4.h"
#include "foundation/util/string.h"
#include "core/refcounted.h"

//------------------------------------------------------------------------------
namespace Util
{
class Variant
{
public:
	/// variant types
	enum Type
	{
		Void,
		Byte,
		Short,
		UShort,
		Int,
		UInt,
		Int64,
		UInt64,
		Float,
		Double,
		Bool,
		vec2,
		vec4,
		quaternions,
		String,
		mat4,
		Object,
		VoidPtr,
		IntArray,
		FloatArray,
		BoolArray,
		vec2Arrayt,
		vec4Arrayt,
		StringArray,
		mat4Arrayt,
		BlobArray,
		GuidArray,
		NumTypes,
	};

	/// default constructor
	Variant();
	/// byte constructor
	Variant(byte rhs);
	/// short constructor
	Variant(short rhs);
	/// ushort constructor
	Variant(ushort rhs);
	/// int constructor
	Variant(int rhs);
	/// uint constructor
	Variant(uint rhs);
	/// int64 constructor
	Variant(int64_t rhs);
	/// uint64 constructor
	Variant(uint64_t rhs);
	/// float constructor
	Variant(float rhs);
	/// double constructor
	Variant(double rhs);
	/// bool constructor
	Variant(bool rhs);
	/// vec2 constructor
	Variant(const Math::vec2& v);
	/// vec4 constructor
	Variant(const Math::vec4& v);
	/// quaternions constructor
	Variant(const Math::quaternions& q);
	/// mat4 constructor
	Variant(const Math::mat4& m);
	/// string constructor
	Variant(const Util::String& rhs);
	/// const char constructor
	Variant(const char* chrPtr);
	/// object constructor
	Variant(Core::RefCounted* ptr);
	/// void pointer constructor
	Variant(void* ptr);
	/// null pointer construction
	Variant(nullptr_t);
	/// int array constructor
	Variant(const Util::Array<int>& rhs);
	/// float array constructor
	Variant(const Util::Array<float>& rhs);
	/// bool array constructor
	Variant(const Util::Array<bool>& rhs);
	/// vec2 array constructor
	Variant(const Util::Array<Math::vec2>& rhs);
	/// vec4 array constructor
	Variant(const Util::Array<Math::vec4>& rhs);
	/// mat4 array constructor
	Variant(const Util::Array<Math::mat4>& rhs);
	/// string array constructor
	Variant(const Util::Array<Util::String>& rhs);
	/// copy constructor
	Variant(const Variant& rhs);

	/// destructor
	~Variant();
	/// set type of attribute
	void SetType(Type t);
	/// get type
	Type GetType() const;
	/// clear content, resets type to void
	void Clear();

	/// assignment operator
	void operator=(const Variant& rhs);
	/// byte assignment operator
	void operator=(byte val);
	/// short assignment operator
	void operator=(short val);
	/// ushort assignment operator
	void operator=(ushort val);
	/// int assignment operator
	void operator=(int val);
	/// uint assignment operator
	void operator=(uint val);
	/// int assignment operator
	void operator=(int64_t val);
	/// uint assignment operator
	void operator=(uint64_t val);
	/// float assignment operator
	void operator=(float val);
	/// double assignment operator
	void operator=(double val);
	/// bool assigment operator
	void operator=(bool val);
	/// vec2 assignment operator
	void operator=(const Math::vec2& val);
	/// vec4 assignment operator
	void operator=(const Math::vec4& val);
	/// quaternions assignment operator
	void operator=(const Math::quaternions& val);
	/// mat4 assignment operator
	void operator=(const Math::mat4& val);
	/// string assignment operator
	void operator=(const Util::String& s);
	/// char pointer assignment
	void operator=(const char* chrPtr);
	/// object assignment
	void operator=(Core::RefCounted* ptr);
	/// object assignment
	void operator=(void* ptr);
	/// int array assignment
	void operator=(const Util::Array<int>& rhs);
	/// float array assignment
	void operator=(const Util::Array<float>& rhs);
	/// bool array assignment
	void operator=(const Util::Array<bool>& rhs);
	/// vec2 array assignment
	void operator=(const Util::Array<Math::vec2>& rhs);
	/// vec4 array assignment
	void operator=(const Util::Array<Math::vec4>& rhs);
	/// mat4 array assignment
	void operator=(const Util::Array<Math::mat4>& rhs);
	/// string array assignment
	void operator=(const Util::Array<Util::String>& rhs);

	/// equality operator
	bool operator==(const Variant& rhs) const;
	/// byte equality operator
	bool operator==(byte rhs) const;
	/// short equality operator
	bool operator==(short rhs) const;
	/// ushort equality operator
	bool operator==(ushort rhs) const;
	/// int equality operator
	bool operator==(int rhs) const;
	/// uint equality operator
	bool operator==(uint rhs) const;
	/// int equality operator
	bool operator==(int64_t rhs) const;
	/// uint equality operator
	bool operator==(uint64_t rhs) const;
	/// float equality operator
	bool operator==(float rhs) const;
	/// double equality operator
	bool operator==(double rhs) const;
	/// bool equality operator
	bool operator==(bool rhs) const;
	/// vec2 equality operator
	bool operator==(const Math::vec2& rhs) const;
	/// vec4 equality operator
	bool operator==(const Math::vec4& rhs) const;
	/// vec4 equality operator
	bool operator==(const Math::quaternions& rhs) const;
	/// string equality operator
	bool operator==(const Util::String& rhs) const;
	/// char ptr equality operator
	bool operator==(const char* chrPtr) const;
	/// pointer equality operator
	bool operator==(Core::RefCounted* ptr) const;
	/// pointer equality operator
	bool operator==(void* ptr) const;

	/// inequality operator
	bool operator!=(const Variant& rhs) const;
	/// byte inequality operator
	bool operator!=(byte rhs) const;
	/// short inequality operator
	bool operator!=(short rhs) const;
	/// ushort inequality operator
	bool operator!=(ushort rhs) const;
	/// int inequality operator
	bool operator!=(int rhs) const;
	/// uint inequality operator
	bool operator!=(uint rhs) const;
	/// int inequality operator
	bool operator!=(int64_t rhs) const;
	/// uint inequality operator
	bool operator!=(uint64_t rhs) const;
	/// float inequality operator
	bool operator!=(float rhs) const;
	/// double inequality operator
	bool operator!=(double rhs) const;
	/// bool inequality operator
	bool operator!=(bool rhs) const;
	/// vec2 inequality operator
	bool operator!=(const Math::vec2& rhs) const;
	/// vec4 inequality operator
	bool operator!=(const Math::vec4& rhs) const;
	/// quaternions inequality operator
	bool operator!=(const Math::quaternions& rhs) const;
	/// string inequality operator
	bool operator!=(const Util::String& rhs) const;
	/// char ptr inequality operator
	bool operator!=(const char* chrPtr) const;
	/// pointer equality operator
	bool operator!=(Core::RefCounted* ptr) const;
	/// pointer equality operator
	bool operator!=(void* ptr) const;

	/// greater operator
	bool operator>(const Variant& rhs) const;
	/// less operator
	bool operator<(const Variant& rhs) const;
	/// greater equal operator
	bool operator>=(const Variant& rhs) const;
	/// less equal operator
	bool operator<=(const Variant& rhs) const;

	/// set byte content
	void SetByte(byte val);
	/// get byte content
	byte GetByte() const;
	/// set short content
	void SetShort(short val);
	/// get short content
	short GetShort() const;
	/// set ushort content
	void SetUShort(ushort val);
	/// get short content
	ushort GetUShort() const;
	/// set integer content
	void SetInt(int val);
	/// get integer content
	int GetInt() const;
	/// set unsigned integer content
	void SetUInt(uint val);
	/// get unsigned integer content
	uint GetUInt() const;
	/// set integer content
	void SetInt64(int64_t val);
	/// get integer content
	int64_t GetInt64() const;
	/// set unsigned integer content
	void SetUInt64(uint64_t val);
	/// get unsigned integer content
	uint64_t GetUInt64() const;
	/// set float content
	void SetFloat(float val);
	/// get float content
	float GetFloat() const;
	/// set double content
	void SetDouble(double val);
	/// get double content
	double GetDouble() const;
	/// set bool content
	void SetBool(bool val);
	/// get bool content
	bool GetBool() const;
	/// set string content
	void SetString(const Util::String& val);
	/// get string content
	const Util::String& GetString() const;
	/// set vec2 content
	void Setvec2(const Math::vec2& val);
	/// get vec2 content
	Math::vec2 Getvec2() const;
	/// set vec4 content
	void Setvec4(const Math::vec4& val);
	/// get vec4 content
	Math::vec4 Getvec4() const;
	/// set quaternions content
	void Setquaternions(const Math::quaternions& val);
	/// get quaternions content
	Math::quaternions Getquaternions() const;
	/// set mat4 content
	void Setmat4(const Math::mat4& val);
	/// get mat4 content
	const Math::mat4& Getmat4() const;
	/// set object pointer
	void SetObject(Core::RefCounted* ptr);
	/// get object pointer
	Core::RefCounted* GetObject() const;
	/// set opaque void pointer
	void SetVoidPtr(void* ptr);
	/// get void pointer
	void* GetVoidPtr() const;
	/// set int array content
	void SetIntArray(const Util::Array<int>& val);
	/// get int array content
	const Util::Array<int>& GetIntArray() const;
	/// set float array content
	void SetFloatArray(const Util::Array<float>& val);
	/// get float array content
	const Util::Array<float>& GetFloatArray() const;
	/// set bool array content
	void SetBoolArray(const Util::Array<bool>& val);
	/// get bool array content
	const Util::Array<bool>& GetBoolArray() const;
	/// set vec2 array content
	void Setvec2Array(const Util::Array<Math::vec2>& val);
	/// get vec2 array content
	const Util::Array<Math::vec2>& Getvec2Array() const;
	/// set vec4 array content
	void Setvec4Array(const Util::Array<Math::vec4>& val);
	/// get vec4 array content
	const Util::Array<Math::vec4>& Getvec4Array() const;
	/// set mat4 array content
	void Setmat4Array(const Util::Array<Math::mat4>& val);
	/// get mat4 array content
	const Util::Array<Math::mat4>& Getmat4Array() const;
	/// set string array content
	void SetStringArray(const Util::Array<Util::String>& val);
	/// get string array content
	const Util::Array<Util::String>& GetStringArray() const;

	/// Templated get method.
	template <typename TYPE>
	TYPE Get() const;

	/// convert value to string
	Util::String ToString() const;
	/// set value from string, if type doesn't match, returns false
	bool SetParseString(const Util::String& string);
	/// get size
	const size_t Size() const;
	/// get pointer to data (use void pointer union)
	const void* AsVoidPtr() const;

	/// create from string
	static Variant FromString(const Util::String& string);
	/// convert type to string
	static Util::String TypeToString(Type t);
	/// convert string to type
	static Type StringToType(const Util::String& str);

private:
	/// delete current content
	void Delete();
	/// copy current content
	void Copy(const Variant& rhs);

	Type type;
	union
	{
		byte i8;
		short i16;
		ushort ui16;
		int i;
		uint u;
		int64_t i64;
		uint64_t u64;
		bool b;
		double d;
		float f[4];
		Math::mat4* m;
		Util::String* string;
		void* voidPtr;
		Core::RefCounted* object;
		Util::Array<int>* intArray;
		Util::Array<float>* floatArray;
		Util::Array<bool>* boolArray;
		Util::Array<Math::vec2>* vec2Array;
		Util::Array<Math::vec4>* vec4Array;
		Util::Array<Math::mat4>* mat4Array;
		Util::Array<Util::String>* stringArray;
	};
};

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant() :
	type(Void),
	string(0)
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::Delete()
{
	if (String == this->type)
	{
		assert(this->string);
		delete(this->string);
		this->string = 0;
	}
	else if (mat4 == this->type)
	{
		assert(this->m);
		delete(this->m);
		this->m = 0;
	}
	else if (Object == this->type)
	{
		if (this->object)
		{
			this->object->Release();
			this->object = 0;
		}
	}
	else if (VoidPtr == this->type)
	{
		this->voidPtr = 0;
	}
	else if (IntArray == this->type)
	{
		assert(this->intArray);
		delete(this->intArray);
		this->intArray = 0;
	}
	else if (FloatArray == this->type)
	{
		assert(this->floatArray);
		delete(this->floatArray);
		this->floatArray = 0;
	}
	else if (BoolArray == this->type)
	{
		assert(this->boolArray);
		delete(this->boolArray);
		this->boolArray = 0;
	}
	else if (vec2Arrayt == this->type)
	{
		assert(this->vec2Array);
		delete(this->vec2Array);
		this->vec2Array = 0;
	}
	else if (vec4Arrayt == this->type)
	{
		assert(this->vec4Array);
		delete(this->vec4Array);
		this->vec4Array = 0;
	}
	else if (mat4Arrayt == this->type)
	{
		assert(this->mat4Array);
		delete(this->mat4Array);
		this->mat4Array = 0;
	}
	else if (StringArray == this->type)
	{
		assert(this->stringArray);
		delete(this->stringArray);
		this->stringArray = 0;
	}
	this->type = Void;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::Clear()
{
	this->Delete();
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::Copy(const Variant& rhs)
{
	assert(Void == this->type);
	this->type = rhs.type;
	switch (rhs.type)
	{
	case Void:
		break;
	case Byte:
		this->i8 = rhs.i8;
		break;
	case Short:
		this->i16 = rhs.i16;
		break;
	case UShort:
		this->ui16 = rhs.ui16;
		break;
	case Int:
		this->i = rhs.i;
		break;
	case UInt:
		this->u = rhs.u;
		break;
	case Int64:
		this->i64 = rhs.i64;
		break;
	case UInt64:
		this->u64 = rhs.u64;
		break;
	case Float:
		this->f[0] = rhs.f[0];
		break;
	case Double:
		this->d = rhs.d;
		break;
	case Bool:
		this->b = rhs.b;
		break;
	case vec2:
		this->f[0] = rhs.f[0];
		this->f[1] = rhs.f[1];
	case vec4:
		this->f[0] = rhs.f[0];
		this->f[1] = rhs.f[1];
		this->f[2] = rhs.f[2];
		this->f[3] = rhs.f[3];
		break;
	case quaternions:
		this->f[0] = rhs.f[0];
		this->f[1] = rhs.f[1];
		this->f[2] = rhs.f[2];
		this->f[3] = rhs.f[3];
		break;
	case String:
		this->string = new Util::String(*rhs.string);
		break;
	case mat4:
		this->m = new Math::mat4(*rhs.m);
		break;
	case Object:
		this->object = rhs.object;
		if (this->object)
		{
			this->object->AddRef();
		}
		break;
	case VoidPtr:
		this->voidPtr = rhs.voidPtr;
		break;
	case IntArray:
		this->intArray = new Util::Array<int>(*rhs.intArray);
		break;
	case FloatArray:
		this->floatArray = new Util::Array<float>(*rhs.floatArray);
		break;
	case BoolArray:
		this->boolArray = new Util::Array<bool>(*rhs.boolArray) ;
		break;
	case vec2Arrayt:
		this->vec2Array = new Util::Array<Math::vec2>(*rhs.vec2Array);
		break;
	case vec4Arrayt:
		this->vec4Array = new Util::Array<Math::vec4>(*rhs.vec4Array);
		break;
	case mat4Arrayt:
		this->mat4Array = new Util::Array<Math::mat4>(*rhs.mat4Array);
		break;
	case StringArray:
		this->stringArray = new Util::Array<Util::String>(*rhs.stringArray);
		break;
	default:
		printf("Variant::Copy(): invalid type!");
		break;
	}
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(const Variant& rhs) :
	type(Void)
{
	this->Copy(rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(byte rhs) :
	type(Byte),
	i8(rhs)
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(short rhs) :
	type(Short),
	i16(rhs)
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(ushort rhs) :
	type(UShort),
	ui16(rhs)
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(int rhs) :
	type(Int),
	i(rhs)
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(uint rhs) :
	type(UInt),
	u(rhs)
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(int64_t rhs) :
	type(Int64),
	i64(rhs)
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(uint64_t rhs) :
	type(UInt64),
	u64(rhs)
{
	// empty
}
//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(float rhs) :
	type(Float)
{
	this->f[0] = rhs;
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(double rhs) :
	type(Double),
	d(rhs)
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(bool rhs) :
	type(Bool),
	b(rhs)
{
	// empty
}


//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(const Math::vec2& rhs) :
	type(vec2)
{
	this->f[0] = rhs.x();
	this->f[1] = rhs.y();
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(const Math::vec4& rhs) :
	type(vec4)
{
	this->f[0] = rhs[0];
	this->f[1] = rhs[1];
	this->f[2] = rhs[2];
	this->f[3] = rhs[3];
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(const Math::quaternions& rhs) :
	type(quaternions)
{
	this->f[0] = rhs.X();
	this->f[1] = rhs.Y();
	this->f[2] = rhs.Z();
	this->f[3] = rhs.W();
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(const Util::String& rhs) :
	type(String)
{
	this->string = new Util::String(rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(const char* chrPtr) :
	type(String)
{
	this->string = new Util::String(chrPtr);
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(Core::RefCounted* ptr) :
	type(Object)
{
	this->object = ptr;
	if (this->object)
	{
		this->object->AddRef();
	}
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(void* ptr) :
	type(VoidPtr)
{
	this->voidPtr = ptr;
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(nullptr_t) :
	type(VoidPtr)
{
	this->voidPtr = nullptr;
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(const Math::mat4& rhs) :
	type(mat4)
{
	this->m = new Math::mat4(rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(const Util::Array<int>& rhs) :
	type(IntArray)
{
	this->intArray = new Util::Array<int>(rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(const Util::Array<float>& rhs) :
	type(FloatArray)
{
	this->floatArray = new Util::Array<float>(rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(const Util::Array<bool>& rhs) :
	type(BoolArray)
{
	this->boolArray = new Util::Array<bool>(rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(const Util::Array<Math::vec2>& rhs) :
	type(vec2Arrayt)
{
	this->vec2Array = new Util::Array<Math::vec2>(rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(const Util::Array<Math::vec4>& rhs) :
	type(vec4Arrayt)
{
	this->vec4Array = new Util::Array<Math::vec4>(rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(const Util::Array<Math::mat4>& rhs) :
	type(mat4Arrayt)
{
	this->mat4Array = new Util::Array<Math::mat4>(rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::Variant(const Util::Array<Util::String>& rhs) :
	type(StringArray)
{
	this->stringArray = new Util::Array<Util::String>(rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline
Variant::~Variant()
{
	this->Delete();
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::SetType(Type t)
{
	this->Delete();
	this->type = t;
	switch (t)
	{
	case String:
		this->string = new(Util::String);
		break;
	case mat4:
		this->m = new(Math::mat4);
		break;
	case Object:
		this->object = 0;
		break;
	case VoidPtr:
		this->voidPtr = 0;
		break;
	case IntArray:
		this->intArray = new(Util::Array<int>);
		break;
	case FloatArray:
		this->floatArray = new(Util::Array<float>);
		break;
	case BoolArray:
		this->boolArray = new(Util::Array<bool>);
		break;
	case vec2Arrayt:
		this->vec2Array = new(Util::Array<Math::vec2>);
		break;
	case vec4Arrayt:
		this->vec4Array = new(Util::Array<Math::vec4>);
		break;
	case mat4Arrayt:
		this->mat4Array = new(Util::Array<Math::mat4>);
		break;
	case StringArray:
		this->stringArray = new(Util::Array<Util::String>);
		break;
	default:
		break;
	}
}

//------------------------------------------------------------------------------
/**
*/
inline Variant::Type
Variant::GetType() const
{
	return this->type;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(const Variant& rhs)
{
	this->Delete();
	this->Copy(rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(byte val)
{
	this->Delete();
	this->type = Byte;
	this->i8 = val;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(short val)
{
	this->Delete();
	this->type = Short;
	this->i16 = val;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(ushort val)
{
	this->Delete();
	this->type = UShort;
	this->ui16 = val;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(int val)
{
	this->Delete();
	this->type = Int;
	this->i = val;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(uint val)
{
	this->Delete();
	this->type = UInt;
	this->u = val;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(int64_t val)
{
	this->Delete();
	this->type = Int64;
	this->i64 = val;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(uint64_t val)
{
	this->Delete();
	this->type = UInt64;
	this->u64 = val;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(float val)
{
	this->Delete();
	this->type = Float;
	this->f[0] = val;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(double val)
{
	this->Delete();
	this->type = Double;
	this->d = val;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(bool val)
{
	this->Delete();
	this->type = Bool;
	this->b = val;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(const Math::vec2& val)
{
	this->Delete();
	this->type = vec2;
	this->f[0] = val.x();
	this->f[1] = val.y();
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(const Math::vec4& val)
{
	this->Delete();
	this->type = vec4;
	this->f[0] = val[0];
	this->f[1] = val[1];
	this->f[2] = val[2];
	this->f[3] = val[3];
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(const Math::quaternions& val)
{
	this->Delete();
	this->type = quaternions;
	this->f[0] = val.X();
	this->f[1] = val.Y();
	this->f[2] = val.Z();
	this->f[3] = val.W();
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(const Util::String& s)
{
	if (String == this->type)
	{
		*this->string = s;
	}
	else
	{
		this->Delete();
		this->string = new Util::String(s);
	}
	this->type = String;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(const char* chrPtr)
{
	*this = Util::String(chrPtr);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(const Math::mat4& val)
{
	if (mat4 == this->type)
	{
		*this->m = val;
	}
	else
	{
		this->Delete();
		this->m = new Math::mat4(val);
	}
	this->type = mat4;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(Core::RefCounted* ptr)
{
	this->Delete();
	this->type = Object;
	this->object = ptr;
	if (this->object)
	{
		this->object->AddRef();
	}
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(void* ptr)
{
	this->Delete();
	this->type = VoidPtr;
	this->voidPtr = ptr;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(const Util::Array<int>& val)
{
	if (IntArray == this->type)
	{
		*this->intArray = val;
	}
	else
	{
		this->Delete();
		this->intArray = new Util::Array<int>(val);
	}
	this->type = IntArray;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(const Util::Array<float>& val)
{
	if (FloatArray == this->type)
	{
		*this->floatArray = val;
	}
	else
	{
		this->Delete();
		this->floatArray = new Util::Array<float>(val);
	}
	this->type = FloatArray;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(const Util::Array<bool>& val)
{
	if (BoolArray == this->type)
	{
		*this->boolArray = val;
	}
	else
	{
		this->Delete();
		this->boolArray = new Util::Array<bool>(val);
	}
	this->type = BoolArray;
}


//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(const Util::Array<Math::vec2>& rhs)
{
	if (vec2Arrayt == this->type)
	{
		*this->vec2Array = rhs;
	}
	else
	{
		this->Delete();
		this->vec2Array = new Util::Array<Math::vec2>(rhs);
	}
	this->type = vec2Arrayt;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(const Util::Array<Math::vec4>& val)
{
	if (vec4Arrayt == this->type)
	{
		*this->vec4Array = val;
	}
	else
	{
		this->Delete();
		this->vec4Array = new Util::Array<Math::vec4>(val);
	}
	this->type = vec4Arrayt;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(const Util::Array<Math::mat4>& val)
{
	if (mat4Arrayt == this->type)
	{
		*this->mat4Array = val;
	}
	else
	{
		this->Delete();
		this->mat4Array = new Util::Array<Math::mat4>(val);
	}
	this->type = mat4Arrayt;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::operator=(const Util::Array<Util::String>& val)
{
	if (StringArray == this->type)
	{
		*this->stringArray = val;
	}
	else
	{
		this->Delete();
		this->stringArray = new Util::Array<Util::String>(val);
	}
	this->type = StringArray;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator==(const Variant& rhs) const
{
	if (rhs.type == this->type)
	{
		switch (rhs.type)
		{
		case Void:
			return true;
		case Byte:
			return (this->i8 == rhs.i8);
		case Short:
			return (this->i16 == rhs.i16);
		case UShort:
			return (this->ui16 == rhs.ui16);
		case Int:
			return (this->i == rhs.i);
		case UInt:
			return (this->u == rhs.u);
		case Int64:
			return (this->i64 == rhs.i64);
		case UInt64:
			return (this->u64 == rhs.u64);
		case Bool:
			return (this->b == rhs.b);
		case Float:
			return (this->f[0] == rhs.f[0]);
		case Double:
			return (this->d == rhs.d);
		case String:
			return ((*this->string) == (*rhs.string));
		case vec2:
			return ((this->f[0] == rhs.f[0]) && (this->f[1] == rhs.f[1]));
		case vec4:
			return ((this->f[0] == rhs.f[0]) &&
				(this->f[1] == rhs.f[1]) &&
				(this->f[2] == rhs.f[2]) &&
				(this->f[3] == rhs.f[3]));
		case quaternions:
			return ((this->f[0] == rhs.f[0]) &&
				(this->f[1] == rhs.f[1]) &&
				(this->f[2] == rhs.f[2]) &&
				(this->f[3] == rhs.f[3]));
		case Object:
			return (this->object == rhs.object);
		case VoidPtr:
			return (this->voidPtr == rhs.voidPtr);
		case mat4:
			return ((*this->m) == (*rhs.m));
		default:
			printf("Variant::operator==(): invalid variant type!");
			return false;
		}
	}
	return false;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator>(const Variant& rhs) const
{
	if (rhs.type == this->type)
	{
		switch (rhs.type)
		{
		case Void:
			return true;
		case Byte:
			return (this->i8 > rhs.i8);
		case Short:
			return (this->i16 > rhs.i16);
		case UShort:
			return (this->ui16 > rhs.ui16);
		case Int:
			return (this->i > rhs.i);
		case UInt:
			return (this->u > rhs.u);
		case Int64:
			return (this->i64 > rhs.i64);
		case UInt64:
			return (this->u64 > rhs.u64);
		case Bool:
			return (this->b > rhs.b);
		case Float:
			return (this->f[0] > rhs.f[0]);
		case Double:
			return (this->d > rhs.d);
		case String:
			return ((*this->string) > (*rhs.string));
		case vec2:
			return ((this->f[0] > rhs.f[0]) && (this->f[1] > rhs.f[1]));
		case vec4:
			return ((this->f[0] > rhs.f[0]) &&
				(this->f[1] > rhs.f[1]) &&
				(this->f[2] > rhs.f[2]) &&
				(this->f[3] > rhs.f[3]));
		case quaternions:
			return ((this->f[0] > rhs.f[0]) &&
				(this->f[1] > rhs.f[1]) &&
				(this->f[2] > rhs.f[2]) &&
				(this->f[3] > rhs.f[3]));
		case Object:
			return (this->object > rhs.object);
		case VoidPtr:
			return (this->voidPtr > rhs.voidPtr);
		default:
			printf("Variant::operator>(): invalid variant type!");
			return false;
		}
	}
	return false;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator<(const Variant& rhs) const
{
	if (rhs.type == this->type)
	{
		switch (rhs.type)
		{
		case Void:
			return true;
		case Byte:
			return (this->i8 < rhs.i8);
		case Short:
			return (this->i16 < rhs.i16);
		case UShort:
			return (this->ui16 < rhs.ui16);
		case Int:
			return (this->i < rhs.i);
		case UInt:
			return (this->u < rhs.u);
		case Int64:
			return (this->i64 < rhs.i64);
		case UInt64:
			return (this->u64 < rhs.u64);
		case Bool:
			return (this->b < rhs.b);
		case Float:
			return (this->f[0] < rhs.f[0]);
		case Double:
			return (this->d > rhs.d);
		case String:
			return ((*this->string) < (*rhs.string));
		case vec2:
			return ((this->f[0] < rhs.f[0]) && (this->f[1] < rhs.f[1]));
		case vec4:
			return ((this->f[0] < rhs.f[0]) &&
				(this->f[1] < rhs.f[1]) &&
				(this->f[2] < rhs.f[2]) &&
				(this->f[3] < rhs.f[3]));
		case quaternions:
			return ((this->f[0] < rhs.f[0]) &&
				(this->f[1] < rhs.f[1]) &&
				(this->f[2] < rhs.f[2]) &&
				(this->f[3] < rhs.f[3]));
		case Object:
			return (this->object < rhs.object);
		case VoidPtr:
			return (this->voidPtr < rhs.voidPtr);
		default:
			printf("Variant::operator<(): invalid variant type!");
			return false;
		}
	}
	return false;
}


//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator>=(const Variant& rhs) const
{
	if (rhs.type == this->type)
	{
		switch (rhs.type)
		{
		case Void:
			return true;
		case Byte:
			return (this->i8 >= rhs.i8);
		case Short:
			return (this->i16 >= rhs.i16);
		case UShort:
			return (this->ui16 >= rhs.ui16);
		case Int:
			return (this->i >= rhs.i);
		case UInt:
			return (this->u >= rhs.u);
		case Int64:
			return (this->i64 >= rhs.i64);
		case UInt64:
			return (this->u64 >= rhs.u64);
		case Bool:
			return (this->b >= rhs.b);
		case Float:
			return (this->f[0] >= rhs.f[0]);
		case Double:
			return (this->d >= rhs.d);
		case String:
			return ((*this->string) >= (*rhs.string));
		case vec2:
			return ((this->f[0] >= rhs.f[0]) && (this->f[1] >= rhs.f[1]));
		case vec4:
			return ((this->f[0] >= rhs.f[0]) &&
				(this->f[1] >= rhs.f[1]) &&
				(this->f[2] >= rhs.f[2]) &&
				(this->f[3] >= rhs.f[3]));
		case quaternions:
			return ((this->f[0] >= rhs.f[0]) &&
				(this->f[1] >= rhs.f[1]) &&
				(this->f[2] >= rhs.f[2]) &&
				(this->f[3] >= rhs.f[3]));
		case Object:
			return (this->object >= rhs.object);
		case VoidPtr:
			return (this->voidPtr >= rhs.voidPtr);
		default:
			printf("Variant::operator>(): invalid variant type!");
			return false;
		}
	}
	return false;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator<=(const Variant& rhs) const
{
	if (rhs.type == this->type)
	{
		switch (rhs.type)
		{
		case Void:
			return true;
		case Byte:
			return (this->i8 <= rhs.i8);
		case Short:
			return (this->i16 <= rhs.i16);
		case UShort:
			return (this->ui16 <= rhs.ui16);
		case Int:
			return (this->i <= rhs.i);
		case UInt:
			return (this->u <= rhs.u);
		case Int64:
			return (this->i64 <= rhs.i64);
		case UInt64:
			return (this->u64 <= rhs.u64);
		case Bool:
			return (this->b <= rhs.b);
		case Float:
			return (this->f[0] <= rhs.f[0]);
		case Double:
			return (this->d >= rhs.d);
		case String:
			return ((*this->string) <= (*rhs.string));
		case vec2:
			return ((this->f[0] <= rhs.f[0]) && (this->f[1] <= rhs.f[1]));
		case vec4:
			return ((this->f[0] <= rhs.f[0]) &&
				(this->f[1] <= rhs.f[1]) &&
				(this->f[2] <= rhs.f[2]) &&
				(this->f[3] <= rhs.f[3]));
		case quaternions:
			return ((this->f[0] <= rhs.f[0]) &&
				(this->f[1] <= rhs.f[1]) &&
				(this->f[2] <= rhs.f[2]) &&
				(this->f[3] <= rhs.f[3]));
		case Object:
			return (this->object <= rhs.object);
		case VoidPtr:
			return (this->voidPtr <= rhs.voidPtr);
		default:
			printf("Variant::operator<(): invalid variant type!");
			return false;
		}
	}
	return false;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator!=(const Variant& rhs) const
{
	return !(*this == rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator==(byte rhs) const
{
	assert(Byte == this->type);
	return (this->i8 == rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator==(short rhs) const
{
	assert(Short == this->type);
	return (this->i16 == rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator==(ushort rhs) const
{
	assert(UShort == this->type);
	return (this->ui16 == rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator==(int rhs) const
{
	assert(Int == this->type);
	return (this->i == rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator==(uint rhs) const
{
	assert(UInt == this->type);
	return (this->u == rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator==(int64_t rhs) const
{
	assert(Int64 == this->type);
	return (this->i64 == rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator==(uint64_t rhs) const
{
	assert(UInt64 == this->type);
	return (this->u64 == rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator==(float rhs) const
{
	assert(Float == this->type);
	return (this->f[0] == rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator==(double rhs) const
{
	assert(Double == this->type);
	return (this->d == rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator==(bool rhs) const
{
	assert(Bool == this->type);
	return (this->b == rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator==(const Util::String& rhs) const
{
	assert(String == this->type);
	return ((*this->string) == rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator==(const char* chrPtr) const
{
	return *this == Util::String(chrPtr);
}


//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator==(const Math::vec2& rhs) const
{
	assert(vec2 == this->type);
	return (this->f[0] == rhs.x() && this->f[1] == rhs.y());
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator==(const Math::vec4& rhs) const
{
	assert(vec4 == this->type);
	return ((this->f[0] == rhs.x()) &&
		(this->f[1] == rhs.y()) &&
		(this->f[2] == rhs.z()) &&
		(this->f[3] == rhs.w()));
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator==(const Math::quaternions& rhs) const
{
	assert(quaternions == this->type);
	return ((this->f[0] == rhs.X()) &&
		(this->f[1] == rhs.Y()) &&
		(this->f[2] == rhs.Z()) &&
		(this->f[3] == rhs.W()));
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator==(Core::RefCounted* ptr) const
{
	assert(Object == this->type);
	return this->object == ptr;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator==(void* ptr) const
{
	assert(VoidPtr == this->type);
	return this->voidPtr == ptr;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator!=(byte rhs) const
{
	assert(Byte == this->type);
	return (this->i8 != rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator!=(short rhs) const
{
	assert(Short == this->type);
	return (this->i16 != rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator!=(ushort rhs) const
{
	assert(UShort == this->type);
	return (this->ui16 != rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator!=(int rhs) const
{
	assert(Int == this->type);
	return (this->i != rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator!=(uint rhs) const
{
	assert(UInt == this->type);
	return (this->u != rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator!=(int64_t rhs) const
{
	assert(Int64 == this->type);
	return (this->i64 != rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator!=(uint64_t rhs) const
{
	assert(UInt64 == this->type);
	return (this->u64 != rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator!=(float rhs) const
{
	assert(Float == this->type);
	return (this->f[0] != rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator!=(double rhs) const
{
	assert(Double == this->type);
	return (this->d != rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator!=(bool rhs) const
{
	assert(Bool == this->type);
	return (this->b != rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator!=(const Util::String& rhs) const
{
	assert(String == this->type);
	return (*this->string) != rhs;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator!=(const char* chrPtr) const
{
	return *this != Util::String(chrPtr);
}


//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator!=(const Math::vec2& rhs) const
{
	assert(vec2 == this->type);
	return (this->f[0] != rhs.x() || this->f[1] != rhs.y());
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator!=(const Math::vec4& rhs) const
{
	assert(vec4 == this->type);
	return ((this->f[0] != rhs.x()) ||
		(this->f[1] != rhs.y()) ||
		(this->f[2] != rhs.z()) ||
		(this->f[3] != rhs.w()));
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator!=(const Math::quaternions& rhs) const
{
	assert(quaternions == this->type);
	return ((this->f[0] != rhs.X()) ||
		(this->f[1] != rhs.Y()) ||
		(this->f[2] != rhs.Z()) ||
		(this->f[3] != rhs.W()));
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator!=(Core::RefCounted* ptr) const
{
	assert(Object == this->type);
	return (this->object == ptr);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::operator!=(void* ptr) const
{
	assert(VoidPtr == this->type);
	return (this->voidPtr == ptr);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::SetByte(byte val)
{
	*this = val;
}

//------------------------------------------------------------------------------
/**
*/
inline byte
Variant::GetByte() const
{
	assert(Byte == this->type);
	return this->i8;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::SetShort(short val)
{
	*this = val;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::SetUShort(ushort val)
{
	*this = val;
}

//------------------------------------------------------------------------------
/**
*/
inline short
Variant::GetShort() const
{
	assert(Short == this->type);
	return this->i16;
}

//------------------------------------------------------------------------------
/**
*/
inline ushort
Variant::GetUShort() const
{
	assert(UShort == this->type);
	return this->ui16;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::SetInt(int val)
{
	*this = val;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::SetUInt(uint val)
{
	*this = val;
}

//------------------------------------------------------------------------------
/**
*/
inline int
Variant::GetInt() const
{
	assert(Int == this->type);
	return this->i;
}

//------------------------------------------------------------------------------
/**
*/
inline uint
Variant::GetUInt() const
{
	assert(UInt == this->type);
	return this->u;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::SetInt64(int64_t val)
{
	assert(Int64 == this->type);
	this->i64 = val;
}

//------------------------------------------------------------------------------
/**
*/
inline int64_t
Variant::GetInt64() const
{
	assert(Int64 == this->type);
	return this->i64;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::SetUInt64(uint64_t val)
{
	assert(UInt64 == this->type);
	this->u64 = val;
}

//------------------------------------------------------------------------------
/**
*/
inline uint64_t
Variant::GetUInt64() const
{
	assert(UInt64 == this->type);
	return this->u64;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::SetFloat(float val)
{
	*this = val;
}

//------------------------------------------------------------------------------
/**
*/
inline float
Variant::GetFloat() const
{
	assert(Float == this->type);
	return this->f[0];
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::SetDouble(double val)
{
	*this = val;
}

//------------------------------------------------------------------------------
/**
*/
inline double
Variant::GetDouble() const
{
	assert(Double == this->type);
	return this->d;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::SetBool(bool val)
{
	*this = val;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Variant::GetBool() const
{
	assert(Bool == this->type);
	return this->b;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::SetString(const Util::String& val)
{
	*this = val;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::String&
Variant::GetString() const
{
	assert(String == this->type);
	return *(this->string);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::Setvec2(const Math::vec2& val)
{
	*this = val;
}

//------------------------------------------------------------------------------
/**
*/
inline Math::vec2
Variant::Getvec2() const
{
	assert(vec2 == this->type);
	return Math::vec2(this->f[0], this->f[1]);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::Setvec4(const Math::vec4& val)
{
	*this = val;
}

//------------------------------------------------------------------------------
/**
*/
inline Math::vec4
Variant::Getvec4() const
{
	assert(vec4 == this->type);
	return Math::vec4(this->f[0], this->f[1], this->f[2], this->f[3]);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::Setquaternions(const Math::quaternions& val)
{
	*this = val;
}

//------------------------------------------------------------------------------
/**
*/
inline Math::quaternions
Variant::Getquaternions() const
{
	assert(quaternions == this->type);
	return Math::quaternions(this->f[0], this->f[1], this->f[2], this->f[3]);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::Setmat4(const Math::mat4& val)
{
	*this = val;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::mat4&
Variant::Getmat4() const
{
	assert(mat4 == this->type);
	return *(this->m);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::SetObject(Core::RefCounted* ptr)
{
	*this = ptr;
}

//------------------------------------------------------------------------------
/**
*/
inline Core::RefCounted*
Variant::GetObject() const
{
	assert(Object == this->type);
	return this->object;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Util::Variant::SetVoidPtr(void* ptr)
{
	*this = ptr;
}

//------------------------------------------------------------------------------
/**
*/
inline void*
Util::Variant::GetVoidPtr() const
{
	assert(VoidPtr == this->type);
	return this->voidPtr;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::SetIntArray(const Util::Array<int>& val)
{
	*this = val;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<int>&
Variant::GetIntArray() const
{
	assert(IntArray == this->type);
	return *(this->intArray);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::SetFloatArray(const Util::Array<float>& val)
{
	*this = val;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<float>&
Variant::GetFloatArray() const
{
	assert(FloatArray == this->type);
	return *(this->floatArray);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::SetBoolArray(const Util::Array<bool>& val)
{
	*this = val;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<bool>&
Variant::GetBoolArray() const
{
	assert(BoolArray == this->type);
	return *(this->boolArray);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::Setvec2Array(const Util::Array<Math::vec2>& val)
{
	*this = val;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Math::vec2>&
Variant::Getvec2Array() const
{
	assert(vec2Arrayt == this->type);
	return *(this->vec2Array);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::Setvec4Array(const Util::Array<Math::vec4>& val)
{
	*this = val;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Math::vec4>&
Variant::Getvec4Array() const
{
	assert(vec4Arrayt == this->type);
	return *(this->vec4Array);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::Setmat4Array(const Util::Array<Math::mat4>& val)
{
	*this = val;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Math::mat4>&
Variant::Getmat4Array() const
{
	assert(mat4Arrayt == this->type);
	return *(this->mat4Array);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Variant::SetStringArray(const Util::Array<Util::String>& val)
{
	*this = val;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Util::String>&
Variant::GetStringArray() const
{
	assert(StringArray == this->type);
	return *(this->stringArray);
}

//------------------------------------------------------------------------------
/**
*/
template <typename TYPE>
inline TYPE
Variant::Get() const
{
	static_assert(true, "Get method for TYPE is not implemented!");
	TYPE ret;
	return ret;
}

//------------------------------------------------------------------------------
/**
*/
template <>
inline byte
Variant::Get() const
{
	return this->GetByte();
}

//------------------------------------------------------------------------------
/**
*/
template <>
inline short
Variant::Get() const
{
	return this->GetShort();
}

//------------------------------------------------------------------------------
/**
*/
template <>
inline int
Variant::Get() const
{
	return this->GetInt();
}

//------------------------------------------------------------------------------
/**
*/
template <>
inline uint
Variant::Get() const
{
	return this->GetUInt();
}

//------------------------------------------------------------------------------
/**
*/
template <>
inline int64_t
Variant::Get() const
{
	return this->GetInt64();
}

//------------------------------------------------------------------------------
/**
*/
template <>
inline uint64_t
Variant::Get() const
{
	return this->GetUInt64();
}

//------------------------------------------------------------------------------
/**
*/
template <>
inline bool
Variant::Get() const
{
	return this->GetBool();
}

//------------------------------------------------------------------------------
/**
*/
template <>
inline double
Variant::Get() const
{
	return this->GetDouble();
}

//------------------------------------------------------------------------------
/**
*/
template <>
inline float
Variant::Get() const
{
	return this->GetFloat();
}

//------------------------------------------------------------------------------
/**
*/
template <>
inline Math::vec2
Variant::Get() const
{
	return this->Getvec2();
}

//------------------------------------------------------------------------------
/**
*/
template <>
inline Math::vec4
Variant::Get() const
{
	return this->Getvec4();
}

//------------------------------------------------------------------------------
/**
*/
template <>
inline Math::quaternions
Variant::Get() const
{
	return this->Getquaternions();
}

//------------------------------------------------------------------------------
/**
*/
template <>
inline Math::mat4
Variant::Get() const
{
	return this->Getmat4();
}

//------------------------------------------------------------------------------
/**
*/
template <>
inline Util::String
Variant::Get() const
{
	return this->GetString();
}

//------------------------------------------------------------------------------
/**
*/
template <>
inline Util::Array<int>
Variant::Get() const
{
	return this->GetIntArray();
}

//------------------------------------------------------------------------------
/**
*/
template <>
inline Util::Array<float>
Variant::Get() const
{
	return this->GetFloatArray();
}

//------------------------------------------------------------------------------
/**
*/
template <>
inline Util::Array<bool>
Variant::Get() const
{
	return this->GetBoolArray();
}

//------------------------------------------------------------------------------
/**
*/
template <>
inline Util::Array<Math::vec2>
Variant::Get() const
{
	return this->Getvec2Array();
}

//------------------------------------------------------------------------------
/**
*/
template <>
inline Util::Array<Math::vec4>
Variant::Get() const
{
	return this->Getvec4Array();
}

//------------------------------------------------------------------------------
/**
*/
template <>
inline Util::Array<Math::mat4>
Variant::Get() const
{
	return this->Getmat4Array();
}

//------------------------------------------------------------------------------
/**
*/
template <>
inline Util::Array<Util::String>
Variant::Get() const
{
	return this->GetStringArray();
}

//------------------------------------------------------------------------------
/**
Todo: Handle array types.
Note:
Doesn't handle vec2, vec4 and mat4 on purpose, since they are indistinguishable from float arrays
*/
inline Util::String
Variant::ToString() const
{
	Util::String retval = "";
	switch (this->type)
	{
	case Void:          break;
	case Byte: { retval = Util::String::FromByte(this->GetByte()); break; }
	case Short: { retval = Util::String::FromShort(this->GetShort()); break; }
	case UShort: { retval = Util::String::FromUShort(this->GetUShort()); break; }
	case Int: { retval = Util::String::FromInt(this->GetInt()); break; }
	case UInt: { retval = Util::String::FromUInt(this->GetUInt()); break; }
	case Float: { retval = Util::String::FromFloat(this->GetFloat()); break; }
	case Double: { retval = Util::String::FromDouble(this->GetDouble()); break; }
	case Bool: { retval = Util::String::FromBool(this->GetBool()); break; }
	case vec2: { retval = Util::String::FromVec2(this->Getvec2()); break; }
	case vec4: { retval = Util::String::FromVec4(this->Getvec4()); break; }
	case quaternions: { retval = Util::String::FromQuaternion(this->Getquaternions()); break; }
	case String: { retval = this->GetString(); break; }
	case mat4: { retval = Util::String::FromMat4(this->Getmat4()); break; }
	case IntArray: { const Util::Array<int>& arr = this->GetIntArray(); for (int i = 0; i < arr.Size(); i++) { retval.AppendInt(arr[i]); retval += (i < arr.Size() - 1 ? "," : ""); } break; }
	case FloatArray: { const Util::Array<float>& arr = this->GetFloatArray(); for (int i = 0; i < arr.Size(); i++) { retval.AppendFloat(arr[i]); retval += (i < arr.Size() - 1 ? "," : ""); } break; }
	case BoolArray: { const Util::Array<bool>& arr = this->GetBoolArray(); for (int i = 0; i < arr.Size(); i++) { retval.AppendBool(arr[i]); retval += (i < arr.Size() - 1 ? "," : ""); } break; }
	case StringArray: { const Util::Array<Util::String>& arr = this->GetStringArray(); for (int i = 0; i < arr.Size(); i++) { retval.Append(arr[i]); retval += (i < arr.Size() - 1 ? "," : ""); } break; }
	default:
		printf("Variant::ToString(): invalid type enum '%d'", this->type);

	}
	return retval;
}

//------------------------------------------------------------------------------
/**
Note:
Will treat vec2, vec4 and mat4 as float arrays
*/
inline bool
Util::Variant::SetParseString(const Util::String& string)
{
	bool retval = false;
	switch (this->type)
	{
	case Int:
	case UInt: { if (string.IsValidInt()) { this->SetUInt(string.AsInt()); retval = true; break; } }
	case Float: { if (string.IsValidFloat()) { this->SetFloat(string.AsFloat()); retval = true; break; } }
	case Bool: { if (string.IsValidBool()) { this->SetBool(string.AsBool()); retval = true; break; } }
	case vec2: { if (string.IsValidVec2()) { this->Setvec2(string.AsVec2()); retval = true; break; } }
	case vec4: { if (string.IsValidVec4()) { this->Setvec4(string.AsVec4()); retval = true; break; } }
	case String: {                                    this->SetString(string); retval = true; break; }
	case mat4: { if (string.IsValidMat4()) { this->Setmat4(string.AsMat4()); retval = true; break; } }

	case IntArray:
	{
		Util::Array<Util::String> tokens = string.Tokenize(",");
		Util::Array<int> result;
		index_t i;
		for (i = 0; i < tokens.Size(); i++)
		{
			if (tokens[i].IsValidInt())
			{
				result.Append(tokens[i].AsInt());
			}
		}
		this->SetIntArray(result);
		retval = true;
		break;
	}
	case FloatArray:
	{
		Util::Array<Util::String> tokens = string.Tokenize(",");
		Util::Array<float> result;
		index_t i;
		for (i = 0; i < tokens.Size(); i++)
		{
			if (tokens[i].IsValidFloat())
			{
				result.Append(tokens[i].AsFloat());
			}
		}
		this->SetFloatArray(result);
		retval = true;
		break;
	}
	case BoolArray:
	{
		Util::Array<Util::String> tokens = string.Tokenize(",");
		Util::Array<bool> result;
		index_t i;
		for (i = 0; i < tokens.Size(); i++)
		{
			if (tokens[i].IsValidBool())
			{
				result.Append(tokens[i].AsBool());
			}
		}
		this->SetBoolArray(result);
		retval = true;
		break;
	}
	case StringArray:
	{
		Util::Array<Util::String> tokens = string.Tokenize(",");
		this->SetStringArray(tokens);
		retval = true;
		break;
	}
	}
	return retval;
}

//------------------------------------------------------------------------------
/**
*/
inline
const size_t
Variant::Size() const
{
	switch (this->type)
	{
	case Void:          return 0;
	case Byte:          return sizeof(uint8);
	case Short:         return sizeof(uint16);
	case UShort:        return sizeof(uint16);
	case Int:           return sizeof(uint32);
	case UInt:          return sizeof(uint32);
	case Int64:         return sizeof(uint64);
	case UInt64:        return sizeof(uint64);
	case Float:         return sizeof(float);
	case Double:        return sizeof(double);
	case Bool:          return sizeof(bool);
	case vec2:		return sizeof(float) * 2;
	case vec4:        return sizeof(float) * 4;
	case quaternions:    return sizeof(float) * 4;
	case String:        return sizeof(void*);
	case mat4:      return sizeof(void*);
	case Object:        return sizeof(void*);
	case VoidPtr:       return sizeof(void*);
	case IntArray:      return sizeof(void*);
	case FloatArray:    return sizeof(void*);
	case BoolArray:     return sizeof(void*);
	case vec2Arrayt:	return sizeof(void*);
	case vec4Arrayt:   return sizeof(void*);
	case mat4Arrayt: return sizeof(void*);
	case StringArray:   return sizeof(void*);
	case GuidArray:     return sizeof(void*);
	case BlobArray:     return sizeof(void*);
	default:
		printf("Variant::Size(): invalid type enum '%d'!", type);
		return 0;
	}
}

//------------------------------------------------------------------------------
/**
*/
inline
const void*
Variant::AsVoidPtr() const
{
	return &this->i8;
}

//------------------------------------------------------------------------------
/**
Only some types can be inferred from string, everything else will be treated as a raw string
*/
inline Variant
Variant::FromString(const Util::String& string)
{
	Variant val;
	if (string.IsValidInt())       val.SetInt(string.AsInt());
	else if (string.IsValidFloat())     val.SetFloat(string.AsFloat());
	//else if (string.IsValidvec2())    val.Setvec2(string.Asvec2());
	else if (string.IsValidVec4())    val.Setvec4(string.AsVec4());
	else if (string.IsValidBool())      val.SetBool(string.AsBool());
	else if (string.IsValidMat4())  val.Setmat4(string.AsMat4());
	else                                val.SetString(string);                  // raw string
	return val;
}

//------------------------------------------------------------------------------
/**
*/
inline Util::String
Variant::TypeToString(Type t)
{
	switch (t)
	{
	case Void:          return "void";
	case Byte:          return "byte";
	case Short:         return "short";
	case UShort:        return "ushort";
	case Int:           return "int";
	case UInt:          return "uint";
	case Int64:         return "int64";
	case UInt64:        return "uint64";
	case Float:         return "float";
	case Double:        return "double";
	case Bool:          return "bool";
	case vec2:		return "vec2";
	case vec4:        return "vec4";
	case quaternions:    return "quaternions";
	case String:        return "string";
	case mat4:      return "mat4";
	case Object:        return "object";
	case VoidPtr:       return "voidptr";
	case IntArray:      return "intarray";
	case FloatArray:    return "floatarray";
	case BoolArray:     return "boolarray";
	case vec2Arrayt:	return "vec2array";
	case vec4Arrayt:   return "vec4array";
	case mat4Arrayt: return "mat4array";
	case StringArray:   return "stringarray";
	case GuidArray:     return "guidarray";
	case BlobArray:     return "blobarray";
	default:
		printf("Variant::TypeToString(): invalid type enum '%d'!", t);
		return 0;
	}
}

//------------------------------------------------------------------------------
/**
*/
inline Variant::Type
Variant::StringToType(const Util::String& str)
{
	if ("void" == str)             return Void;
	else if ("byte" == str)             return Byte;
	else if ("short" == str)            return Short;
	else if ("ushort" == str)           return UShort;
	else if ("int" == str)              return Int;
	else if ("uint" == str)             return UInt;
	else if ("int64" == str)            return Int64;
	else if ("uint64" == str)           return UInt64;
	else if ("float" == str)            return Float;
	else if ("double" == str)           return Double;
	else if ("bool" == str)             return Bool;
	else if ("vec2" == str)			return vec2;
	else if ("vec4" == str)           return vec4;
	else if ("color" == str)            return vec4; // NOT A BUG!
	else if ("quaternions" == str)       return quaternions;
	else if ("string" == str)           return String;
	else if ("mat4" == str)         return mat4;
	else if ("object" == str)           return Object;
	else if ("voidptr" == str)          return VoidPtr;
	else if ("intarray" == str)         return IntArray;
	else if ("floatarray" == str)       return FloatArray;
	else if ("boolarray" == str)        return BoolArray;
	else if ("vec2array" == str)		return vec2Arrayt;
	else if ("vec4array" == str)      return vec4Arrayt;
	else if ("mat4array" == str)    return mat4Arrayt;
	else if ("stringarray" == str)      return StringArray;
	else if ("guidarray" == str)        return GuidArray;
	else if ("blobarray" == str)        return BlobArray;
	else
	{
		printf("Variant::StringToType(): invalid type string '%s'!", str.AsCharPtr());
		return Void;
	}
}


} // namespace Util
  //------------------------------------------------------------------------------
