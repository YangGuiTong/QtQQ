#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>

#include "public_type.h"

#pragma execution_character_set("utf-8") // qt֧����ʾ����

enum ButtonType{
	MIN_BUTTON = 0,			// ��С�����رհ�ť
	MIX_MAX_BUTTON = 1,		// ��С������󻯼��رհ�ť
	QNLY_CLOSE_BUTTON = 2,	// ֻ�йرհ�ť
	
	BUTTON_TYPE_COUNT					// Ĭ��ֵ
};

class TitleBar : public QWidget{
	Q_OBJECT

public:
	TitleBar(QWidget *parent = nullptr);
	~TitleBar();

	// ���ñ�����ͼ��
	void setrTitleIcon(const QString &filePath);
	// ���ñ���������
	void setTitleContent(const QString &titleContent);
	// ���ñ���������
	void setTitleWidth(int width);
	// ���ñ�������ť����
	void setButtonType(ButtonType buttonType);

	// ���洰�����ǰ���ڵ�λ�ü���С
	void saveRestoreInfo(const QPoint &point, const QSize &size);
	void getRestoreInfo(QPoint &point, QSize &size);

private:
	// ��ͼ�¼�
	void paintEvent(QPaintEvent *event);
	// ���˫���¼�
	void mouseDoubleClickEvent(QMouseEvent *event);
	// ��갴���¼�
	void mousePressEvent(QMouseEvent *event);
	// ����ƶ��¼�
	void mouseMoveEvent(QMouseEvent *event);
	// ����ɿ��¼�
	void mouseReleaseEvent(QMouseEvent *event);

	void initControl();									// ��ʼ���ؼ�
	void initConnections();								// ��ʼ���ź���۵�����
	void loadStyleSheet(const QString &sheetName);		// ������ʽ��


signals:
	// ��������ť���ʱ�������ź�
	void signalButtonMinClicked();		// ��С����ť
	void signalButtonRestoreClicked();	// ��󻯻�ԭ��ť
	void signalButtonMaxClicked();		// ��󻯰�ť
	void signalButtonCloseClicked();	// �رհ�ť


private slots:
	// ��ť��Ӧ�ۺ���
	void onButtonMinClicked();			
	void onButtonRestoreClicked();
	void onButtonMaxClicked();
	void onButtonCloseClicked();


private:
	QLabel *m_pIcon;					// ������ͼ��
	QLabel *m_pTitleContent;			// ����������
	QPushButton *m_pButtonMin;			// ��С����ť
	QPushButton *m_pButtonRestore;		// ��󻯻�ԭ��ť
	QPushButton *m_pButtonMax;			// ��󻯰�ť
	QPushButton *m_ButtonClose;			// �رհ�ť

	// ���߀ԭ���o���������ڱ��洰��λ�ü���С
	QPoint m_restorePos;
	QSize m_restoreSize;

	// �ƶ�����ı���
	bool m_isPressed;
	QPoint m_startMovePos;

	QString m_titleContent;		// ����������
	ButtonType m_buttonType;	// ��������ť����
};
