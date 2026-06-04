#pragma once

#include <array>
#include <stdint.h>

#include <string>
#include <vector>

/*
 This is a modified version of the ImGui code as given on the GitHUB repository
 Effekseer/ImGradientHDR.
 Modifications are indicated by +++++++ like comment statements next to or above and
 below added or modified code
*/


const int32_t MarkerMax = 20;

struct ImGradientHDRState
{
	struct ColorMarker
	{
		float Position;
		std::array<float, 3> Color;
		float Intensity;
	};

	struct AlphaMarker
	{
		float Position;
		float Alpha;
	};

	int ColorCount = 0;
	int AlphaCount = 0;
	std::array<ColorMarker, MarkerMax> Colors;
	std::array<AlphaMarker, MarkerMax> Alphas;

	ColorMarker* GetColorMarker(int32_t index);

	AlphaMarker* GetAlphaMarker(int32_t index);

	bool AddColorMarker(float x, std::array<float, 3> color, float intensity);

	bool AddAlphaMarker(float x, float alpha);

	bool RemoveColorMarker(int32_t index);

	bool RemoveAlphaMarker(int32_t index);

	std::array<float, 4> GetCombinedColor(float x) const;

	std::array<float, 4> GetColorAndIntensity(float x) const;

	float GetAlpha(float x) const;

	// +++++++++++
	int min_grad_value = 0;
	int max_grad_value = 1;
	float current_gradient_position = 0.0f;
	float current_gradient_value = 0.0f;
	std::array<float, 4> current_gradient_color = { 0.0f,0.0f,0.0f,0.0f };
	// +++++++++++
};

enum class ImGradientHDRMarkerType
{
	Color,
	Alpha,
	Unknown,
};

struct ImGradientHDRTemporaryState
{
	ImGradientHDRMarkerType selectedMarkerType = ImGradientHDRMarkerType::Unknown;
	int selectedIndex = -1;

	ImGradientHDRMarkerType draggingMarkerType = ImGradientHDRMarkerType::Unknown;
	int draggingIndex = -1;
};

bool ImGradientHDR(int32_t gradientID, ImGradientHDRState& state, ImGradientHDRTemporaryState& temporaryState, bool isMarkerShown = true);

// ++++++++++++++++
bool export_gradient(std::string file_pathname, ImGradientHDRState state);

// Note : This only exports gradient color data
void export_gradient_data(ImGradientHDRState state);

bool import_gradient(std::string file_pathname, ImGradientHDRState& state);

bool read_gradient_data(std::vector<std::string> lines, int &line_number, ImGradientHDRState &state);
// ++++++++++++++++
