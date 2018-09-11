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

#include <djvGraphics/PPMLoad.h>

#include <djvGraphics/Image.h>
#include <djvGraphics/PixelDataUtil.h>

#include <djvCore/FileIOUtil.h>

//------------------------------------------------------------------------------
// djvPPMLoad
//------------------------------------------------------------------------------

djvPPMLoad::djvPPMLoad(djvCoreContext * context) :
    djvImageLoad(context)
{}

djvPPMLoad::~djvPPMLoad()
{}

void djvPPMLoad::open(const djvFileInfo & in, djvImageIOInfo & info)
    throw (djvError)
{
    //DJV_DEBUG("djvPPMLoad::open");
    //DJV_DEBUG_PRINT("in = " << in);
    //DJV_DEBUG_PRINT("type = " << in.type());
    _file = in;
    djvFileIO io;
    _open(in.fileName(in.sequence().start()), info, io);
    if (djvFileInfo::SEQUENCE == _file.type())
    {
        info.sequence.frames = _file.sequence().frames;
    }
}

void djvPPMLoad::read(djvImage & image, const djvImageIOFrameInfo & frame)
    throw (djvError)
{
    //DJV_DEBUG("djvPPMLoad::read");
    //DJV_DEBUG_PRINT("frame = " << frame);
    image.colorProfile = djvColorProfile();
    image.tags = djvImageTags();

    // Open the file.
    const QString fileName =
        _file.fileName(frame.frame != -1 ? frame.frame : _file.sequence().start());
    //DJV_DEBUG_PRINT("file name = " << fileName);
    djvImageIOInfo info;
    QScopedPointer<djvFileIO> io(new djvFileIO);
    _open(fileName, info, *io);

    // Read the file.
    io->readAhead();
    djvPixelData * data = frame.proxy ? &_tmp : &image;
    if (djvPPM::DATA_BINARY == _data && _bitDepth != 1)
    {
        if ((io->size() - io->pos()) < djvPixelDataUtil::dataByteCount(info))
        {
            throw djvError(
                djvPPM::staticName,
                djvImageIO::errorLabels()[djvImageIO::ERROR_READ]);
        }
        data->set(info, io->mmapP(), io.data());
        io.take();
    }
    else
    {
        data->set(info);
        const int channels = djvPixel::channels(info.pixel);
        if (djvPPM::DATA_BINARY == _data && 1 == _bitDepth)
        {
            const quint64 scanlineByteCount = djvPPM::scanlineByteCount(
                info.size.x, channels, _bitDepth, _data);
            std::vector<quint8> scanline(scanlineByteCount);
            //DJV_DEBUG_PRINT("scanline = " <<
            //    static_cast<int>(scanlineByteCount));
            for (int y = 0; y < info.size.y; ++y)
            {
                io->get(scanline.data(), scanlineByteCount);
                const quint8 * inP = scanline.data();
                quint8 * outP = data->data(0, y);
                for (int i = info.size.x - 1; i >= 0; --i)
                {
                    outP[i] = (inP[i / 8] >> (7 - (i % 8))) & 1 ? 0 : 255;
                }
            }
        }
        else
        {
            for (int y = 0; y < info.size.y; ++y)
            {
                djvPPM::asciiLoad(
                    *io,
                    data->data(0, y),
                    info.size.x * channels,
                    _bitDepth);
            }
        }
    }

    // Proxy scale the image.
    if (frame.proxy)
    {
        info.size = djvPixelDataUtil::proxyScale(info.size, frame.proxy);
        info.proxy = frame.proxy;
        image.set(info);
        djvPixelDataUtil::proxyScale(_tmp, image, frame.proxy);
    }

    //DJV_DEBUG_PRINT("image = " << image);
}

void djvPPMLoad::_open(const QString & in, djvImageIOInfo & info, djvFileIO & io)
    throw (djvError)
{
    //DJV_DEBUG("djvPPMLoad::_open");
    //DJV_DEBUG_PRINT("in = " << in);

    // Open the file.
    io.setEndian(djvMemory::endian() != djvMemory::MSB);
    io.open(in, djvFileIO::READ);
    char magic [] = { 0, 0, 0 };
    io.get(magic, 2);
    //DJV_DEBUG_PRINT("magic = " << magic);
    if (magic[0] != 'P')
    {
        throw djvError(
            djvPPM::staticName,
            djvImageIO::errorLabels()[djvImageIO::ERROR_UNRECOGNIZED]);
    }
    switch (magic[1])
    {
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6': break;
        default:
            throw djvError(
                djvPPM::staticName,
                djvImageIO::errorLabels()[djvImageIO::ERROR_UNSUPPORTED]);
    }
    const int ppmType = magic[1] - '0';
    //DJV_DEBUG_PRINT("ppm type = " << ppmType);

    // Read the header.
    char tmp[djvStringUtil::cStringLength] = "";
    int width  = 0;
    int height = 0;
    djvFileIOUtil::word(io, tmp, djvStringUtil::cStringLength);
    width = QString(tmp).toInt();
    djvFileIOUtil::word(io, tmp, djvStringUtil::cStringLength);
    height = QString(tmp).toInt();
    int maxValue = 0;
    if (ppmType != 1 && ppmType != 4)
    {
        djvFileIOUtil::word(io, tmp, djvStringUtil::cStringLength);
        maxValue = QString(tmp).toInt();
    }
    //DJV_DEBUG_PRINT("max value = " << maxValue);

    // Information.
    info.fileName = in;
    info.size = glm::ivec2(width, height);
    info.mirror.y = true;
    if (1 == ppmType || 4 == ppmType)
    {
        _bitDepth = 1;
    }
    else
    {
        _bitDepth = maxValue < 256 ? 8 : 16;
    }
    //DJV_DEBUG_PRINT("bit depth = " << _bitDepth);
    int channels = 0;
    switch (ppmType)
    {
        case 1:
        case 2:
        case 4:
        case 5: channels = 1; break;
        case 3:
        case 6: channels = 3; break;
    }
    if (! djvPixel::pixel(
        channels,
        _bitDepth != 1 ? _bitDepth : 8,
        djvPixel::INTEGER,
        info.pixel))
    {
        throw djvError(
            djvPPM::staticName,
            djvImageIO::errorLabels()[djvImageIO::ERROR_UNSUPPORTED]);
    }
    _data =
        (1 == ppmType || 2 == ppmType || 3 == ppmType) ?
        djvPPM::DATA_ASCII :
        djvPPM::DATA_BINARY;

    if (djvPPM::DATA_BINARY == _data)
    {
        info.endian = djvMemory::MSB;
    }
}