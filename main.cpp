#include <iostream>
#include <vector>
#include <string_view>
#include <fstream>
#include "lodepng.h"
#include <cstring>
#include <mutex>
#include <random>
#include <thread>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>
#define MAX_THREAD 4

#define PI 3.14159265
using namespace std;

using namespace cv;
std::mutex myMutex;


std::vector<unsigned char> decode(string_view filename,
    unsigned int& width, unsigned int& height) {

    std::vector<unsigned char> result;
    unsigned error = lodepng::decode(result, width, height, filename.data());
    if (error) {
        std::cerr << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
    }
    return result;
}


void encode_filter_red(string_view filename, std::vector<unsigned char>& image, unsigned width,unsigned h, unsigned height) {
    for (size_t i = h; i < height; i++) {
        for (size_t j = 0; j < width * 4; j += 4) {
            image[i * width * 4 + j + 0]; // Red component
            image[i * width * 4 + j + 1]=0; // Green component
            image[i * width * 4 + j + 2]=0; // Blue component
            image[i * width * 4 + j + 3]; // Alpha component

        }

        unsigned error = lodepng::encode(filename.data(), image, width, height);
        if (error) {
            std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
        }
    }
}

void encode_filter_tr(string_view filename, std::vector<unsigned char>& image, unsigned width, unsigned height) {
    unsigned int h1 = (height/4);
    unsigned int h2 = (height/2);
    unsigned int h3 = ((3*height)/4);
    thread t1(encode_filter_red,filename,std::ref(image),width,0,h1);
    thread t2(encode_filter_red,filename,std::ref(image),width,h1,h2);
    thread t3(encode_filter_red,filename,std::ref(image),width,h2,h3);
    thread t4(encode_filter_red,filename,std::ref(image),width,h3,height);
    t1.join(); t2.join(); t3.join(); t4.join();

}

void encode_filter_green(string_view filename, std::vector<unsigned char>& image, unsigned width,unsigned h, unsigned height) {
    for (size_t i = h; i < height; i++) {
        for (size_t j = 0; j < width * 4; j += 4) {
            image[i * width * 4 + j + 0]=0; // Red component
            image[i * width * 4 + j + 1]; // Green component
            image[i * width * 4 + j + 2]=0; // Blue component
            image[i * width * 4 + j + 3]; // Alpha component

        }

        unsigned error = lodepng::encode(filename.data(), image, width, height);
        if (error) {
            std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
        }
    }
}
void encode_filter_tg(string_view filename, std::vector<unsigned char>& image, unsigned width, unsigned height) {
    unsigned int h1 = (height/4);
    unsigned int h2 = (height/2);
    unsigned int h3 = ((3*height)/4);
    thread t1(encode_filter_green,filename,std::ref(image),width,0,h1);
    thread t2(encode_filter_green,filename,std::ref(image),width,h1,h2);
    thread t3(encode_filter_green,filename,std::ref(image),width,h2,h3);
    thread t4(encode_filter_green,filename,std::ref(image),width,h3,height);
    t1.join(); t2.join(); t3.join(); t4.join();

}



void encode_filter_blue(string_view filename, std::vector<unsigned char>& image, unsigned width,unsigned h, unsigned height) {

    for (size_t i = h; i < height; i++) {
        for (size_t j = 0; j < width * 4; j += 4) {
            image[i * width * 4 + j + 0]=0; // Red component
            image[i * width * 4 + j + 1]=0; // Green component
            image[i * width * 4 + j + 2]; // Blue component
            image[i * width * 4 + j + 3]; // Alpha component

        }

        unsigned error = lodepng::encode(filename.data(), image, width, height);
        if (error) {
            std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
        }
    }
}

void encode_filter_tb(string_view filename, std::vector<unsigned char>& image, unsigned width, unsigned height) {
    unsigned int h1 = (height/4);
    unsigned int h2 = (height/2);
    unsigned int h3 = ((3*height)/4);
    thread t1(encode_filter_blue,filename,std::ref(image),width,0,h1);
    thread t2(encode_filter_blue,filename,std::ref(image),width,h1,h2);
    thread t3(encode_filter_blue,filename,std::ref(image),width,h2,h3);
    thread t4(encode_filter_blue,filename,std::ref(image),width,h3,height);
    t1.join(); t2.join(); t3.join(); t4.join();

}
template <typename T, typename  P>
void rotar_Imagen(string_view filename, vector<T> imagen, P width, P height) {
    vector<unsigned char> temporal;
    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width * 4; j += 4) {
            temporal.push_back(imagen[(i * width * 4 + j + 0) * cos(45)]); // Red component
            temporal.push_back(imagen[(i * width * 4 + j + 1) * cos(45)]); // Green component
            temporal.push_back(imagen[(i * width * 4 + j + 2) * cos(45)]); // Blue component
            temporal.push_back(imagen[(i * width * 4 + j + 3) * sin(45)]); // Opacidad
        }
    }
    unsigned error = lodepng::encode(filename.data(), imagen, width, height);
    if (error) {
        cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
    }
}




int main()
{

    unsigned int w;
    unsigned int h;
    string path="../in.png";
    ifstream input1(path);
    auto image = decode(path, w, h);
    // Un comment if you want to check buffer content
    /*for (size_t i = 0; i < h; i++) {
        for (size_t j = 0; j < w * 4; j += 4) {
            int r = image[0*(i * w * 4 + j + 0)]; // Red component
            int g = image[(i * w * 4 + j + 1)*0]; // Green component
            int b = image[i * w * 4 + j + 2]; // Blue component
            int a = image[i * w * 4 + j + 3]; // Alpha component
            std::cout << r << " ";
            std::cout << g << " ";
            std::cout << b << " ";
            std::cout << a << "|";
        }
        std::cout << endl;
    }
*/
    //encode_filter_red("../out2.png", image, w, h);
    //encode_filter_green("../out3.png", image, w, h);
    //encode_filter_blue("../out4.png", image, w, h);

    //encode_filter_tr("../out4.png", image, w, h);
    //encode_filter_tg("../out4.png", image, w, h);
    //encode_filter_tb("../out4.png", image, w, h);

    rotar_Imagen("../rotated.png",image,w,h);



    return 0;


};
