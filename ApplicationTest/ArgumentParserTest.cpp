#include <Application/ArgumentParser.h>

#include <QtCore/QStringList>
#include <QtCore/QEventLoop>
#include <QtCore/QTimer>

#include <gtest/gtest.h>
#include <QtTest/QSignalSpy>

using namespace GGS::Application;

class ArgumentParserTest : public ::testing::Test {
public:
  ArgumentParserTest() 
    : spy(&parser, SIGNAL(commandRecieved(QString, QStringList)))
  {
  }

  void SetUp()
  {
    parser.clear();
  }

  ArgumentParser parser;
  QSignalSpy spy;
};

TEST_F(ArgumentParserTest, cacheTest)
{
  QStringList commandlist1;
  commandlist1 << "fake" 
    << "/test1" << "123" << "qwe" << "asd" 
    << "/asd1" << "iiiiii1"
    << "-asd2" << "iiiiii2"
    << "--asd3" << "iiiiii3"
    << "--asd3" << "iiiiii4";

  QHash<QString, QStringList> expected;
  QStringList expectedArgs;
  expectedArgs << "iiiiii1";
  expected["asd1"] = expectedArgs;
  expectedArgs.clear();
  expectedArgs << "iiiiii2";
  expected["asd2"] = expectedArgs;
  expectedArgs.clear();
  expectedArgs << "iiiiii4";
  expected["asd3"] = expectedArgs;
  expectedArgs.clear();
  expectedArgs << "123" << "qwe" << "asd" ;
  expected["test1"] = expectedArgs;

  parser.parse(commandlist1);

  QEventLoop loop;
  QTimer::singleShot(1000, &loop, SLOT(quit()));
  QTimer::singleShot(0, &parser, SLOT(initFinished()));

  loop.exec();
  ASSERT_EQ(4, spy.count());
  
  for(int i = 0; i < spy.count(); ++i) {
    QString key = spy.at(i).at(0).toString();
    ASSERT_TRUE(expected.contains(key));
    QStringList recievedArgs = spy.at(i).at(1).toStringList();
    QStringList expectedAruments = expected[key];
    ASSERT_EQ(expectedAruments.size(), recievedArgs.size());
    for (int j = 0; j < expectedAruments.size(); ++j) {
      ASSERT_EQ(expectedAruments.at(j), recievedArgs.at(j));
    }
  }
}

TEST_F(ArgumentParserTest, afterInitTest)
{
  QStringList commandlist1;
  commandlist1 << "fake" 
    << "/test1" << "123" << "qwe" << "asd" 
    << "/asd1" << "iiiiii1"
    << "-asd2" << "iiiiii2"
    << "--asd3" << "iiiiii3"
    << "--asd3" << "iiiiii4";

  QHash<QString, QStringList> expected;
  QStringList expectedArgs;
  expectedArgs << "iiiiii1";
  expected["asd1"] = expectedArgs;
  expectedArgs.clear();
  expectedArgs << "iiiiii2";
  expected["asd2"] = expectedArgs;
  expectedArgs.clear();
  expectedArgs << "iiiiii4";
  expected["asd3"] = expectedArgs;
  expectedArgs.clear();
  expectedArgs << "123" << "qwe" << "asd" ;
  expected["test1"] = expectedArgs;

  parser.parse(commandlist1);
  parser.initFinished();

  QEventLoop loop;
  QTimer::singleShot(1000, &loop, SLOT(quit()));
  //QTimer::singleShot(0, &parser, SLOT(initFinished()));

  loop.exec();
  ASSERT_EQ(4, spy.count());

  for(int i = 0; i < spy.count(); ++i) {
    QString key = spy.at(i).at(0).toString();
    ASSERT_TRUE(expected.contains(key));
    QStringList recievedArgs = spy.at(i).at(1).toStringList();
    QStringList expectedAruments = expected[key];
    ASSERT_EQ(expectedAruments.size(), recievedArgs.size());
    for (int j = 0; j < expectedAruments.size(); ++j) {
      ASSERT_EQ(expectedAruments.at(j), recievedArgs.at(j));
    }
  }
}

TEST_F(ArgumentParserTest, uriArgsTest)
{
  QStringList commandlist1;
  commandlist1 << "/uri" << "gamenet://somecommand/args1/args2/";

  QHash<QString, QStringList> expected;
  QStringList expectedArgs;
  expectedArgs << "args1";
  expectedArgs << "args2";
  expected["somecommand"] = expectedArgs;

  parser.parse(commandlist1);
  parser.initFinished();

  QEventLoop loop;
  QTimer::singleShot(1000, &loop, SLOT(quit()));
  loop.exec();
  ASSERT_EQ(1, spy.count());

  for(int i = 0; i < spy.count(); ++i) {
    QString key = spy.at(i).at(0).toString();
    ASSERT_TRUE(expected.contains(key));
    QStringList recievedArgs = spy.at(i).at(1).toStringList();
    QStringList expectedAruments = expected[key];
    ASSERT_EQ(expectedAruments.size(), recievedArgs.size());
    for (int j = 0; j < expectedAruments.size(); ++j) {
      ASSERT_EQ(expectedAruments.at(j), recievedArgs.at(j));
    }
  }
}

TEST_F(ArgumentParserTest, uriArgsTest2)
{
  QStringList commandlist1;
  commandlist1 << "/uri:gamenet://somecommand/args1/args2/";

  QHash<QString, QStringList> expected;
  QStringList expectedArgs;
  expectedArgs << "args1";
  expectedArgs << "args2";
  expected["somecommand"] = expectedArgs;

  parser.parse(commandlist1);
  parser.initFinished();

  QEventLoop loop;
  QTimer::singleShot(1000, &loop, SLOT(quit()));
  loop.exec();
  ASSERT_EQ(1, spy.count());

  for(int i = 0; i < spy.count(); ++i) {
    QString key = spy.at(i).at(0).toString();
    ASSERT_TRUE(expected.contains(key));
    QStringList recievedArgs = spy.at(i).at(1).toStringList();
    QStringList expectedAruments = expected[key];
    ASSERT_EQ(expectedAruments.size(), recievedArgs.size());
    for (int j = 0; j < expectedAruments.size(); ++j) {
      ASSERT_EQ(expectedAruments.at(j), recievedArgs.at(j));
    }
  }
}

TEST_F(ArgumentParserTest, noArgsTest)
{
  QStringList commandlist;
  parser.parse(commandlist);
  parser.initFinished();

  QEventLoop loop;
  QTimer::singleShot(1000, &loop, SLOT(quit()));
  loop.exec();
  
  ASSERT_EQ(1, spy.count());

  QString key = spy.at(0).at(0).toString();
  ASSERT_EQ("empty", key);
}