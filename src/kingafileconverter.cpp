/*
   Copyright (C) 2012  Gianluca Merlo <gianluca.merlo@gmail.com>

   This file is part of Kinga.

   Kinga is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Kinga is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Kinga.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __KINGA_FILECONVERTER_CPP__
#define __KINGA_FILECONVERTER_CPP__

#include "kingafileconverter.hpp"

KingaFileConverter::KingaFileConverter(QStringList filelist,
    unsigned int width, unsigned int height, unsigned int density_x,
    unsigned int density_y, unsigned int colors, QWidget* parent) : QWidget(parent)
{
    FileList = filelist;
    Width = width;
    Height = height;
    DensityX = density_x;
    DensityY = density_y;
    Colors = colors;
}

void KingaFileConverter::Run(void)
{
    QProgressDialog* progress_dialog = new QProgressDialog(tr("Converting files..."),tr("Cancel"),0,FileList.size()-1);
    progress_dialog->setWindowTitle(tr("Converting..."));
    progress_dialog->setWindowModality(Qt::WindowModal);
    connect(this,SIGNAL(FileConverted(int)),progress_dialog,SLOT(setValue(int)));
    connect(this,SIGNAL(FileConverted(QString)),progress_dialog,SLOT(setLabelText(QString)));
    for(int i=0;i<FileList.size();i++){
        try {
            Image image;
            image.read(qPrintable(FileList.at(i)));
            image.density(Geometry(DensityX,DensityY));
            if( image.columns() > image.rows() )
                image.rotate(90);
            image.quantizeColorSpace(GRAYColorspace);
            image.quantizeColors(Colors);
            image.quantize();
            image.resize(Geometry(Width,Height));
            QString outputname;
            QTextStream outputstream(&outputname);
            outputstream << qSetFieldWidth(4) << qSetPadChar(QChar('0')) << i << ".png";
            image.write(qPrintable(outputname));
            emit FileConverted(i);
            QFileInfo fileinfo(FileList.at(i));
            emit FileConverted(fileinfo.fileName());
            if( progress_dialog->wasCanceled() ) break;
        } catch( Error& magick_error ) {
            QMessageBox::critical(this,tr("Error"),magick_error.what(),QMessageBox::Ok,QMessageBox::Ok);
            break;
        }
    }
    emit FileConverted(FileList.size()-1);
    if(progress_dialog->wasCanceled()) {
        QMessageBox::warning(this,tr("Kinga"),tr("Conversion canceled"),QMessageBox::Ok,QMessageBox::Ok);
    } else {
        QMessageBox::information(this,tr("Kinga"),tr("Conversion successful"),QMessageBox::Ok,QMessageBox::Ok);
    }
}

#endif
