#pragma once
//------------------------------------------------------------------------------
/**
    @class Util::String

    Universal string class based on Nebula3's String Class. An empty string object is always 32
    bytes big. The string class tries to avoid costly heap allocations
    with the following tactics:

    - a local embedded buffer is used if the string is short enough
    - if a heap buffer must be allocated, care is taken to reuse an existing
      buffer instead of allocating a new buffer if possible (usually if
      an assigned string fits into the existing buffer, the buffer is reused
      and not re-allocated)

    Heap allocations are performed through a local heap which should
    be faster then going through the process heap.

    Besides the usual string manipulation methods, the String class also
    offers methods to convert basic Nebula3 datatypes from and to string,
    and a group of methods which manipulate filename strings.

    @copyright	See LICENCE file
*/

#include "config.h"
#include "foundation/util/array.h"

#include "foundation/math/vec4.h"
#include "foundation/math/mat4.h"
#include "foundation/memory/windows/winmemory.h"
#include "foundation/math/vec2.h"


//------------------------------------------------------------------------------
namespace Util
{
class String
{
public:
    /// override new operator
    void* operator new(size_t s);
    /// override delete operator
    void operator delete(void* ptr);

    /// constructor
    String();
    /// copy constructor
    String(const String& rhs);
    /// construct from C string
    String(const char* cStr);
    /// destructor
    ~String();

    /// assignment operator
    void operator=(const String& rhs);
    /// assign from const char*
    void operator=(const char* cStr);
    /// += operator
    void operator +=(const String& rhs);
    /// equality operator
    friend bool operator==(const String& a, const String& b);
    /// shortcut equality operator
    friend bool operator==(const String& a, const char* cStr);
    /// shortcut equality operator
    friend bool operator==(const char* cStr, const String& a);
    /// inequality operator
    friend bool operator !=(const String& a, const String& b);
    /// less-then operator
    friend bool operator <(const String& a, const String& b);
    /// greater-then operator
    friend bool operator >(const String& a, const String& b);
    /// less-or-equal operator
    friend bool operator <=(const String& a, const String& b);
    /// greater-then operator
    friend bool operator >=(const String& a, const String& b);
    /// read-only index operator
    char operator[](index_t i) const;
    /// read/write index operator
    char& operator[](index_t i);

    /// reserve internal buffer size to prevent heap allocs
    void Reserve(size_t newSize);
    /// return length of string
    size_t Length() const;
    /// clear the string
    void Clear();
    /// return true if string object is empty
    bool IsEmpty() const;
    /// return true if string object is not empty
    bool IsValid() const;
    /// copy to char buffer (return false if buffer is too small)
    bool CopyToBuffer(char* buf, size_t bufSize) const;

    /// append string
    void Append(const String& str);
    /// append c-string
    void Append(const char* str);
    /// append a range of characters
    void AppendRange(const char* str, size_t numChars);

