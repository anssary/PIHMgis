/***************************************************************************
                          qgspoint.cpp -  description
                             -------------------
    begin                : Sat Jun 22 2002
    copyright            : (C) 2002 by Gary E.Sherman
    email                : sherman at mrcc.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
 /* $Id: qgspoint.cpp 6942 2007-05-13 13:40:35Z mhugent $ */


#include "qgspoint.h"

#include <QTextStream>


QgsPoint::QgsPoint(const QgsPoint& p)
{
  m_x = p.x();
  m_y = p.y();
}

QString QgsPoint::stringRep() const
{
  QString rep;
  QTextOStream ot(&rep);
  ot.precision(12);
  ot << m_x << ", " << m_y;
  return rep;
}

QString QgsPoint::stringRep(int thePrecision) const
{
  QString rep = QString::number(m_x,'f',thePrecision) + QString(",") + 
  QString::number( m_y,'f',thePrecision);
  return rep;
}


QString QgsPoint::wellKnownText()
{
  return QString("POINT(%1 %2)").arg(QString::number(m_x, 'f', 18)).arg(QString::number(m_y, 'f', 18));
}

double QgsPoint::sqrDist(double x, double y) const
{
    return (m_x-x)*(m_x-x)+(m_y-y)*(m_y-y);
}

double QgsPoint::sqrDist(const QgsPoint& other)
{
  return sqrDist(other.x(), other.y());
}

// operators
bool QgsPoint::operator==(const QgsPoint & other)
{
  if ((m_x == other.x()) && (m_y == other.y()))
    return true;
  else
    return false;
}

bool QgsPoint::operator!=(const QgsPoint & other) const
{
  if ((m_x == other.x()) && (m_y == other.y()))
    return false;
  else
    return true;
}

QgsPoint & QgsPoint::operator=(const QgsPoint & other)
{
  if (&other != this)
    {
      m_x = other.x();
      m_y = other.y();
    }

  return *this;
}

void QgsPoint::multiply(const double& scalar)
{
  m_x *= scalar;
  m_y *= scalar;
}