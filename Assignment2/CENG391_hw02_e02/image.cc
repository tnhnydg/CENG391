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
#include "math.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>

using std::ofstream;
using std::ios;
using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::fopen;
using std::fputc;
using std::exit;
using std::fclose;
using std::fscanf;
using std::fread;

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

void Image::set_rect(int x, int y, int width, int height, uchar value)
{
        if (x < 0) {
                width += x;
                x = 0;
        }

        if (y < 0) {
                height += y;
                y = 0;
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

bool Image::write_pnm(const std::string& filename) const
{
        if (m_n_channels != 1) {
                cerr << "[ERROR][CENG391::Image] Currently only grayscale images can be saved as PNM files!\n";
                return false;
        }

        const string magic_head = "P5";
        ofstream fout;
        string extended_name = filename + ".pgm";
        fout.open(extended_name.c_str(), ios::out | ios::binary);
        fout << magic_head << "\n";
        fout << m_width << " " << m_height << " 255\n";
        for (int y = 0; y < m_height; ++y) {
                const uchar *row_data = data(y);
                fout.write(reinterpret_cast<const char*>(row_data), m_width*sizeof(uchar));
        }
        fout.close();

        return true;
}

Image* Image::read_pnm(const std::string& filename)
{
        FILE *pnm = fopen(filename.c_str(), "rb");
        if (!pnm) {
                fprintf(stderr, "Could not open image file %s\n", filename.c_str());
                exit(EXIT_FAILURE);
        }

        char ch1, ch2;
        if (fscanf(pnm, "%c%c", &ch1, &ch2) != 2) {
                fprintf(stderr, "Could not read image header from %s\n", filename.c_str());
                exit(EXIT_FAILURE);
        }

        if(ch1 != 'P' || (ch2 != '5' && ch2 != '6')) {
                fprintf(stderr, "Image %s is not a valid binary PGM or PPM file", filename.c_str());
                exit(EXIT_FAILURE);
        }

        int n_ch = -1;
        if(ch2 == '5') {
                n_ch = 1;
        } else if(ch2 == '6') {
                n_ch = 3;
        }

        int c = fgetc(pnm);
        while (c == ' ' || c == '\n' || c == '\t') {
                c = fgetc(pnm);
        }

        while (c == '#') {
                while (c != '\n') {
                        c = fgetc(pnm);
                        if (c == EOF) {
                                fprintf(stderr, "%s does not contain image data", filename.c_str());
                                exit(EXIT_FAILURE);
                        }
                }
                c = fgetc(pnm);
        }
        ungetc(c, pnm);

        int pnm_width;
        int pnm_height;
        int pnm_levels;
        if (fscanf(pnm, "%d %d %d", &pnm_width, &pnm_height, &pnm_levels) != 3) {
                fprintf(stderr, "Could not read image attributes from %s", filename.c_str());
                exit(EXIT_FAILURE);
        }
        c = fgetc(pnm);

        Image* img = new Image(pnm_width, pnm_height, n_ch);
        for (int y = 0; y < img->m_height; ++y) {
                uchar* row = img->data(y);
                if (fread((void*)row, sizeof(uchar), img->m_width*n_ch, pnm) != img->m_width*n_ch) {
                        fprintf(stderr, "Could not read data line %d from %s", y, filename.c_str());
                        exit(EXIT_FAILURE);
                }
        }
        fclose(pnm);

        return img;
}

Image* Image::scaleup_nn(int scale)
{
	Image* n_img = Image::new_gray(w()*scale, h()*scale);
	double ratio = 1.0/scale;
	double x,y;
	for(int j=0;j<h()*scale;++j){
		for(int i=0;i<w()*scale;++i){
			x = int(j*ratio);
			y = int(i*ratio); 
			n_img->data()[ j*w()*scale + i] = data()[ (int) (x*w() + y) ]; 
		}	
	}

	return n_img;	
}

Image* Image::scaleup_bilinear_helper(Image* img,int scale)
{
	Image* n_img = Image::new_gray((img->w())*scale, (img->h())*scale);

	n_img->set_zero();
	int A,B,C,D;
	A = img->data()[0];
	B = img->data()[1];
	C = img->data()[2];
	D = img->data()[3];

	float x_factor = 1 / (((float)(img->w()))*scale-1);
	float y_factor = 1 / (((float)(img->h()))*scale-1);

	for(int j=0;j<(img->h())*scale;++j){
		for(int i=0;i<(img->w())*scale;++i){

			float x_dif = i * x_factor;
			float y_dif = j * y_factor;		
			
			int formula = A*(1-x_dif)*(1-y_dif) + B*(x_dif)*(1-y_dif) + C*(y_dif)*(1-x_dif) + D*(x_dif)*(y_dif);
			n_img->data()[ j*(img->w())*scale + i] = formula; 
		}	
	}

	return n_img;

}

Image* Image::scaleup_bilinear_oldversion(int scale) //Completely my own solution but has little deficiency
{
	Image* scaled_img = Image::new_gray(w()*scale,h()*scale);
	scaled_img->set_zero();

	for(int j=0;j<h()/2;++j){
		for(int i=0;i<w()/2;++i){
			Image* n_img = Image::new_gray(2, 2);
			n_img->data()[0] = data()[j*w()*2 + i*2];
			n_img->data()[1] = data()[j*w()*2 + i*2 + 1];
			n_img->data()[2] = data()[j*w()*2 + i*2 + w()];
			n_img->data()[3] = data()[j*w()*2 + i*2 + w() + 1]; 
			Image* img_p = scaleup_bilinear_helper(n_img,scale);

			for(int l =0; l<scale*2;l++)
			{
				for(int k=0;k<scale*2;++k)
				{
					scaled_img->data()[(w()/2)*scale*l+k] = img_p->data()[l*scale*2 + k];
					//incomplete 	
				}
			}	
		}	
	}

	return scaled_img;

}

Image* Image::scaleup_bilinear(int scale) // I implemented this method after studied example codes from the internet
{
	int new_width = w()*scale;
	int new_height = h()*scale;	

	int rowGroupId,clmGroupId,index,A,B,C,D,formula;
	float x_dist_f,y_dist_f,x_dif,y_dif;
	
	Image* new_img = Image::new_gray(new_width, new_height);

	float nw = w()-1; //To compute the distance factor 
	float nh = h()-1;

	float x_factor = nw / new_width;
	float y_factor = nh / new_height;

	int count = 0;
	for (int j=0;j<new_height;++j){
		for(int i=0;i<new_width;++i){
			x_dist_f = (x_factor * i);
			y_dist_f = (y_factor * j);
			rowGroupId = (int)x_dist_f;
			clmGroupId = (int)y_dist_f;
			x_dif    = x_dist_f - rowGroupId;
			y_dif    = y_dist_f - clmGroupId; 
			index = clmGroupId * w() + rowGroupId; // pixel index from the old image

			A = data()[index];
			B = data()[index + 1];
			C = data()[index + w()];
			D = data()[index + w() + 1];

			formula = (int) A*(1-x_dif)*(1-y_dif) + B*(x_dif)*(1-y_dif) + C*(y_dif)*(1-x_dif) + D*(x_dif)*(y_dif);
			new_img->data()[count++] = formula;


		}		
	}

	return new_img;

}

/*int Image::new_coordinate(int x, int y,float angle,char select)
{
	float radian = angle * M_PI / 180.0;
	if(select == 'x'){
		return x*cos(radian) + y*sin(radian);
	}
	else if(select == 'y'){
		return -x*sin(radian) + y*cos(radian);
	}
}*/ 

int Image::new_dimension(int width, int height,float angle,char select)
{
	float radian = angle * M_PI / 180.0;
	if(select == 'w'){
		return ceil(abs(width*cos(radian)) + abs(height*sin(radian)));
	}
	else if(select == 'h'){
		return	ceil(abs(width*sin(radian)) + abs(height*cos(radian)));
	}
}

float Image::rotationMatrix(int x, int y, int w, int h, float angle,char select)
{
	float radian = angle * M_PI / 180.0;
	if(select == 'x'){
		return ((x-w/2)*cos(radian))-((y-h/2)*sin(radian));
	}
	else if(select == 'y'){
		return ((x-w/2)*sin(radian))+((y-h/2)*cos(radian));
	}
}

Image* Image::rotate_bilinear(float angle)// I implemented this method after studied example codes from the internet
{
	Image* new_img = Image::new_gray(w(), h());
	float radian = angle * M_PI / 180.0;

	int w_half = w()/2;//To shift the image
	int h_half = h()/2;
	
	int A,B,C,D,index;
	float p1,p2,pavg,new_x,new_y,x,y,rounded_x,rounded_y;

	for(int j=0;j<h();j++){
    		for(int i=0;i<w();i++){
        		
			new_x = rotationMatrix(i,j,w(),h(),angle,'x') + w_half;
			new_y = rotationMatrix(i,j,w(),h(),angle,'y') + h_half;

			rounded_x = floor(new_x);
			rounded_y = floor(new_y);

			x = (int)floor(new_x);
			y = (int)floor(new_y);

			index = new_y * w() + new_x; 

			if(new_x > -1 && new_x < w()-1)
			{
				if(new_y > -1 && new_y < h()-1)
				{
					A = data()[(int)(x+(w()*y))];   
					B = data()[(int)(x+(w()*(y+1)))];
					C = data()[(int)((x+1)+(w()*y))];
					D = data()[(int)((x+1)+(w()*(y+1)))];

           				p1 = A + (C-A)*(new_x-rounded_x);
            			  	p2 = B + (D-B)*(new_x-rounded_x);
            			  	pavg = p1 + (p2-p1)*(new_y-rounded_y);
            				new_img->data()[i+(w()*j)]= pavg;
				}
			} 
    		}
	}
	return new_img;
}


Image* Image::rotate_full_bilinear(float angle)
{
	int new_width = new_dimension(w(),h(),angle,'w');
	int new_height = new_dimension(w(),h(),angle,'h');

	Image* new_img = Image::new_gray(new_width, new_height);

	float radian = angle * M_PI / 180.0;

	int w_half = new_width/2;//To shift the image
	int h_half = new_height/2;
	
	int A,B,C,D,index;
	float p1,p2,pavg,new_x,new_y,x,y,rounded_x,rounded_y;

	for(int j=0;j<new_height;j++){
    		for(int i=0;i<new_width;i++){
        		
			new_x = rotationMatrix(i,j,new_width,new_height,angle,'x') + w_half;
			new_y = rotationMatrix(i,j,new_width,new_height,angle,'y') + h_half;

			rounded_x = floor(new_x);
			rounded_y = floor(new_y);

			x = (int)floor(new_x);
			y = (int)floor(new_y);

			index = new_y * new_width + new_x; 
			
					
			if(new_x > -1 && new_x < new_width-1)
			{
				if(new_y > -1 && new_y < new_height-1)
				{
					A = data()[(int)(x+(new_width*y))];   
					B = data()[(int)(x+(new_width*(y+1)))];
					C = data()[(int)((x+1)+(new_width*y))];
					D = data()[(int)((x+1)+(new_width*(y+1)))];

           				p1 = A + (C-A)*(new_x-rounded_x);
            			  	p2 = B + (D-B)*(new_x-rounded_x);
            			  	pavg = p1 + (p2-p1)*(new_y-rounded_y);
            				new_img->data()[i+(new_width*j)]= pavg;
				}
			} 
    		}
	}
	return new_img;

}




}
