//------------------------------------------------------------------------------
// Copyright (c) 2004-2015 Darby Johnston
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the names of the copyright holders nor the names of any
//   contributors may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

//! \file djvFFmpegUtil.h

#ifndef DJV_FFMPEG_UTIL_H
#define DJV_FFMPEG_UTIL_H

#include <QString>

#if defined(DJV_LINUX)
#define __STDC_CONSTANT_MACROS
#endif // DJV_LINUX

extern "C"
{

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

} // extern "C"

//! \addtogroup djvFFmpegPlugin
//@{

//------------------------------------------------------------------------------
//! \struct djvFFmpegUtil
//!
//! This struct provides utilites for working with FFmpeg.
//------------------------------------------------------------------------------

struct djvFFmpegUtil
{
    //! This class provides a memory-managed wrapper for a FFmpeg packet.
    
    class Packet
    {
    public:

        Packet();
        
        ~Packet();

        AVPacket & operator () ();

        const AVPacket & operator () () const;

    private:

        AVPacket _p;
    };

	//! Get the internal time base represented as fractional value.
	
	static AVRational timeBaseQ();
    
    //! Convert an FFmpeg return code to a string.
    
    static QString toString(int);
};

//@} // djvFFmpegPlugin

#endif // DJV_FFMPEG_PLUGIN_H

