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

#ifndef __KINGA_MAINWINDOW_CPP__
#define __KINGA_MAINWINDOW_CPP__

#include "kingamainwindow.hpp"

KingaMain::KingaMain(QWidget* parent) : QWidget(parent)
{
    setWindowTitle("Kinga");
    resize(640,480);
    QDir::setCurrent(QDir::homePath());

    // Menu
    QMenuBar* menu_bar = new QMenuBar(this);
    QMenu* menu_file = menu_bar->addMenu(tr("&File"));
    QAction* action_add_files = new QAction(tr("&Add Files"),this);
    QAction* action_add_recursively = new QAction(tr("Add &Recursively"),this);
    QAction* action_run = new QAction(tr("&Run"),this);
    QAction* action_exit = new QAction(tr("&Exit"),this);
    menu_file->addAction(action_add_files);
    menu_file->addAction(action_add_recursively);
    menu_file->addAction(action_run);
    menu_file->addAction(action_exit);
    QMenu* menu_help = menu_bar->addMenu(tr("&Help"));
    QAction* action_about = new QAction(tr("&About"),this);
    menu_help->addAction(action_about);

    // File manager
    KingaFileManager* file_manager = new KingaFileManager(this);

    // Output control
    QGroupBox* box_output = new QGroupBox(tr("Output"),this);
    QLabel* label_output_path = new QLabel(tr("Path:"),this);
    QLineEdit* output_path_editor = new QLineEdit(QDir::homePath(),box_output);
    output_path_editor->setReadOnly(true);
    QPushButton* button_browse_output = new QPushButton(tr("Browse"),box_output);
    QComboBox* preset_selector = new QComboBox(box_output);
    preset_selector->addItem("Select a preset...");
    preset_selector->addItem("Kindle 1st Gen.");
    preset_selector->addItem("Kindle 2nd Gen.");
    preset_selector->addItem("Kindle 3rd Gen.");
    preset_selector->addItem("Kindle 4th Gen.");
    preset_selector->addItem("Kindle DX");
    QPushButton* button_run = new QPushButton(tr("Run"),box_output);

    // Layout: output control
    QHBoxLayout* layout_output = new QHBoxLayout(box_output);
    layout_output->addWidget(label_output_path,0,Qt::AlignLeft);
    layout_output->addWidget(output_path_editor,0,Qt::AlignLeft);
    layout_output->addWidget(button_browse_output,0,Qt::AlignLeft);
    layout_output->addWidget(preset_selector,0,Qt::AlignLeft);
    layout_output->addWidget(button_run,1,Qt::AlignRight);
    box_output->setLayout(layout_output);

    // Layout: main
    QVBoxLayout* layout_main = new QVBoxLayout(this);
    layout_main->addWidget(menu_bar);
    layout_main->addWidget(file_manager);
    layout_main->addWidget(box_output);
    setLayout(layout_main);

    // Assign create objects to private attributes
    FileManager = file_manager;
    OutputPathEditor = output_path_editor;
    PresetSelector = preset_selector;

    // Connections
    connect(action_add_files,SIGNAL(triggered()),FileManager,SLOT(OnAddFilesClicked()));
    connect(action_add_recursively,SIGNAL(triggered()),FileManager,SLOT(OnAddRecursivelyClicked()));
    connect(action_run,SIGNAL(triggered()),this,SLOT(RunClicked()));
    connect(action_exit,SIGNAL(triggered()),this,SLOT(close()));
    connect(action_about,SIGNAL(triggered()),this,SLOT(OnAboutClicked()));
    connect(button_browse_output,SIGNAL(clicked()),this,SLOT(OnBrowseOutputClicked()));
    connect(button_run,SIGNAL(clicked()),this,SLOT(RunClicked()));
}

void KingaMain::OnAboutClicked(void) {
    QMessageBox::about(this,tr("About Kinga"),tr("Kinga is just a small gui to batch-convert images for reading on various E-Reader models.\nCopyright (C) 2012 Gianluca Merlo, released under the GNU GPLv3 (see LICENSE file)"));
}

void KingaMain::OnBrowseOutputClicked(void)
{
    QString output_path = QFileDialog::getExistingDirectory(this,tr("Select Output Directory"),QDir::homePath());
    if(!output_path.isNull()) OutputPathEditor->setText(output_path);
}

void KingaMain::RunClicked(void)
{
    int preset_number = PresetSelector->currentIndex();
    unsigned int width,height,density_x,density_y,colors;
    switch(preset_number) {
        case KINGA_PRESET_UNDEFINED: {
            QMessageBox message_box;
            message_box.setIcon(QMessageBox::Critical);
            message_box.setText(tr("Select a preset first"));
            message_box.exec();
            return;
            break;
        }
        case KINGA_PRESET_KINDLE1:
            width = 600;
            height = 800;
            density_x = 167;
            density_y = 167;
            colors = 4;
            break;
        case KINGA_PRESET_KINDLE2:
            width = 600;
            height = 800;
            density_x = 167;
            density_y = 167;
            colors = 16;
            break;
        case KINGA_PRESET_KINDLE3:
            width = 600;
            height = 800;
            density_x = 167;
            density_y = 167;
            colors = 16;
            break;
        case KINGA_PRESET_KINDLE4:
            width = 600;
            height = 800;
            density_x = 167;
            density_y = 167;
            colors = 16;
            break;
        case KINGA_PRESET_KINDLEDX:
            width = 824;
            height = 1200;
            density_x = 150;
            density_y = 150;
            colors = 16;
            break;
        default: {
            QMessageBox message_box;
            message_box.setIcon(QMessageBox::Critical);
            message_box.setText(tr("Preset selection error"));
            message_box.exec();
            return;
            break;
        }
    }
    QStringList filelist = FileManager->GetFiles();
    QDir::setCurrent(OutputPathEditor->text());
    KingaFileConverter* file_converter = new KingaFileConverter(filelist,width,height,density_x,density_y,colors,this);
    file_converter->Run();
    QDir::setCurrent(QDir::homePath());
}

#endif
