/***************************************************************************
                qgscsexception.h - Coordinate System Exception
                             -------------------
  begin                : 2004-12-29
  copyright            : (C) 2004 by Gary Sherman
  email                : sherman at mrcc dot com
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/* $Id: qgscsexception.h 6415 2007-01-09 02:39:15Z wonder $ */
#ifndef QGSCSEXCEPTION_H
#define QGSCSEXCEPTION_H

#include "qgsexception.h"
class CORE_EXPORT QgsCsException : public QgsException
{
  public:
    QgsCsException(std::string const &what) : QgsException(what){};
    QgsCsException(QString const &what) : QgsException(what){};
    
};
#endif //QGCSEXCEPTION_H