#pragma once

#include <QtWidgets/QWidget>
#include <QTreeWidgetItem>

#include "ui_CCMainWindow.h"
#include "BasicWindow.h"
#include "RootContatItem.h"
#include "Contactltem.h"


class CCMainWindow : public BasicWindow
{
    Q_OBJECT

public:
    CCMainWindow(QString account, bool isAccountLogin, QWidget *parent = nullptr);
    ~CCMainWindow();

	
	void setUserName(const QString &username);
	void setLevePixmap(int level);
	void setHeadPixmap(const QString &headPath);			// ����ͷ��
	void setStatusMenuIcon(const QString &statusPath);		// ��������״̬

	// ���Ӧ�ò���(appͼƬ·����app����������)
	QWidget *addOtherAppExtension(const QString &appPath, const QString appName);
	void initContactTree();

private:
	void initTimer();										// ��ʼ���ȼ���ʱ��
	void initControl();
	void updateSeachStyle();								// ����������ʽ
	void addCompanyDeps(QTreeWidgetItem *pRootGroupItem, int DepID);
	QString getHeadPixturePath();							// ��ȡ��¼�ߵ�ͷ��·��

	// ��ȡ���ݿ��е������¼
	void ReadDatabaseMessage();

	// ��ʼ������ǩ��
	void InitSign();

private:
	void resizeEvent(QResizeEvent *event);
	bool eventFilter(QObject *obj, QEvent *event);
	void mousePressEvent(QMouseEvent *event);

private slots:
	void onAppIconClicked();
	void onItemClicked(QTreeWidgetItem *item, int column);
	void onItemExpanded(QTreeWidgetItem *item);
	void onItemCollapsed(QTreeWidgetItem *item);
	void onItemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    Ui::CCMainWindowClass ui;

	QString m_account;
	bool m_isAccountLogin;

	//QMap<QTreeWidgetItem *, QString> m_groupMap;	// ���з�����
};
