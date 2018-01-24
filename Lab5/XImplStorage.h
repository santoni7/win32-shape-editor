#pragma once
#include <unordered_map>
#include <typeinfo>
#include "XShapeImpl.h"
using TypeInfoRef = std::reference_wrapper<const std::type_info>;
struct Hasher {
	std::size_t operator()(TypeInfoRef code) const
	{
		return code.get().hash_code();
	}
}; 
struct EqualTo {
	bool operator()(TypeInfoRef lhs, TypeInfoRef rhs) const
	{
		return lhs.get() == rhs.get();
	}
};

/* Singleton type-mapped collection */
class XImplStorage 
{
private:
	std::unordered_map<TypeInfoRef, XShapeImpl*, Hasher, EqualTo> m;
	static XImplStorage *_instance;
	XImplStorage() = default;
	XImplStorage(const XImplStorage&) = delete;
public:
	static XImplStorage* inst() {
		if (!_instance) _instance = new XImplStorage();
		return _instance;
	}

	template <typename T>
	void AddImpl(T* imp);
	template <typename T>
	T* GetImpl();
};


template <typename T>
void XImplStorage::AddImpl(T* imp) {
	m[typeid(T)] = imp;
}

template <typename T>
T* XImplStorage::GetImpl() {
	auto it = m.find(typeid(T));
	if (it != m.end()) return static_cast<T*>(it->second);
	return nullptr;
}