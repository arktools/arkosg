/*
 * Viewer.hpp
 * Copyright (C) James Goppert 2010 <jgoppert@users.sourceforge.net>
 *
 * Viewer.hpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Viewer.hpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef Viewer_HPP
#define Viewer_HPP
#include <osgViewer/Viewer>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/scoped_ptr.hpp>
#include <osgDB/ReadFile>

// include the plugins we need
#ifdef USE_FREETYPE
        USE_OSGPLUGIN(freetype)
#endif

USE_DOTOSGWRAPPER_LIBRARY(osg)
USE_DOTOSGWRAPPER_LIBRARY(osgUtil)
USE_DOTOSGWRAPPER_LIBRARY(osgDB)
USE_DOTOSGWRAPPER_LIBRARY(osgFX)
USE_DOTOSGWRAPPER_LIBRARY(osgParticle)
USE_DOTOSGWRAPPER_LIBRARY(osgShadow)
USE_DOTOSGWRAPPER_LIBRARY(osgSim)
USE_DOTOSGWRAPPER_LIBRARY(osgTerrain)
USE_DOTOSGWRAPPER_LIBRARY(osgText)
USE_DOTOSGWRAPPER_LIBRARY(osgViewer)
USE_DOTOSGWRAPPER_LIBRARY(osgVolume)
USE_DOTOSGWRAPPER_LIBRARY(osgWidget)

USE_SERIALIZER_WRAPPER_LIBRARY(osg)
USE_SERIALIZER_WRAPPER_LIBRARY(osgUtil)
USE_SERIALIZER_WRAPPER_LIBRARY(osgDB)
USE_SERIALIZER_WRAPPER_LIBRARY(osgAnimation)
USE_SERIALIZER_WRAPPER_LIBRARY(osgFX)
USE_SERIALIZER_WRAPPER_LIBRARY(osgManipulator)
USE_SERIALIZER_WRAPPER_LIBRARY(osgParticle)
USE_SERIALIZER_WRAPPER_LIBRARY(osgShadow)
USE_SERIALIZER_WRAPPER_LIBRARY(osgSim)
USE_SERIALIZER_WRAPPER_LIBRARY(osgTerrain)
USE_SERIALIZER_WRAPPER_LIBRARY(osgText)
USE_SERIALIZER_WRAPPER_LIBRARY(osgVolume)

// include the platform specific GraphicsWindow implementation.
//USE_GRAPHICSWINDOW()

namespace arkosg
{

/**
 * The purpose of this class is to create a viewer for simulation purposes.
 */
class Viewer : public osgViewer::Viewer
{
public:
    /**
     * Constructor
     */
    Viewer(int fps = 20);
    virtual ~Viewer();
    /**
     * Override default run to be mutex lockable
     */
    int run();
    /**
     * Function to frame viewer
     */
    void loop();
    /**
     * Unlock the mutex and allow the viewer to frame
     */
    void unlock();
    /**
     * Lock the mutex and prevent viewer from framing
     */
    void lock();
private:
    boost::scoped_ptr<boost::thread> myThread; // viewer thread
    boost::mutex myMutex; // mutex lock to protect data for multi-threading
    int myFps; // Approxmiate frames per second, neglects framing time
};

} // arkosg

#endif

// vim:ts=4:sw=4
