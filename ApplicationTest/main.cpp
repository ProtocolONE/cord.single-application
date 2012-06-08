
#include <QtCore/QCoreApplication>
#include <gtest/gtest.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    testing::InitGoogleTest(&argc, argv);
    int r = RUN_ALL_TESTS();
    return r;
}

#include <QtCore/QUrl>
#include <QtCore/QStringList>

TEST(fake,fake)
{
  //QString str("qwjklehqjklweh");
  QString str("qwe://asd/zxc/1111/");
  QUrl url(str);
  bool res = url.isValid();
  QString prefix = url.scheme();
  QString commandName = url.host();
  QString argsString = url.path();
  QStringList args = argsString.split('/', QString::SkipEmptyParts);
  QString tmp = args.join("|");
}