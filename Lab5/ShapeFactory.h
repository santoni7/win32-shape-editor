#pragma once
#include "shape.h"
#include "basic_shapes.h"
#include "LineOOShape.h"
#include "CubeShape.h"
#include <type_traits>

class ShapeFactory
{

public:
	template <typename T> static
		typename std::enable_if<std::is_base_of<Shape, T>::value, T>::type	*shape(MPoint p1, MPoint p2)
	{
		return new T(p1, p2);
	}
	template <typename T> static
		typename std::enable_if<std::is_base_of<Shape, T>::value, T>::type	*shape()
	{
		return new T();
	}
};
