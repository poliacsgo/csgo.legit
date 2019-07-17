#pragma once

class game_event
{
public:
	virtual ~game_event() = 0;
	virtual const char* GetName() const = 0;
	virtual bool  IsReliable() const = 0;
	virtual bool  IsLocal() const = 0;
	virtual bool  IsEmpty(const char* KeyName = NULL) = 0;
	virtual bool  get_bool(const char* KeyName = NULL, bool defaultValue = false) = 0;
	virtual int   get_int(const char* KeyName = NULL, int defaultValue = 0) = 0;
	virtual unsigned long long GetUint64(char const* KeyName = NULL, unsigned long long defaultValue = 0) = 0;
	virtual float get_float(const char* KeyName = NULL, float defaultValue = 0.0f) = 0;
	virtual const char* get_string(const char* KeyName = NULL, const char* defaultValue = "") = 0;
	virtual const wchar_t* GetWString(char const* KeyName = NULL, const wchar_t* defaultValue = L"") = 0;

	virtual void set_bool(const char* KeyName, bool value) = 0;
	virtual void SetInt(const char* KeyName, int value) = 0;
	virtual void SetUInt64(const char* KeyName, unsigned long long value) = 0;
	virtual void SetFloat(const char* KeyName, float value) = 0;
	virtual void SetString(const char* KeyName, const char* value) = 0;
	virtual void SetWString(const char* KeyName, const wchar_t* value) = 0;
};

class game_event_listener {
public:
	virtual ~game_event_listener() { }

	virtual void fire_game_event(game_event* event) = 0;
	virtual int get_debug_id() = 0;

public:
	int debug_id;
};