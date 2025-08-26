// Copyright 2025 Dolphin Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include "DolphinQt/QtUtils/VerticalTabsTabWidget.h"

#include <QProxyStyle>
#include <QStyleOptionTab>
#include <QStylePainter>

namespace
{
class VerticalTabStyle : public QProxyStyle
{
public:
  void drawControl(const ControlElement element, const QStyleOption* const option,
                   QPainter* const painter, const QWidget* const widget) const override
  {
    if (element == CE_TabBarTabLabel)
    {
      const QStyleOptionTab* const tab_option{qstyleoption_cast<const QStyleOptionTab*>(option)};
      if (tab_option)
      {
        QStyleOptionTab tab_option_copy(*tab_option);
        tab_option_copy.shape = QTabBar::RoundedNorth;
        QProxyStyle::drawControl(element, &tab_option_copy, painter, widget);
        return;
      }
    }

    QProxyStyle::drawControl(element, option, painter, widget);
  }

  QSize sizeFromContents(const ContentsType type, const QStyleOption* const option,
                         const QSize& size, const QWidget* const widget) const override
  {
    QSize content_size{QProxyStyle::sizeFromContents(type, option, size, widget)};
    if (type == QStyle::CT_TabBarTab)
    {
      content_size.transpose();
    }
    return content_size;
  }
};

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
      initStyleOption(&option, i);
      painter.drawControl(QStyle::CE_TabBarTabShape, option);

      painter.save();

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
