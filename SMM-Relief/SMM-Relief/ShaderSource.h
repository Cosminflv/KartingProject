#pragma once
#include <string>

class ShaderSource
{
public:
	ShaderSource() = default;

	std::string VertexSource;
	std::string FragmentSource;
	std::string ShadowVertexSource; // New shader stage for shadow mapping
	std::string ShadowFragmentSource; // New shader stage for shadow mapping
};