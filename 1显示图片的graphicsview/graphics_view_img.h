#ifndef GRAPHICS_VIEW_IMG_H
#define GRAPHICS_VIEW_IMG_H

#include <QGraphicsPixmapItem>
#include <QResizeEvent>
#include <QGraphicsView>

class GraphicsViewImg : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsViewImg(QWidget *parent);

public:
    void set_img(const QPixmap &img);
    QSize get_size();

signals:
    void trig_size_changed();

protected:
    void resizeEvent(QResizeEvent *event);

private:
    void resize_img();

private:
    QGraphicsPixmapItem *img_slice;

};

#endif // GRAPHICS_VIEW_IMG_H
