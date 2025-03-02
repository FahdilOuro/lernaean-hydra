/******************************************************************************
 * Project:  libspatialindex - A C++ library for spatial indexing
 * Author:   Marios Hadjieleftheriou, mhadji@gmail.com
 ******************************************************************************
 * Copyright (c) 2004, Marios Hadjieleftheriou
 *
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
******************************************************************************/

#pragma once

namespace SpatialIndex
{
	class SIDX_DLL TimePoint : public Point, public ITimeShape
	{
	public:
		TimePoint();
		TimePoint(const ts_type* pCoords, const Tools::IInterval& ti, uint32_t dimension);
		TimePoint(const ts_type* pCoords, ts_type tStart, ts_type tEnd, uint32_t dimension);
		TimePoint(const Point& p, const Tools::IInterval& ti);
		TimePoint(const Point& p, ts_type tStart, ts_type tEnd);
		TimePoint(const TimePoint& p);
		virtual ~TimePoint();

		virtual TimePoint& operator=(const TimePoint& p);
		virtual bool operator==(const TimePoint& p) const;

		//
		// IObject interface
		//
		virtual TimePoint* clone();

		//
		// ISerializable interface
		//
		virtual uint32_t getByteArraySize();
		virtual void loadFromByteArray(const byte* data);
		virtual void storeToByteArray(byte** data, uint32_t& len);

		//
		// ITimeShape interface
		//
		virtual bool intersectsShapeInTime(const ITimeShape& in) const;
		virtual bool intersectsShapeInTime(const Tools::IInterval& ivI, const ITimeShape& in) const;
		virtual bool containsShapeInTime(const ITimeShape& in) const;
		virtual bool containsShapeInTime(const Tools::IInterval& ivI, const ITimeShape& in) const;
		virtual bool touchesShapeInTime(const ITimeShape& in) const;
		virtual bool touchesShapeInTime(const Tools::IInterval& ivI, const ITimeShape& in) const;
		virtual ts_type getAreaInTime() const;
		virtual ts_type getAreaInTime(const Tools::IInterval& ivI) const;
		virtual ts_type getIntersectingAreaInTime(const ITimeShape& r) const;
		virtual ts_type getIntersectingAreaInTime(const Tools::IInterval& ivI, const ITimeShape& r) const;

		//
		// IInterval interface
		//
		virtual Tools::IInterval& operator=(const Tools::IInterval&);
		virtual ts_type getLowerBound() const;
		virtual ts_type getUpperBound() const;
		virtual void setBounds(ts_type, ts_type);
		virtual bool intersectsInterval(const Tools::IInterval& ti) const;
		virtual bool intersectsInterval(Tools::IntervalType t, const ts_type start, const ts_type end) const;
		virtual bool containsInterval(const Tools::IInterval& ti) const;
		virtual Tools::IntervalType getIntervalType() const;

		virtual void makeInfinite(uint32_t dimension);
		virtual void makeDimension(uint32_t dimension);

	public:
		ts_type m_startTime;
		ts_type m_endTime;

		friend SIDX_DLL std::ostream& operator<<(std::ostream& os, const TimePoint& pt);
	}; // TimePoint

	SIDX_DLL std::ostream& operator<<(std::ostream& os, const TimePoint& pt);
}
