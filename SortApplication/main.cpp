#include "SortApplication.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SortApplication window;
    window.show();
    return app.exec();
}
