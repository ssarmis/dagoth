#pragma once

#include "general.h"
#include <glm/vec2.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <cmath>

struct NoiseBitmap {
	u32 width;
	u32 height;
	u8* data;

	NoiseBitmap(u32 width, u32 height):width(width),height(height), data(nullptr) {};

	void GenerateNoiseBitmap(u32 size);
	glm::vec2 GenerateRandomUnitVector2();
};

static inline r32 Fade(r32 t) {
	return t * t * t * (t * (t * 6 - 15) + 10);
}

static inline r32 EaseInSine(r32 t) {
	return 1 - std::cos((t * M_PI) / 2);
}

glm::vec2 NoiseBitmap::GenerateRandomUnitVector2() {
	glm::vec2 a(-1, -1);
	glm::vec2 b(1, 1);
	return glm::linearRand(a, b);
}

static inline r32 Lerp(r32 a, r32 b, r32 t) {
	return a * (1.f - t) + b * t;
}

static inline glm::vec2 Normalize(glm::vec2 vector) {
	if (glm::length(vector) == 0.0) {
		return vector;
	}
	r32 length = glm::length(vector);

	return glm::normalize(vector);
}

glm::vec2 RandomGradient(int ix, int iy) {
	// No precomputed gradients mean this works for any number of grid coordinates
	const unsigned w = 8 * sizeof(unsigned);
	const unsigned s = w / 2; // rotation width
	unsigned a = ix, b = iy;
	a *= 3284157443; b ^= a << s | a >> w - s;
	b *= 1911520717; a ^= b << s | b >> w - s;
	a *= 2048419325;
	float random = a * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi]
	glm::vec2 v;
	v.x = sin(random); v.y = cos(random);
	return v;
}
// https://en.wikipedia.org/wiki/Perlin_noise
#if 0
void NoiseBitmap::GenerateNoiseBitmap(u32 cellSize, r32 scale) {
	u32 fullSize = width * height;
	data = new u8[fullSize];
	for (int i = 0; i < fullSize; ++i) {
		data[i] = 0;
	}

	u32 gridCellSize = cellSize;
	u32 gridWidth = width / gridCellSize;
	u32 gridHeight = height / gridCellSize;

	glm::vec2* gradientVectors = new glm::vec2[(gridWidth + 1) * (gridHeight + 1)];
	for (int y = 0; y <= gridHeight; ++y) {
		for (int x = 0; x <= gridWidth; ++x) {
			// glm::vec2 gradient = (GenerateRandomUnitVector2() * glm::vec2(2, 2)) - glm::vec2(1, 1);
			gradientVectors[x + y * gridWidth] = GenerateRandomUnitVector2();
		}
	}

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			/*
			u32 roundedX = std::floor((r32)((r32)x / (r32)gridCellSize) + 0.5);
			u32 roundedY = std::floor((r32)((r32)y / (r32)gridCellSize) + 0.5);
			glm::vec2 closestGradientVector = gradientVectors[roundedX + roundedY * gridWidth];
			*/
			glm::vec2 position(x, y);

			u32 gridCoordX0 = x / gridCellSize;
			u32 gridCoordY0 = y / gridCellSize;
			u32 gridCoordX1 = gridCoordX0 + 1;
			u32 gridCoordY1 = gridCoordY0 + 1;

			u32 x0 = (gridCoordX0) * gridCellSize;
			u32 x1 = (gridCoordX1) * gridCellSize;
			u32 y0 = (gridCoordY0) * gridCellSize;
			u32 y1 = (gridCoordY1) * gridCellSize;

			glm::vec2 corners[4] = {
				glm::vec2(x0, y0),
				glm::vec2(x1, y0),
				glm::vec2(x0, y1),
				glm::vec2(x1, y1)
			};

			glm::vec2 offsetVectors[4];
			offsetVectors[0] = Normalize(position - corners[0]);
			offsetVectors[1] = Normalize(position - corners[1]);
			offsetVectors[2] = Normalize(position - corners[2]);
			offsetVectors[3] = Normalize(position - corners[3]);

			r32 dotProducts[4];
			dotProducts[0] = glm::dot(RandomGradient(gridCoordX0, gridCoordY0), offsetVectors[0]);
			dotProducts[1] = glm::dot(RandomGradient(gridCoordX1, gridCoordY0), offsetVectors[1]);
			dotProducts[2] = glm::dot(RandomGradient(gridCoordX0, gridCoordY1), offsetVectors[2]);
			dotProducts[3] = glm::dot(RandomGradient(gridCoordX1, gridCoordY1), offsetVectors[3]);

			r32 dx = ((r32)x - (r32)x0) / (r32)gridCellSize;
			r32 dy = ((r32)y - (r32)y0) / (r32)gridCellSize;

			//dx = xx - position.x;
			//dy = yy - position.y;

			dx = Fade(dx);
			dy = Fade(dy);

			r32 xx0 = Lerp(dotProducts[0], dotProducts[1], dx);
			r32 xx1 = Lerp(dotProducts[2], dotProducts[3], dx);

			r32 result = ((Lerp(xx0, xx1, dy) + 1) / 2) * 255;
			//std::cout << result << std::endl;
			if (result < 0) { result = 0; }
			if (result > 255) { result = 255; }
			data[x + y * width] = (u8)result;
		}
	}
}
#else
void NoiseBitmap::GenerateNoiseBitmap(u32 size) {
	u32 fullSize = width * height;
	data = new u8[fullSize];
	for (int i = 0; i < fullSize; ++i) {
		data[i] = 0;
	}

	u32 stepSize = size / 2;
	r32 randomUpperLimit = 200;

	while (stepSize != 1) {
		/*
		X     Y

		   a

		Z     W
		*/

		u32 halfSize = stepSize / 2;
		for (int y = 0; y < height; y +=stepSize) {
			for (int x = 0; x < width; x += stepSize) {
				r32 X = data[x + y * width];
				r32 Y = data[x + stepSize + y * width];
				r32 Z = data[x + (y + stepSize) * width];
				r32 W = data[x + stepSize + (y + stepSize) * width];

				r32 a = (X + Y + Z + W) / 4.0 + glm::linearRand(0.0f, randomUpperLimit);
				data[x + y * width] = a;
			}
		}

		/*
			   F
	  
			X  b  Y

		 I	c  A  d  G

			Z  e  W

			   H
		*/

		for (int y = halfSize; y < height; y += halfSize) {
			for (int x = halfSize; x < width; x += halfSize) {
				r32 neighbours = 3;

				r32 X = data[x - halfSize + (y - halfSize) * width];
				r32 Y = data[x + halfSize + (y - halfSize) * width];
				r32 A = data[x + y * width];
				r32 F = 0;
				if (y - halfSize >= 0) {
					 F = data[x + (y - stepSize) * width];
					 neighbours = 4;
				}

				r32 b = (X + Y + A + F) / neighbours + glm::linearRand(0.0f, randomUpperLimit);
				data[x + (y - halfSize) * width] = b;

				neighbours = 3;
				r32 W = data[x + halfSize + (y + halfSize) * width];
				r32 G = 0;
				if (x + stepSize < width) {
					G = data[x + stepSize + y * width];
					neighbours = 4;
				}
				r32 d = (Y + A + W + G) / neighbours + glm::linearRand(0.0f, randomUpperLimit);
				data[x + halfSize + y * width] = d;

				neighbours = 3;
				r32 Z = data[x - halfSize + (y + halfSize) * width];
				r32 H = 0;
				if (y + stepSize < height) {
					H = data[x + (y + stepSize) * width];
					neighbours = 4;
				} 
				r32 e = (A + W + H + Z) / neighbours + glm::linearRand(0.0f, randomUpperLimit);
				data[x + (y + halfSize) * width] = e;

				neighbours = 3;
				r32 I = 0;
				if (x - stepSize >= 0) {
					I = data[x - stepSize + y * width];
					neighbours = 4;
				}
				r32 c = (X + A + Z + I) / neighbours + glm::linearRand(0.0f, randomUpperLimit);
			}
		}
		randomUpperLimit /= 2.0;
		stepSize /= 2;
	}
}



#endif

