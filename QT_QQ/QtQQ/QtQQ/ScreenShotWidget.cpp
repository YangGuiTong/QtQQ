#include "screenshotwidget.h"

#include <QStandardPaths>
 
ScreenShotWidget::ScreenShotWidget(QWidget *parent) :
    QWidget(parent)
{
    //初始化截图激活标志
    m_screenshot_active=false;
    //设置窗口，无边框，顶部显示
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setWindowModality(Qt::ApplicationModal);
    //窗口背景为透明
    setAttribute(Qt::WA_TranslucentBackground);
    //设置窗口跟踪鼠标移动事件
    setMouseTracking(true);
    //设置保存、取消按钮以及长宽label并暂时隐藏
    buttonSave=new QPushButton(this);
    buttonSave->setParent(this);
    buttonSave->setFixedSize(35,30);
    buttonSave->setText("保存");
    buttonSave->setStyleSheet("QPushButton {font-size:8pt; color:green; font-weight:800;}");
    buttonSave->setVisible(false);
    buttonCancel=new QPushButton(this);
    buttonCancel->setParent(this);
    buttonCancel->setFixedSize(35,30);
    buttonCancel->setText("关闭");
    buttonCancel->setStyleSheet("QPushButton {font-size:8pt; color:red;font-weight:100;}");
    buttonCancel->setVisible(false);
    lblHeightWidth=new QLabel(this);
    lblHeightWidth->setParent(this);
    lblHeightWidth->setFixedSize(100,10);
    lblHeightWidth->setStyleSheet("QLabel {color:white;}");
    lblHeightWidth->setVisible(false);
    //设置放大镜
    lblMagnifyGlass=new QLabel(this);
    lblMagnifyGlass->setParent(this);
    lblMagnifyGlass->setFixedSize(100,100);
    lblMagnifyGlass->setVisible(true);
    lblMagnifyGlass->setText("wo zai zhe");
    //设置鼠标光标模式为十字
    setCursor(Qt::CursorShape::CrossCursor);
    //连接按钮的信号槽
    connect(this->buttonSave,SIGNAL(clicked()),this,SLOT(on_buttonSave_Clicked()));
    connect(this->buttonCancel,SIGNAL(clicked()),this,SLOT(on_buttonCancel_Clicked()));
}
 
void ScreenShotWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        QRect rect(m_startPos,m_endPos);
        if((rect.contains(event->pos())||m_cursorMode!=0)&&m_startPos!=m_endPos){
            m_oldPos=event->pos();
        }
        else if(m_startPos==m_endPos){
            //记录鼠标点击的起始位置
            m_startPos = event->pos();
            //截图激活标志
            m_screenshot_active=true;
        }
    }
    else if(event->button()==Qt::RightButton)//重新截图
    {
        if(m_endPos.isNull())
            on_buttonCancel_Clicked();
        m_startPos=QPoint(0,0);
        m_endPos=QPoint(0,0);
        buttonSave->setVisible(false);
        buttonCancel->setVisible(false);
        lblHeightWidth->setVisible(false);
        setCursor(Qt::CursorShape::CrossCursor);
        update();
    }
}
 
