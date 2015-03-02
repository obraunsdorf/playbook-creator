#include "dialogs/mainDialog.h"
#include <QApplication>
#include <boost/version.hpp>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainDialog w;
    w.show();

    std::cout << "built with Qt version: " << QT_VERSION_STR << std::endl;
    std::cout << "built with boost version: " << BOOST_LIB_VERSION << std::endl;
    return a.exec();
}
