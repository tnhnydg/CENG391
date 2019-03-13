// ------------------------------
// Written by Mustafa Ozuysal
// Contact <mustafaozuysal@iyte.edu.tr> for comments and bug reports
// ------------------------------
// Copyright (c) 2018, Mustafa Ozuysal
// All rights reserved.
// ------------------------------
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the copyright holders nor the
//       names of his/its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
// ------------------------------
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// ------------------------------
#include "image.h"

#include <iostream>
#include <fstream>

using std::ofstream;
using std::ios;
using std::cerr;
using std::string;
using std::cout;
using std::endl;

namespace ceng391 {

Image::Image(int width, int height, int n_channels, int step)
{
        m_width = width;
        m_height = height;
        m_n_channels = n_channels;

        m_step = m_width*m_n_channels;
        if (m_step < step)
                m_step = step;
        m_data = new uchar[m_step*height];
}

Image::~Image()
{
        delete [] m_data;
}

Image* Image::new_gray(int width, int height)
{
        return new Image(width, height, 1);
}

Image* Image::new_rgb(int width, int height)
{
        return new Image(width, height, 3);
}

void Image::set_rect(int x, int y, int width, int height, uchar value) //Actually, we don't need this function anymore
{
        if (x < 0) {
                x = 0;
                width += x;
        }

        if (y < 0) {
                y = 0;
                height += y;
        }

        for (int j = y; j < y+height; ++j) {
                if (j >= m_height)
                        break;
                uchar* row_data = data(j);
                for (int i = x; i < x+width; ++i) {
                        if (i >= m_width)
                                break;
                        for (int c = 0; c < m_n_channels; ++c)
                                row_data[i*m_n_channels + c] = value;
                }
        }
}

void Image::set_rect_rgb(int x,int y,int width, int height, uchar redv, uchar greenv, uchar bluev)
{

	uchar* valuelist = new uchar[3];
	if(m_n_channels == 1){
		valuelist[0] = (redv + greenv + bluev)/3;
		set_rect_imp(x,y,width,height,valuelist);
	}
	else if(m_n_channels == 3){
		valuelist[0] = redv;
		valuelist[1] = greenv;
		valuelist[2] = bluev;
		
		set_rect_imp(x,y,width,height,valuelist);
	}
	else{
		cout << "[ERROR] : The images with number of channels other than 1 and 3 are not allowed." << endl;
	}
}

void Image::set_rect_imp(int x,int y, int width, int height, uchar* valuelist)
{
	if (x < 0) {
                x = 0;
                width += x;
        }

        if (y < 0) {
                y = 0;
                height += y;
        }

        for (int j = y; j < y+height; ++j) {
                if (j >= m_height)
                        break;
                uchar* row_data = data(j);
                for (int i = x; i < x+width; ++i) {
                        if (i >= m_width)
                                break;
                        for (int c = 0; c < m_n_channels; ++c)
                                row_data[i*m_n_channels + c] = valuelist[c];
                }
        }
}

bool Image::write_pnm_imp(const std::string& filename, const std::string& magic_head, const std::string& fileExtension) const
{
	ofstream fout;

	string extended_name = filename + fileExtension;
	fout.open(extended_name.c_str(), ios::out | ios::binary);
	fout << magic_head << "\n";
	fout << m_width << " " << m_height << " 255\n";
	for (int y = 0; y < m_height; ++y) {
                	const uchar *row_data = data(y);   
                	fout.write(reinterpret_cast<const char*>(row_data), m_n_channels*m_width*sizeof(uchar));
        }
        fout.close();
	
	return true;
}

bool Image::write_pnm(const std::string& filename) const
{

	
        ofstream fout;

	if (m_n_channels == 1) {
		cout << "PGM" << endl;
		write_pnm_imp(filename,"P5",".pgm");
	}
	else if (m_n_channels == 3){
		cout << "PPM" << endl;
		write_pnm_imp(filename,"P6",".ppm");
	}
	else{}
	
	return true;
}


}
