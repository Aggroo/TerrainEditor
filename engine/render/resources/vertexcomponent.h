#pragma once
#include "config.h"
#include "foundation/util/string.h"

namespace Render
{

enum VertexComponentBits
{
	Coord = (1 << 0),      // 3 floats
	Normal = (1 << 1),      // 3 floats
	NormalB4N = (1 << 2),      // 4 unsigned bytes, normalized
	Uv0 = (1 << 3),      // 2 floats
	Uv0S2 = (1 << 4),      // 2 shorts, 4.12 fixed point
	Uv1 = (1 << 5),      // 2 floats
	Uv1S2 = (1 << 6),      // 2 shorts, 4.12 fixed point
	Uv2 = (1 << 7),      // 2 floats
	Uv2S2 = (1 << 8),      // 2 shorts, 4.12 fixed point
	Uv3 = (1 << 9),      // 2 floats
	Uv3S2 = (1 << 10),     // 2 shorts, 4.12 fixed point
	Color = (1 << 11),     // 4 floats
	ColorUB4N = (1 << 12),     // 4 unsigned bytes, normalized
	Tangent = (1 << 13),     // 3 floats
	TangentB4N = (1 << 14),     // 4 unsigned bytes, normalized
	Binormal = (1 << 15),     // 3 floats
	BinormalB4N = (1 << 16),     // 4 unsigned bytes, normalized
	Weights = (1 << 17),     // 4 floats
	WeightsUB4N = (1 << 18),     // 4 unsigned bytes, normalized
	JIndices = (1 << 19),     // 4 floats
	JIndicesUB4 = (1 << 20),     // 4 unsigned bytes

	NumVertexComponents = 21,
	AllComponents = ((1 << NumVertexComponents) - 1),
};

class VertexComponent
{
public:

	enum SemanticName
	{
		Position = 0,
		Normal = 1,
		TexCoord1 = 2,
		Tangent = 3,
		Binormal = 4,
		Color = 5,
		TexCoord2 = 6,
		SkinWeights = 7,
		SkinJIndices = 8,

		Invalid,
	};

	enum Format
	{
		Float,      //> one-component float, expanded to (float, 0, 0, 1)
		Float2,     //> two-component float, expanded to (float, float, 0, 1)
		Float3,     //> three-component float, expanded to (float, float, float, 1)
		Float4,     //> four-component float
		UByte4,     //> four-component unsigned byte
		Byte4,		//> four-component signed byte
		Short2,     //> two-component signed short, expanded to (value, value, 0, 1)
		Short4,     //> four-component signed short
		UByte4N,    //> four-component normalized unsigned byte (value / 255.0f)
		Byte4N,		//> four-component normalized signed byte (value / 127.0f)
		Short2N,    //> two-component normalized signed short (value / 32767.0f)
		Short4N		//> four-component normalized signed short (value / 32767.0f)
	};

	/// stride type tells if the compoent should be per-instance or per-vertex
	enum StrideType
	{
		PerVertex,
		PerInstance
	};

    /// default constructor
    VertexComponent();
    /// constructor
    VertexComponent(SemanticName semName, uint semIndex, Format format, uint streamIndex = 0, StrideType strideType = PerVertex, size_t stride = 0);
    /// compares semantic names so that we can assure order of vertexcomponents
    bool operator<(const VertexComponent& rhs) const;
    /// get semantic name
    SemanticName GetSemanticName() const;
    /// get semantic index
    uint GetSemanticIndex() const;
    /// get vertex component format
    Format GetFormat() const;
    /// get stream index
    uint GetStreamIndex() const;
    /// get the byte size of the vertex component
    size_t GetByteSize() const;
    /// get a unique signature of the vertex component
    Util::String GetSignature() const;
    /// get stride type
    StrideType GetStrideType() const;
    /// get stride between instances
    size_t GetStride() const;
    /// convert string to semantic name
    static SemanticName StringToSemanticName(const  Util::String& str);
    /// convert semantic name to string
    static  Util::String SemanticNameToString(SemanticName n);
    /// convert string to format
    static Format StringToFormat(const  Util::String& str);
    /// convert format to string
    static Util::String FormatToString(Format f);
    /// get the byte offset of this component (only valid when part of a VertexLayout)
    uint GetByteOffset() const;



private:

    SemanticName semName;
    uint semIndex;
    Format format;
    StrideType strideType;
    size_t stride;
    uint streamIndex;

};


//------------------------------------------------------------------------------
/**
*/
inline
VertexComponent::VertexComponent() :
    semName(Invalid),
    semIndex(0),
    format(Float),
    streamIndex(0),
    strideType(PerVertex),
    stride(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
VertexComponent::VertexComponent(SemanticName semName_, uint semIndex_, Format format_, uint streamIndex_, StrideType strideType_, size_t stride_) :
    semName(semName_),
    semIndex(semIndex_),
    format(format_),
    streamIndex(streamIndex_),
    strideType(strideType_),
    stride(stride_)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline bool
VertexComponent::operator<(const VertexComponent& rhs) const
{
    return this->semName < rhs.semName;
}


//------------------------------------------------------------------------------
/**
*/
inline VertexComponent::SemanticName
VertexComponent::GetSemanticName() const
{
    return this->semName;
}

//------------------------------------------------------------------------------
/**
*/
inline uint
VertexComponent::GetSemanticIndex() const
{
    return this->semIndex;
}

//------------------------------------------------------------------------------
/**
*/
inline VertexComponent::Format
VertexComponent::GetFormat() const
{
    return this->format;
}

//------------------------------------------------------------------------------
/**
*/
inline uint
VertexComponent::GetStreamIndex() const
{
    return this->streamIndex;
}

//------------------------------------------------------------------------------
/**
*/
inline VertexComponent::StrideType
VertexComponent::GetStrideType() const
{
    return this->strideType;
}

//------------------------------------------------------------------------------
/**
*/
inline size_t
VertexComponent::GetStride() const
{
    return this->stride;
}

//------------------------------------------------------------------------------
/**
*/
inline size_t
VertexComponent::GetByteSize() const
{
    switch (this->format)
    {
    case Float:     return 4;
    case Float2:    return 8;
    case Float3:    return 12;
    case Float4:    return 16;
    case UByte4:    return 4;
    case Byte4:		return 4;
    case Short2:    return 4;
    case Short4:    return 8;
    case UByte4N:   return 4;
    case Byte4N:	return 4;
    case Short2N:   return 4;
    case Short4N:   return 8;

    }
    //cant happen
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
inline Util::String
VertexComponent::SemanticNameToString(SemanticName n)
{
    switch (n)
    {
    case Position:      return "Position";
    case Normal:        return "Normal";
    case Tangent:       return "Tangent";
    case Binormal:      return "Binormal";
    case TexCoord1:     return "TexCoord";
    case Color:         return "Color";
    case SkinWeights:   return "SkinWeights";
    case SkinJIndices:  return "SkinJIndices";
    default:
        std::printf("VertexComponent::SemanticNameToString(): invalid SemanticName code!");
        return "";
    }
}

//------------------------------------------------------------------------------
/**
*/
inline VertexComponent::SemanticName
VertexComponent::StringToSemanticName(const Util::String& str)
{
    if (str == "Position") return Position;
    else if (str == "Normal") return Normal;
    else if (str == "Tangent") return Tangent;
    else if (str == "Binormal") return Binormal;
    else if (str == "TexCoord") return TexCoord1;
    else if (str == "Color") return Color;
    else if (str == "SkinWeights") return SkinWeights;
    else if (str == "SkinJIndices") return SkinJIndices;
    else
    {
        //std::printf("VertexComponent::StringToSemanticName(): invalid string '%s'!", str);
        return Invalid;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline  Util::String
VertexComponent::FormatToString(Format f)
{
    switch (f)
    {
    case Float:     return "Float";
    case Float2:    return "Float2";
    case Float3:    return "Float3";
    case Float4:    return "Float4";
    case UByte4:    return "UByte4";
    case Short2:    return "Short2";
    case Short4:    return "Short4";
    case UByte4N:   return "UByte4N";
    case Short2N:   return "Short2N";
    case Short4N:   return "Short4N";

    default:
        std::printf("VertexComponent::FormatToString(): invalid Format code!");
        return "";
    }
}

//------------------------------------------------------------------------------
/**
*/
inline VertexComponent::Format
VertexComponent::StringToFormat(const  Util::String& str)
{
    if (str == "Float") return Float;
    else if (str == "Float2") return Float2;
    else if (str == "Float3") return Float3;
    else if (str == "Float4") return Float4;
    else if (str == "UByte4") return UByte4;
    else if (str == "Short2") return Short2;
    else if (str == "Short4") return Short4;
    else if (str == "UByte4N") return UByte4N;
    else if (str == "Short2N") return Short2N;
    else if (str == "Short4N") return Short4N;
    else
    {
        //std::printf("VertexComponent::StringToFormat(): invalid string '%s'!\n", str);
        return Float;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline  Util::String
VertexComponent::GetSignature() const
{
    Util::String str;
    switch (this->semName)
    {
    case Position:      str = "pos"; break;
    case Normal:        str = "nrm"; break;
    case Tangent:       str = "tan"; break;
    case Binormal:      str = "bin"; break;
    case TexCoord1:     str = "tex"; break;
    case Color:         str = "clr"; break;
    case SkinWeights:   str = "skw"; break;
    case SkinJIndices:  str = "sji"; break;
    default:
        break;
    }
    //str.append(this->semIndex);    
    //str.append(FormatToString(this->format));
    return str;
}

}