void ScreenShotWidget::mouseMoveEvent(QMouseEvent *event)
{
    QRect rect(m_startPos,m_endPos);
    QPoint point=getPointInWhichSqure(event->pos());
    //截图后不点击鼠标时的光标更新
    if(!m_screenshot_active && m_startPos!=m_endPos && !event->buttons()){
        if(point==m_topLeft) {setCursor(Qt::CursorShape::SizeFDiagCursor);m_cursorMode=1;}
        else if(point==m_topRight) {setCursor(Qt::CursorShape::SizeBDiagCursor);m_cursorMode=2;}
        else if(point==m_bottomLeft) {setCursor(Qt::CursorShape::SizeBDiagCursor);m_cursorMode=3;}
        else if(point==m_bottomRight) {setCursor(Qt::CursorShape::SizeFDiagCursor);m_cursorMode=4;}
        else if(point==m_topCenter) {setCursor(Qt::CursorShape::SizeVerCursor);m_cursorMode=5;}
        else if(point==m_bottomCenter) {setCursor(Qt::CursorShape::SizeVerCursor);m_cursorMode=6;}
        else if(point==m_leftCenter) {setCursor(Qt::CursorShape::SizeHorCursor);m_cursorMode=7;}
        else if(point==m_rightCenter) {setCursor(Qt::CursorShape::SizeHorCursor);m_cursorMode=8;}
        else if(rect.contains(event->pos())) {setCursor(Qt::CursorShape::ClosedHandCursor);m_cursorMode=9;}
        else {setCursor(Qt::CursorShape::ArrowCursor);m_cursorMode=0;}
    }
    //鼠标左键按下
    if(event->buttons() & Qt::LeftButton)
    {
        //截图中位置更新
        if(m_screenshot_active){
            m_endPos = event->pos();
        }
        //拖动已截图区域
        if(rect.contains(event->pos()) && !m_screenshot_active &&cursor().shape()==Qt::ClosedHandCursor){
            m_distancePos=event->pos()-m_oldPos;
            m_oldPos=event->pos();
            m_startPos=m_startPos+m_distancePos;
            m_endPos=m_endPos+m_distancePos;
            //边界条件限制
            if(m_startPos.x()<=0){m_startPos.rx()=0;m_endPos.rx()=m_endPos.x()-m_distancePos.x();}
            if(m_startPos.y()<=0){m_startPos.ry()=0;m_endPos.ry()=m_endPos.y()-m_distancePos.y();}
            if(m_endPos.x()>=this->width()){m_endPos.rx()=this->width();m_startPos.rx()=m_startPos.x()-m_distancePos.x();}
            if(m_endPos.y()>=this->height()){m_endPos.ry()=this->height();m_startPos.ry()=m_startPos.y()-m_distancePos.y();}
        }
        else if(cursor().shape()==Qt::SizeFDiagCursor)
        {
            if(m_cursorMode==1){
                m_distancePos=event->pos()-m_oldPos;
                m_oldPos=event->pos();
                m_startPos=m_startPos+m_distancePos;
            }
            if(m_cursorMode==4){
                m_distancePos=event->pos()-m_oldPos;
                m_oldPos=event->pos();
                m_endPos=m_endPos+m_distancePos;
            }
        }
        else if(cursor().shape()==Qt::SizeBDiagCursor)
        {
            if(m_cursorMode==2){
                m_distancePos=event->pos()-m_oldPos;
                m_oldPos=event->pos();
                m_startPos.ry()=m_startPos.y()+m_distancePos.y();
                m_endPos.rx()=m_endPos.x()+m_distancePos.x();
            }
            if(m_cursorMode==3){
                m_distancePos=event->pos()-m_oldPos;
                m_oldPos=event->pos();
                m_startPos.rx()=m_startPos.x()+m_distancePos.x();
                m_endPos.ry()=m_endPos.y()+m_distancePos.y();
            }
        }
        else if(cursor().shape()==Qt::SizeHorCursor)
        {
            if(m_cursorMode==7){
                m_distancePos=event->pos()-m_oldPos;
                m_oldPos=event->pos();
                m_startPos.rx()=m_startPos.x()+m_distancePos.x();
            }
            if(m_cursorMode==8){
                m_distancePos=event->pos()-m_oldPos;
                m_oldPos=event->pos();
                m_endPos.rx()=m_endPos.x()+m_distancePos.x();
            }
        }
        else if(cursor().shape()==Qt::SizeVerCursor)
        {
            if(m_cursorMode==5){
                m_distancePos=event->pos()-m_oldPos;
                m_oldPos=event->pos();
                m_startPos.ry()=m_startPos.y()+m_distancePos.y();
            }
            if(m_cursorMode==6){
                m_distancePos=event->pos()-m_oldPos;
                m_oldPos=event->pos();
                m_endPos.ry()=m_endPos.y()+m_distancePos.y();
            }
        }
        update();
    }
 
}
 
void ScreenShotWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if (m_screenshot_active) {
        m_screenshot_active=false;
        if (m_endPos != m_startPos) {
            //弹出保存/取消按钮
            buttonSave->setVisible(true);
            buttonCancel->setVisible(true);
        }
    }
}
 
void ScreenShotWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(rect(),QColor(128,128,128,128));
    QColor color(11,218,81);
    QPen pen(color,1,Qt::SolidLine);
    painter.setPen(pen);
    //边界条件限制
    if(m_startPos.x()>=m_endPos.x())m_endPos.rx()=m_startPos.x();
    if(m_startPos.y()>=m_endPos.y())m_endPos.ry()=m_startPos.y();
    //画矩形,并将内部置为透明
    QRect rect(m_startPos, m_endPos);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    painter.fillRect(rect,QColor(0,0,0,0));
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawRect(rect.normalized());
    // 获取矩形的四个顶点和四条边的中点
    m_topLeft=QPoint(rect.left(),rect.top());
    m_topRight=QPoint(rect.right()+1,rect.top());
    m_bottomLeft=QPoint(rect.left(),rect.bottom()+1);
    m_bottomRight=QPoint(rect.right()+1,rect.bottom()+1);
    m_topCenter=QPoint(rect.center().x(), rect.top());
    m_bottomCenter=QPoint(rect.center().x(), rect.bottom()+1);
    m_leftCenter=QPoint(rect.left(), rect.center().y());
    m_rightCenter=QPoint(rect.right()+1, rect.center().y()+1);
 
    // 定义一个小矩形的大小，用来画实心点
    int pointSize = 5;
    QRect pointRect(0, 0, pointSize, pointSize);
 
    // 在每个点的位置画一个小矩形，作为实心点
    pointRect.moveCenter(m_topLeft);
    painter.fillRect(pointRect, color);
    pointRect.moveCenter(m_topRight);
    painter.fillRect(pointRect, color);
    pointRect.moveCenter(m_bottomLeft);
    painter.fillRect(pointRect, color);
    pointRect.moveCenter(m_bottomRight);
    painter.fillRect(pointRect, color);
    pointRect.moveCenter(m_topCenter);
    painter.fillRect(pointRect, color);
    pointRect.moveCenter(m_bottomCenter);
    painter.fillRect(pointRect, color);
    pointRect.moveCenter(m_leftCenter);
    painter.fillRect(pointRect, color);
    pointRect.moveCenter(m_rightCenter);
    painter.fillRect(pointRect, color);
    //显示放大镜
    if(cursor().shape()!=Qt::ClosedHandCursor)
    {
        QRect cursorRect(QCursor::pos().x()-25,QCursor::pos().y()-25,51,51);
        QScreen *screen=QGuiApplication::primaryScreen();
        QPixmap cursorPixmap=screen->grabWindow(QApplication::desktop()->winId(),cursorRect.x(),cursorRect.y(),cursorRect.width(),cursorRect.height());
        cursorPixmap=cursorPixmap.scaled(cursorPixmap.width()*2,cursorPixmap.height()*2,Qt::KeepAspectRatio);
        QPoint startPoint(QCursor::pos().x()+26,QCursor::pos().y()+26);
        painter.drawPixmap(startPoint.x(),startPoint.y(),cursorPixmap.width(),cursorPixmap.height(),cursorPixmap);
        painter.drawRect(startPoint.x(),startPoint.y(),cursorPixmap.width(),cursorPixmap.height());
        QPen pen2(color,2,Qt::SolidLine);
        painter.setPen(pen2);
        painter.drawLine(startPoint.x(),startPoint.y()+cursorPixmap.height()/2+1,startPoint.x()+cursorPixmap.width(),startPoint.y()+cursorPixmap.height()/2+1);
        painter.drawLine(startPoint.x()+cursorPixmap.width()/2+1,startPoint.y(),startPoint.x()+cursorPixmap.width()/2+1,startPoint.y()+cursorPixmap.height());
    }
    update();
    //显示按钮与标签
    if(this->height()<m_endPos.y()+30){
        buttonSave->move(rect.x()+rect.width()-buttonSave->width()-10,rect.y()+rect.height()-30);
        buttonCancel->move(rect.x()+rect.width()-buttonSave->width()-buttonCancel->width()-8,rect.y()+rect.height()-30);
    }
    else{
        buttonSave->move(rect.x()+rect.width()-buttonSave->width()-10,rect.y()+rect.height());
        buttonCancel->move(rect.x()+rect.width()-buttonSave->width()-buttonCancel->width()-8,rect.y()+rect.height());
    }
    lblHeightWidth->setVisible(true);
    lblHeightWidth->move(rect.x(),rect.y()-12);
    lblHeightWidth->setText(QString::number(rect.width())+"x"+QString::number(rect.height()));
}
 
