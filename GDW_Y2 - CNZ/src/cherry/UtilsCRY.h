#pragma once

#include <memory>
#include <string>

// the value of TypeName gets replaced with whatever was brouught in.

// preventing copies of certain game objects.
#define NoCopy(TypeName) \
	TypeName(const TypeName& other) = delete; \
	TypeName& operator =(const TypeName& other) = delete;

#define NoMove(TypeName) \
	TypeName(const TypeName&& other) = delete; \
	TypeName& operator =(const TypeName&& other) = delete;

#define GraphicsClass(TypeName)\
	typedef std::shared_ptr<TypeName> Sptr;\
	NoMove(TypeName);\
	NoCopy(TypeName); \
	std::string DebugName;
