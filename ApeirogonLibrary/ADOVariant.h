#pragma once
class ADOVariant
{
public:
	_variant_t mVar;

	inline ADOVariant() {}
	inline ADOVariant(const _variant_t& src)	{ mVar = src; }
	inline ADOVariant(VARENUM vartype)			{ V_VT(&mVar) = vartype; }

	inline ADOVariant(char a)					{ mVar = (long)a; }
	inline ADOVariant(short a)					{ mVar = (long)a; }
	inline ADOVariant(int a)					{ mVar = (long)a; }
	inline ADOVariant(long a)					{ mVar = a; }
	inline ADOVariant(int64_t a)				{ mVar = a; }
	inline ADOVariant(unsigned char a)			{ mVar = a; }
	inline ADOVariant(unsigned short a)			{ mVar = a; }
	inline ADOVariant(unsigned int a)			{ mVar = a; }
	inline ADOVariant(unsigned long a)			{ mVar = a; }
	inline ADOVariant(uint64_t a)				{ mVar = a; }
	inline ADOVariant(float a)					{ mVar = a; }
	inline ADOVariant(double a)					{ mVar = a; }
	inline ADOVariant(const wchar_t* a)			{ mVar = _bstr_t(a); }
	inline ADOVariant(const char* a)			{ mVar = _bstr_t(a); }
	//inline explicit ADOVariant(const String& a) { mVar = _bstr_t(a.GetString()); }
	//inline ADOVariant(UUID a) { mVar = Win32Guid::From(a).ToBracketString().GetString(); }
	//inline ADOVariant(GUID a) { mVar = a.ToBracketString().GetString(); }
	inline ADOVariant(bool a) { mVar = (long)a; }
	//inline ADOVariant(const COleDateTime& a) { mVar = a; }
	//inline ADOVariant(const ByteArray& a) { FromByteArray(a); }
	//inline ADOVariant(const ByteArrayPtr a) { FromByteArray(a); }

	inline operator char() const			{ return (char)mVar; }
	inline operator short() const			{ return (short)mVar; }
	inline operator int() const				{ return (long)mVar; }
	inline operator long() const			{ return mVar; }
	inline operator unsigned char() const	{ return mVar; }
	inline operator unsigned short() const	{ return mVar; }
	inline operator unsigned int() const	{ return mVar; }
	inline operator unsigned long() const	{ return mVar; }
	inline operator float() const			{ return mVar; }
	inline operator double() const			{ return mVar; }
	inline operator const wchar_t*() const	{ return ((_bstr_t)mVar); }
	inline operator const char*() const		{ return ((_bstr_t)mVar); }
	//inline operator String() const {  return String((const PNTCHAR*)(_bstr_t)m_val); }
	//inline operator UUID() const {  return Win32Guid::ToNative(Guid::GetFromString((_bstr_t)m_val)); }
	//inline operator Guid() const {  return Guid::GetFromString((_bstr_t)m_val); }
	inline operator bool() const			{ return mVar; }
	//inline operator COleDateTime() const {  return m_val; }
	//inline operator int64_t() const { return m_val; }

	inline bool IsNull() const { return mVar.vt == VT_NULL; }
};

