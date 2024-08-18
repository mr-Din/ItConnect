#include <QGraphicsDropShadowEffect>
#include <QWidget>

#pragma once

namespace utility {
inline void addShadowCurrentObj(QWidget* wgt)
{
    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect;
    shadowEffect->setBlurRadius(5);
    shadowEffect->setOffset(2, 2);
    shadowEffect->setColor(Qt::black);
    wgt->setGraphicsEffect(shadowEffect);
}

template<typename Obj>
void addShadowToObj (QWidget* parent)
{
    for (auto* child : parent->findChildren<Obj>())
    {
        addShadowCurrentObj(child);
    }
}
} // utility
