#include "TcpServer.h"
#include "TcpSocket.h"
#include "public_type.h"
#include "qaesencryption.h"

#include <QCryptographicHash>

#include <QSqlQuery>
#include <QMessageBox>


extern QMultiMap<int, QMap<int, QJsonArray>> g_message_info;	// �����¼


TcpServer::TcpServer(int prot) : m_port(prot) { }

TcpServer::~TcpServer() { }

bool TcpServer::run() {

	if (this->listen(QHostAddress::AnyIPv4, m_port)) {
		QString text = QString::fromLocal8Bit(QString("�����������˿� %1 �ɹ���").arg(m_port).toUtf8());
		MyLogDEBUG(text.toUtf8());
		qDebug() << QString::fromLocal8Bit(text.toUtf8());

		return true;
	}

	MyLogDEBUG(QString("�����������˿� %1 ʧ�ܣ�").arg(m_port).toUtf8());
	qDebug() << QString::fromLocal8Bit(QString("�����������˿� %1 ʧ�ܣ�").arg(m_port).toLocal8Bit());
	
	return false;
}

void TcpServer::incomingConnection(qintptr socketDescriptor) {
	QString text = QString("�µ����ӣ�%1").arg(socketDescriptor);
	MyLogDEBUG(text .toUtf8());
	qDebug() << QString::fromLocal8Bit(text.toLocal8Bit());
	
	TcpSocket *tcpsocket = new TcpSocket();
	tcpsocket->setSocketDescriptor(socketDescriptor);
	tcpsocket->run();

	// �յ��ͻ������ݺ�server���д���
	//connect(tcpsocket, SIGNAL(signalGetDataFromClient(QByteArray &, int)), 
	//		this, SLOT(SocketDataProcessiong(QByteArray &SendData, int descriptor)));
	connect(tcpsocket, &TcpSocket::signalGetDataFromClient, this, &TcpServer::SocketDataProcessiong);

	// �յ��ͻ��˶Ͽ����Ӻ�server���д���
	connect(tcpsocket, &TcpSocket::signalClientDisconnect, this, &TcpServer::SocketDisconnecgted);

	// ��socket��ӵ�������
	m_tcpSocketConnectList.append(tcpsocket);
}

void TcpServer::SocketDataProcessiong(QByteArray & SendData, int descriptor) {

	bool flag = false;

	for (int i = 0; i < m_tcpSocketConnectList.size(); i++) {
		QTcpSocket *item = m_tcpSocketConnectList.at(i);
		int des = item->socketDescriptor();	// ��ȡtcpsocket������
		if (des == descriptor) {
			QString text = QString("����IP�� %1  ���������ݣ�%2")
				.arg(item->peerAddress().toString()).arg(QString(SendData));
			MyLogDEBUG(text .toUtf8());
			qDebug() << QString::fromLocal8Bit(text.toLocal8Bit());

			if (!flag) {
				processPendingData(SendData);
				flag = !flag;
			}

			// �㲥
			emit signalTcpMsgComes(SendData);
		}

	}
}

void TcpServer::SocketDisconnecgted(int descriptor) { 
	for (int i = 0; i < m_tcpSocketConnectList.size(); i++) {
		QTcpSocket *item = m_tcpSocketConnectList.at(i);
		int des = item->socketDescriptor();	// ��ȡtcpsocket������
		if (des == descriptor || -1 == des) {
			m_tcpSocketConnectList.removeAt(i);
			item->deleteLater();

			QString text = QString("tcpSocket��������%1  IP�� %2  �Ͽ�����!").arg(descriptor).arg(item->peerAddress().toString());
			MyLogDEBUG(text .toUtf8());
			qDebug() << QString::fromLocal8Bit(text.toLocal8Bit());

			break;
		}

	}
}




