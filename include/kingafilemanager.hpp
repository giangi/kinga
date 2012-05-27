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

#ifndef __KINGA_FILEMANAGER_HPP__
#define __KINGA_FILEMANAGER_HPP__

#include <QAbstractItemView>
#include <QDir>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QList>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QStringList>
#include <QVBoxLayout>
#include <QWidget>

class KingaFileManager : public QWidget
{
    Q_OBJECT

public:
    KingaFileManager(QWidget* parent = 0);
    QStringList GetFiles(void);

private slots:
    void OnSelectionChanged(void);
    void OnAddFilesClicked(void);
    void OnAddRecursivelyClicked(void);
    void OnMoveUpClicked(void);
    void OnMoveDownClicked(void);
    void OnRemoveClicked(void);

private:
    QListWidget* FileListWidget;
    QPushButton* AddFilesButton;
    QPushButton* AddRecursivelyButton;
    QPushButton* MoveUpButton;
    QPushButton* MoveDownButton;
    QPushButton* RemoveButton;
    int FirstSelectedIndex;
    int LastSelectedIndex;
    QStringList GetFilesRecursively(QString folder_path);
};

#endif
