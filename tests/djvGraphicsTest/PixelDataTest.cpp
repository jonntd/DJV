//------------------------------------------------------------------------------
// Copyright (c) 2004-2018 Darby Johnston
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
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

#include <djvGraphicsTest/PixelDataTest.h>

#include <djvGraphics/PixelData.h>

#include <djvCore/Assert.h>
#include <djvCore/Debug.h>

#include <QString>

using namespace djv::Core;
using namespace djv::Graphics;

namespace djv
{
    namespace GraphicsTest
    {
        void PixelDataTest::run(int &, char **)
        {
            DJV_DEBUG("PixelDataTest::run");
            ctors();
            members();
            operators();
        }

        void PixelDataTest::ctors()
        {
            DJV_DEBUG("PixelDataTest::ctors");
            {
                const Graphics::PixelDataInfo info;
                DJV_ASSERT(glm::ivec2() == info.size);
            }
            {
                const Graphics::PixelDataInfo info(glm::ivec2(1, 2), Graphics::Pixel::LA_U8);
                DJV_ASSERT(glm::ivec2(1, 2) == info.size);
                DJV_ASSERT(Graphics::Pixel::LA_U8 == info.pixel);
            }
            {
                const Graphics::PixelDataInfo info(1, 2, Graphics::Pixel::LA_U8);
                DJV_ASSERT(glm::ivec2(1, 2) == info.size);
                DJV_ASSERT(Graphics::Pixel::LA_U8 == info.pixel);
            }
            {
                const Graphics::PixelDataInfo info("name", glm::ivec2(1, 2), Graphics::Pixel::LA_U8);
                DJV_ASSERT("name" == info.fileName);
                DJV_ASSERT(glm::ivec2(1, 2) == info.size);
                DJV_ASSERT(Graphics::Pixel::LA_U8 == info.pixel);
            }
            {
                const Graphics::PixelDataInfo info("name", 1, 2, Graphics::Pixel::LA_U8);
                DJV_ASSERT("name" == info.fileName);
                DJV_ASSERT(glm::ivec2(1, 2) == info.size);
                DJV_ASSERT(Graphics::Pixel::LA_U8 == info.pixel);
            }
            {
                const Graphics::PixelData data;
                DJV_ASSERT(glm::ivec2() == data.size());
            }
            {
                const Graphics::PixelData data(Graphics::PixelDataInfo(1, 2, Graphics::Pixel::LA_U8));
                DJV_ASSERT(glm::ivec2(1, 2) == data.size());
                DJV_ASSERT(Graphics::Pixel::LA_U8 == data.pixel());
            }
        }

        void PixelDataTest::members()
        {
            DJV_DEBUG("PixelDataTest::members");
            {
                Graphics::PixelData data;
                DJV_ASSERT(!data.isValid());
                const Graphics::PixelDataInfo info(1, 2, Graphics::Pixel::LA_U8);
                data.set(info);
                DJV_ASSERT(info == data.info());
                DJV_ASSERT(data.isValid());
                data.zero();
                DJV_ASSERT(0 == *data.data());
                DJV_ASSERT(0 == *data.data(0, 1));
                const Graphics::PixelData & tmp = data;
                DJV_ASSERT(0 == *tmp.data());
                DJV_ASSERT(0 == *tmp.data(0, 1));
                DJV_ASSERT(2 == data.pixelByteCount());
                DJV_ASSERT(2 == data.scanlineByteCount());
                DJV_ASSERT(4 == data.dataByteCount());
            }
        }

        void PixelDataTest::operators()
        {
            DJV_DEBUG("PixelDataTest::operators");
            {
                Graphics::PixelDataInfo::Mirror a, b;
                a.x = b.x = true;
                DJV_ASSERT(a == b);
                DJV_ASSERT(a != Graphics::PixelDataInfo::Mirror());
            }
            {
                Graphics::PixelDataInfo a, b;
                a.size = b.size = glm::ivec2(1, 2);
                DJV_ASSERT(a == b);
                DJV_ASSERT(a != Graphics::PixelDataInfo());
            }
            {
                Graphics::PixelData
                    a(Graphics::PixelDataInfo(1, 2, Graphics::Pixel::LA_U8)),
                    b(Graphics::PixelDataInfo(1, 2, Graphics::Pixel::LA_U8));
                a.zero();
                b.zero();
                DJV_ASSERT(a == b);
                DJV_ASSERT(a != Graphics::PixelData());
            }
            {
                DJV_DEBUG_PRINT(Graphics::PixelDataInfo::PROXY_1_2);
                DJV_DEBUG_PRINT(Graphics::PixelDataInfo::Mirror(true, true));
            }
        }

    } // namespace GraphicsTest
} // namespace djv
