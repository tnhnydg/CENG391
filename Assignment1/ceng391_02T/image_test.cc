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
#include <cstdlib>
#include <iostream>

#include "image.h"

using std::cout;
using std::endl;
using ceng391::Image;

Image* create_bayer();

int main(int argc, char** argv)
{
        Image* gray = Image::new_gray(128, 128);
        cout << "(" << gray->w() << "x" << gray->h() << ") channels: "
             << gray->n_ch() << " step: " << gray->step() << endl;
        gray->set_zero();
	gray->set_rect_rgb(32,32,64,64,255,255,255);
        //gray->set_rect(32, 32, 64, 64, 120);
        gray->write_pnm("/tmp/test_image");
        delete gray;
	
	Image* bayer = create_bayer();
	bayer->write_pnm("/tmp/bayer");
	
	

        return EXIT_SUCCESS;
}

Image* create_bayer()
{
	Image* bayer = Image::new_rgb(256,256);

	for(int k=0;k<16;k++){
		for(int i=0;i<16;i++){
			bayer->set_rect_rgb(i*16,k*16,8,8,255,0,0);
			bayer->set_rect_rgb(i*16+8,k*16,8,8,0,255,0);
		}

		for(int j=0;j<16;j++){
			bayer->set_rect_rgb(j*16,k*16+8,8,8,0,255,0);
			bayer->set_rect_rgb(j*16+8,k*16+8,8,8,0,0,255);
		}
	}

	return bayer;
}