/********************************************************************************************************************************************************

	���ݰ���ʽ��
		�ı����ݰ���ʽ��Ⱥ�ı�־ + ����ϢԱ��QQ�� + ����ϢԱ��QQ�ţ�ȺQQ�ţ� + ��Ϣ���ͣ�1�� + ���ݳ��� + ����
		�������ݰ���ʽ��Ⱥ�ı�־ + ����ϢԱ��QQ�� + ����ϢԱ��QQ�ţ�ȺQQ�ţ� + ��Ϣ���ͣ�0�� + ������� + images + ��������
		�ļ����ݰ���ʽ��Ⱥ�ı�־ + ����ϢԱ��QQ�� + ����ϢԱ��QQ�ţ�ȺQQ�ţ� + ��Ϣ���ͣ�2�� + �ļ��ֽ��� + bytes + �ļ��� + data_begin + �ļ�����

		Ⱥ�ı�־ռ1λ��0��ʾ���ģ�1��ʾȺ��
		��Ϣ����ռ1λ��0��ʾ������Ϣ��1��ʾ�ı���Ϣ��2��Ŷ���ļ���Ϣ

		QQ��ռ5λ��ȺQQ��ռ4λ�����ݳ���ռ5λ����������ռ3λ
		��ע�⣺��Ⱥ�ı�־Ϊ1ʱ�������ݰ���û������ϢԱ��QQ�ţ���ʱ����ϢȺQQ�ţ�
				��Ⱥ�ı�־Ϊ0ʱ�������ݰ���û������ϢȺQQ�ţ���������ϢԱ��QQ�ţ���

		Ⱥ���ı���Ϣ�磺1100012001100005Hello		��ʾQQ10001��Ⱥ2001�����ı���Ϣ��������5��������Hello
		����ͼƬ��Ϣ�磺0100011000202images060125		��ʾQQ10001��QQ10002���ͱ�����Ϣ�����������2������������60.png��125.png
		Ⱥ���ļ���Ϣ�磺1100052000210bytestest.txtdata_beginHelloworld	��ʾQQ10005��Ⱥ2000�����ļ���Ϣ���ļ�������10���ļ�����test.txt���ļ�������Helloworld

		Ⱥ���ļ���Ϣ������1 10001 2001  1 00005 Hello
		����ͼƬ��Ϣ������0 10001 10002 0 2 060��125
		Ⱥ���ļ���Ϣ������1 10005 2000  2 10 test.txt Helloworld

*********************************************************************************************************************************************************/
void TcpServer::processPendingData(QByteArray &SendData) {

	const static int groupFlagWidth = 1;	// Ⱥ�ı�־ռλ
	const static int groupWidth = 4;		// ȺQQ�ſ��
	const static int employeeWidth = 5;		// Ա��QQ�ſ��
	const static int msgTypeWidth = 1;		// ��Ϣ���Ϳ��
	const static int msgLengthWidth = 5;	// �ı���Ϣ���ȵĿ��
	const static int pictureWidth = 3;		// ����ͼƬ�Ŀ��


	QByteArray btData(SendData);

	btData = decodedText(btData);


	QString strData = btData.data();
	QString strWindowID;	// ���촰��id��Ⱥ������Ⱥ�ţ���������Ա��qq��
	QString strSendEmployeeID, strRecvieEmployeeID;		// ���Ͷ�QQ�źͽ��ն�QQ��
	QString strMsg;			// ����

	int msgLen;				// ���ݳ���
	int msgType;			// ��������
	int groupType;			// Ⱥ�����ͣ�1��ʾȺ�ģ�0��ʾ����


	strSendEmployeeID = strData.mid(groupFlagWidth, employeeWidth);	// ��ȡ���Ͷ�QQ��


	if (btData[0] == '1') {		// Ⱥ��
		groupType = 1;
		strWindowID = strData.mid(groupFlagWidth + employeeWidth, groupWidth);		// ��ȡ���ն�Ⱥ��

		QChar cMsgType = btData[groupFlagWidth + employeeWidth + groupWidth];
		if (cMsgType == '1') {			// �ı���Ϣ
			msgType = 1;
			msgLen = strData.mid(groupFlagWidth + employeeWidth + groupWidth + msgTypeWidth, msgLengthWidth).toInt();	// ��ȡ��Ϣ����
			strMsg = strData.mid(groupFlagWidth + employeeWidth + groupWidth + msgTypeWidth + msgLengthWidth, msgLen);	// ��ȡ�ı���Ϣ

		} else if (cMsgType == '0') {	// ������Ϣ
			msgType = 0;
			int posImages = strData.indexOf("images");
			strMsg = strData.right(strData.length() - posImages - QString("images").length());	// ��ȡ���б������ƣ���Ϣ����

		}

	} else {	// ����
		groupType = 0;
		
		strRecvieEmployeeID = strData.mid(groupFlagWidth + employeeWidth, employeeWidth);	// ������QQ��
		//strWindowID = strSendEmployeeID;													// ������QQ��
		strWindowID = strRecvieEmployeeID;													// ������QQ��



		// ��ȡ��Ϣ������
		QChar cMsgType = btData[groupFlagWidth + employeeWidth + employeeWidth];
		if (cMsgType == '1') {			// �ı���Ϣ
			msgType = 1;

			// �ı���Ϣ����
			msgLen = strData.mid(groupFlagWidth + employeeWidth + employeeWidth + msgTypeWidth, msgLengthWidth).toInt();
			// �ı���Ϣ
			strMsg = strData.mid(groupFlagWidth + employeeWidth + employeeWidth + msgTypeWidth + msgLengthWidth, msgLen);

		} else if (cMsgType == '0') {	// ������Ϣ
			msgType = 0;
			int posImages = strData.indexOf("images");
			int imagesWidth = QString("images").length();
			//strMsg = strData.right(strData.length() - posImages - imagesWidth);	// ��ȡ���б������ƣ���Ϣ����
			strMsg = strData.mid(posImages + imagesWidth);

		} 
	}


	if (msgType != 2) {
		
		if (0 == groupType) {	// ����
			// �����¼���߶���Ҫͬʱ����
			MessageInsert(strSendEmployeeID, strWindowID, msgType, groupType, strMsg);
			MessageInsert(strWindowID, strSendEmployeeID, msgType, groupType, strMsg, 1);
		} else {
			// Ⱥ��ֻ�����һ����¼
			MessageInsert(strSendEmployeeID, strWindowID, msgType, groupType, strMsg);
		}

	}
}

