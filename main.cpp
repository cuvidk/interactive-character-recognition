#include "ocrappmanager.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    OcrAppManager manager;

    return a.exec();
}
