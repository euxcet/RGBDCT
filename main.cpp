#include <boost/gil/extension/io/jpeg_io.hpp>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>

using namespace std;

#define WIDTH 512
#define HEIGHT 424


int height, width;

struct Point {
	int r, g, b, d;
}point[600][600];

unsigned char r[WIDTH * HEIGHT];
unsigned char g[WIDTH * HEIGHT];
unsigned char b[WIDTH * HEIGHT];


class Compressor {
public:
	Compressor() { r = g = b = NULL; }
	Compressor(const unsigned char* _r, const unsigned char* _g, const unsigned char* _b) {
		r = new unsigned char[WIDTH * HEIGHT * sizeof(unsigned char)];
		g = new unsigned char[WIDTH * HEIGHT * sizeof(unsigned char)];
		b = new unsigned char[WIDTH * HEIGHT * sizeof(unsigned char)];
		for(int i = 0; i < WIDTH * HEIGHT; i++)  {
			r[i] = _r[i], g[i] = _g[i], b[i] = _b[i];
		}
	}
	~Compressor() {
		delete r;
		delete g;
		delete b;
	}
	void load(const unsigned char* _r, const unsigned char* _g, const unsigned char* _b) {
		if (r == NULL) {
			r = new unsigned char[WIDTH * HEIGHT * sizeof(unsigned char)];
		}
		if (g == NULL) {
			g = new unsigned char[WIDTH * HEIGHT * sizeof(unsigned char)];
		}
		if (b == NULL) {
			b = new unsigned char[WIDTH * HEIGHT * sizeof(unsigned char)];
		}
		for(int i = 0; i < WIDTH * HEIGHT; i++)  {
			r[i] = _r[i], g[i] = _g[i], b[i] = _b[i];
		}
	}
	void compress_with_boost(const char* outfile) {
		cout << outfile << endl;
		boost::gil::rgb8c_planar_view_t view =
			boost::gil::planar_rgb_view(WIDTH, HEIGHT, r, g, b, WIDTH);

		boost::gil::jpeg_write_view(outfile, view);
	}
	void compress() {
	}
private:
	unsigned char* r;
	unsigned char* g;
	unsigned char* b;
};


class Decompressor {
};


Compressor *compressor;

void draw(const char* infile, const char* outfile) {
	ifstream f(infile);
	f >> height >> width;
	for(int i = 0; i < HEIGHT; i++) {
		for(int j = 0; j < WIDTH; j++) {
			int pos = i * width + j;
			int R,G,B;
			f >> point[0][0].d >> R >> G >> B;
			r[pos] = R; g[pos] = G; b[pos] = B;
		}
	}
	compressor -> load(r, g, b);
	compressor -> compress_with_boost(outfile);
}

int main() {
	compressor = new Compressor();
	for(int i = 1001; i <= 1217; i++) {
		char data[20] = "../data/0000.txt";
		char img[20] = "../depth/0000.ppm";
		int p = i, j = 11;
		while (p) {
			data[j--] = '0' + p % 10;
			img[j + 2] = '0' + p % 10;
			p /= 10;
		}
		//draw(data, img);
		draw(data, "1.ppm");
		break;
	}
}
//230ms rgb to jpg
