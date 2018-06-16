#pragma once
//------------------------------------------------------------------------------
/**
    @class Util::FourCC
    
    A four-character-code is a quasi-human-readable 32-bit-id.
    
    @copyright	See LICENCE file
*/
#include "core/types.h"
#include "string.h"
#include "core/debug.h"

//------------------------------------------------------------------------------
namespace Util
{
class FourCC
{
public:
    /// default constructor
    FourCC();
    /// construct from 32-bit-value (e.g. FourCC('ABCD'))
    FourCC(uint f);
    /// construct from string
    FourCC(const String& s);
    /// equality operator
    bool operator==(const FourCC& rhs) const;
    /// inequality operator
    bool operator!=(const FourCC& rhs) const;
    /// less-then operator
    bool operator<(const FourCC& rhs) const;
    /// less-or-equal operator
    bool operator<=(const FourCC& rhs) const;
    /// greater-then operator
    bool operator>(const FourCC& rhs) const;
    /// greater-or-equal operator
    bool operator>=(const FourCC& rhs) const;
    /// return true if valid
    bool IsValid() const;
    /// set from 32-bit-value
    void SetFromUInt(uint f);
    /// get as 32-bit-value
    uint AsUInt() const;
    /// set as string
    void SetFromString(const String& s);
    /// get as string
    String AsString() const;
    /// convert fourcc to string
    static String ToString(const FourCC& f);
    /// convert string to fourcc
    static FourCC FromString(const String& s);
	/// return a 32-bit hash code for the string
	index_t HashCode() const;

private:
    uint fourCC;
};

//------------------------------------------------------------------------------
/**
*/
inline
FourCC::FourCC() :
    fourCC(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
FourCC::FourCC(uint f) :
    fourCC(f)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
FourCC::FourCC(const String& s)
{
    this->SetFromString(s);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
FourCC::operator==(const FourCC& rhs) const
{
    return (this->fourCC == rhs.fourCC);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
FourCC::operator!=(const FourCC& rhs) const
{
    return (this->fourCC != rhs.fourCC);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
FourCC::operator>(const FourCC& rhs) const
{
    return (this->fourCC > rhs.fourCC);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
FourCC::operator>=(const FourCC& rhs) const
{
    return (this->fourCC >= rhs.fourCC);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
FourCC::operator<(const FourCC& rhs) const
{
    return (this->fourCC < rhs.fourCC);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
FourCC::operator<=(const FourCC& rhs) const
{
    return (this->fourCC <= rhs.fourCC);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
FourCC::IsValid() const
{
    return (0 != this->fourCC);
}

//------------------------------------------------------------------------------
/**
*/
inline void
FourCC::SetFromUInt(uint f)
{
    _assert(0 != f);
    this->fourCC = f;
}

//------------------------------------------------------------------------------
/**
*/
inline uint
FourCC::AsUInt() const
{
    return this->fourCC;
}

//------------------------------------------------------------------------------
/**
*/
inline void
FourCC::SetFromString(const String& s)
{
    *this = FromString(s);
}

//------------------------------------------------------------------------------
/**
*/
inline String
FourCC::AsString() const
{
    return ToString(*this);
}

//------------------------------------------------------------------------------
/**
*/
inline String
FourCC::ToString(const FourCC& f)
{
    _assert(f.IsValid());
    String str("xzyw");
    str[0] = (char) ((f.fourCC & 0xFF000000) >> 24);
    str[1] = (char) ((f.fourCC & 0x00FF0000) >> 16);
    str[2] = (char) ((f.fourCC & 0x0000FF00) >> 8);
    str[3] = (char) (f.fourCC & 0x000000FF);
    return str;
}

//------------------------------------------------------------------------------
/**
*/
inline FourCC
FourCC::FromString(const String& s)
{
    _assert(s.IsValid() && (s.Length() == 4));
    return FourCC(uint(s[3] | s[2]<<8 | s[1]<<16 | s[0]<<24));
}

//------------------------------------------------------------------------------
/**
*/
inline index_t
FourCC::HashCode() const
{
	return this->fourCC;
}

} // namespace Util
//------------------------------------------------------------------------------
