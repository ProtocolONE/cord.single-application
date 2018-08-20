#include <Application/SingleApplication.h>
#include <Application/SingleApplicationEventFilter.h>

#include <QtCore/QObject>
#include <QtCore/QSettings>

#define SIGNAL_CONNECT_CHECK(X) { bool result = X; Q_ASSERT_X(result, __FUNCTION__ , #X); }

namespace P1 {
  namespace Application {
    SingleApplication::SingleApplication(int &argc, char *argv[], const QString& uniqueApplicationName)
      : QApplication(argc, argv, true)
      , _mutex(INVALID_HANDLE_VALUE)
      , _eventFilter(new SingleApplicationEventFilter(this))
    {
      QString fullName = QString("Global\\%1").arg(uniqueApplicationName);
      Q_ASSERT(fullName.size() < MAX_PATH);
      wchar_t name[MAX_PATH] = {0};
      fullName.toWCharArray(name);
      this->_mutex = ::CreateMutexW(0, 0, name);
      this->_isAlreadyRunning = ::GetLastError() != ERROR_SUCCESS;
      SIGNAL_CONNECT_CHECK(QObject::connect(&this->_argumentParser, SIGNAL(commandRecieved(QString, QStringList)), 
        this, SIGNAL(commandRecieved(QString, QStringList))));
      this->_argumentParser.parse(this->arguments());

      SIGNAL_CONNECT_CHECK(QObject::connect(this->_eventFilter, SIGNAL(forceQuit()), this, SIGNAL(forceQuit())));
      this->installNativeEventFilter(this->_eventFilter);
    }

    SingleApplication::~SingleApplication()
    {
      if (this->_mutex != INVALID_HANDLE_VALUE)
        ::CloseHandle(this->_mutex);
    }

    bool SingleApplication::notify(QObject* receiver, QEvent* event)
    {
      try {
        return QApplication::notify(receiver, event);
      } catch(std::exception& e) {
        qCritical() << "Exception thrown:" << e.what();
      }
      return false;
    }

    bool SingleApplication::isAlreadyRunning() const
    {
      return this->_isAlreadyRunning;
    }

    void SingleApplication::startListen()
    {
      this->_server = new QTcpServer(this);
      SIGNAL_CONNECT_CHECK(QObject::connect(this->_server, SIGNAL(newConnection()), this, SLOT(clientConnected())));
      this->_server->listen(QHostAddress::LocalHost, 0);
      quint16 port = this->_server->serverPort();
      qDebug() << "Listen started: " << port;
      QSettings settings(this->_ipcPortPath, QSettings::NativeFormat);
      settings.setValue("SharedPort", port);
    }

    void SingleApplication::clientConnected()
    {
      QTcpSocket *client = this->_server->nextPendingConnection();
      SIGNAL_CONNECT_CHECK(QObject::connect(client, SIGNAL(readyRead()), this, SLOT(clientReadyRead())));
      SIGNAL_CONNECT_CHECK(QObject::connect(client, SIGNAL(disconnected()), this, SLOT(clientDisconnected())));
    }

    void SingleApplication::clientReadyRead()
    {
      QTcpSocket *client = qobject_cast<QTcpSocket*>(QObject::sender());
      if (!client)
        return;

      QByteArray buffer = client->readAll();
      QString message = QString::fromUtf8(buffer.data(), buffer.size());
      emit this->messageRecived(message);

      QStringList args = message.split('|', QString::SkipEmptyParts);
      this->_argumentParser.parse(args);
    }

    void SingleApplication::clientDisconnected()
    {
      QTcpSocket *client = qobject_cast<QTcpSocket*>(QObject::sender());
      if (!client)
        return;

      client->deleteLater();
    }

    void SingleApplication::sendMessage(const QString& message)
    {
      QSettings settings(this->_ipcPortPath, QSettings::NativeFormat);
      bool ok;
      quint16 port = settings.value("SharedPort", 0).toUInt(&ok);
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

    void SingleApplication::sendArguments()
    {
      QStringList args;
      this->sendArguments(args);
    }

    void SingleApplication::sendArguments(const QStringList& additionalArguments)
    {
      QStringList args = this->arguments();
      args.removeFirst();
      Q_FOREACH(QString arg, additionalArguments) {
        args << arg;
      }

      QString tmp = args.join("|");
      this->sendMessage(tmp);
    }

    void SingleApplication::initializeFinished()
    {
      this->_argumentParser.initFinished();
    }

    bool SingleApplication::containsCommand(const QString& name)
    {
      return this->_argumentParser.contains(name);
    }

    QStringList SingleApplication::getCommandArguments(const QString& name)
    {
      return this->_argumentParser.commandArguments(name);
    }

    void SingleApplication::allowSecondInstance()
    {
      if (this->_mutex != INVALID_HANDLE_VALUE) {
        ::CloseHandle(this->_mutex);
        this->_mutex = INVALID_HANDLE_VALUE;
      }
    }
  }
}