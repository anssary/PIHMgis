/***************************************************************************
             qgsgeometry.cpp - Vertex Index into a QgsGeometry
             -------------------------------------------------
Date                 : 08 May 2005
Copyright            : (C) 2005 by Brendan Morley
email                : morb at ozemail dot com dot au
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/* $Id: qgsgeometryvertexindex.cpp 6903 2007-04-20 10:13:52Z wonder $ */

#include "qgsgeometryvertexindex.h"
#include "qgslogger.h"


QgsGeometryVertexIndex::QgsGeometryVertexIndex()
{
  mIndex.clear();
}    

QgsGeometryVertexIndex::QgsGeometryVertexIndex( QgsGeometryVertexIndex const & rhs )
{      
  mIndex = rhs.mIndex;
}


QgsGeometryVertexIndex & QgsGeometryVertexIndex::operator=( QgsGeometryVertexIndex const & rhs )
{
  if(&rhs != this)
  {
    mIndex = rhs.mIndex;
  }
  return *this;
}

QgsGeometryVertexIndex::~QgsGeometryVertexIndex()
{
  // NOOP
}    

void QgsGeometryVertexIndex::push_back(int& i)
{
  mIndex.push_back(i);
}

int QgsGeometryVertexIndex::back() const
{
#ifdef QGISDEBUG
  QString str;
  for(std::vector<int>::const_iterator it = mIndex.begin(); it != mIndex.end(); ++it)
  {
    str += " ";
    str += QString::number(*it);
  }
  QgsDebugMsg("mIndex contains:" + str + ".");
#endif
  return mIndex.back();
}


int QgsGeometryVertexIndex::get_at(int i) const
{
#ifdef QGISDEBUG
  QString str;
  for(std::vector<int>::const_iterator it = mIndex.begin(); it != mIndex.end(); ++it)
  {
    str += " ";
    str += QString::number(*it);
  }
  QgsDebugMsg("mIndex contains:" + str + ".");
#endif
  return mIndex[i];
}


void QgsGeometryVertexIndex::clear()
{
  mIndex.clear();
}

void QgsGeometryVertexIndex::increment_back()
{
  int& n = mIndex.back();
  n++;
}

void QgsGeometryVertexIndex::decrement_back()
{
  int& n = mIndex.back();
  n--;
}

void QgsGeometryVertexIndex::assign_back(int& i)
{
  int& n = mIndex.back();
  n = i;
}

QString QgsGeometryVertexIndex::toString()
{
  QString s;
  
  for ( std::vector<int>::const_iterator iter  = mIndex.begin();
                                         iter != mIndex.end();
                                         iter++)
  {
    s += QString::number(*iter) + " ";
  }

  return s;
}    
