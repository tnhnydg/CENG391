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

#include <QApplication>
#include <QScrollBar>

#include "image.h"
#include "image_window.h"

using ceng391::ImageWindow;
using ceng391::Image;

int main(int argc, char** argv)
{
        QApplication app(argc, argv);

        Image* gray = Image::new_gray(640, 640);
        gray->set_rect(160, 160, 320, 320, 255);
	
        ImageWindow* img_win = new ImageWindow("Viewer", gray);
	img_win->setFixedSize(640,640);
	QScrollBar* sb1 = new QScrollBar(Qt::Horizontal,img_win);
	QScrollBar* sb2 = new QScrollBar(Qt::Horizontal,img_win);
	sb1->setGeometry(0,620,640,20);
	sb2->setGeometry(0,0,640,20);
	sb1->setValue(50);
	img_win->show();

	QObject::connect(sb1,SIGNAL(valueChanged(int)),img_win,SLOT(changeBrightness(int))); 
	QObject::connect(sb2,SIGNAL(valueChanged(int)),img_win,SLOT(changeContrast(int)));


        //delete gray;

        return app.exec();
}








