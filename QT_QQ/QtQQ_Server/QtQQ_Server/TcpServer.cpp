#include "TcpServer.h"
#include "TcpSocket.h"
#include "public_type.h"
#include "qaesencryption.h"

#include <QCryptographicHash>

#include <QSqlQuery>
#include <QMessageBox>


extern QMultiMap<int, QMap<int, QJsonArray>> g_message_info;	// 聊天记录


TcpServer::TcpServer(int prot) : m_port(prot) { }

TcpServer::~TcpServer() { }

bool TcpServer::run() {

	if (this->listen(QHostAddress::AnyIPv4, m_port)) {
		QString text = QString::fromLocal8Bit(QString("服务器监听端口 %1 成功！").arg(m_port).toUtf8());
		MyLogDEBUG(text.toUtf8());
		qDebug() << QString::fromLocal8Bit(text.toUtf8());

		return true;
	}

	MyLogDEBUG(QString("服务器监听端口 %1 失败！").arg(m_port).toUtf8());
	qDebug() << QString::fromLocal8Bit(QString("服务器监听端口 %1 失败！").arg(m_port).toLocal8Bit());
	
	return false;
}

void TcpServer::incomingConnection(qintptr socketDescriptor) {
	QString text = QString("新的连接：%1").arg(socketDescriptor);
	MyLogDEBUG(text .toUtf8());
	qDebug() << QString::fromLocal8Bit(text.toLocal8Bit());
	
	TcpSocket *tcpsocket = new TcpSocket();
	tcpsocket->setSocketDescriptor(socketDescriptor);
	tcpsocket->run();

	// 收到客户端数据后，server进行处理
	//connect(tcpsocket, SIGNAL(signalGetDataFromClient(QByteArray &, int)), 
	//		this, SLOT(SocketDataProcessiong(QByteArray &SendData, int descriptor)));
	connect(tcpsocket, &TcpSocket::signalGetDataFromClient, this, &TcpServer::SocketDataProcessiong);

	// 收到客户端断开连接后，server进行处理
	connect(tcpsocket, &TcpSocket::signalClientDisconnect, this, &TcpServer::SocketDisconnecgted);

	// 将socket添加到链表中
	m_tcpSocketConnectList.append(tcpsocket);
}

void TcpServer::SocketDataProcessiong(QByteArray & SendData, int descriptor) {

	bool flag = false;

	for (int i = 0; i < m_tcpSocketConnectList.size(); i++) {
		QTcpSocket *item = m_tcpSocketConnectList.at(i);
		int des = item->socketDescriptor();	// 获取tcpsocket描述符
		if (des == descriptor) {
			QString text = QString("来自IP： %1  发来的数据：%2")
				.arg(item->peerAddress().toString()).arg(QString(SendData));
			MyLogDEBUG(text .toUtf8());
			qDebug() << QString::fromLocal8Bit(text.toLocal8Bit());

			if (!flag) {
				processPendingData(SendData);
				flag = !flag;
			}

			// 广播
			emit signalTcpMsgComes(SendData);
		}

	}
}

void TcpServer::SocketDisconnecgted(int descriptor) { 
	for (int i = 0; i < m_tcpSocketConnectList.size(); i++) {
		QTcpSocket *item = m_tcpSocketConnectList.at(i);
		int des = item->socketDescriptor();	// 获取tcpsocket描述符
		if (des == descriptor || -1 == des) {
			m_tcpSocketConnectList.removeAt(i);
			item->deleteLater();

			QString text = QString("tcpSocket描述符：%1  IP： %2  断开连接!").arg(descriptor).arg(item->peerAddress().toString());
			MyLogDEBUG(text .toUtf8());
			qDebug() << QString::fromLocal8Bit(text.toLocal8Bit());

			break;
		}

	}
}




