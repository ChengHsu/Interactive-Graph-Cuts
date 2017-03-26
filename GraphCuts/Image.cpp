#include "Image.h"
using namespace std;
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cvaux.h>
#include <opencv2/imgproc/imgproc.hpp>




// You'll get the implementation of these functions after the pset 0 deadline
long long Image::number_of_elements()const {
	return image_data.size();
}

// Accessors and Setters
const float & Image::operator()(int x) const {
	if (x < 0 || x >= number_of_elements())
		throw OutOfBoundsException();
	return image_data[x];
}

const float & Image::operator()(int x, int y) const {
	if (x < 0 || x >= width())
		throw OutOfBoundsException();
	if (y < 0 || y >= height())
		throw OutOfBoundsException();

	return image_data[x*stride_[0] + y*stride_[1]];

}

const float & Image::operator()(int x, int y, int z) const {
	if (x < 0 || x >= width())
		throw OutOfBoundsException();
	if (y < 0 || y >= height())
		throw OutOfBoundsException();
	if (z < 0 || z >= channels())
		throw OutOfBoundsException();

	return image_data[x*stride_[0] + y*stride_[1] + stride_[2] * z];

}

float & Image::operator()(int x) {
	if (x < 0 || x >= number_of_elements())
		throw OutOfBoundsException();
	return image_data[x];
}

float & Image::operator()(int x, int y) {
	if (x < 0 || x >= width())
		throw OutOfBoundsException();
	if (y < 0 || y >= height())
		throw OutOfBoundsException();

	return image_data[x*stride_[0] + y*stride_[1]];
}

float & Image::operator()(int x, int y, int z) {
	if (x < 0 || x >= width())
		throw OutOfBoundsException();
	if (y < 0 || y >= height())
		throw OutOfBoundsException();
	if (z < 0 || z >= channels())
		throw OutOfBoundsException();

	return image_data[x*stride_[0] + y*stride_[1] + stride_[2] * z];

}




Image::Image(int x, int y, int z, const std::string &name_) {
	initImgData(x, y, z, name_);
	long long size_of_data = 1;
	for (int k = 0; k < dimensions(); k++) {
		size_of_data *= dim_values[k];
	}
	image_data = std::vector<float>(size_of_data, 0);

}

void Image::initImgData(int x, int y, int z, const std::string &name_) {
	dim_values[0] = 0;
	dim_values[1] = 0;
	dim_values[2] = 0;
	stride_[0] = 0;
	stride_[1] = 0;
	stride_[2] = 0;

	dims = 0;
	long long size_of_data = 1;
	if (x < 0)
		throw NegativeDimensionException();
	if (y< 0)
		throw NegativeDimensionException();
	if (z < 0)
		throw NegativeDimensionException();

	image_name = name_;
	dims++;
	dim_values[0] = x;
	size_of_data *= x;
	stride_[0] = 1;
	if (y > 0) {
		dims++;
		dim_values[1] = y;
		size_of_data *= y;
		stride_[1] = x;
	}
	else {
		return;
	}

	if (z>0)  {
		dims++;
		dim_values[2] = z;
		size_of_data *= z;
		stride_[2] = x*y;
	}
	else {
		return;
	}

}

Image::Image(const std::string & filename) {
	std::vector<unsigned char> uint8_image;
	unsigned int height_;
	unsigned int width_;
	unsigned int channels_ = 4;
	unsigned int outputchannels_ = 3; // Throw away transparency
	lodepng::decode(uint8_image, width_, height_, filename.c_str()); // In column major order with packed color values

	image_data = std::vector<float>(height_*width_*outputchannels_, 0);

	for (unsigned int x = 0; x < width_; x++) {
		for (unsigned int y = 0; y < height_; y++) {
			for (unsigned int c = 0; c < outputchannels_; c++) {
				image_data[x + y*width_ + c*width_*height_] = uint8_to_float(uint8_image[c + x*channels_ + y*channels_*width_]);
			}
		}
	}

	initImgData(width_, height_, outputchannels_, filename);

}

Image::~Image() { } // Nothing to clean up



void Image::write(const std::string &filename) const {
	if (channels() != 1 && channels() != 3 && channels() != 4)
		throw ChannelException();
	int png_channels = 4;
	std::vector<unsigned char> uint8_image(height()*width()*png_channels, 255);
	int c;
	for (int x = 0; x < width(); x++) {
		for (int y = 0; y < height(); y++) {
			for (c = 0; c < channels(); c++) {
				uint8_image[c + x*png_channels + y*png_channels*width()] = float_to_uint8(image_data[x + y*width() + c*width()*height()]);
			}
			for (; c < 3; c++) { // Only executes when there is one channel

				uint8_image[c + x*png_channels + y*png_channels*width()] = float_to_uint8(image_data[x + y*width() + 0 * width()*height()]);
			}
		}
	}
	lodepng::encode(filename.c_str(), uint8_image, width(), height());
}

void Image::output(int num) const {
	std::ostringstream ss;
	if (num == 1)
		ss << "./Output/foreground.png";
	else if (num == 2)
		ss << "./Output/background.png";
	std::string filename = ss.str();
	cout << "writing to " << filename << endl;
	write(filename);

}

float Image::uint8_to_float(const unsigned char &in) {
	return ((float)in) / (255.0f);
}

unsigned char Image::float_to_uint8(const float &in) {
	float out = in;
	if (out < 0)
		out = 0;
	if (out > 1)
		out = 1;
	return (unsigned char)(255.0f*out);

}


