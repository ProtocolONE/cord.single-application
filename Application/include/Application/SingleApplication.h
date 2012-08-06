/****************************************************************************
** This file is a part of Syncopate Limited GameNet Application or it parts.
**
** Copyright (©) 2011 - 2012, Syncopate Limited and/or affiliates. 
** All rights reserved.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
****************************************************************************/

#ifndef _GGS_APPLICATION_SINGLEAPPLICATION_H_
#define _GGS_APPLICATION_SINGLEAPPLICATION_H_

#include <Application/Application_global.h>
#include <Application/ArgumentParser.h>

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtGui/QApplication>
#include <Windows.h>

namespace GGS {
  namespace Application {
    class APPLICATION_EXPORT SingleApplication : public QApplication
    {
      Q_OBJECT
    public:
      SingleApplication(int &argc, char *argv[], const QString& uniqueApplicationName);
      ~SingleApplication();

      bool isAlreadyRunning() const;

      void startListen();
      void sendArguments();
      void sendArguments(const QStringList& additionalArguments);
      void sendMessage(const QString& message);

      void setIpcPortPath(const QString& ipcPortPath);
      const QString& ipcPortPath();

      bool containsCommand(const QString& name);
      QStringList getCommandArguments(const QString& name);

      virtual bool notify(QObject* receiver, QEvent* event);

    public slots:
      void initializeFinished();
      void allowSecondInstance();

    signals:
      void messageRecived(QString message);
      void commandRecieved(QString name, QStringList arguments);
      void sendMessageFinished();

    private slots:
      void clientConnected();
      void clientReadyRead();
      void clientDisconnected();

    private:
      QString _ipcPortPath;
      bool _isAlreadyRunning;
      HANDLE _mutex;
      QTcpServer *_server;
      ArgumentParser _argumentParser;
    };
  }
}
#endif // _GGS_APPLICATION_SINGLEAPPLICATION_H_