/********************************************************************************************************************************************************

	数据包格式：
		文本数据包格式：群聊标志 + 发信息员工QQ号 + 收信息员工QQ号（群QQ号） + 信息类型（1） + 数据长度 + 数据
		表情数据包格式：群聊标志 + 发信息员工QQ号 + 收信息员工QQ号（群QQ号） + 信息类型（0） + 表情个数 + images + 表情名称
		文件数据包格式：群聊标志 + 发信息员工QQ号 + 收信息员工QQ号（群QQ号） + 信息类型（2） + 文件字节数 + bytes + 文件名 + data_begin + 文件数据

		群聊标志占1位，0表示单聊，1表示群聊
		信息类型占1位，0表示表情信息，1表示文本信息，2白哦是文件信息

		QQ号占5位，群QQ号占4位，数据长度占5位，表情名称占3位
		（注意：当群聊标志为1时，则数据包中没有收信息员工QQ号，而时收信息群QQ号；
				当群聊标志为0时，则数据包中没有收信息群QQ号，而是收信息员工QQ号；）

		群聊文本信息如：1100012001100005Hello		表示QQ10001向群2001发送文本信息，长度是5，数据是Hello
		单聊图片信息如：0100011000202images060125		表示QQ10001向QQ10002发送表情信息，表情个数是2，表情名称是60.png和125.png
		群聊文件信息如：1100052000210bytestest.txtdata_beginHelloworld	表示QQ10005向群2000发送文件信息，文件长度是10，文件名是test.txt，文件内容是Helloworld

		群聊文件信息解析：1 10001 2001  1 00005 Hello
		单聊图片信息解析：0 10001 10002 0 2 060和125
		群聊文件信息解析：1 10005 2000  2 10 test.txt Helloworld

*********************************************************************************************************************************************************/
void TcpServer::processPendingData(QByteArray &SendData) {

	const static int groupFlagWidth = 1;	// 群聊标志占位
	const static int groupWidth = 4;		// 群QQ号宽度
	const static int employeeWidth = 5;		// 员工QQ号宽度
	const static int msgTypeWidth = 1;		// 信息类型宽度
	const static int msgLengthWidth = 5;	// 文本信息长度的宽度
	const static int pictureWidth = 3;		// 表情图片的宽度


	QByteArray btData(SendData);

	btData = decodedText(btData);


	QString strData = btData.data();
	QString strWindowID;	// 聊天窗口id，群聊则是群号，单聊则是员工qq号
	QString strSendEmployeeID, strRecvieEmployeeID;		// 发送端QQ号和接收端QQ号
	QString strMsg;			// 数据

	int msgLen;				// 数据长度
	int msgType;			// 数据类型
	int groupType;			// 群聊类型，1表示群聊，0表示单聊


	strSendEmployeeID = strData.mid(groupFlagWidth, employeeWidth);	// 获取发送端QQ号


	if (btData[0] == '1') {		// 群聊
		groupType = 1;
		strWindowID = strData.mid(groupFlagWidth + employeeWidth, groupWidth);		// 获取接收端群号

		QChar cMsgType = btData[groupFlagWidth + employeeWidth + groupWidth];
		if (cMsgType == '1') {			// 文本信息
			msgType = 1;
			msgLen = strData.mid(groupFlagWidth + employeeWidth + groupWidth + msgTypeWidth, msgLengthWidth).toInt();	// 获取信息长度
			strMsg = strData.mid(groupFlagWidth + employeeWidth + groupWidth + msgTypeWidth + msgLengthWidth, msgLen);	// 获取文本信息

		} else if (cMsgType == '0') {	// 表情信息
			msgType = 0;
			int posImages = strData.indexOf("images");
			strMsg = strData.right(strData.length() - posImages - QString("images").length());	// 获取所有表情名称，信息数据

		}

	} else {	// 单聊
		groupType = 0;
		
		strRecvieEmployeeID = strData.mid(groupFlagWidth + employeeWidth, employeeWidth);	// 接收者QQ号
		//strWindowID = strSendEmployeeID;													// 发送者QQ号
		strWindowID = strRecvieEmployeeID;													// 接收者QQ号



		// 获取信息的类型
		QChar cMsgType = btData[groupFlagWidth + employeeWidth + employeeWidth];
		if (cMsgType == '1') {			// 文本信息
			msgType = 1;

			// 文本信息长度
			msgLen = strData.mid(groupFlagWidth + employeeWidth + employeeWidth + msgTypeWidth, msgLengthWidth).toInt();
			// 文本信息
			strMsg = strData.mid(groupFlagWidth + employeeWidth + employeeWidth + msgTypeWidth + msgLengthWidth, msgLen);

		} else if (cMsgType == '0') {	// 表情信息
			msgType = 0;
			int posImages = strData.indexOf("images");
			int imagesWidth = QString("images").length();
			//strMsg = strData.right(strData.length() - posImages - imagesWidth);	// 获取所有表情名称，信息数据
			strMsg = strData.mid(posImages + imagesWidth);

		} 
	}


	if (msgType != 2) {
		
		if (0 == groupType) {	// 单聊
			// 聊天记录两边都需要同时插入
			MessageInsert(strSendEmployeeID, strWindowID, msgType, groupType, strMsg);
			MessageInsert(strWindowID, strSendEmployeeID, msgType, groupType, strMsg, 1);
		} else {
			// 群聊只需插入一条记录
			MessageInsert(strSendEmployeeID, strWindowID, msgType, groupType, strMsg);
		}

	}
}

