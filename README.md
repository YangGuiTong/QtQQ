# QtQQ
仿QQ毕业设计



环境：
QT版本：5.9.6

编码工具：Visual Studio 2017



1.标题栏设计完毕    						2023年11月26日

2.目前还在持续开发中... 				2024年2月25日

3.增加Mysql数据库						2024年3月3日

4.开始增加服务端代码					2024年3月7日

5.增加发送文件小功能					2024年3月10日

6.项目结束，目前已将代码写完，还有很多需要优化的地方，有空再优化一下；	2024年3月10日22:30



优化：

1.增加了聊天记录保存，但是有两个bug，第一是不能即时刷新出来，需要切换到其他窗体后再次返回才会有；第二是多次返回来后聊天记录是叠加的，重复记录很多；

2.优化了服务端，新增用户时，可以设置密码；使用qq号查询用户时，可以修改用户信息；优化了界面；	2024年4月9日

3.简单粗暴的完成了聊天记录的处理逻辑；即修复了优化1的bug；其实就是没收到消息就会启用定时器将聊天信息重新刷新一遍，当消息越来越多时，就会越来越耗费时间去处理，就先这样吧，时间赶，赶着完成毕业论文和设计，赶着答辩；	2024年4月10日 23:55

4.增加了登录页面保存账号密码功能；如果首页有勾选保存密码，且登录成功，则账号密码保存在tradeprintinfo.ini文件中；另外，还隐藏了 找回密码 和 注册按钮，因为在我这毕业设计场景中使用不到；		2024年4月11日 22:42

5.增加了登录账号状态判断，如果账号是被注销的，则提示“账号已被注销”。	2024年4月12日17:02

6.增加登录成功后，从数据库读取个性签名，显示在页面上；当点击进行编辑，失去焦点后，自动更新数据库。	2024年4月21日15:10

7.完善，将信息进行AES加密后，再进行网络传输！使用到：#include "qaesencryption.h"和#include < QCryptographicHas.h >，其中，qaesencryption.h是第三方库，QCryptographicHas.h是QT内部自带。		2024年4月21日16:50

8.增加截图功能(网上扒的代码，稍微改了一下)，点击截图按钮后，可进行截图，只能保存在本地，不能发送出去。修复发送文件页面，点击关闭后程序崩溃问题。		2024年4月21日23:34

9.修复修改用户信息保存的图片路径斜杠问题；在客户端发送文件打开文件那块，每次点击文件按钮，都会new一个对象，且内存没有释放，这里会有内存泄漏的，还不知道怎么释放，后期再看看优化；聊天记录刷新时间改为350ms。	2024年4月21日23:47
