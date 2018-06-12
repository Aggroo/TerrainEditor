//------------------------------------------------------------------------------
//  @file		string.cc
//  @copyright	See LICENCE file
//------------------------------------------------------------------------------
#include "config.h"
#include "string.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <cstdarg>

namespace Util
{

//------------------------------------------------------------------------------
/**
*/
bool
String::CopyToBuffer(char* buf, size_t bufSize) const
{
    assert(0 != buf);
    assert(bufSize > 1);

    //#if __WIN32__
    //    HRESULT hr = StringCchCopy(buf, bufSize, this->AsCharPtr());
    //    return SUCCEEDED(hr);    
    //#else
        strncpy(buf, this->AsCharPtr(), bufSize - 1);
        buf[bufSize - 1] = 0;
        return this->strLen < bufSize;
    //#endif
}

//------------------------------------------------------------------------------
/**
*/
void __cdecl
String::Format(const char* fmtString, ...)
{
    va_list argList;
    va_start(argList, fmtString);
    char buf[4096]; // an 4 kByte buffer
    //#if __WIN32__
    //    // need to use non-CRT thread safe function under Win32
    //    StringCchVPrintf(buf, sizeof(buf), fmtString, argList);
    //#elif (__WII__ || __PS3__ || __OSX__ || linux)
		vsnprintf(buf, sizeof(buf), fmtString, argList);
    //#else
    //    _vsnprintf(buf, sizeof(buf), fmtString, argList);
    //#endif
    *this = buf;
    va_end(argList);
}

//------------------------------------------------------------------------------
/**
*/
void __cdecl
String::FormatArgList(const char* fmtString, va_list argList)
{
    char buf[4096]; // an 4 kByte buffer
    //#if __WIN32__
    //    // need to use non-CRT thread safe function under Win32
    //    StringCchVPrintf(buf, sizeof(buf), fmtString, argList);
    //#elif (__WII__ || __PS3__ || __OSX__ || linux)
		vsnprintf(buf, sizeof(buf), fmtString, argList);
    //#else
    //    _vsnprintf(buf, sizeof(buf), fmtString, argList);
    //#endif
    *this = buf;
}

//------------------------------------------------------------------------------
/**
*/
String
String::Sprintf(const char* fmtString, ...)
{
    String format;
    va_list argList;
    va_start(argList, fmtString);
    format.FormatArgList(fmtString, argList);
    va_end(argList);
    return format;
}

//------------------------------------------------------------------------------
/**
    Sets a new string content. This will handle all special cases and try
    to minimize heap allocations as much as possible.
*/
void
String::Set(const char* str, size_t length)
{
    if (0 == str)
    {
        // a null string pointer is valid as a special case
        this->Delete();
        this->localBuffer[0] = 0;
    }
    else if ((0 == this->heapBuffer) && (length < LocalStringSize))
    {
        // the new contents fits into the local buffer,
        // however, if there is already an external buffer
        // allocated, we use the external buffer!
        this->Delete();
        Memory::Copy(this->localBuffer, str, length);
        this->localBuffer[length] = 0;
    }
    else if (length < this->heapBufferSize)
    {
        // the new contents fits into the existing buffer
        Memory::Copy(this->heapBuffer, str, length);
        this->heapBuffer[length] = 0;
        this->localBuffer[0] = 0;
    }
    else
    {
        // need to allocate bigger heap buffer
        this->Alloc(length + 1);
        Memory::Copy(this->heapBuffer, str, length);
        this->heapBuffer[length] = 0;
        this->localBuffer[0] = 0;
    }
    this->strLen = length;
}

//------------------------------------------------------------------------------
/**
*/
void
String::AppendRange(const char* append, size_t length)
{
    assert(append);
    if (length > 0)
    {
        size_t newLength = this->strLen + length;
        
        // if there is an external buffer allocated, we need to stay there
        // even if the resulting string is smaller then LOCALSTRINGSIZE,
        // a small string in an external buffer may be the result
        // of a call to Reserve()
        if ((0 == this->heapBuffer) && (newLength < LocalStringSize))
        {
            // the result fits into the local buffer
            Memory::Copy(this->localBuffer + this->strLen, append, length);
            this->localBuffer[newLength] = 0;
        }
        else if (newLength < this->heapBufferSize)
        {
            // the result fits into the existing buffer
			Memory::Copy(this->heapBuffer + this->strLen, append, length);
            this->heapBuffer[newLength] = 0;
        }
        else
        {
            // need to re-allocate
            this->Realloc(newLength + newLength / 2);
			//NOTE: This was localbuffer earlier, but i'm pretty sure it is supposed to be heapbuffer. I changed it and everything seems to work right now atleast.
			Memory::Copy(this->heapBuffer + this->strLen, append, length);
            this->heapBuffer[newLength] = 0;
        }
        this->strLen = newLength;
    }
}

//------------------------------------------------------------------------------
/**
    Tokenize the string into a String array. 

    @param  whiteSpace      a string containing the whitespace characters
    @return                 a string array of tokens
*/
size_t
String::Tokenize(const String& whiteSpace, Array<String>& outTokens) const
{
    outTokens.Clear();
    String str(*this);
    char* ptr = const_cast<char*>(str.AsCharPtr());
    const char* token;
    while (0 != (token = strtok(ptr, whiteSpace.AsCharPtr())))
    {
        outTokens.Append(token);
        ptr = 0;
    }
    return outTokens.Size();
}

//------------------------------------------------------------------------------
/**
    This is the slow-but-convenient Tokenize() method. Slow since the
    returned string array will be constructed anew with every method call.
    Consider the Tokenize() method which takes a string array as input,
    since this may allow reusing of the array, reducing heap allocations.
*/
Array<String>
String::Tokenize(const String& whiteSpace) const
{
    Array<String> tokens;
    this->Tokenize(whiteSpace, tokens);
    return tokens;
}

//------------------------------------------------------------------------------
/**
    Tokenize a string, but keeps the string within the fence-character
    intact. For instance for the sentence:

    He said: "I don't know."

    A Tokenize(" ", '"', tokens) would return:

    token 0:    He
    token 1:    said:
    token 2:    I don't know.
*/
size_t
String::Tokenize(const String& whiteSpace, char fence, Array<String>& outTokens) const
{
    outTokens.Clear();
    String str(*this);
    char* ptr = const_cast<char*>(str.AsCharPtr());    
    char* end = ptr + strlen(ptr);
    while (ptr < end)
    {
        char* c;

        // skip white space
        while (*ptr && strchr(whiteSpace.AsCharPtr(), *ptr))
        {
            ptr++;
        }
        if (*ptr)
        {
            // check for fenced area
            if ((fence == *ptr) && (0 != (c = strchr(++ptr, fence))))
            {
                *c++ = 0;
                outTokens.Append(ptr);
                ptr = c;
            }
            else if (0 != (c = strpbrk(ptr, whiteSpace.AsCharPtr())))
            {
                *c++ = 0;
                outTokens.Append(ptr);
                ptr = c;
            }
            else
            {
                outTokens.Append(ptr);
                break;
            }
        }
    }
    return outTokens.Size();
}

//------------------------------------------------------------------------------
/**
    Slow version of Tokenize() with fence character. See above Tokenize()
    for details.
*/
Array<String>
String::Tokenize(const String& whiteSpace, char fence) const
{
    Array<String> tokens;
    this->Tokenize(whiteSpace, fence, tokens);
    return tokens;
}

//------------------------------------------------------------------------------
/**
    Extract a substring range.
*/
String
String::ExtractRange(index_t from, size_t numChars) const
{
    assert(from <= this->strLen);
    assert((from + numChars) <= this->strLen);
    const char* str = this->AsCharPtr();
    String newString;
    newString.Set(&(str[from]), numChars);
    return newString;
}

//------------------------------------------------------------------------------
/**
    Extract a substring until the end of the original string.
*/
String
String::ExtractToEnd(index_t fromIndex) const
{
    return this->ExtractRange(fromIndex, this->strLen - fromIndex);
}

//------------------------------------------------------------------------------
/**
    Terminates the string at the first occurance of one of the characters
    in charSet.
*/
void
String::Strip(const String& charSet)
{
    char* str = const_cast<char*>(this->AsCharPtr());
    char* ptr = strpbrk(str, charSet.AsCharPtr());
    if (ptr)
    {
        *ptr = 0;
    }
    this->strLen = (size_t) strlen(this->AsCharPtr());
}

//------------------------------------------------------------------------------
/**
    Return the index of a substring, or InvalidIndex if not found.
*/
index_t
String::FindStringIndex(const String& s, index_t startIndex) const
{
    assert(startIndex < this->strLen);
    assert(s.IsValid());
    index_t i;
    for (i = startIndex; i < this->strLen; i++)
    {
        if ((this->strLen - i) < s.strLen)
        {
            break;
        }

        if (strncmp(&(this->AsCharPtr()[i]), s.AsCharPtr(), s.strLen) == 0)
        {
            return i;
        }
    }
    return InvalidIndex;
}

//------------------------------------------------------------------------------
/**
    Return index of character in string, or InvalidIndex if not found.
*/
index_t
String::FindCharIndex(char c, index_t startIndex) const
{
    if (this->strLen > 0)
    {
        assert(startIndex < this->strLen);
        const char* ptr = strchr(this->AsCharPtr() + startIndex, c);
        if (ptr)
        {
            return index_t(ptr - this->AsCharPtr());
        }
    }
    return InvalidIndex;
}

//------------------------------------------------------------------------------
/**
    Removes all characters in charSet from the left side of the string.
*/
void
String::TrimLeft(const String& charSet)
{
    assert(charSet.IsValid());
    if (this->IsValid())
    {
        size_t charSetLen = charSet.strLen;
        index_t thisIndex = 0;
        bool stopped = false;
        while (!stopped && (thisIndex < this->strLen))
        {
            index_t charSetIndex;
            bool match = false;
            for (charSetIndex = 0; charSetIndex < charSetLen; charSetIndex++)
            {
                if ((*this)[thisIndex] == charSet[charSetIndex])
                {
                    // a match
                    match = true;
                    break;
                }
            }
            if (!match)
            {
                // stop if no match
                stopped = true;
            }
            else
            {
                // a match, advance to next character
                ++thisIndex;
            }
        }
        String trimmedString(&(this->AsCharPtr()[thisIndex]));
        *this = trimmedString;
    }
}

//------------------------------------------------------------------------------
/**
    Removes all characters in charSet from the right side of the string.
*/
void
String::TrimRight(const String& charSet)
{
    assert(charSet.IsValid());
    if (this->IsValid())
    {
        size_t charSetLen = charSet.strLen;
        int thisIndex = (int)this->strLen - 1;   // NOTE: may not be unsigned (thus not index_t!)
        bool stopped = false;
        while (!stopped && (thisIndex >= 0))
        {
            index_t charSetIndex;
            bool match = false;
            for (charSetIndex = 0; charSetIndex < charSetLen; charSetIndex++)
            {
                if ((*this)[thisIndex] == charSet[charSetIndex])
                {
                    // a match
                    match = true;
                    break;
                }
            }
            if (!match)
            {
                // stop if no match
                stopped = true;
            }
            else
            {
                // a match, advance to next character
                --thisIndex;
            }
        }
        String trimmedString;
        trimmedString.Set(this->AsCharPtr(), thisIndex + 1);
        *this = trimmedString;
    }
}

//------------------------------------------------------------------------------
/**
    Trim both sides of a string.
*/
void
String::Trim(const String& charSet)
{
    this->TrimLeft(charSet);
    this->TrimRight(charSet);
}

//------------------------------------------------------------------------------
/**
    Substitute every occurance of origStr with substStr.
*/
void
String::SubstituteString(const String& matchStr, const String& substStr)
{
    assert(matchStr.IsValid());

    const char* ptr = this->AsCharPtr();
    size_t matchStrLen = matchStr.strLen;
    String dest;

    // walk original string for occurances of str
    const char* occur;
    while (0 != (occur = strstr(ptr, matchStr.AsCharPtr())))
    {
        // append string fragment until match
        dest.AppendRange(ptr, size_t(occur - ptr));

        // append replacement string
        if (substStr.Length() > 0)
        {
            dest.Append(substStr);
        }

        // adjust source pointer
        ptr = occur + matchStrLen;
    }
    dest.Append(ptr);
    *this = dest;
}

//------------------------------------------------------------------------------
/**
    Return a String object containing the last directory of the path, i.e.
    a category.

    - 17-Feb-04     floh    fixed a bug when the path ended with a slash
*/
String
String::ExtractLastDirName() const
{
    String pathString(*this);
    char* lastSlash = pathString.GetLastSlash();
    
    // special case if path ends with a slash
    if (lastSlash)
    {
        if (0 == lastSlash[1])
        {
            *lastSlash = 0;
            lastSlash = pathString.GetLastSlash();
        }

        char* secLastSlash = 0;
        if (0 != lastSlash)
        {
            *lastSlash = 0; // cut filename
            secLastSlash = pathString.GetLastSlash();
            if (secLastSlash)
            {
                *secLastSlash = 0;
                return String(secLastSlash+1);
            }
        }
    }
    return "";
}

//------------------------------------------------------------------------------
/**
    Return a String object containing the part before the last
    directory separator.
    
    NOTE: I left my fix in that returns the last slash (or colon), this was 
    necessary to tell if a dirname is a normal directory or an assign.     

    - 17-Feb-04     floh    fixed a bug when the path ended with a slash
*/
String
String::ExtractDirName() const
{
    String pathString(*this);
    char* lastSlash = pathString.GetLastSlash();

    // special case if path ends with a slash
    if (lastSlash)
    {
        if (0 == lastSlash[1])
        {
            *lastSlash = 0;
            lastSlash = pathString.GetLastSlash();
        }
        if (lastSlash)
        {
            *++lastSlash = 0;
        }
    }
    pathString.strLen = (size_t) strlen(pathString.AsCharPtr());
    return pathString;
}

//------------------------------------------------------------------------------
/**
    Pattern-matching, TCL-style.
*/
bool
String::MatchPattern(const String& string, const String& pattern)
{
    const char* str = string.AsCharPtr();
    const char* pat = pattern.AsCharPtr();

    char c2;
    bool always = true;
    while (always != false)
    {
        if (*pat == 0) 
        {
            if (*str == 0) return true;
            else           return false;
        }
        if ((*str == 0) && (*pat != '*')) return false;
        if (*pat=='*') 
        {
            pat++;
            if (*pat==0) return true;
            while (always)
            {
                if (String::MatchPattern(str, pat)) return true;
                if (*str==0) return false;
                str++;
            }
        }
        if (*pat=='?') goto match;
        if (*pat=='[') 
        {
            pat++;
            while (always) 
            {
                if ((*pat==']') || (*pat==0)) return false;
                if (*pat==*str) break;
                if (pat[1] == '-') 
                {
                    c2 = pat[2];
                    if (c2==0) return false;
                    if ((*pat<=*str) && (c2>=*str)) break;
                    if ((*pat>=*str) && (c2<=*str)) break;
                    pat+=2;
                }
                pat++;
            }
            while (*pat!=']') 
            {
                if (*pat==0) 
                {
                    pat--;
                    break;
                }
                pat++;
            }
            goto match;
        }
    
        if (*pat=='\\') 
        {
            pat++;
            if (*pat==0) return false;
        }
        if (*pat!=*str) return false;

match:
        pat++;
        str++;
    }
    // can't happen
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
String::ReplaceChars(const String& charSet, char replacement)
{
    assert(charSet.IsValid());
    char* ptr = const_cast<char*>(this->AsCharPtr());
    char c;
    while (0 != (c = *ptr))
    {
        if (strchr(charSet.AsCharPtr(), c))
        {
            *ptr = replacement;
        }
        ptr++;
    }
}

#if !__OSX__    
//------------------------------------------------------------------------------
/**
    Returns content as mat4. Note: this method doesn't check whether the
    contents is actually a valid mat4. Use the IsValidMat4() method
    for this!
*/
Math::mat4
String::AsMat4() const
{
    Array<String> tokens(16, 0); 
    this->Tokenize(", \t\n", tokens);
    assert(tokens.Size() == 16);
    Math::mat4 m(tokens[0].AsFloat(),  tokens[1].AsFloat(),  tokens[2].AsFloat(),  tokens[3].AsFloat(),
                     tokens[4].AsFloat(),  tokens[5].AsFloat(),  tokens[6].AsFloat(),  tokens[7].AsFloat(),
                    tokens[8].AsFloat(),  tokens[9].AsFloat(),  tokens[10].AsFloat(), tokens[11].AsFloat(),
                     tokens[12].AsFloat(), tokens[13].AsFloat(), tokens[14].AsFloat(), tokens[15].AsFloat());
    return m;
}
#endif
    
//------------------------------------------------------------------------------
/**
*/
String
String::Concatenate(const Array<String>& strArray, const String& whiteSpace)
{
    String res;
    res.Reserve(256);
    index_t i;
    size_t num = strArray.Size();
    for (i = 0; i < num; i++)
    {
        res.Append(strArray[i]);
        if ((i + 1) < num)
        {
            res.Append(whiteSpace);
        }
    }
    return res;
}

//------------------------------------------------------------------------------
/**
*/
void
String::SetCharPtr(const char* s)
{
    size_t len = 0;
    if (s)
    {
        len = (size_t) strlen((const char*)s);
    }
    this->Set(s, len);
}

//------------------------------------------------------------------------------
/**
*/
void
String::Append(const char* str)
{
    assert(0 != str);
    this->AppendRange(str, (size_t) strlen(str));
}

//------------------------------------------------------------------------------
/**
    Terminates the string at the given index.
*/
void
String::TerminateAtIndex(index_t index)
{
    assert(index < this->strLen);
    char* ptr = const_cast<char*>(this->AsCharPtr());
    ptr[index] = 0;
    this->strLen = (size_t) strlen(ptr);
}

//------------------------------------------------------------------------------
/**
    Remove the file extension.
*/
void
String::StripFileExtension()
{
    char* str = const_cast<char*>(this->AsCharPtr());
    char* ext = strrchr(str, '.');
    if (ext)
    {
        *ext = 0;
        this->strLen = (size_t) strlen(this->AsCharPtr());
    }
}

//------------------------------------------------------------------------------
/**
*/
void
String::StripAssignPrefix()
{
	char* str = const_cast<char*>(this->AsCharPtr());
	char* ext = strchr(str, ':');
	if (ext)
	{
		this->SetCharPtr(ext + 1);
	}
}

//------------------------------------------------------------------------------
/**
*/
bool 
operator==(const String& a, const String& b)
{
    return strcmp(a.AsCharPtr(), b.AsCharPtr()) == 0;
}

//------------------------------------------------------------------------------
/**
*/
bool 
operator==(const String& a, const char* cStr)
{
    assert(0 != cStr);
    return strcmp(a.AsCharPtr(), cStr) == 0;
}

//------------------------------------------------------------------------------
/**
*/
bool 
operator==(const char* cStr, const String& b)
{
    assert(0 != cStr);
    return strcmp(cStr, b.AsCharPtr()) == 0;
}

//------------------------------------------------------------------------------
/**
*/
bool 
operator != (const String& a, const String& b)
{
    return strcmp(a.AsCharPtr(), b.AsCharPtr()) != 0;
}

//------------------------------------------------------------------------------
/**
*/
bool
operator < (const String& a, const String& b)
{
	return strcmp(a.AsCharPtr(), b.AsCharPtr()) < 0;
}

//------------------------------------------------------------------------------
/**
*/
bool
operator > (const String& a, const String& b)
{
	return strcmp(a.AsCharPtr(), b.AsCharPtr()) > 0;
}

//------------------------------------------------------------------------------
/**
*/
bool
operator <= (const String& a, const String& b)
{
	return strcmp(a.AsCharPtr(), b.AsCharPtr()) <= 0;
}

//------------------------------------------------------------------------------
/**
*/
bool
operator >= (const String& a, const String& b)
{
	return strcmp(a.AsCharPtr(), b.AsCharPtr()) >= 0;
}

//------------------------------------------------------------------------------
/**
*/
void
String::ToLower()
{
    char* str = const_cast<char*>(this->AsCharPtr());
    char c;
    while (0 != (c = *str))
    {
        *str++ = (char) tolower(c);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
String::ToUpper()
{
    char* str = const_cast<char*>(this->AsCharPtr());
    char c;
    while (0 != (c = *str))
    {
        *str++ = (char) toupper(c);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
String::FirstCharToUpper()
{
    char* str = const_cast<char*>(this->AsCharPtr());    
    *str = (char) toupper(*str);
}

//------------------------------------------------------------------------------
/**
    Returns true if string contains one of the characters from charset.
*/
bool
String::ContainsCharFromSet(const String& charSet) const
{
    char* str = const_cast<char*>(this->AsCharPtr());
    char* ptr = strpbrk(str, charSet.AsCharPtr());
    return (0 != ptr);
}

//------------------------------------------------------------------------------
/**
    @return     string representing the filename extension (maybe empty)
*/
String
String::GetFileExtension() const
{
    const char* str = this->AsCharPtr();
    const char* ext = strrchr(str, '.');
    if (ext)
    {
        ext++;
        return String(ext);
    }
    return String("");
}

//------------------------------------------------------------------------------
/**
    Get a pointer to the last directory separator.
*/
char*
String::GetLastSlash() const
{
    const char* s = this->AsCharPtr();
    const char* lastSlash = strrchr(s, '/');
    if (0 == lastSlash) lastSlash = strrchr(s, '\\');
    if (0 == lastSlash) lastSlash = strrchr(s, ':');
    return const_cast<char*>(lastSlash);
}

//------------------------------------------------------------------------------
/**
*/
bool
String::IsValidBool() const
{
    static const char* bools[] = {
        "no", "yes", "off", "on", "false", "true", 0
    };
    index_t i = 0;
    while (bools[i] != 0)
    {
        if (0 == stricmp(bools[i], this->AsCharPtr()))
        {
            return true;
        }
        i++;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
    Returns content as integer. Note: this method doesn't check whether the
    contents is actually a valid integer. Use the IsValidInteger() method
    for this!
*/
int
String::AsInt() const
{
    return atoi(this->AsCharPtr());
}

//------------------------------------------------------------------------------
/**
    Returns content as float. Note: this method doesn't check whether the
    contents is actually a valid float. Use the IsValidInt() method
    for this!
*/
float
String::AsFloat() const
{
    return float(atof(this->AsCharPtr()));
}

//------------------------------------------------------------------------------
/**
*/
bool
String::AsBool() const
{
    static const char* bools[] = {
        "no", "yes", "off", "on", "false", "true", "0","1",0
    };
    index_t i = 0;
    while (bools[i] != 0)
    {
        if (0 == stricmp(bools[i], this->AsCharPtr()))
        {
            return 1 == (i & 1);
        }
        i++;
    }
    //_error("Invalid string value for bool!");
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
String::IsDigit(char c)
{
    return (0 != isdigit(int(c)));
}

//------------------------------------------------------------------------------
/**
*/
bool
String::IsAlpha(char c)
{
    return (0 != isalpha(int(c)));
}

//------------------------------------------------------------------------------
/**
*/
bool
String::IsAlNum(char c)
{
    return (0 != isalnum(int(c)));
}

//------------------------------------------------------------------------------
/**
*/
bool
String::IsLower(char c)
{
    return (0 != islower(int(c)));
}

//------------------------------------------------------------------------------
/**
*/
bool
String::IsUpper(char c)
{
    return (0 != isupper(int(c)));
}

//------------------------------------------------------------------------------
/**
*/
int
String::StrCmp(const char* str0, const char* str1)
{
    assert(str0 && str1);
    return strcmp(str0, str1);
}

//------------------------------------------------------------------------------
/**
*/
int
String::StrLen(const char* str)
{
    assert(str);
    return (int)strlen(str);
}

//------------------------------------------------------------------------------
/**
*/
const char*
String::StrChr(const char* str, int c)
{
    assert(str);
    return strchr(str, c);
}

//------------------------------------------------------------------------------
/**
*/
//Dictionary<String,String>
//String::ParseKeyValuePairs(const String& str)
//{
//    Dictionary<String,String> res;
//    Array<String> tokens = str.Tokenize(" \t\n=", '"');
//    assert(0 == (tokens.Size() & 1)); // num tokens must be even
//    index_t i;
//    for (i = 0; i < tokens.Size(); i += 2)
//    {
//        res.Add(tokens[i], tokens[i + 1]);
//    }
//    return res;
//}

//------------------------------------------------------------------------------
/**
*/
void 
String::ChangeFileExtension(const Util::String& newExt)
{
    this->StripFileExtension();
    this->Append("." + newExt);
}

//------------------------------------------------------------------------------
/**
*/
void
String::ChangeAssignPrefix(const Util::String& newPref)
{
	this->StripAssignPrefix();
	*this = newPref + ":" + *this;
}

//------------------------------------------------------------------------------
/**
*/
//String
//String::FromBlob(const Util::Blob& b)
//{
//    assert(b.IsValid());
//    
//    unsigned char * buf = (unsigned char*)Memory::Alloc(Memory::ScratchHeap,b.Size() * 2 +1);
//    buf[b.Size() * 2] = 0;
//    static char hex[]= "0123456789abcdef";        
//    
//    const unsigned char * ptr = (const unsigned char*)b.GetPtr();
//    for(size_t i = 0 ; i < b.Size(); i ++)
//    {
//        buf[i * 2 + 0] = hex[ptr[i] >> 4  ];
//        buf[i * 2 + 1] = hex[ptr[i] & 0x0F];
//    }
//
//    Util::String str;
//    str.SetCharPtr((const char*)buf);
//    return str;
//}


//------------------------------------------------------------------------------
/**
*/
//Util::Blob 
//String::AsBlob() const
//{
//    Util::Blob b;
//    size_t s = this->heapBufferSize >> 1;
//    b.Reserve(s);
//    unsigned char * ptr = (unsigned char*)b.GetPtr();
//    size_t c = 0;
//    for(size_t i = 0 ; i < s; i++)
//    {
//        unsigned char temp = this->heapBuffer[c++];
//
//        if(temp > 0x60)
//        {
//            temp -= 39;
//        }
//        temp -= 48; 
//        temp *= 16;
//
//        temp += heapBuffer[c];
//        if(heapBuffer[c++] > 0x60)
//        {
//            temp -= 39;
//        }
//        temp -= 48;
//        ptr[i] = temp;
//    }
//    return b;
//
//}

}
