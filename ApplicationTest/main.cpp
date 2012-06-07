
#include <QtCore/QCoreApplication>
#include <gtest/gtest.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    testing::InitGoogleTest(&argc, argv);
    int r = RUN_ALL_TESTS();
    return r;
}


TEST(sample, sample)
{
}