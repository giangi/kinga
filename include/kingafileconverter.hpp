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

#ifndef __KINGA_FILECONVERTER_HPP__
#define __KINGA_FILECONVERTER_HPP__

#include <QChar>
#include <QFileInfo>
#include <QMessageBox>
#include <QObject>
#include <QProgressDialog>
#include <QString>
#include <QStringList>
#include <QTextStream>

#include <Magick++.h>

using namespace Magick;

class KingaFileConverter : public QWidget
{
    Q_OBJECT

public:
    KingaFileConverter(QStringList filelist, unsigned int width,
        unsigned int height, unsigned int density_x, unsigned int density_y,
        unsigned int colors, QWidget* parent = 0);
    void Run(void);

signals:
    void FileConverted(int index);
    void FileConverted(QString filename);

private:
    QStringList FileList;
    unsigned int Width;
    unsigned int Height;
    unsigned int DensityX;
    unsigned int DensityY;
    unsigned int Colors;
};
#endif
