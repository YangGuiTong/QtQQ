#pragma once

#include <QtWidgets/QWidget>
#include "ui_CCMainWindow.h"

class CCMainWindow : public QWidget
{
    Q_OBJECT

public:
    CCMainWindow(QWidget *parent = nullptr);
    ~CCMainWindow();

private:
    Ui::CCMainWindowClass ui;
};
