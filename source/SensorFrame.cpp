// SensorFrame.cpp
// Copyright (c) 2013 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

#include "SoundplaneModelA.h"
#include "SensorFrame.h"
#include <iostream>
#include <cmath>

template <class c>
c (clamp)(const c& x, const c& min, const c& max)
{
	return (x < min) ? min : (x > max ? max : x);
}

template <class c>
inline bool (within)(const c& x, const c& min, const c& max)
{
	return ((x >= min) && (x < max));
}

float get(const SensorFrame& a, int col, int row)
{
	return a[row*SensorGeometry::width + col];
}

void set(SensorFrame& a, int col, int row, float val)
{
	a[row*SensorGeometry::width + col] = val;
}

float getColumnSum(const SensorFrame& a, int col)
{
	float sum = 0.f;
	for(int j=0; j<SensorGeometry::height; ++j)
	{
		sum += get(a, col, j);
	}
	return sum;
}

SensorFrame add(const SensorFrame& a, const SensorFrame& b)
{
	SensorFrame out;
	for(int i=0; i<SensorGeometry::elements; ++i)
	{
		out[i] = a[i] + b[i];
	}
	return out;
}

SensorFrame subtract(const SensorFrame& a, const SensorFrame& b)
{
	SensorFrame out;
	for(int i=0; i<SensorGeometry::elements; ++i)
	{
		out[i] = a[i] - b[i];
	}
	return out;
}

SensorFrame multiply(const SensorFrame& a, const SensorFrame& b)
{
	SensorFrame out;
	for(int i=0; i<SensorGeometry::elements; ++i)
	{
		out[i] = a[i]*b[i];
	}
	return out;
}

SensorFrame divide(const SensorFrame& a, const SensorFrame& b)
{
	SensorFrame out;
	for(int i=0; i<SensorGeometry::elements; ++i)
	{
		out[i] = a[i]/b[i];
	}
	return out;
}

SensorFrame add(const SensorFrame& a, const float k)
{
	SensorFrame out;
	for(int i=0; i<SensorGeometry::elements; ++i)
	{
		out[i] = a[i] + k;
	}
	return out;
}

SensorFrame subtract(const SensorFrame& a, const float k)
{
	SensorFrame out;
	for(int i=0; i<SensorGeometry::elements; ++i)
	{
		out[i] = a[i] - k;
	}
	return out;
}

SensorFrame multiply(const SensorFrame& a, const float k)
{
	SensorFrame out;
	for(int i=0; i<SensorGeometry::elements; ++i)
	{
		out[i] = a[i]*k;
	}
	return out;
}

SensorFrame divide(const SensorFrame& a, const float k)
{
	SensorFrame out;
	for(int i=0; i<SensorGeometry::elements; ++i)
	{
		out[i] = a[i]/k;
	}
	return out;
}

SensorFrame fill(const float k)
{
	SensorFrame out;
	for(int i=0; i<SensorGeometry::elements; ++i)
	{
		out[i] = k;
	}
	return out;
}

SensorFrame max(const SensorFrame& b, const float k)
{
	SensorFrame out;
	for(int i=0; i<SensorGeometry::elements; ++i)
	{
		out[i] = std::max(b[i], k);
	}
	return out;
}

SensorFrame min(const SensorFrame& b, const float k)
{
	SensorFrame out;
	for(int i=0; i<SensorGeometry::elements; ++i)
	{
		out[i] = std::min(b[i], k);
	}
	return out;
}

SensorFrame clamp(const SensorFrame& b, const float k, const float m)
{
	SensorFrame out;
	for(int i=0; i<SensorGeometry::elements; ++i)
	{
		out[i] = clamp(b[i], k, m);
	}
	return out;
}

SensorFrame sqrt(const SensorFrame& b)
{
	SensorFrame out;
	for(int i=0; i<SensorGeometry::elements; ++i)
	{
		out[i] = sqrtf(b[i]);
	}
	return out;
}

SensorFrame getCurvatureX(const SensorFrame& in)
{
    SensorFrame out{};
	
	// rows
	for(int j=0; j<SensorGeometry::height; ++j)
	{
		float z = 0.f;
		float zm1 = 0.f;
		float dz = 0.f;
		float dzm1 = 0.f;
		float ddz = 0.f;
		
		for(int i=0; i <= SensorGeometry::width; ++i)
		{
			if(within(i, 0, SensorGeometry::width))
			{
				z = in[j*SensorGeometry::width + i];
			}
			else
			{
				z = 0.f;
			}
			dz = z - zm1;
			ddz = dz - dzm1;					
			zm1 = z;
			dzm1 = dz;
			
			if(i >= 1)
			{
				out[(j)*SensorGeometry::width + i - 1] = std::max(-ddz, 0.f);
			}
		}
	}	
	
	return out;
}

