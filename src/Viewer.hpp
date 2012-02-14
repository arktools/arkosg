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

// Static linking of OSG needs special macros
#ifdef OSG_LIBRARY_STATIC
#include <osgDB/Registry>
USE_GRAPHICSWINDOW();
// Image formats
USE_OSGPLUGIN(bmp);
USE_OSGPLUGIN(dds);
USE_OSGPLUGIN(hdr);
USE_OSGPLUGIN(pic);
USE_OSGPLUGIN(pnm);
USE_OSGPLUGIN(rgb);
USE_OSGPLUGIN(tga);
#ifdef OSG_JPEG_ENABLED
  USE_OSGPLUGIN(jpeg);
#endif
#ifdef OSG_PNG_ENABLED
  USE_OSGPLUGIN(png);
#endif
#ifdef OSG_TIFF_ENABLED
  USE_OSGPLUGIN(tiff);
#endif
// Model formats
USE_OSGPLUGIN(3ds);
USE_OSGPLUGIN(ac);
USE_OSGPLUGIN(ive);
USE_OSGPLUGIN(osg);
USE_OSGPLUGIN(txf);
#endif

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
