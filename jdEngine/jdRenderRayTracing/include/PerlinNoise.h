#pragma once
#include "jdRenderPrerequisites.h"
namespace jdEngineSDK {
		class PerlinNoise
		{
		public:
				// Initialize with the reference values for the permutation vector
				PerlinNoise();
				// Generate a new permutation vector based on the value of seed
				PerlinNoise(unsigned int seed);
				// Get a noise value, for 2D images z can have any value
				float noise(float x, float y, float z);
				void reset(unsigned int seed);
		private:
				// The permutation vector
				Vector<int> p; 
				float fade(float t);
				float lerp(float t, float a, float b);
				float grad(int hash, float x, float y, float z);
		};

}