    /// convert string to lower case
    void ToLower();
    /// convert string to upper case
    void ToUpper();
    /// convert first char of string to upper case
    void FirstCharToUpper();
    /// tokenize string into a provided String array (faster if tokens array can be reused)
    size_t Tokenize(const String& whiteSpace, Array<String>& outTokens) const;
    /// tokenize string into a provided String array, SLOW since new array will be constructed
    Array<String> Tokenize(const String& whiteSpace) const;
    /// tokenize string, keep strings within fence characters intact (faster if tokens array can be reused)
    size_t Tokenize(const String& whiteSpace, char fence, Array<String>& outTokens) const;
    /// tokenize string, keep strings within fence characters intact, SLOW since new array will be constructed
    Array<String> Tokenize(const String& whiteSpace, char fence) const;
    /// extract substring
    String ExtractRange(index_t fromIndex, size_t numChars) const;
    /// extract substring to end of this string
    String ExtractToEnd(index_t fromIndex) const;
    /// terminate string at first occurence of character in set
    void Strip(const String& charSet);
    /// return start index of substring, or InvalidIndex if not found
    index_t FindStringIndex(const String& s, index_t startIndex = 0) const;
    /// return index of character in string, or InvalidIndex if not found
    index_t FindCharIndex(char c, index_t startIndex = 0) const;
    /// terminate string at given index
    void TerminateAtIndex(index_t index);
    /// returns true if string contains any character from set
    bool ContainsCharFromSet(const String& charSet) const;
    /// delete characters from charset at left side of string
    void TrimLeft(const String& charSet);
    /// delete characters from charset at right side of string
    void TrimRight(const String& charSet);
    /// trim characters from charset at both sides of string
    void Trim(const String& charSet);
    /// substitute every occurance of a string with another string
    void SubstituteString(const String& str, const String& substStr);
    /// substiture every occurance of a character with another character
    void SubstituteChar(char c, char subst);
    /// format string printf-style
    void __cdecl Format(const char* fmtString, ...);
    /// format string printf-style with varargs list
    void __cdecl FormatArgList(const char* fmtString, va_list argList);
    /// static constructor for string using printf
    static String Sprintf(const char* fmtString, ...);
    /// return true if string only contains characters from charSet argument
    bool CheckValidCharSet(const String& charSet) const;
    /// replace any char set character within a srtring with the replacement character
    void ReplaceChars(const String& charSet, char replacement);
    /// concatenate array of strings into new string
    static String Concatenate(const Array<String>& strArray, const String& whiteSpace);
    /// pattern matching
    static bool MatchPattern(const String& str, const String& pattern);
    /// return a 32-bit hash code for the string
    index_t HashCode() const;

    /// set content to char ptr
    void SetCharPtr(const char* s);
    /// set as char ptr, with explicit length
    void Set(const char* ptr, size_t length);
	/// set as byte value
	void SetByte(byte val);
	/// set as ubyte value
	void SetUByte(ubyte val);
	/// set as short value
	void SetShort(short val);
	/// set as ushort value
	void SetUShort(ushort val);
    /// set as int value
    void SetInt(int val);
	/// set as uint value
	void SetUInt(uint val);
	/// set as long value
	void SetLong(long val);
	/// set as long value
	void SetSizeT(size_t val);
	/// set as long long value
	void SetLongLong(long long val);
    /// set as float value
    void SetFloat(float val);
    /// set as bool value
    void SetBool(bool val);
	/// set as vec3 value
    void SetVec2(const Math::vec2& v);
	/// set as vec3 value
	void SetVec3(const Math::vec3& v);
    /// set as vec4 value
    void SetVec4(const Math::vec4& v);
    /// set as mat4 value
    void SetMat4(const Math::mat4& v);
    void SetQuaternion(const Math::quaternions& v);

    /// generic setter
    //template<typename T> void Set(const T& t);

    /// append int value
    void AppendInt(int val);
    /// append float value
    void AppendFloat(float val);
    /// append bool value
    void AppendBool(bool val);
    
    /// append vec4 value
    void AppendVec4(const Math::vec4& v);
    /// append mat4 value
    void AppendMat4(const Math::mat4& v);

    /// generic append
    //template<typename T> void Append(const T& t);

    /// return contents as character pointer
    const char* AsCharPtr() const;
    /// return contents as integer
    int AsInt() const;
	/// return contents as long long
	long long AsLongLong() const;
    /// return contents as float
    float AsFloat() const;
	/// set as double value
	void SetDouble(double val);
    /// return contents as bool
    bool AsBool() const;
    
    /// return contents as vec4
    Math::vec4 AsVec4() const;
    /// return contents as mat4
    Math::mat4 AsMat4() const;
    
    /// return contents as blob
	//TODO: implement blobs!
    //Util::Blob AsBlob() const;

    /// convert to "anything"
    //template<typename T> T As() const;

    /// return true if the content is a valid integer
    bool IsValidInt() const;
    /// return true if the content is a valid float
    bool IsValidFloat() const;
    bool IsValidVec2() const;
    /// return true if the content is a valid bool
    bool IsValidBool() const;
    /// return true if the content is a valid vec4
    bool IsValidVec4() const;
    /// return true if content is a valid mat4
    bool IsValidMat4() const;
    Math::vec2 AsVec2() const;

