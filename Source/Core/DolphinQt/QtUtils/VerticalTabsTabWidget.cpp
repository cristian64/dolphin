// Copyright 2025 Dolphin Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include "DolphinQt/QtUtils/VerticalTabsTabWidget.h"

#include <QStyleOptionTab>
#include <QStylePainter>

namespace
{
class HorizontalTabBar : public QTabBar
{
public:
  explicit HorizontalTabBar(QWidget* const parent = nullptr) : QTabBar(parent)
  {
    setUsesScrollButtons(false);
  }

protected:
  QSize tabSizeHint(const int index) const override
  {
    return QTabBar::tabSizeHint(index).transposed();
  }

  void paintEvent(QPaintEvent* const event) override
  {
    QStylePainter painter(this);
    QStyleOptionTab option;

    const int tab_count{count()};
    for (int i{0}; i < tab_count; ++i)
    {
      painter.save();

      initStyleOption(&option, i);
      painter.drawControl(QStyle::CE_TabBarTabShape, option);

      const QSize size{option.rect.size().transposed()};
      QRect rect(QPoint(), size);
      rect.moveCenter(option.rect.center());
      option.rect = rect;

      const QPoint center{tabRect(i).center()};
      painter.translate(center);
      painter.rotate(90.0);
      painter.translate(-center);
      painter.drawControl(QStyle::CE_TabBarTabLabel, option);

      painter.restore();
    }
  }
};
}  // namespace

namespace QtUtils
{

VerticalTabsTabWidget::VerticalTabsTabWidget(QWidget* const parent) : QTabWidget(parent)
{
  setTabBar(new HorizontalTabBar);
  setTabPosition(QTabWidget::TabPosition::West);
}

}  // namespace QtUtils
