#include <gtest/gtest.h>
#include <QtGui/QApplication>
#include <QtCore/QDebug>
#include <QtCore/QProcess>
#include <QtCore/QStringList>

#include <Windows.h>

class SingleApplicationTest : public ::testing::Test 
{
public:
  virtual void SetUp()
  {
    fixturePath = QString("%1/SingleApplicationFixture.exe").arg(QApplication::applicationDirPath());
    killAll("SingleApplicationFixture.exe");
  }

  QString getStartResult()
  {
    QProcess p1;
    p1.start(fixturePath);
    p1.setReadChannel(QProcess::StandardError);
    p1.waitForReadyRead(5000);
    QString result1(p1.readAllStandardError());
    p1.close();
    p1.waitForStarted(5000);
    return result1.trimmed();
  }

  void killAll(const QString& name)
  {
    QString dir = QCoreApplication::applicationDirPath();
    QString exe = "cmd.exe";
    QString arg1 = QString(" /C \"taskkill /F /IM %1\" ").arg(name);
    QString open("open");

    SHELLEXECUTEINFOW shex;
    ZeroMemory(&shex, sizeof(shex));

    shex.cbSize			  = sizeof( SHELLEXECUTEINFO ); 
    shex.fMask			  = 0; 
    shex.hwnd			    = 0;
    shex.lpVerb			  = reinterpret_cast<const WCHAR*>(open.utf16()); 
    shex.lpFile			  = reinterpret_cast<const WCHAR*>(exe.utf16()); 
    shex.lpParameters	= reinterpret_cast<const WCHAR*>(arg1.utf16()); 
    shex.lpDirectory	= reinterpret_cast<const WCHAR*>(dir.utf16()); 
    shex.nShow			  = SW_NORMAL; 

    ::ShellExecuteExW(&shex);
  }

  QString fixturePath;
};

TEST_F(SingleApplicationTest, DoubleExecute)
{
  ASSERT_EQ("false", getStartResult());
  ASSERT_EQ("false", getStartResult());
  QProcess p1;
  p1.start(fixturePath);
  p1.waitForStarted(5000);
  ASSERT_EQ("true", getStartResult());
  ASSERT_EQ("true", getStartResult());
  p1.close();
  p1.waitForFinished(5000);
  ASSERT_EQ("false", getStartResult());
  ASSERT_EQ("false", getStartResult());
}

TEST_F(SingleApplicationTest, MessageTest)
{
  QStringList args1;
  args1 << "123" << "test" << "asd";
  QProcess server;
  server.start(fixturePath, args1);
  server.setReadChannel(QProcess::StandardError);
  server.waitForStarted(5000);
  server.waitForReadyRead(10000);
  server.readAllStandardError(); // clear all current output

  QProcess client;
  client.start(fixturePath, args1);
  client.waitForStarted(5000);
  server.waitForReadyRead(10000);
  
  QString result(server.readAllStandardError());
  
  server.close();
  client.close();

  ASSERT_EQ("\"123|test|asd|-activate\"", result.trimmed());
}