void ScreenShotWidget::on_buttonSave_Clicked()
{
	auto dtCurtime = QDateTime::currentDateTime();  //设置当前时间为保存时间
	QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
	desktopPath += ("//QT_" + dtCurtime.toString("yyyyMMddhhmmss"));
	// 弹出文件保存对话框
	QString filePath = QFileDialog::getSaveFileName(nullptr,
													QObject::tr("Save File"),
													desktopPath, 
													QObject::tr("PNG(*.png);;JPG(*.jpg);;BMP(*.bmp)"));
	if (filePath.isEmpty()) {
		// 用户取消了保存
		return;
	}
	QThread::msleep(50);		// 不加延时，会将保存的那个窗口也给保存进入


    //清除笔刷,创建一个矩形区域
    close();
	QThread::msleep(50);
    QRect rect = QRect(m_startPos, m_endPos);

    //获取矩形区域截图(减去笔刷)，存储在pixmap中
    QScreen *screen=QGuiApplication::primaryScreen();
    QPixmap pixmap=screen->grabWindow(QApplication::desktop()->winId(),rect.x(),rect.y(),rect.width(),rect.height());
    if (!pixmap.save(filePath)) QMessageBox::warning(this, tr("提示"), tr("保存图片失败"));
}
 
void ScreenShotWidget::on_buttonCancel_Clicked()
{
    //关闭透明页面
    close();
}
 
QPoint ScreenShotWidget::getPointInWhichSqure(QPoint point)
{
    int distance=8;
    if(abs(point.x()-m_topLeft.x())+abs(point.y()-m_topLeft.y())<distance) return m_topLeft;
    else if(abs(point.x()-m_topRight.x())+abs(point.y()-m_topRight.y())<distance) return m_topRight;
    else if(abs(point.x()-m_bottomLeft.x())+abs(point.y()-m_bottomLeft.y())<distance) return m_bottomLeft;
    else if(abs(point.x()-m_bottomRight.x())+abs(point.y()-m_bottomRight.y())<distance) return m_bottomRight;
    else if(abs(point.x()-m_topCenter.x())+abs(point.y()-m_topCenter.y())<distance) return m_topCenter;
    else if(abs(point.x()-m_bottomCenter.x())+abs(point.y()-m_bottomCenter.y())<distance) return m_bottomCenter;
    else if(abs(point.x()-m_leftCenter.x())+abs(point.y()-m_leftCenter.y())<distance) return m_leftCenter;
    else if(abs(point.x()-m_rightCenter.x())+abs(point.y()-m_rightCenter.y())<distance) return m_rightCenter;
    else return QPoint(-1,-1);
}