void TcpServer::MessageInsert(const QString sender, const QString receiver, const int msgType, const int groupType, const QString strMsg, const int second) {
	
	QJsonObject messageObj;
	QMap<int, QJsonArray> messageMap;
	QList<QMap<int, QJsonArray>> messageList;
	QJsonArray messageArr;
	int updateFlag = 0;	// 1�Ǹ��£�0�ǲ���

	QString groupSender = sender;
	if (1 == second) {
		groupSender = receiver;
	}

	if (0 == msgType) {		// ������Ϣ
		// �������
		messageObj.insert("sender", groupSender);
		messageObj.insert("message", "0" + strMsg);

	} else if (1 == msgType) {	// �ı���Ϣ
		messageObj.insert("sender", groupSender);
		messageObj.insert("message", "1" + strMsg);
	} else {
		MyLogDEBUG("�����������󣬲����������¼���棡��");
		return;
	}


	if (1 == groupType) {		// Ⱥ��
		messageMap = g_message_info.value(receiver.toInt());

		if (0 >= messageMap.size()) {
			MyLogDEBUG("û���ҵ���Ӧ�������¼�������µļ�¼��");
			updateFlag = 0;

			messageArr.append(messageObj);
			messageMap.insert(receiver.toInt(), messageArr);

		} else {
			updateFlag = 1;

			// 1.�����Ƴ�ԭ�ȵ������¼
			g_message_info.remove(receiver.toInt(), messageMap);

			// 2.������Ϣ����
			messageArr = messageMap.first();
			messageArr.append(messageObj);
			messageMap.insert(receiver.toInt(), messageArr);
		}

		// 3.Ȼ�������²����ȥ
		g_message_info.insert(receiver.toInt(), messageMap);

		// �������ݸ������ݿ�
		MessageSaveDataBase(receiver, receiver, updateFlag, messageArr);
	
	} else if (0 == groupType) {	// ����	

		// �ҵ�ƥ��������ߵ�qq��Ӧ�������¼
		messageList = g_message_info.values(sender.toInt());
		for (int i = 0; i < messageList.size(); i++) {

			QMap<int, QJsonArray> tmpMap = messageList.at(i);
			QString receiverKey = QString::number(tmpMap.firstKey());
			if (receiverKey == receiver) {
				messageMap = tmpMap;	// ��ȡ���������¼map
				break;
			}
		}

		
		if (0 >= messageMap.size()) {
			MyLogDEBUG("û���ҵ���Ӧ�������¼�������µļ�¼��");
			updateFlag = 0;

			messageArr.append(messageObj);
			messageMap.insert(receiver.toInt(), messageArr);

		} else {
			updateFlag = 1;

			// 1.�����Ƴ�ԭ�ȵ������¼
			g_message_info.remove(sender.toInt(), messageMap);

			// 2.������Ϣ����
			messageArr = messageMap.first();
			messageArr.append(messageObj);
			messageMap.insert(receiver.toInt(), messageArr);
		}

		// 3.Ȼ�������²����ȥ
		g_message_info.insert(sender.toInt(), messageMap);

		// �������ݸ������ݿ�
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


	if (0 == updateFlag) {		// ����
		sql = QString("INSERT INTO tab_chat(sender, receiver, message, update_time, created_time) \
					  VALUES(%1, %2, '%3', CURRENT_TIMESTAMP, CURRENT_TIMESTAMP);")
					  .arg(sender.toInt()).arg(receiver.toInt()).arg(message);


	} else if (1 == updateFlag) {	// ����
		sql = QString("UPDATE tab_chat SET message = '%1', update_time = CURRENT_TIMESTAMP WHERE sender = %2 AND receiver = %3;")
			  .arg(message).arg(sender.toInt()).arg(receiver.toInt());

	} else {
		MyLogDEBUG("�������ݱ�־���󣡣�");
		return;
	}

	QSqlQuery query;
	if (!query.exec(sql)) {
		MyLogDEBUG(QString("ִ�����ݿ�ָ�%1  ʧ�ܣ�").arg(sql).toLocal8Bit());
		QMessageBox::information(nullptr, tr("��ʾ"), tr(QString("ִ�����ݿ�ָ�%1  ʧ�ܣ�").arg(sql).toLocal8Bit()));
	}
}

QByteArray TcpServer::encodedText(QByteArray data) {
	//��Կ����AES_128,���ܷ�ʽECB,��䷽ʽZERO
	QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::ECB, QAESEncryption::ZERO);

	//ʹ��QCryptographicHash����Կ���м���
	QByteArray hashKey = QCryptographicHash::hash(_key_.toUtf8(), QCryptographicHash::Sha1);

	//��Դ���ݼ���
	QByteArray encodedText = encryption.encode(data, hashKey);

	//QByteArrayתQString (toBase64()����ȥ��)
	QString encodeTextStr = QString::fromLatin1(encodedText.toBase64());
	qDebug() << "encodedText:" << encodeTextStr;

	return encodedText;
}

QByteArray TcpServer::decodedText(QByteArray data) {
	//��Կ����AES_128,���ܷ�ʽECB,��䷽ʽZERO
	QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::ECB, QAESEncryption::ZERO);

	//ʹ��QCryptographicHash����Կ���м���
	QByteArray hashKey = QCryptographicHash::hash(_key_.toUtf8(), QCryptographicHash::Sha1);

	//����
	QByteArray decodedText = encryption.decode(data, hashKey);

	//QByteArrayתQString
	QString decodedTextStr = QString::fromLatin1(decodedText);
	qDebug() << "decodedText:" << decodedTextStr;

	return decodedText;
}

