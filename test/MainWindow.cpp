/*
 * MainWindow.cpp
 * Copyright (C) James Goppert 2010 <james.goppert@gmail.com>
 *
 * MainWindow.cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MainWindow.cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "MainWindow.hpp"
#include <QFileDialog>
#include <QMessageBox>
#include <osg/Vec3d>

#include <cstdlib>
#include <fstream>

MainWindow::MainWindow() : sceneRoot(new osg::Group)
{
    setupUi(this);
    viewer->setSceneData(sceneRoot);
    viewer->setCameraManipulator(new osgGA::TrackballManipulator);
    viewer->getCameraManipulator()->setHomePosition(osg::Vec3d(30,30,-30),osg::Vec3d(0,0,0),osg::Vec3d(0,0,-1),false);
    viewer->getCameraManipulator()->home(0);
    sceneRoot->addChild(new mavsim::visualization::Frame(20,"N","E","D"));

	// read initial settings
	QCoreApplication::setOrganizationName("arkTools");
    QCoreApplication::setOrganizationDomain("github.com/arkTools");
    QCoreApplication::setApplicationName("qtTest");

	// load plane
    try
    {
        plane = new mavsim::visualization::Plane(DATADIR+std::string("/models/plane.ac"));
        plane->addChild(new mavsim::visualization::Frame(15,"X","Y","Z"));
        sceneRoot->addChild(plane);
    }
    catch(const std::exception & e)
    {
		showMsg(e.what());		
    }
}

MainWindow::~MainWindow()
{
    delete viewer;
}

void MainWindow::showMsg(const QString & str)
{
	QMessageBox msgBox;
	msgBox.setText(str);
	msgBox.exec();
};

// vim:ts=4:sw=4
