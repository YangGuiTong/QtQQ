/*
Navicat MySQL Data Transfer

Source Server         : Mysql数据库
Source Server Version : 80020
Source Host           : localhost:3306
Source Database       : qtqq

Target Server Type    : MYSQL
Target Server Version : 80020
File Encoding         : 65001

Date: 2024-04-21 16:47:50
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for tab_accounts
-- ----------------------------
DROP TABLE IF EXISTS `tab_accounts`;
CREATE TABLE `tab_accounts` (
  `employeeID` int NOT NULL,
  `account` char(20) NOT NULL,
  `code` char(20) NOT NULL,
  PRIMARY KEY (`account`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- ----------------------------
-- Records of tab_accounts
-- ----------------------------
INSERT INTO `tab_accounts` VALUES ('10002', 'rener', 'rener');
INSERT INTO `tab_accounts` VALUES ('10001', 'renyi', 'renyi');
INSERT INTO `tab_accounts` VALUES ('10008', 'shier', 'shier');
INSERT INTO `tab_accounts` VALUES ('10007', 'shiyi', 'shiyi');
INSERT INTO `tab_accounts` VALUES ('10005', 'yaner', 'yaner');
INSERT INTO `tab_accounts` VALUES ('10006', 'yansan', 'yansan');
INSERT INTO `tab_accounts` VALUES ('10004', 'yanyi', 'yanyi');
INSERT INTO `tab_accounts` VALUES ('10010', '秃头小猿', '111111');

-- ----------------------------
-- Table structure for tab_chat
-- ----------------------------
DROP TABLE IF EXISTS `tab_chat`;
CREATE TABLE `tab_chat` (
  `id` int unsigned NOT NULL AUTO_INCREMENT COMMENT 'id',
  `sender` text NOT NULL COMMENT '发送者',
  `receiver` text NOT NULL COMMENT '接收者',
  `message` text COMMENT '消息',
  `update_time` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP COMMENT '更新时间',
  `created_time` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP COMMENT '创建时间',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=73 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- ----------------------------
-- Records of tab_chat
-- ----------------------------
INSERT INTO `tab_chat` VALUES ('64', '10004', '10007', '[\n    {\n        \"message\": \"0057\",\n        \"sender\": \"10004\"\n    },\n    {\n        \"message\": \"0073\",\n        \"sender\": \"10004\"\n    },\n    {\n        \"message\": \"1gfsdgfsd\",\n        \"sender\": \"10004\"\n    },\n    {\n        \"message\": \"1sgsd个人告诉对方\",\n        \"sender\": \"10007\"\n    },\n    {\n        \"message\": \"1哈喽，美女\",\n        \"sender\": \"10004\"\n    }\n]\n', '2024-04-10 23:21:38', '2024-04-10 23:21:38');
INSERT INTO `tab_chat` VALUES ('65', '10007', '10004', '[\n    {\n        \"message\": \"0057\",\n        \"sender\": \"10004\"\n    },\n    {\n        \"message\": \"0073\",\n        \"sender\": \"10004\"\n    },\n    {\n        \"message\": \"1gfsdgfsd\",\n        \"sender\": \"10004\"\n    },\n    {\n        \"message\": \"1sgsd个人告诉对方\",\n        \"sender\": \"10007\"\n    },\n    {\n        \"message\": \"1哈喽，美女\",\n        \"sender\": \"10004\"\n    }\n]\n', '2024-04-10 23:21:38', '2024-04-10 23:21:38');
INSERT INTO `tab_chat` VALUES ('66', '10007', '10002', '[\n    {\n        \"message\": \"1放大发v公司法\",\n        \"sender\": \"10007\"\n    },\n    {\n        \"message\": \"1四个五个\",\n        \"sender\": \"10007\"\n    }\n]\n', '2024-03-24 11:55:12', '2024-03-24 11:55:12');
INSERT INTO `tab_chat` VALUES ('67', '10002', '10007', '[\n    {\n        \"message\": \"1放大发v公司法\",\n        \"sender\": \"10007\"\n    },\n    {\n        \"message\": \"1四个五个\",\n        \"sender\": \"10007\"\n    }\n]\n', '2024-03-24 11:55:12', '2024-03-24 11:55:12');
INSERT INTO `tab_chat` VALUES ('68', '10007', '10009', '[\n    {\n        \"message\": \"1是根深蒂固\",\n        \"sender\": \"10007\"\n    }\n]\n', '2024-03-24 11:55:16', '2024-03-24 11:55:16');
INSERT INTO `tab_chat` VALUES ('69', '10009', '10007', '[\n    {\n        \"message\": \"1是根深蒂固\",\n        \"sender\": \"10007\"\n    }\n]\n', '2024-03-24 11:55:16', '2024-03-24 11:55:16');
INSERT INTO `tab_chat` VALUES ('70', '2002', '2002', '[\n    {\n        \"message\": \"0072\",\n        \"sender\": \"10004\"\n    },\n    {\n        \"message\": \"0060\",\n        \"sender\": \"10005\"\n    },\n    {\n        \"message\": \"1喝咖啡\",\n        \"sender\": \"10004\"\n    }\n]\n', '2024-04-10 23:51:45', '2024-04-10 23:51:45');
INSERT INTO `tab_chat` VALUES ('71', '2000', '2000', '[\n    {\n        \"message\": \"1asdfasfdf你好\",\n        \"sender\": \"10004\"\n    },\n    {\n        \"message\": \"0008\",\n        \"sender\": \"10006\"\n    },\n    {\n        \"message\": \"0028\",\n        \"sender\": \"10004\"\n    },\n    {\n        \"message\": \"1你好世界\",\n        \"sender\": \"10007\"\n    },\n    {\n        \"message\": \"1来来啦\",\n        \"sender\": \"10005\"\n    },\n    {\n        \"message\": \"1俺来咯\",\n        \"sender\": \"10005\"\n    },\n    {\n        \"message\": \"1知道啦\",\n        \"sender\": \"10007\"\n    },\n    {\n        \"message\": \"1是的\",\n        \"sender\": \"10004\"\n    },\n    {\n        \"message\": \"1\",\n        \"sender\": \"10005\"\n    },\n    {\n        \"message\": \"1你打vu哦了\",\n        \"sender\": \"10004\"\n    },\n    {\n        \"message\": \"1晚安啦\",\n        \"sender\": \"10005\"\n    },\n    {\n        \"message\": \"0043\",\n        \"sender\": \"10005\"\n    },\n    {\n        \"message\": \"0060\",\n        \"sender\": \"10005\"\n    },\n    {\n        \"message\": \"1shide\",\n        \"sender\": \"10005\"\n    },\n    {\n        \"message\": \"1udidi对地\",\n        \"sender\": \"10004\"\n    },\n    {\n        \"message\": \"1okkok\",\n        \"sender\": \"10004\"\n    },\n    {\n        \"message\": \"1bug解决\",\n        \"sender\": \"10004\"\n    },\n    {\n        \"message\": \"1完毕\",\n        \"sender\": \"10005\"\n    },\n    {\n        \"message\": \"1你好\",\n        \"sender\": \"10004\"\n    },\n    {\n        \"message\": \"1wo好\",\n        \"sender\": \"10005\"\n    },\n    {\n        \"message\": \"0063057062056\",\n        \"sender\": \"10005\"\n    },\n    {\n        \"message\": \"1呱呱呱呱呱呱呱呱呱呱呱呱呱呱呱古古怪怪啊谁说的呱呱呱呱呱呱呱呱呱呱呱呱呱呱呱水水水水水水水水水水水水水水水水水水涛涛涛涛涛涛涛涛涛涛涛涛涛涛涛涛vvvvvvvvvvvvvvvvvvvvvv日日日日日日日日日日日日日日日日日日靠靠靠靠靠靠靠靠靠靠靠靠靠靠靠靠靠靠靠宣传部嘻嘻嘻嘻嘻嘻嘻嘻嘻嘻嘻嘻嘻嘻嘻嘻你你你你你你你你你你你你你你你你你你你\",\n        \"sender\": \"10005\"\n    },\n    {\n        \"message\": \"1怎么这么乱，。。。。\",\n        \"sender\": \"10007\"\n    },\n    {\n        \"message\": \"0108108\",\n        \"sender\": \"10005\"\n    },\n    {\n        \"message\": \"1OK，数据加密发送测试完毕。\",\n        \"sender\": \"10007\"\n    },\n    {\n        \"message\": \"0124\",\n        \"sender\": \"10005\"\n    }\n]\n', '2024-04-21 16:46:13', '2024-04-21 16:46:13');
INSERT INTO `tab_chat` VALUES ('72', '2003', '2003', '[\n    {\n        \"message\": \"0031\",\n        \"sender\": \"10007\"\n    },\n    {\n        \"message\": \"1钱钱钱钱钱钱钱钱钱钱钱\",\n        \"sender\": \"10007\"\n    }\n]\n', '2024-04-10 23:30:06', '2024-04-10 23:30:06');

-- ----------------------------
-- Table structure for tab_department
-- ----------------------------
DROP TABLE IF EXISTS `tab_department`;
CREATE TABLE `tab_department` (
  `departmentID` int NOT NULL AUTO_INCREMENT,
  `department_name` char(64) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  `picture` varchar(256) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  `sign` varchar(256) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  PRIMARY KEY (`departmentID`)
) ENGINE=InnoDB AUTO_INCREMENT=2004 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- ----------------------------
-- Records of tab_department
-- ----------------------------
INSERT INTO `tab_department` VALUES ('2000', '公司群', 'E:\\git_my_demo\\graduation_project\\QT_QQ\\QtQQ\\QtQQ\\Resources\\qtqq_images\\gsq.png', '这个群一天也不想呆了');
INSERT INTO `tab_department` VALUES ('2001', '人事部', 'E:\\git_my_demo\\graduation_project\\QT_QQ\\QtQQ\\QtQQ\\Resources\\qtqq_images\\rsb.png', '今天工作不努力，明天努力找工作');
INSERT INTO `tab_department` VALUES ('2002', '研发部', 'E:\\git_my_demo\\graduation_project\\QT_QQ\\QtQQ\\QtQQ\\Resources\\qtqq_images\\yfb.png', '使用C/C++、QT语言，力挽狂澜');
INSERT INTO `tab_department` VALUES ('2003', '市场部', 'E:\\git_my_demo\\graduation_project\\QT_QQ\\QtQQ\\QtQQ\\Resources\\qtqq_images\\scb.png', '到处造谣拐骗');

-- ----------------------------
-- Table structure for tab_employees
-- ----------------------------
DROP TABLE IF EXISTS `tab_employees`;
CREATE TABLE `tab_employees` (
  `departmentID` int NOT NULL,
  `employeeID` int NOT NULL,
  `employee_name` char(20) NOT NULL,
  `employee_sign` char(200) DEFAULT NULL,
  `status` tinyint NOT NULL DEFAULT '1' COMMENT '1有效 2注销',
  `picture` char(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL COMMENT '员工头像的路径',
  `online` tinyint DEFAULT '1' COMMENT '在线状态 1离线 2在线',
  PRIMARY KEY (`employeeID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- ----------------------------
-- Records of tab_employees
-- ----------------------------
INSERT INTO `tab_employees` VALUES ('2001', '10001', 'renyi', '风一样的女子', '2', 'E:\\git_my_demo\\graduation_project\\QT_QQ\\QtQQ\\QtQQ\\Resources\\qtqq_images\\r1.png', '1');
INSERT INTO `tab_employees` VALUES ('2001', '10002', 'rener', '奋斗中', '1', 'E:\\git_my_demo\\graduation_project\\QT_QQ\\QtQQ\\QtQQ\\Resources\\qtqq_images\\r2.png', '1');
INSERT INTO `tab_employees` VALUES ('2002', '10004', 'yanyi', '调BUG中', '1', 'E:\\git_my_demo\\graduation_project\\QT_QQ\\QtQQ\\QtQQ\\Resources\\qtqq_images\\y1.png', '2');
INSERT INTO `tab_employees` VALUES ('2002', '10005', 'yaner', '改BUG中，太难啦~', '1', 'E:\\git_my_demo\\graduation_project\\QT_QQ\\QtQQ\\QtQQ\\Resources\\qtqq_images\\y2.png', '2');
INSERT INTO `tab_employees` VALUES ('2002', '10006', 'yansan', '写BUG中', '2', 'E:git_my_demograduation_projectQT_QQQtQQQtQQResourcesqtqq_imagesy3.png', '1');
INSERT INTO `tab_employees` VALUES ('2003', '10007', 'shiyi', '梦游中', '1', 'E:\\git_my_demo\\graduation_project\\QT_QQ\\QtQQ\\QtQQ\\Resources\\qtqq_images\\s1.png', '2');
INSERT INTO `tab_employees` VALUES ('2003', '10008', 'shier', '目标明确，坚定不够', '1', 'E:\\git_my_demo\\graduation_project\\QT_QQ\\QtQQ\\QtQQ\\Resources\\qtqq_images\\s2.png', '1');
INSERT INTO `tab_employees` VALUES ('2003', '10010', '秃头小猿', '秃头代表强大', '2', 'E:git_my_demograduation_projectQT_QQQtQQ_ServerQtQQ_ServerQT.png', '1');