SensorFrame getCurvatureY(const SensorFrame& in)
{
    SensorFrame out{};
	
	// cols
	for(int i=0; i<SensorGeometry::width; ++i)
	{
		float z = 0.f;
		float zm1 = 0.f;
		float dz = 0.f;
		float dzm1 = 0.f;
		float ddz = 0.f;
		for(int j=0; j <= SensorGeometry::height; ++j)
		{
			if(within(j, 0, SensorGeometry::height))
			{
				z = in[j*SensorGeometry::width + i];
			}
			else
			{
				z = 0.f;
			}
			
			dz = z - zm1;
			ddz = dz - dzm1;					
			zm1 = z;
			dzm1 = dz;
			if(j >= 1)
			{
				out[(j - 1)*SensorGeometry::width + i] = std::max(-ddz, 0.f);
			}
		}
	}
	
	return out;
}

SensorFrame getCurvatureXY(const SensorFrame& in)
{
	return sqrt(multiply(getCurvatureX(in), getCurvatureY(in)));
} 

SensorFrame calibrate(const SensorFrame& in, const SensorFrame& calibrateMean)
{
	return subtract(divide(in, calibrateMean), 1.0f);
} 

void dumpFrameAsASCII(std::ostream& s, const SensorFrame& f)
{
	const char* g = " .:;+=xX$&";
	int w = SensorGeometry::width;
	int h = SensorGeometry::height;
	
	int scale = static_cast<int>(strlen(g));
	for (int j=0; j<h; ++j)
	{
		s << "|";
		for(int i=0; i<w; ++i)
		{
			int v = (f[j*w + i]*scale);
			s << g[clamp(v, 0, scale - 1)];
		}
		s << "|\n";
	}
}

void dumpFrame(std::ostream& s, const SensorFrame& f)
{
	int w = SensorGeometry::width;
	int h = SensorGeometry::height;
	
	for (int j=0; j<h; ++j)
	{
		for(int i=0; i<w; ++i)
		{
			float v = f[j*w + i];
			
			printf("%2.2f ", v);

		}
		std::cout << "\n";
	}
}

void dumpFrameStats(std::ostream& s, const SensorFrame& f)
{
	int w = SensorGeometry::width;
	int h = SensorGeometry::height;
	
	float min = MAXFLOAT;
	float max = -min;
	float sum = 0.f;
	for (int j=0; j<h; ++j)
	{
		for(int i=0; i<w; ++i)
		{
			float v = f[j*w + i];
			
			sum += v;
			min = std::min(min, v);
			max = std::max(max, v);
		}
	}
	s << "min: " << min << " max: " << max << " sum: " << sum << "\n";
}

// set data from edge carriers, unused on Soundplane A, to duplicate
// actual data nearby.
void clearFrameEdges(SensorFrame& dest)
{
	float *pDest = dest.data();
	float *pDestRow;
	for(int i=0; i<kSoundplaneAPickupsPerBoard; ++i)
	{
		pDestRow = pDest + kSoundplaneANumCarriers*2*i;
		const float zl = pDestRow[2];
		pDestRow[1] = zl;
		pDestRow[0] = 0;
		const float zr = pDestRow[kSoundplaneANumCarriers*2 - 3];
		pDestRow[kSoundplaneANumCarriers*2 - 2] = zr;
		pDestRow[kSoundplaneANumCarriers*2 - 1] = 0;
	}
}

// return difference between the sums of first rows of each frame.
float firstRowFrameDiff(const SensorFrame& p0, const SensorFrame& p1)
{
	float sum = 0.f;
	for(int i = 0; i < SensorGeometry::width; i++)
	{
		sum += fabs(p1[i] - p0[i]);
	}
	return sum;
}


// --------------------------------------------------------------------------------
#pragma mark unpacking data

// combine two surface payloads to a single frame of floating point pressure values.
//
void rawPayloadsToFrame(unsigned char *pSrc0, unsigned char *pSrc1, SensorFrame& dest)
{
	float *pDest = dest.data();
	unsigned short a, b;
	float *pDestRow0, *pDestRow1;

	// evey three bytes of payload provide 24 bits,
	// which is two 12-bit magnitude values packed
	// ml Lh HM
	//
	int c = 0;
	for(int i=0; i<kSoundplaneAPickupsPerBoard; ++i)
	{
		pDestRow0 = pDest + kSoundplaneANumCarriers*2*i;
		pDestRow1 = pDestRow0 + kSoundplaneANumCarriers;
		for (int j = 0; j < kSoundplaneANumCarriers; j += 2)
		{
			a = pSrc0[c+1] & 0x0F;  // 000h
			a <<= 8;        // 0h00
			a |= pSrc0[c];      // 0hml
			pDestRow0[j] = a / 4096.f;

			a = pSrc0[c+2];     // 00HM
			a <<= 4;        // 0HM0
			a |= ((pSrc0[c+1] & 0xF0) >> 4);  // 0HML
			pDestRow0[j + 1] = a / 4096.f;

			// flip surface 2

			b = pSrc1[c+1] & 0x0F;  // 000h
			b <<= 8;        // 0h00
			b |= pSrc1[c];      // 0hml
			pDestRow1[kSoundplaneANumCarriers - 1 - j] = b / 4096.f;

			b = pSrc1[c+2];     // 00HM
			b <<= 4;        // 0HM0
			b |= ((pSrc1[c+1] & 0xF0) >> 4);  // 0HML
			pDestRow1[kSoundplaneANumCarriers - 2 - j] = b / 4096.f;

			c += 3;
		}
	}
}

