/******************************************************************************
 * Project:  libspatialindex - A C++ library for spatial indexing
 * Author:   Marios Hadjieleftheriou, mhadji@gmail.com
 ******************************************************************************
 * Copyright (c) 2003, Marios Hadjieleftheriou
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

#include "tools/Tools.h"

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661922
#endif

typedef float ts_type;

namespace SpatialIndex
{
	class Point;
	class LineSegment;
	class Region;


	typedef int64_t id_type;

	SIDX_DLL enum CommandType
	{
		CT_NODEREAD = 0x0,
		CT_NODEDELETE,
		CT_NODEWRITE
	};

	class SIDX_DLL InvalidPageException : public Tools::Exception
	{
	public:
		InvalidPageException(id_type id);
		virtual ~InvalidPageException() {}
		virtual std::string what();

	private:
		std::string m_error;
	}; // InvalidPageException

	//
	// Interfaces
	//

	class SIDX_DLL IShape : public Tools::ISerializable
	{
	public:
		virtual bool intersectsShape(const IShape& in) const = 0;
		virtual bool containsShape(const IShape& in) const = 0;
		virtual bool touchesShape(const IShape& in) const = 0;
		virtual void getCenter(Point& out) const = 0;
		virtual uint32_t getDimension() const = 0;
		virtual void getMBR(Region& out) const = 0;
		virtual ts_type getArea() const = 0;
		virtual ts_type getMinimumDistance(const IShape& in) const = 0;
		virtual ~IShape() {}
	}; // IShape

	class SIDX_DLL ITimeShape : public Tools::IInterval
	{
	public:
		virtual bool intersectsShapeInTime(const ITimeShape& in) const = 0;
		virtual bool intersectsShapeInTime(const Tools::IInterval& ivI, const ITimeShape& in) const = 0;
		virtual bool containsShapeInTime(const ITimeShape& in) const = 0;
		virtual bool containsShapeInTime(const Tools::IInterval& ivI, const ITimeShape& in) const = 0;
		virtual bool touchesShapeInTime(const ITimeShape& in) const = 0;
		virtual bool touchesShapeInTime(const Tools::IInterval& ivI, const ITimeShape& in) const = 0;
		virtual ts_type getAreaInTime() const = 0;
		virtual ts_type getAreaInTime(const Tools::IInterval& ivI) const = 0;
		virtual ts_type getIntersectingAreaInTime(const ITimeShape& r) const = 0;
		virtual ts_type getIntersectingAreaInTime(const Tools::IInterval& ivI, const ITimeShape& r) const = 0;
		virtual ~ITimeShape() {}
	}; // ITimeShape

	class SIDX_DLL IEvolvingShape
	{
	public:
		virtual void getVMBR(Region& out) const = 0;
		virtual void getMBRAtTime(ts_type t, Region& out) const = 0;
		virtual ~IEvolvingShape() {}
	}; // IEvolvingShape

	class SIDX_DLL IEntry : public Tools::IObject
	{
	public:
		virtual id_type getIdentifier() const = 0;
		virtual void getShape(IShape** out) const = 0;
		virtual ~IEntry() {}
	}; // IEntry

	class SIDX_DLL INode : public IEntry, public Tools::ISerializable
	{
	public:
		virtual uint32_t getChildrenCount() const = 0;
		virtual id_type getChildIdentifier(uint32_t index) const = 0;
		virtual void getChildData(uint32_t index, uint32_t& len, byte** data) const = 0;
		virtual void getChildShape(uint32_t index, IShape** out) const = 0;
		virtual uint32_t getLevel() const = 0;
		virtual bool isIndex() const = 0;
		virtual bool isLeaf() const = 0;
		virtual ~INode() {}
	}; // INode

	class SIDX_DLL IData : public IEntry
	{
	public:
		virtual void getData(uint32_t& len, byte** data) const = 0;
		virtual void getDataOpt(uint32_t& len, byte** data) const = 0;		
		virtual ~IData() {}
	}; // IData

	class SIDX_DLL IDataStream : public Tools::IObjectStream
	{
	public:
		virtual IData* getNext() = 0;
		virtual ~IDataStream() {}
	}; // IDataStream

	class SIDX_DLL ICommand
	{
	public:
		virtual void execute(const INode& in) = 0;
		virtual ~ICommand() {}
	}; // ICommand

	class SIDX_DLL INearestNeighborComparator
	{
	public:
		virtual ts_type getMinimumDistance(const IShape& query, const IShape& entry) = 0;
		virtual ts_type getMinimumDistance(const IShape& query, const IData& data) = 0;
		virtual ~INearestNeighborComparator() {}
	}; // INearestNeighborComparator

	class SIDX_DLL IStorageManager
	{
	public:
		virtual void loadByteArray(const id_type id, uint32_t& len, byte** data) = 0;
		virtual void storeByteArray(id_type& id, const uint32_t len, const byte* const data) = 0;
		virtual void deleteByteArray(const id_type id) = 0;
		virtual void flush() = 0;
		virtual ~IStorageManager() {}
	}; // IStorageManager

	class SIDX_DLL IVisitor
	{
	public:
		virtual void visitNode(const INode& in) = 0;
		virtual void visitData(const IData& in) = 0;
		virtual void visitData(byte * Data) = 0;
		
		virtual void visitData(std::vector<const IData*>& v) = 0;
		virtual ~IVisitor() {}
	}; // IVisitor

	class SIDX_DLL IQueryStrategy
	{
	public:
		virtual void getNextEntry(const IEntry& previouslyFetched, id_type& nextEntryToFetch, bool& bFetchNextEntry) = 0;
		virtual ~IQueryStrategy() {}
	}; // IQueryStrategy

	class SIDX_DLL IStatistics
	{
	public:
		virtual uint64_t getReads() const = 0;
		virtual uint64_t getWrites() const = 0;
		virtual uint32_t getNumberOfNodes() const = 0;
		virtual uint64_t getNumberOfData() const = 0;
		virtual ~IStatistics() {}
	}; // IStatistics

	class SIDX_DLL ISpatialIndex
	{
	public:
		virtual void insertData(uint32_t len, const byte* pData, const IShape& shape, id_type shapeIdentifier) = 0;
		virtual bool deleteData(const IShape& shape, id_type shapeIdentifier) = 0;
		virtual void containsWhatQuery(const IShape& query, IVisitor& v)  = 0;
		virtual void intersectsWithQuery(const IShape& query, IVisitor& v) = 0;
		virtual void pointLocationQuery(const Point& query, IVisitor& v) = 0;
		virtual void nearestNeighborQuery(uint32_t k, const IShape& query, IVisitor& v, INearestNeighborComparator& nnc) = 0;	
		virtual void nearestNeighborQueryOptimized(uint32_t k, const IShape& query, IVisitor& v, INearestNeighborComparator& nnc) = 0;		
		virtual void nearestNeighborQuery(uint32_t k, const IShape& query, IVisitor& v) = 0;
		virtual void selfJoinQuery(const IShape& s, IVisitor& v) = 0;
		virtual void queryStrategy(IQueryStrategy& qs) = 0;
		virtual void getIndexProperties(Tools::PropertySet& out) const = 0;
		virtual void addCommand(ICommand* in, CommandType ct) = 0;
		virtual bool isIndexValid() = 0;
		virtual void getStatistics(IStatistics** out) const = 0;
	  	virtual inline void printIt2() = 0;
virtual inline void stats_init(void) = 0 ; 

virtual inline void stats_init_counters(void) = 0 ;
virtual inline void stats_count_total_nodes (int  )=0;
virtual inline void stats_count_total_ts (int )=0;
virtual inline void stats_count_leaf_nodes(int )=0;  
 virtual inline void stats_init_leaf_counters (void )=0;
 virtual inline void stats_get_leaf_counters (void ) =0; 
 virtual inline void stats_clean_leaf_counters (void )=0;
 virtual inline void stats_count_checked_ts(int ) =0;
virtual inline void stats_count_checked_nodes(int ) =0;  
virtual inline void stats_reset_query_counters(void)=0;
virtual inline void stats_reset_partial_counters(void)=0;
virtual inline void stats_count_partial_seq_input(void)=0;
virtual inline void stats_count_partial_seq_output(void)=0;
virtual inline void stats_count_partial_rand_input(void)=0;
virtual inline void stats_count_partial_rand_output(void)=0;
virtual inline void stats_count_input_time_start(void)=0;
virtual inline void stats_count_output_time_start(void)=0;
virtual inline void stats_count_total_time_start(void)=0;
virtual inline void stats_count_partial_input_time_start(void)=0;
virtual inline void stats_count_partial_output_time_start(void)=0;
virtual inline void stats_count_partial_time_start(void)=0;
virtual inline void stats_count_input_time_end(void)=0;
virtual inline void stats_count_output_time_end(void)=0;
virtual inline void stats_count_total_time_end(void)=0;
virtual inline void stats_count_partial_input_time_end(void)=0;
virtual inline void stats_count_partial_output_time_end(void)=0;
virtual inline void stats_count_partial_time_end(void)=0;
virtual inline void stats_update_idx_stats(void)=0;
 virtual inline void stats_update_query_stats(ts_type dist, int )=0;
virtual void get_index_footprint(const char * filename) = 0;
 virtual void print_index_stats(char * dataset, const char * index_filename, bool print_leaves) = 0;
virtual void print_query_stats(char * queries, unsigned int query_num)=0;
 
		virtual ~ISpatialIndex() {}

	}; // ISpatialIndex

	namespace StorageManager
	{
		SIDX_DLL enum StorageManagerConstants
		{
			EmptyPage = -0x1,
			NewPage = -0x1
		};

		class SIDX_DLL IBuffer : public IStorageManager
		{
		public:
			virtual uint64_t getHits() = 0;
			virtual void clear() = 0;
			virtual ~IBuffer() {}
		}; // IBuffer

		SIDX_DLL  IStorageManager* returnMemoryStorageManager(Tools::PropertySet& in);
		SIDX_DLL  IStorageManager* createNewMemoryStorageManager();

		SIDX_DLL  IStorageManager* returnDiskStorageManager(Tools::PropertySet& in);
		SIDX_DLL  IStorageManager* createNewDiskStorageManager(std::string& baseName, uint32_t pageSize);
		SIDX_DLL  IStorageManager* loadDiskStorageManager(std::string& baseName);

		SIDX_DLL  IBuffer* returnRandomEvictionsBuffer(IStorageManager& ind, Tools::PropertySet& in);
		SIDX_DLL  IBuffer* createNewRandomEvictionsBuffer(IStorageManager& in, uint32_t capacity, bool bWriteThrough);
	}

	//
	// Global functions
	//
	SIDX_DLL  std::ostream& operator<<(std::ostream&, const ISpatialIndex&);
	SIDX_DLL  std::ostream& operator<<(std::ostream&, const IStatistics&);
}

#include "Point.h"
#include "Region.h"
#include "LineSegment.h"
#include "TimePoint.h"
#include "TimeRegion.h"
#include "MovingPoint.h"
#include "MovingRegion.h"
#include "RTree.h"
//#include "MVRTree.h"
//#include "TPRTree.h"
#include "Version.h"


// typedef SpatialIndex::Tools::PoolPointer<Region> RegionPtr;
// typedef SpatialIndex::Tools::PoolPointer<Point> PointPtr;
// typedef SpatialIndex::Tools::PoolPointer<TimeRegion> TimeRegionPtr;
// typedef SpatialIndex::Tools::PoolPointer<MovingRegion> MovingRegionPtr;
