#ifndef MYCHART_H
#define MYCHART_H

#include <QObject>
#include <QMouseEvent>
#include <QChartView>

class MyChart : public QChartView
{
public:
    MyChart();

protected:
    bool viewportEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

};

#endif // MYCHART_H
