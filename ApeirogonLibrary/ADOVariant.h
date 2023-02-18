#pragma once
class ADOVariant
{
public:
	_variant_t mVar;

	inline ADOVariant() {}
	inline ADOVariant(const _variant_t& src)			{ mVar = src; }
	inline ADOVariant(VARENUM vartype)					{ V_VT(&mVar) = vartype; }

	inline ADOVariant(char a)							{ mVar = (long)a; }
	inline ADOVariant(short a)							{ mVar = (long)a; }
	inline ADOVariant(int a)							{ mVar = (long)a; }
	inline ADOVariant(long a)							{ mVar = a; }
	inline ADOVariant(int64_t a)						{ mVar = a; }
	inline ADOVariant(unsigned char a)					{ mVar = a; }
	inline ADOVariant(unsigned short a)					{ mVar = a; }
	inline ADOVariant(unsigned int a)					{ mVar = a; }
	inline ADOVariant(unsigned long a)					{ mVar = a; }
	inline ADOVariant(uint64_t a)						{ mVar = a; }
	inline ADOVariant(float a)							{ mVar = a; }
	inline ADOVariant(double a)							{ mVar = a; }
	inline ADOVariant(const wchar_t* a)					{ mVar = _bstr_t(a); }
	inline ADOVariant(const char* a)					{ mVar = _bstr_t(a); }
	inline explicit ADOVariant(const std::string& a)	{ mVar = _bstr_t(a.c_str()); }
	//inline ADOVariant(UUID a) { mVar = Win32Guid::From(a).ToBracketString().GetString(); }
	inline ADOVariant(GUID a)
	{
		GUID guid = a;
		char guid_cstr[39];
		snprintf(guid_cstr, sizeof(guid_cstr),
			"{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
			guid.Data1, guid.Data2, guid.Data3,
			guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
			guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);

		std::string guidStr = guid_cstr;
		mVar = _bstr_t(guidStr.c_str());
	}
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
	inline operator std::string() const		{ return std::string((_bstr_t)mVar); }
	inline operator GUID() const
	{
		GUID guid;
		sscanf_s((_bstr_t)mVar,
			"{%8x-%4hx-%4hx-%2hhx%2hhx-%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx}",
			&guid.Data1, &guid.Data2, &guid.Data3,
			&guid.Data4[0], &guid.Data4[1], &guid.Data4[2], &guid.Data4[3],
			&guid.Data4[4], &guid.Data4[5], &guid.Data4[6], &guid.Data4[7]);

		return guid;
	}
	inline operator bool() const			{ return mVar; }
	//inline operator COleDateTime() const {  return m_val; }
	inline operator int64_t() const { return mVar; }

	inline bool IsNull() const { return mVar.vt == VT_NULL; }
};

