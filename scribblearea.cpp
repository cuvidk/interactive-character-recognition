#include "scribblearea.h"

ScribbleArea::ScribbleArea(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    scribbling = false;
    demoRunning = false;
}

void ScribbleArea::mousePressEvent(QMouseEvent *event)
{
    if (!demoRunning && event->button() == Qt::LeftButton)
    {
        scribbling = true;
        lastPoint = event->pos();
    }
}

void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{
    if (!demoRunning && (event->buttons() & Qt::LeftButton) && scribbling)
        drawLineTo(event->pos());
}

void ScribbleArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (!demoRunning && event->button() == Qt::LeftButton && scribbling)
    {
        scribbling = false;
        drawLineTo(event->pos());
    }
}

void ScribbleArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect rect = event->rect();
    painter.drawImage(rect, image, rect);
}

void ScribbleArea::resizeEvent(QResizeEvent *event)
{
    if (width() > image.width() || height() > image.height())
    {
        int newWidth = qMax(width(), image.width());
        int newHeight = qMax(height(), image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

void ScribbleArea::resizeImage(QImage *image, const QSize& newSize)
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(0, 0, 0));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

void ScribbleArea::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&image);

    int penSize = 10;
    painter.setPen(QPen(Qt::white, penSize, Qt::SolidLine,
                        Qt::SquareCap, Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);

    int rad = (penSize / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized().adjusted(
               -rad, -rad, +rad, +rad));

    lastPoint = endPoint;
}

void ScribbleArea::clearScreen()
{
    demoRunning = false;
    image.fill(qRgb(0, 0, 0));
    update();
}

void ScribbleArea::emitImageSignal()
{
    emit imageSignal(image);
}

void ScribbleArea::setImage(QImage newImage)
{
    image = newImage.scaled(width(), height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    update();
}

void ScribbleArea::setAreaUnrenderable()
{
    demoRunning = true;
}