void TcpServer::MessageInsert(const QString sender, const QString receiver, const int msgType, const int groupType, const QString strMsg, const int second) {
	
	QJsonObject messageObj;
	QMap<int, QJsonArray> messageMap;
	QList<QMap<int, QJsonArray>> messageList;
	QJsonArray messageArr;
	int updateFlag = 0;	// 1是更新，0是插入

	QString groupSender = sender;
	if (1 == second) {
		groupSender = receiver;
	}

	if (0 == msgType) {		// 表情信息
		// 组合数据
		messageObj.insert("sender", groupSender);
		messageObj.insert("message", "0" + strMsg);

	} else if (1 == msgType) {	// 文本信息
		messageObj.insert("sender", groupSender);
		messageObj.insert("message", "1" + strMsg);
	} else {
		MyLogDEBUG("数据类型有误，不进行聊天记录保存！！");
		return;
	}


	if (1 == groupType) {		// 群聊
		messageMap = g_message_info.value(receiver.toInt());

		if (0 >= messageMap.size()) {
			MyLogDEBUG("没有找到对应的聊天记录，插入新的记录。");
			updateFlag = 0;

			messageArr.append(messageObj);
			messageMap.insert(receiver.toInt(), messageArr);

		} else {
			updateFlag = 1;

			// 1.首先移除原先的聊天记录
			g_message_info.remove(receiver.toInt(), messageMap);

			// 2.插入信息数据
			messageArr = messageMap.first();
			messageArr.append(messageObj);
			messageMap.insert(receiver.toInt(), messageArr);
		}

		// 3.然后在重新插入回去
		g_message_info.insert(receiver.toInt(), messageMap);

		// 将新数据更新数据库
		MessageSaveDataBase(receiver, receiver, updateFlag, messageArr);
	
	} else if (0 == groupType) {	// 单聊	

		// 找到匹配的收信者的qq对应的聊天记录
		messageList = g_message_info.values(sender.toInt());
		for (int i = 0; i < messageList.size(); i++) {

			QMap<int, QJsonArray> tmpMap = messageList.at(i);
			QString receiverKey = QString::number(tmpMap.firstKey());
			if (receiverKey == receiver) {
				messageMap = tmpMap;	// 获取到了聊天记录map
				break;
			}
		}

		
		if (0 >= messageMap.size()) {
			MyLogDEBUG("没有找到对应的聊天记录，插入新的记录。");
			updateFlag = 0;

			messageArr.append(messageObj);
			messageMap.insert(receiver.toInt(), messageArr);

		} else {
			updateFlag = 1;

			// 1.首先移除原先的聊天记录
			g_message_info.remove(sender.toInt(), messageMap);

			// 2.插入信息数据
			messageArr = messageMap.first();
			messageArr.append(messageObj);
			messageMap.insert(receiver.toInt(), messageArr);
		}

		// 3.然后在重新插入回去
		g_message_info.insert(sender.toInt(), messageMap);

		// 将新数据更新数据库
		MessageSaveDataBase(sender, receiver, updateFlag, messageArr);
		//MessageSaveDataBase(receiver, sender, updateFlag, messageArr);

		//if (1 == second) {
		//	MessageSaveDataBase(receiver, sender, updateFlag, messageArr);
		//} else {
		//	MessageSaveDataBase(sender, receiver, updateFlag, messageArr);
		//}
	}
}

void TcpServer::MessageSaveDataBase(const QString sender, const QString receiver, const int updateFlag, const QJsonArray messageArr) {
	QString sql = "";

	QString message = QJsonDocument(messageArr).toJson();


	if (0 == updateFlag) {		// 插入
		sql = QString("INSERT INTO tab_chat(sender, receiver, message, update_time, created_time) \
					  VALUES(%1, %2, '%3', CURRENT_TIMESTAMP, CURRENT_TIMESTAMP);")
					  .arg(sender.toInt()).arg(receiver.toInt()).arg(message);


	} else if (1 == updateFlag) {	// 更新
		sql = QString("UPDATE tab_chat SET message = '%1', update_time = CURRENT_TIMESTAMP WHERE sender = %2 AND receiver = %3;")
			  .arg(message).arg(sender.toInt()).arg(receiver.toInt());

	} else {
		MyLogDEBUG("操作数据标志有误！！");
		return;
	}

	QSqlQuery query;
	if (!query.exec(sql)) {
		MyLogDEBUG(QString("执行数据库指令：%1  失败！").arg(sql).toLocal8Bit());
		QMessageBox::information(nullptr, tr("提示"), tr(QString("执行数据库指令：%1  失败！").arg(sql).toLocal8Bit()));
	}
}

QByteArray TcpServer::encodedText(QByteArray data) {
	//密钥长度AES_128,加密方式ECB,填充方式ZERO
	QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::ECB, QAESEncryption::ZERO);

	//使用QCryptographicHash对密钥进行加密
	QByteArray hashKey = QCryptographicHash::hash(_key_.toUtf8(), QCryptographicHash::Sha1);

	//对源数据加密
	QByteArray encodedText = encryption.encode(data, hashKey);

	//QByteArray转QString (toBase64()不能去掉)
	QString encodeTextStr = QString::fromLatin1(encodedText.toBase64());
	qDebug() << "encodedText:" << encodeTextStr;

	return encodedText;
}

QByteArray TcpServer::decodedText(QByteArray data) {
	//密钥长度AES_128,加密方式ECB,填充方式ZERO
	QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::ECB, QAESEncryption::ZERO);

	//使用QCryptographicHash对密钥进行加密
	QByteArray hashKey = QCryptographicHash::hash(_key_.toUtf8(), QCryptographicHash::Sha1);

	//解密
	QByteArray decodedText = encryption.decode(data, hashKey);

	//QByteArray转QString
	QString decodedTextStr = QString::fromLatin1(decodedText);
	qDebug() << "decodedText:" << decodedTextStr;

	return decodedText;
}

