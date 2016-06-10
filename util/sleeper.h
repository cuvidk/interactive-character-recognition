#ifndef SLEEPER_H
#define SLEEPER_H

#include <QThread>

class Sleeper : public QThread {
public:
   void sleep(int ms) { QThread::sleep(ms); }
};

#endif // SLEEPER_H
