#ifndef GOLDITEMDELEGATE_H
#define GOLDITEMDELEGATE_H

#include <QObject>
#include <QItemDelegate>
#include <QDebug>
#include <QPainter>
#include <QImage>
#include <math.h>
#include <QMessageBox>
class GoldItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    GoldItemDelegate(QObject* parent = nullptr) {}
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        painter->save ();
        QRectF rect;
        rect.setX(option.rect.x());
        rect.setY(option.rect.y());
        rect.setWidth( option.rect.width()-1);
        rect.setHeight(option.rect.height()-1);

        painter->setPen( Qt::black );

        float currentPrice = index.data().value<float>();
        currentPrice = roundf(currentPrice * 100) / 100;
        //qDebug() << "current price:  " << currentPrice;

        QRect textRect = QRect(rect.left() + 5, rect.top() + 10, rect.width() - 40, rect.height () - 10);
        QRect imageRect = QRect(rect.left() + 10 + rect.width () - 50, rect.top() + 5, 38 , rect.height () - 5);
        painter->drawText (textRect, QString::number (currentPrice));

        //qDebug() << "rect widht " << rect.width () << " rect height " << rect.height ();
        //qDebug() << "textRect widht " << textRect.width () << " textRect height " << textRect.height ();
        //qDebug() << "imageRect widht " << imageRect.width () << " imageRect height " << imageRect.height ();

        QModelIndex prevIndex = index.model ()->index (index.row () - 1, index.column ());
        if(prevIndex.isValid ()) {
            float previousPrice = prevIndex.data ().value<float>();
            previousPrice = roundf(previousPrice * 100) / 100;
            float deltaPrice = currentPrice - previousPrice;
            if(deltaPrice < 0) {
                QImage *img = new QImage(":/images/arrow_down.png");
                painter->drawImage(imageRect, *img);
            }
            if(deltaPrice > 0) {
                QImage *img = new QImage(":/images/arrow_up.png");
                painter->drawImage(imageRect, *img);
            }
            //qDebug() << "previous price: " << previousPrice;
        }
        painter->restore ();
    }
};

#endif // GOLDITEMDELEGATE_H
