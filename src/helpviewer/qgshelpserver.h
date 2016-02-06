/***************************************************************************
                           qgshelpserver.h
    Receive help context numbers from client process for help viewer
                             -------------------
    begin                : 2005-07-07
    copyright            : (C) 2005 by Tom Elwertowski
    email                : telwertowski at comcast.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/* $Id: qgshelpserver.h 4234 2005-11-19 23:35:22Z gsherman $ */
#ifndef QGSHELPSERVER_H
#define QGSHELPSERVER_H
#include <q3serversocket.h>
#include <q3socket.h>

/*!
 * \class QgsHelpContextServer
 * \brief Listens for localhost connection and creates socket.
 */
class QgsHelpContextServer : public Q3ServerSocket
{
  Q_OBJECT
public:
  QgsHelpContextServer(QObject* parent = 0);
  ~QgsHelpContextServer();
  virtual void newConnection(int socket);

signals:
  void setContext(const QString&);
};

/*!
 * \class QgsHelpContextSocket
 * \brief Receives and passes context numbers to viewer.
 */
class QgsHelpContextSocket : public Q3Socket
{
  Q_OBJECT
public:
  QgsHelpContextSocket(int socket, QObject *parent = 0);
  ~QgsHelpContextSocket();

signals:
  void setContext(const QString&);

private slots:
  void readClient();
};

#endif // QGSHELPSERVER_H