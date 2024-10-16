#ifndef GOLDITEM_H
#define GOLDITEM_H

#include <QDateTime>
#include <QFloat16>

class GoldItem
{
public:
    GoldItem() {}
    GoldItem(QDateTime dt, qfloat16 pp, qfloat16 ps) {
        m_DateTime = dt;
        m_PricePurchase = pp;
        m_PriceSell = ps;
    }

    QDateTime DateTime() const { return m_DateTime; }
    void setDateTime(const QDateTime &DateTime) { m_DateTime = DateTime; }

    double PricePurchase() const { return m_PricePurchase; }
    void setPricePurchase(const double &PricePurchase) { m_PricePurchase = PricePurchase; }

    double PriceSell() const { return m_PriceSell; }
    void setPriceSell(const double &PriceSell) { m_PriceSell = PriceSell; }

private:
    QDateTime m_DateTime;
    double m_PricePurchase;
    double m_PriceSell;
};

#endif // GOLDITEM_H
