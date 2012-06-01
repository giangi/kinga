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

#ifndef __KINGA_FILEMANAGER_CPP__
#define __KINGA_FILEMANAGER_CPP__

#include "kingafilemanager.hpp"

KingaFileManager::KingaFileManager(QWidget* parent) : QWidget(parent)
{
    // Main file manager
    QListWidget* file_list_widget = new QListWidget(this);
    // Change selection style for the list of files
    file_list_widget->setSelectionMode(QAbstractItemView::ContiguousSelection);
    // Enable drag and drop
    file_list_widget->setDragEnabled(true);
    file_list_widget->setDragDropMode(QAbstractItemView::InternalMove);
    file_list_widget->viewport()->setAcceptDrops(true);
    file_list_widget->setDropIndicatorShown(true);
    // Automatically scroll on drag
    file_list_widget->setAutoScroll(true);
    // Container for file actions, and buttons
    QWidget* box_file_buttons = new QWidget(this);
    QPushButton* button_add_files = new QPushButton(tr("Add Files"),this);
    QPushButton* button_add_recursively = new QPushButton(tr("Add Recursively"),this);
    QPushButton* button_file_move_up = new QPushButton(tr("Move Up"),this);
    QPushButton* button_file_move_down = new QPushButton(tr("Move Down"),this);
    QPushButton* button_file_remove = new QPushButton(tr("Remove"),this);

    // Layout: file action buttons
    QVBoxLayout* layout_file_buttons = new QVBoxLayout(box_file_buttons);
    layout_file_buttons->addWidget(button_add_files,0,Qt::AlignTop);
    layout_file_buttons->addWidget(button_add_recursively,0,Qt::AlignTop);
    layout_file_buttons->addWidget(button_file_move_up,0,Qt::AlignTop);
    layout_file_buttons->addWidget(button_file_move_down,0,Qt::AlignTop);
    layout_file_buttons->addWidget(button_file_remove,1,Qt::AlignTop);
    box_file_buttons->setLayout(layout_file_buttons);

    // Layout: main widget
    QHBoxLayout* layout_file_area = new QHBoxLayout(this);
    layout_file_area->addWidget(file_list_widget);
    layout_file_area->addWidget(box_file_buttons);
    setLayout(layout_file_area);

    // Assign create objects to private attributes
    FileListWidget = file_list_widget;
    AddFilesButton = button_add_files;
    AddRecursivelyButton = button_add_recursively;
    MoveUpButton = button_file_move_up;
    MoveDownButton = button_file_move_down;
    RemoveButton = button_file_remove;

    // Disable file buttons at creation
    MoveUpButton->setEnabled(false);
    MoveDownButton->setEnabled(false);
    RemoveButton->setEnabled(false);

    // Connections
    connect(FileListWidget,SIGNAL(itemSelectionChanged()),this,SLOT(OnSelectionChanged()));
    connect(AddFilesButton,SIGNAL(clicked()),this,SLOT(OnAddFilesClicked()));
    connect(AddRecursivelyButton,SIGNAL(clicked()),this,SLOT(OnAddRecursivelyClicked()));
    connect(MoveUpButton,SIGNAL(clicked()),this,SLOT(OnMoveUpClicked()));
    connect(MoveDownButton,SIGNAL(clicked()),this,SLOT(OnMoveDownClicked()));
    connect(RemoveButton,SIGNAL(clicked()),this,SLOT(OnRemoveClicked()));
}

QStringList KingaFileManager::GetFiles(void)
{
    QStringList filelist;
    for(int i=0;i<FileListWidget->count();i++)
        filelist.push_back(FileListWidget->item(i)->text());
    return filelist;
}

//
// Slots
//

void KingaFileManager::OnSelectionChanged(void)
{
    QList<QListWidgetItem*> selected_list = FileListWidget->selectedItems();

    int first_selected_index = FileListWidget->count();
    int last_selected_index = -1;
    for(int i=0;i<selected_list.size();i++) {
        int item_index = FileListWidget->row(selected_list.at(i));
        if(item_index < first_selected_index) first_selected_index = item_index;
        if(item_index > last_selected_index) last_selected_index = item_index;
    }
    FirstSelectedIndex = first_selected_index;
    LastSelectedIndex = last_selected_index;

    MoveUpButton->setEnabled(false);
    MoveDownButton->setEnabled(false);
    RemoveButton->setEnabled(false);

    if( FirstSelectedIndex > 0 ) MoveUpButton->setEnabled(true);
    if( LastSelectedIndex < FileListWidget->count()-1 ) MoveDownButton->setEnabled(true);
    if( ! selected_list.isEmpty() ) RemoveButton->setEnabled(true);
}

QStringList KingaFileManager::GetFilesRecursively(QString folder_path) {
    QDir dir = QDir(folder_path);
    QStringList files;
    QStringList dirs;
    QStringList valid_files;
    valid_files << "*.jpg" << "*.jpeg" << "*.png";
    QFileInfoList fileinfos = dir.entryInfoList(valid_files,QDir::Files|QDir::NoSymLinks);
    for(int i=0; i<fileinfos.size();i++) files << fileinfos.at(i).absoluteFilePath();
    QFileInfoList dirinfos = dir.entryInfoList(QDir::Dirs|QDir::NoSymLinks|QDir::NoDotAndDotDot);
    for(int i=0; i<dirinfos.size();i++) files << GetFilesRecursively(dirinfos.at(i).absoluteFilePath());
    return files;
}

void KingaFileManager::OnAddFilesClicked(void)
{
    QStringList files = QFileDialog::getOpenFileNames(this,tr("Add Files"),QDir::homePath(),tr("Image Files (*.png *.jpg *.jpeg) ;; All Files (*.*)"));
    if(!files.isEmpty()) FileListWidget->addItems(files);
}

void KingaFileManager::OnAddRecursivelyClicked(void)
{
    QString dirpath = QFileDialog::getExistingDirectory(this,tr("Select Folder"),QDir::homePath());
    if(!dirpath.isNull()) FileListWidget->addItems(GetFilesRecursively(dirpath));
}

void KingaFileManager::OnMoveUpClicked(void)
{
    int first_selected_index = FirstSelectedIndex;
    int last_selected_index = LastSelectedIndex;
    if( first_selected_index != 0 ) {
        for(int i=first_selected_index;i<=last_selected_index;i++) {
            QListWidgetItem* item = FileListWidget->takeItem(i);
            FileListWidget->insertItem(i-1,item);
        }
        for(int i=first_selected_index-1;i<=last_selected_index-1;i++)
            FileListWidget->item(i)->setSelected(true);
    }
}

void KingaFileManager::OnMoveDownClicked(void)
{
    int first_selected_index = FirstSelectedIndex;
    int last_selected_index = LastSelectedIndex;
    if( last_selected_index != FileListWidget->count() - 1 ) {
        for(int i=last_selected_index;i>=first_selected_index;i--) {
            QListWidgetItem* item = FileListWidget->takeItem(i);
            FileListWidget->insertItem(i+1,item);
        }
        for(int i=first_selected_index+1;i<=last_selected_index+1;i++)
            FileListWidget->item(i)->setSelected(true);
    }
}

void KingaFileManager::OnRemoveClicked(void)
{
    int first_selected_index = FirstSelectedIndex;
    int last_selected_index = LastSelectedIndex;
    for(int i=first_selected_index;i<=last_selected_index;i++) {
        QListWidgetItem* item = FileListWidget->takeItem(first_selected_index);
        delete item;
    }
    // Trigger a selection change
    OnSelectionChanged();
}

#endif