    /// generic valid checker
    template<typename T> bool IsValid() const;

	/// construct a string from a byte
	static String FromByte(byte i);
	/// construct a string from a ubyte
	static String FromUByte(ubyte i);
	/// construct a string from a short
	static String FromShort(short i);
	/// construct a string from a ushort
	static String FromUShort(ushort i);
    /// construct a string from an int
    static String FromInt(int i);
	/// construct a string from a uint
	static String FromUInt(uint i);
	/// construct a string from a long
	static String FromLong(long i);
	/// construct a string from a size_t
	static String FromSize(size_t i);
	/// construct a string from a long long
	static String FromLongLong(long long i);
    /// construct a string from a float
    static String FromFloat(float f);
	/// construct a string from a double
	static String FromDouble(double f);
    /// construct a string from a bool
    static String FromBool(bool b);
	/// construct a string from vec2
    static String FromVec2(const Math::vec2& v);
	/// construct a string from vec3
	static String FromVec3(const Math::vec3& v);
    /// construct a string from vec4
    static String FromVec4(const Math::vec4& v);
    /// construct a string from mat4
    static String FromMat4(const Math::mat4& m);
	///construct a string from a quaternion
    static String FromQuaternion(const Math::quaternions& q);

    /// create from blob
    //static String FromBlob(const Util::Blob & b);
	
    /// convert from "anything"
    //template<typename T> static String From(const T& t);

    /// get filename extension without dot
    String GetFileExtension() const;
    /// check file extension
    bool CheckFileExtension(const String& ext) const;
    /// convert backslashes to slashes
    void ConvertBackslashes();
    /// remove file extension
    void StripFileExtension();
    /// change file extension
    void ChangeFileExtension(const Util::String& newExt);
	/// remove assign prefix (for example tex:)
	void StripAssignPrefix();
	/// change assign prefix
	void ChangeAssignPrefix(const Util::String& newPref);
    /// extract the part after the last directory separator
    String ExtractFileName() const;
    /// extract the last directory of the path
    String ExtractLastDirName() const;
    /// extract the part before the last directory separator
    String ExtractDirName() const;
    /// extract path until last slash
    String ExtractToLastSlash() const;
    /// replace illegal filename characters
    void ReplaceIllegalFilenameChars(char replacement);

    /// test if provided character is a digit (0..9)
    static bool IsDigit(char c);
    /// test if provided character is an alphabet character (A..Z, a..z)
    static bool IsAlpha(char c);
    /// test if provided character is an alpha-numeric character (A..Z,a..z,0..9)
    static bool IsAlNum(char c);
    /// test if provided character is a lower case character
    static bool IsLower(char c);
    /// test if provided character is an upper-case character
    static bool IsUpper(char c);

    /// lowlevel string compare wrapper function
    static int StrCmp(const char* str0, const char* str1);
    /// lowlevel string length function
    static int StrLen(const char* str);
    /// find character in string
    static const char* StrChr(const char* str, int c);

    /// parse key/value pair string ("key0=value0 key1=value1")
    //static Dictionary<String,String> ParseKeyValuePairs(const String& str);

private:
    /// delete contents
    void Delete();
    /// get pointer to last directory separator
    char* GetLastSlash() const;
    /// allocate the string buffer (discards old content)
    void Alloc(size_t size);
    /// (re-)allocate the string buffer (copies old content)
    void Realloc(size_t newSize);

