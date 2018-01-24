#include "stdafx.h"
#include "XComposite.h"
#include <algorithm>

void XComposite::Add(XShape * shape)
{
	v.push_back(shape);
}

void XComposite::Remove(XShape * shape)
{
	v.erase(std::remove(v.begin(), v.end(), shape), v.end());
}

XShape* XComposite::GetChild(int n)
{
	if (n >= 0 && n < v.size()) return v[n];
	return nullptr;
}


std::vector<XShape*>& XComposite::GetChildren()
{
	return v;
}

void XComposite::Render(XContext &ctx)
{
	for (auto it = v.begin(); it != v.end(); ++it) {
		(*it)->Render(ctx);
	}
}

void XComposite::RenderSimple(XContext &ctx)
{
	for (auto it = v.begin(); it != v.end(); ++it) {
		(*it)->RenderSimple(ctx);
	}
}
