#ifndef OCRAPPMANAGER_H
#define OCRAPPMANAGER_H

#include <QObject>

class MainWindow;
class ScribbleArea;
class CharacterClassifier;

class OcrAppManager
{
public:
    OcrAppManager();
    ~OcrAppManager();

private:
    MainWindow *mainWindow;
    ScribbleArea *scribbleArea;
    CharacterClassifier *characterClassifier;

    void manageConnections();
};

#endif // OCRAPPMANAGER_H
