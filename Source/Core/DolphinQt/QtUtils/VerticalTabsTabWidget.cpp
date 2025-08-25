// Copyright 2025 Dolphin Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include "DolphinQt/QtUtils/VerticalTabsTabWidget.h"

#include <QProxyStyle>
#include <QStyleOptionTab>

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
}  // namespace

namespace QtUtils
{

VerticalTabsTabWidget::VerticalTabsTabWidget(QWidget* const parent) : QTabWidget(parent)
{
  setTabPosition(QTabWidget::TabPosition::West);

  static VerticalTabStyle s_vertical_tab_style;
  tabBar()->setStyle(&s_vertical_tab_style);
}

}  // namespace QtUtils
