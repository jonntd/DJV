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

namespace djv
{
    namespace Core
    {
#define _BOX2_INIT() \
    x(Box2::position.x), \
    y(Box2::position.y), \
    w(Box2::size.x),     \
    h(Box2::size.y)

        template<typename T, precision P>
        inline Box2<T, P>::Box2() :
            position(T(0), T(0)),
            size(T(0), T(0)),
            _BOX2_INIT()
        {}

        template<typename T, precision P>
        inline Box2<T, P>::Box2(const Box2<T, P> & in) :
            position(in.position),
            size(in.size),
            _BOX2_INIT()
        {}

        template<typename T, precision P>
        inline Box2<T, P>::Box2(const glm::tvec2<T, P> & position, const glm::tvec2<T, P> & size) :
            position(position),
            size(size),
            _BOX2_INIT()
        {}

        template<typename T, precision P>
        inline Box2<T, P>::Box2(const glm::tvec2<T, P> & size) :
            position(T(0), T(0)),
            size(size),
            _BOX2_INIT()
        {}

        template<typename T, precision P>
        inline Box2<T, P>::Box2(T _x, T _y, T _w, T _h) :
            _BOX2_INIT()
        {
            x = _x;
            y = _y;
            w = _w;
            h = _h;
        }

        template<typename T, precision P>
        inline Box2<T, P>::Box2(T _w, T _h) :
            position(T(0), T(0)),
            _BOX2_INIT()
        {
            w = _w;
            h = _h;
        }

        template<typename T, precision P>
        inline bool Box2<T, P>::isValid() const
        {
            return size.x > T(0) && size.y > T(0);
        }

        template<typename T, precision P>
        inline void Box2<T, P>::zero()
        {
            position.x = position.y = T(0);
            size.x = size.y = T(0);
        }

        template<typename T, precision P>
        inline glm::tvec2<T, P> Box2<T, P>::lowerRight() const
        {
            return position + size;
        }

        //! \todo Is this specialization for integer box types correct?
        template<>
        inline glm::ivec2 Box2<int>::lowerRight() const
        {
            return position + size - 1;
        }

        template<typename T, precision P>
        inline void Box2<T, P>::setLowerRight(const glm::tvec2<T, P> & in)
        {
            size = in - position;
        }

        //! \todo Is this specialization for integer box types correct?
        template<>
        inline void Box2<int>::setLowerRight(const glm::ivec2 & in)
        {
            size = in - position + 1;
        }

        template<typename T, precision P>
        inline Box2<T, P> & Box2<T, P>::operator = (const Box2<T, P> & in)
        {
            if (&in != this)
            {
                position = in.position;
                size = in.size;
            }
            return *this;
        }

        template<typename T, precision P>
        inline Box2<T, P> & Box2<T, P>::operator *= (const glm::tvec2<T, P> & in)
        {
            position *= in;
            size *= in;
            return *this;
        }

        template<typename T, precision P>
        inline Box2<T, P> & Box2<T, P>::operator /= (const glm::tvec2<T, P> & in)
        {
            position /= in;
            size /= in;
            return *this;
        }

        template<typename T, precision P>
        inline Box2<T, P> & Box2<T, P>::operator *= (T in)
        {
            position *= in;
            size *= in;
            return *this;
        }

        template<typename T, precision P>
        inline Box2<T, P> & Box2<T, P>::operator /= (T in)
        {
            position /= in;
            size /= in;
            return *this;
        }

        template<typename T, precision P>
        inline Box2<T, P>::operator Box2<int, P>() const
        {
            return Box2<int, P>(
                static_cast<int>(x),
                static_cast<int>(y),
                static_cast<int>(w),
                static_cast<int>(h));
        }

        template<typename T, precision P>
        inline Box2<T, P>::operator Box2<float, P>() const
        {
            return Box2<float, P>(
                static_cast<float>(x),
                static_cast<float>(y),
                static_cast<float>(w),
                static_cast<float>(h));
        }

    } // namespace Core
} // namespace djv
