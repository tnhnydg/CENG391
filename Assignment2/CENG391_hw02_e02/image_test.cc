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

int main(int argc, char** argv)
{
        Image* img = Image::read_pnm("house.pgm");
	Image* img2 = img->rotate_full_bilinear(45);
	/*Image* img3 = img->rotate(90);
	Image* img4 = img->rotate(135);
	Image* img5 = img->rotate(180);
	Image* img6 = img->rotate(360);*/
	//Image* img3 = img->scaleup_bilinear(4);
	img2->write_pnm("/tmp/img45");
	/*img3->write_pnm("/tmp/img90");
	img4->write_pnm("/tmp/img135");
	img5->write_pnm("/tmp/img180");
	img6->write_pnm("/tmp/img360");*/
	//img3->write_pnm("/tmp/img3");

	/*Image* img = Image::new_gray(640,640);
	img->set_zero();
	img->set_rect(0,0,320,320,250);
	img->set_rect(320,0,640,320,180);
	img->set_rect(0,320,320,640,90);
	img->set_rect(320,320,640,640,30);
	img->write_pnm("/tmp/img");*/

        delete img;

        return EXIT_SUCCESS;
}