    enum
    {
        LocalStringSize = 20,
    };
    char* heapBuffer;
    char localBuffer[LocalStringSize];
    size_t strLen;
    size_t heapBufferSize;
};

//------------------------------------------------------------------------------
/**
*/
__forceinline void*
String::operator new(size_t size)
{
    #if _DEBUG
		assert(size == sizeof(String));
    #endif
	
	return Memory::Alloc(Memory::StringDataHeap, size);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
String::operator delete(void* ptr)
{
    return Memory::Free(Memory::StringDataHeap, ptr);
}

//------------------------------------------------------------------------------
/**
*/
inline
String::String() :
    heapBuffer(0),
    strLen(0),
    heapBufferSize(0)
{
    this->localBuffer[0] = 0;
}

//------------------------------------------------------------------------------
/**
*/
inline void
String::Delete()
{
    if (this->heapBuffer)
    {
        Memory::Free(Memory::StringDataHeap, (void*) this->heapBuffer);
        this->heapBuffer = 0;
    }
    this->localBuffer[0] = 0;
    this->strLen = 0;
    this->heapBufferSize = 0;
}

//------------------------------------------------------------------------------
/**
*/
inline 
String::~String()
{
    this->Delete();
}

//------------------------------------------------------------------------------
/**
    Allocate a new heap buffer, discards old contents.
*/
inline void
String::Alloc(size_t newSize)
{
    assert(newSize > (this->strLen + 1));
    assert(newSize > this->heapBufferSize);

    // free old buffer
    if (this->heapBuffer)
    {
        Memory::Free(Memory::StringDataHeap, (void*) this->heapBuffer);
        this->heapBuffer = 0;
    }

    // allocate new buffer
    this->heapBuffer = (char*) Memory::Alloc(Memory::StringDataHeap, newSize);
    this->heapBufferSize = newSize;
    this->localBuffer[0] = 0;
}

//------------------------------------------------------------------------------
/**
    (Re-)allocate external buffer and copy existing string contents there.
*/
inline void
String::Realloc(size_t newSize)
{
    assert(newSize > (this->strLen + 1));
    assert(newSize > this->heapBufferSize);

    // allocate a new buffer
    char* newBuffer = (char*) Memory::Alloc(Memory::StringDataHeap, newSize);

    // copy existing contents there...
    if (this->strLen > 0)
    {
        const char* src = this->AsCharPtr();
        Memory::Copy(newBuffer, src, this->strLen);
    }
    newBuffer[this->strLen] = 0;

    // assign new buffer
    if (this->heapBuffer)
    {
        Memory::Free(Memory::StringDataHeap, (void*) this->heapBuffer);
        this->heapBuffer = 0;
    }
    this->localBuffer[0] = 0;
    this->heapBuffer = newBuffer;
    this->heapBufferSize = newSize;
}

//------------------------------------------------------------------------------
/**
    Reserves internal space to prevent excessive heap re-allocations.
    If you plan to do many Append() operations this may help alot.
*/
inline void
String::Reserve(size_t newSize)
{
    if (newSize > this->heapBufferSize)
    {
        this->Realloc(newSize);
    }
}

//------------------------------------------------------------------------------
/**
*/
inline void
String::SetByte(byte val)
{
	this->Format("%d", val);
}

//------------------------------------------------------------------------------
/**
*/
inline void
String::SetUByte(ubyte val)
{
	this->Format("%u", val);
}

//------------------------------------------------------------------------------
/**
*/
inline void
String::SetShort(short val)
{
	this->Format("%d", val);
}

//------------------------------------------------------------------------------
/**
*/
inline void
String::SetUShort(ushort val)
{
	this->Format("%u", val);
}

//------------------------------------------------------------------------------
/**
*/
inline void
String::SetInt(int val)
{
    this->Format("%d", val);
}

//------------------------------------------------------------------------------
/**
*/
inline void
String::SetUInt(uint val)
{
	this->Format("%u", val);
}

//------------------------------------------------------------------------------
/**
*/
inline void
String::SetLong(long val)
{
	this->Format("%ld", val);
}

//------------------------------------------------------------------------------
/**
*/
inline void
String::SetSizeT(size_t val)
{
	this->Format("%zu", val);
}

//------------------------------------------------------------------------------
/**
*/
inline void
String::SetLongLong(long long val)
{
	this->Format("%lld", val);
}
//------------------------------------------------------------------------------
/**
*/
inline void
String::SetFloat(float val)
{
    this->Format("%.6f", val);
}

//------------------------------------------------------------------------------
/**
*/
inline void
String::SetDouble(double val)
{
	this->Format("%.6f", val);
}

//------------------------------------------------------------------------------
/**
*/
inline void
String::SetBool(bool val)
{
    if (val)
    {
        this->SetCharPtr("true");
    }
    else
    {
        this->SetCharPtr("false");
    }
}

//------------------------------------------------------------------------------
/**
*/
inline void
String::SetVec2(const Math::vec2& v)
{
	this->Format("%.6f,%.6f", v.x(), v.y());
}

//------------------------------------------------------------------------------
/**
*/
inline void
String::SetVec3(const Math::vec3& v)
{
	this->Format("%.6f,%.6f,%.6f", v.x(), v.y(), v.z());
}

//------------------------------------------------------------------------------
/**
*/
inline void
String::SetVec4(const Math::vec4& v)
{
    this->Format("%.6f,%.6f,%.6f,%.6f", v.x(), v.y(), v.z(), v.w());
}

//------------------------------------------------------------------------------
/**
*/
inline void
String::SetMat4(const Math::mat4& m)
{
    this->Format("%.6f, %.6f, %.6f, %.6f, "
                 "%.6f, %.6f, %.6f, %.6f, "
                 "%.6f, %.6f, %.6f, %.6f, "
                 "%.6f, %.6f, %.6f, %.6f",
                 m[0], m[1], m[2], m[3],
                 m[4], m[5], m[6], m[7],
                 m[8], m[9], m[10], m[11],
                 m[12], m[13], m[14], m[15]);
}

//------------------------------------------------------------------------------
/**
*/
inline void
String::SetQuaternion(const Math::quaternions& v)
{
	this->Format("%.6f,%.6f,%.6f,%.6f", v.X(), v.Y(), v.Z(), v.W());
}

//------------------------------------------------------------------------------
/**
*/
inline
String::String(const char* str) :
    heapBuffer(0),
    strLen(0),
    heapBufferSize(0)
{
    this->localBuffer[0] = 0;
    this->SetCharPtr(str);
}

//------------------------------------------------------------------------------
/**
*/
inline
String::String(const String& rhs) :
    heapBuffer(0),
    strLen(0),
    heapBufferSize(0)
{
    this->localBuffer[0] = 0;
    this->SetCharPtr(rhs.AsCharPtr());
}

//------------------------------------------------------------------------------
/**
*/
inline const char*
String::AsCharPtr() const
{
    if (this->heapBuffer)
    {
        return this->heapBuffer;
    }
    else
    {
        return this->localBuffer;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline void
String::operator=(const String& rhs)
{
    if (&rhs != this)
    {
        this->SetCharPtr(rhs.AsCharPtr());
    }
}

//------------------------------------------------------------------------------
/**
*/
inline void
String::operator=(const char* cStr)
{
    this->SetCharPtr(cStr);
}

//------------------------------------------------------------------------------
/**
*/
inline void
String::Append(const String& str)
{
    this->AppendRange(str.AsCharPtr(), str.strLen);
}

//------------------------------------------------------------------------------
/**
*/
inline void
String::operator += (const String& rhs)
{
    this->Append(rhs);    
}

//------------------------------------------------------------------------------
/**
*/
inline char
String::operator[](index_t i) const
{
    assert(i <= this->strLen);
    return this->AsCharPtr()[i];
}

//------------------------------------------------------------------------------
/**
    NOTE: unlike the read-only indexer, the terminating 0 is NOT a valid
    part of the string because it may not be overwritten!!!
*/
inline char&
String::operator[](index_t i)
{
    assert(i <= this->strLen);
    return (char&)(this->AsCharPtr())[i];
}

//------------------------------------------------------------------------------
/**
*/
inline size_t
String::Length() const
{
    return this->strLen;
}

//------------------------------------------------------------------------------
/**
*/
inline void
String::Clear()
{
    this->Delete();
}

//------------------------------------------------------------------------------
/**
*/
inline bool
String::IsEmpty() const
{
    return (0 == this->strLen);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
String::IsValid() const
{
    return (0 != this->strLen);
}

//------------------------------------------------------------------------------
/**
    This method computes a hash code for the string. The method is
    compatible with the Util::HashTable class.
*/
inline index_t
String::HashCode() const
{
    index_t hash = 0;
    const char* ptr = this->AsCharPtr();
    size_t len = this->strLen;
    index_t i;
    for (i = 0; i < len; i++)
    {
        hash += ptr[i];
        hash += hash << 10;
        hash ^= hash >>  6;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    hash &= ~(1<<31);       // don't return a negative number (in case index_t is defined signed)
    return hash;
}

//------------------------------------------------------------------------------
/**
*/
static inline String
operator+(const String& s0, const String& s1)
{
    String newString = s0;
    newString.Append(s1);
    return newString;
}

//------------------------------------------------------------------------------
/**
    Replace character with another.
*/
inline void
String::SubstituteChar(char c, char subst)
{
    char* ptr = const_cast<char*>(this->AsCharPtr());
    index_t i;
    for (i = 0; i <= this->strLen; i++)
    {
        if (ptr[i] == c)
        {
            ptr[i] = subst;
        }
    }
}

//------------------------------------------------------------------------------
/**
    Converts backslashes to slashes.
*/
inline void
String::ConvertBackslashes()
{
    this->SubstituteChar('\\', '/');
}

//------------------------------------------------------------------------------
/**
*/
inline bool
String::CheckFileExtension(const String& ext) const
{
    return (this->GetFileExtension() == ext);
}

//------------------------------------------------------------------------------
/**
    Return a String object containing the part after the last
    path separator.
*/
inline String
String::ExtractFileName() const
{
    String pathString;
    const char* lastSlash = this->GetLastSlash();
    if (lastSlash)
    {
        pathString = &(lastSlash[1]);
    }
    else
    {
        pathString = *this;
    }
    return pathString;
}

//------------------------------------------------------------------------------
/**
    Return a path string object which contains of the complete path
    up to the last slash. Returns an empty string if there is no
    slash in the path.
*/
inline String
String::ExtractToLastSlash() const
{
    String pathString(*this);
    char* lastSlash = pathString.GetLastSlash();
    if (lastSlash)
    {
        lastSlash[1] = 0;
    }
    else
    {
        pathString = "";
    }
    return pathString;
}

//------------------------------------------------------------------------------
/**
    Return true if the string only contains characters which are in the defined
    character set.
*/
inline bool
String::CheckValidCharSet(const String& charSet) const
{
    index_t i;
    for (i = 0; i < this->strLen; i++)
    {
        if (charSet.FindCharIndex((*this)[i], 0) == InvalidIndex)
        {
            return false;
        }
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
String::IsValidInt() const
{
    return this->CheckValidCharSet(" \t-+01234567890");
}

//------------------------------------------------------------------------------
/**
    Note: this method is not 100% correct, it just checks for invalid characters.
*/
inline bool
String::IsValidFloat() const
{
    return this->CheckValidCharSet(" \t-+.e1234567890");
}

//------------------------------------------------------------------------------
/**
Note: this method is not 100% correct, it just checks for invalid characters.
*/
inline bool
String::IsValidVec2() const
{
	Array<String> tokens(2, 0);
	this->Tokenize(", \t", tokens);
	return this->CheckValidCharSet(" \t-+.,e1234567890") && tokens.Size() == 2;
}

//------------------------------------------------------------------------------
/**
    Note: this method is not 100% correct, it just checks for invalid characters.
*/
inline bool
String::IsValidVec4() const
{
	Array<String> tokens(4, 0);
	this->Tokenize(", \t", tokens);
    return this->CheckValidCharSet(" \t-+.,e1234567890") && tokens.Size() == 4;
}

//------------------------------------------------------------------------------
/**
    Note: this method is not 100% correct, it just checks for invalid characters.
*/
inline bool
String::IsValidMat4() const
{
	Array<String> tokens(16, 0);
	this->Tokenize(", \t", tokens);
    return this->CheckValidCharSet(" \t-+.,e1234567890") && tokens.Size() == 16;
}

//------------------------------------------------------------------------------
/**
Returns content as vec2. Note: this method doesn't check whether the
contents is actually a valid vec2. Use the IsValidVec2() method
for this!
*/
inline Math::vec2
String::AsVec2() const
{
	Array<String> tokens(2, 0);
	this->Tokenize(", \t", tokens);
	assert(tokens.Size() == 2);
	Math::vec2 v(tokens[0].AsFloat(), tokens[1].AsFloat());
	return v;
}

//------------------------------------------------------------------------------
/**
    Returns content as vec4. Note: this method doesn't check whether the
    contents is actually a valid vec4. Use the IsValidVec4() method
    for this!
*/
inline Math::vec4
String::AsVec4() const
{
    Array<String> tokens(4, 0);
    this->Tokenize(", \t", tokens);
    assert(tokens.Size() == 4);
    Math::vec4 v(tokens[0].AsFloat(), tokens[1].AsFloat(), tokens[2].AsFloat(), tokens[3].AsFloat());
    return v;
}

//------------------------------------------------------------------------------
/**
*/
inline void
String::ReplaceIllegalFilenameChars(char replacement)
{
    this->ReplaceChars("\\/:*?\"<>|", replacement);
}

//------------------------------------------------------------------------------
/**
*/
inline String
String::FromInt(int i)
{	
    String str;
    str.SetInt(i);
    return str;
}

//------------------------------------------------------------------------------
/**
*/
inline String
String::FromLong(long i)
{	
	String str;
	str.SetLong(i);
	return str;
}

//------------------------------------------------------------------------------
/**
*/
inline String
String::FromFloat(float f)
{
    String str;
    str.SetFloat(f);
    return str;
}

//------------------------------------------------------------------------------
/**
*/
inline String
String::FromDouble(double i)
{
	String str;
	str.SetDouble(i);
	return str;
}

//------------------------------------------------------------------------------
/**
*/
inline String
String::FromBool(bool b)
{
    String str;
    str.SetBool(b);
    return str;
}

//------------------------------------------------------------------------------
/**
*/
inline String
String::FromVec2(const Math::vec2& v)
{
	String str;
	str.SetVec2(v);
	return str;
}


//------------------------------------------------------------------------------
/**
*/
inline String
String::FromVec3(const Math::vec3& v)
{
	String str;
	str.SetVec3(v);
	return str;
}

//------------------------------------------------------------------------------
/**
*/
inline String
String::FromVec4(const Math::vec4& v)
{
    String str;
    str.SetVec4(v);
    return str;
}

//------------------------------------------------------------------------------
/**
*/
inline String
String::FromMat4(const Math::mat4& m)
{
    String str;
    str.SetMat4(m);
    return str;
}

//------------------------------------------------------------------------------
/**
*/
inline String
String::FromQuaternion(const Math::quaternions& q)
{
	String str;
	str.SetQuaternion(q);
	return str;
}

//------------------------------------------------------------------------------
/**
*/
inline void
String::AppendInt(int val)
{
    this->Append(FromInt(val));
}

//------------------------------------------------------------------------------
/**
*/
inline void
String::AppendFloat(float val)
{
    this->Append(FromFloat(val));
}

//------------------------------------------------------------------------------
/**
*/
inline void
String::AppendBool(bool val)
{
    this->Append(FromBool(val));
}

//------------------------------------------------------------------------------
/**
*/
inline void
String::AppendVec4(const Math::vec4& val)
{
    this->Append(FromVec4(val));
}

//------------------------------------------------------------------------------
/**
*/
inline void
String::AppendMat4(const Math::mat4& val)
{
    this->Append(FromMat4(val));
}

} // namespace Util
//------------------------------------------------------------------------------

