#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>

#include "public_type.h"

#pragma execution_character_set("utf-8") // qt支持显示中文

enum ButtonType{
	MIN_BUTTON = 0,			// 最小化及关闭按钮
	MIX_MAX_BUTTON = 1,		// 最小化、最大化及关闭按钮
	QNLY_CLOSE_BUTTON = 2,	// 只有关闭按钮
	
	BUTTON_TYPE_COUNT					// 默认值
};

class TitleBar : public QWidget{
	Q_OBJECT

public:
	TitleBar(QWidget *parent = nullptr);
	~TitleBar();

	// 设置标题栏图标
	void setrTitleIcon(const QString &filePath);
	// 设置标题栏内容
	void setTitleContent(const QString &titleContent);
	// 设置标题栏长度
	void setTitleWidth(int width);
	// 设置标题栏按钮类型
	void setButtonType(ButtonType buttonType);

	// 保存窗口最大化前窗口的位置及大小
	void saveRestoreInfo(const QPoint &point, const QSize &size);
	void getRestoreInfo(QPoint &point, QSize &size);

private:
	// 绘图事件
	void paintEvent(QPaintEvent *event);
	// 鼠标双击事件
	void mouseDoubleClickEvent(QMouseEvent *event);
	// 鼠标按下事件
	void mousePressEvent(QMouseEvent *event);
	// 鼠标移动事件
	void mouseMoveEvent(QMouseEvent *event);
	// 鼠标松开事件
	void mouseReleaseEvent(QMouseEvent *event);

	void initControl();									// 初始化控件
	void initConnections();								// 初始化信号与槽的连接
	void loadStyleSheet(const QString &sheetName);		// 加载样式表


signals:
	// 标题栏按钮点击时触发的信号
	void signalButtonMinClicked();		// 最小化按钮
	void signalButtonRestoreClicked();	// 最大化还原按钮
	void signalButtonMaxClicked();		// 最大化按钮
	void signalButtonCloseClicked();	// 关闭按钮


private slots:
	// 按钮响应槽函数
	void onButtonMinClicked();			
	void onButtonRestoreClicked();
	void onButtonMaxClicked();
	void onButtonCloseClicked();


private:
	QLabel *m_pIcon;					// 标题栏图标
	QLabel *m_pTitleContent;			// 标题栏内容
	QPushButton *m_pButtonMin;			// 最小化按钮
	QPushButton *m_pButtonRestore;		// 最大化还原按钮
	QPushButton *m_pButtonMax;			// 最大化按钮
	QPushButton *m_ButtonClose;			// 关闭按钮

	// 最大化還原按鈕变量，用于保存窗体位置及大小
	QPoint m_restorePos;
	QSize m_restoreSize;

	// 移动窗体的变量
	bool m_isPressed;
	QPoint m_startMovePos;

	QString m_titleContent;		// 标题栏内容
	ButtonType m_buttonType;	// 标题栏按钮类型
};
