#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>

class ScribbleArea : public QWidget
{
    Q_OBJECT
public:
    explicit ScribbleArea(QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

signals:
    void imageSignal(QImage image);
    void imageToLearnSignal(QImage image);

public slots:
    void clearScreen();
    void emitImageSignal();
    void emitImageToLearn();
    void setImage(QImage newImage);
    void setAreaUnrenderable();

private:
    bool scribbling;
    bool demoRunning;
    QImage image;
    QPoint lastPoint;

    void drawLineTo(const QPoint& endPoint);
    void resizeImage(QImage* image, const QSize& size);
};

#endif // SCRIBBLEAREA_H
