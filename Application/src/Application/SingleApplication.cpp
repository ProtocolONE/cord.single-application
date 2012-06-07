/****************************************************************************
** This file is a part of Syncopate Limited GameNet Application or it parts.
**
** Copyright (©) 2011 - 2012, Syncopate Limited and/or affiliates. 
** All rights reserved.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
****************************************************************************/

#include <Application/SingleApplication.h>
#include <QtCore/QObject>
#include <QtCore/QSettings>

namespace GGS {
  namespace Application {

    SingleApplication::SingleApplication(int &argc, char *argv[], const QString& uniqueApplicationName)
      : QApplication(argc, argv, true)
      , _mutex(INVALID_HANDLE_VALUE)
    {
      QString fullName = QString("Global\\%1").arg(uniqueApplicationName);
      Q_ASSERT(fullName.size() < MAX_PATH);
      wchar_t name[MAX_PATH] = {0};
      fullName.toWCharArray(name);
      this->_mutex = ::CreateMutexW(0, 0, name);
      this->_isAlreadyRunning = ::GetLastError() != ERROR_SUCCESS;
    }

    SingleApplication::~SingleApplication()
    {
      if (this->_mutex != INVALID_HANDLE_VALUE)
        ::CloseHandle(this->_mutex);
    }

    bool SingleApplication::isAlreadyRunning() const
    {
      return this->_isAlreadyRunning;
    }

    void SingleApplication::startListen()
    {
      this->_server = new QTcpServer(this);
      QObject::connect(this->_server, SIGNAL(newConnection()), this, SLOT(clientConnected()));
      this->_server->listen(QHostAddress::LocalHost, 0);
      quint16 port = this->_server->serverPort();
      qDebug() << "Listen started: " << port;
      QSettings settings(this->_ipcPortPath, QSettings::NativeFormat);
      settings.setValue("sharedport", port);
    }

    void SingleApplication::clientConnected()
    {
      QTcpSocket *client = this->_server->nextPendingConnection();
      QObject::connect(client, SIGNAL(readyRead()), this, SLOT(clientReadyRead()));
      QObject::connect(client, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
    }

    void SingleApplication::clientReadyRead()
    {
      QTcpSocket *client = qobject_cast<QTcpSocket*>(QObject::sender());
      QByteArray buffer = client->readAll();
      QString message = QString::fromUtf8(buffer.data(), buffer.size());
      emit this->messageRecived(message);
    }

    void SingleApplication::clientDisconnected()
    {
      QTcpSocket *client = qobject_cast<QTcpSocket*>(QObject::sender());
      client->deleteLater();
    }

    void SingleApplication::sendMessage(const QString& message)
    {
      //QSettings settings("HKEY_CURRENT_USER\\Software\\GGS\\QGNA", QSettings::NativeFormat);
      QSettings settings(this->_ipcPortPath, QSettings::NativeFormat);
      bool ok;
      quint16 port = settings.value("sharedport", 0).toUInt(&ok);
      if (!ok || port == 0) {
        qDebug() << "Server port not found";
        emit this->sendMessageFinished();
        return;
      }

      QTcpSocket *client = new QTcpSocket(this);
      client->connectToHost(QHostAddress::LocalHost, port);
      if (!client->waitForConnected(5000)) {
        qDebug() << "Can't connect to server";
        emit this->sendMessageFinished();
        return;
      }

      client->write(message.toUtf8());
      client->waitForBytesWritten(5000);
      emit this->sendMessageFinished();
    }

    void SingleApplication::setIpcPortPath(const QString& ipcPortPath)
    {
      this->_ipcPortPath = ipcPortPath;
    }

    const QString& SingleApplication::ipcPortPath()
    {
      return this->_ipcPortPath;
    }

  }
}