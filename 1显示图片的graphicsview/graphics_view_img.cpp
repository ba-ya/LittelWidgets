#include "graphics_view_img.h"

#include <QGraphicsScene>
#include <QGraphicsView>

GraphicsViewImg::GraphicsViewImg(QWidget *parent) : QGraphicsView(parent) {
    QGraphicsScene *scene = new QGraphicsScene();
    setScene(scene);

    // img
    img_slice = scene->addPixmap(QPixmap());
    img_slice->setTransformOriginPoint(img_slice->boundingRect().topLeft());
    img_slice->setZValue(0);

    // 禁用滚动条
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void GraphicsViewImg::set_img(const QPixmap &img)
{
    img_slice->setPixmap(img);
    resize_img();
}

QSize GraphicsViewImg::get_size()
{
    return QSize(viewport()->width(), viewport()->height());
}

void GraphicsViewImg::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    emit trig_size_changed();
}

void GraphicsViewImg::resize_img()
{
    auto w = viewport()->width();
    auto h = viewport()->height();
    scene()->setSceneRect(0, 0, w, h);

    img_slice->setPos((w - img_slice->pixmap().width()) / 2,
                      (h - img_slice->pixmap().height()) / 2);
    fitInView(img_slice, Qt::KeepAspectRatio);
}
