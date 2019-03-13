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
#include "image_window.h"

#include <iostream>

using std::cerr;
using std::cout;
using std::endl;

namespace ceng391 {

static QImage* convert_to_qimage(const Image* img);
void update();

ImageWindow::ImageWindow(const QString &title, Image *img)
{
        setWindowTitle(title);

	Image* img2 = new Image(*img);

	this->orijinal_img = img2;
	this->img = img;

	m_label = new QLabel(this);
	update();
}

QImage* convert_to_qimage(const Image* img)
{
        if (img->n_ch() == 1) {
                QImage *qimg = new QImage(img->data(), img->w(), img->h(), img->step(), QImage::Format_Grayscale8);
                return qimg;
        } else {
                cerr << "[ERROR][ImageWindow] Can only load grayscale images!" << endl;
                return 0;
        }
}


void ImageWindow::changeBrightness(int value){//To change the brightness we should simply increase or decrease the values of each pixel.
	
	cout << "changeBrightness : " << value << endl;

	memcpy(this->img->data(),&this->orijinal_img->data()[0],this->img->step()*this->img->h()*sizeof(uchar));
	
	int vp = ((value-50)*255)/50;
	img->changeBrightness(vp);	
	
	update();
}

void ImageWindow::changeContrast(int value)//To increase the contrast we should increase the difference between pixel values.
{
	cout << "changeContrast : " << value << endl;

	memcpy(this->img->data(),&this->orijinal_img->data()[0],this->img->step()*this->img->h()*sizeof(uchar));

	img->changeContrast(value);

	update();
}

void ImageWindow::update()
{
	Image* img = getImg();

	if (img != 0) {
                QImage* qimg = convert_to_qimage(img);
                if (qimg != 0) {
                        m_label->setPixmap(QPixmap::fromImage(*qimg));
                        resize(img->w(), img->h());
                        delete qimg;
                }
        }	
}





}
