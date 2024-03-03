/*
Navicat MySQL Data Transfer

Source Server         : Mysql数据库
Source Server Version : 80020
Source Host           : localhost:3306
Source Database       : qtqq

Target Server Type    : MYSQL
Target Server Version : 80020
File Encoding         : 65001

Date: 2024-03-03 23:12:39
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
INSERT INTO `tab_accounts` VALUES ('10003', 'rensan', 'rensan');
INSERT INTO `tab_accounts` VALUES ('10001', 'renyi', 'renyi');
INSERT INTO `tab_accounts` VALUES ('10008', 'shier', 'shier');
INSERT INTO `tab_accounts` VALUES ('10009', 'shisan', 'shisan');
INSERT INTO `tab_accounts` VALUES ('10007', 'shiyi', 'shiyi');
INSERT INTO `tab_accounts` VALUES ('10005', 'yaner', 'yaner');
INSERT INTO `tab_accounts` VALUES ('10006', 'yansan', 'yansan');
INSERT INTO `tab_accounts` VALUES ('10004', 'yanyi', 'yanyi');

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
INSERT INTO `tab_employees` VALUES ('2001', '10001', '人1', '风一样的女子', '1', 'E:\\git_my_demo\\graduation_project\\QT_QQ\\QtQQ\\QtQQ\\Resources\\qtqq_images\\r1.png', '1');
INSERT INTO `tab_employees` VALUES ('2001', '10002', '人2', '奋斗中', '1', 'E:\\git_my_demo\\graduation_project\\QT_QQ\\QtQQ\\QtQQ\\Resources\\qtqq_images\\r2.png', '1');
INSERT INTO `tab_employees` VALUES ('2001', '10003', '人3', '出差中', '1', 'E:\\git_my_demo\\graduation_project\\QT_QQ\\QtQQ\\QtQQ\\Resources\\qtqq_images\\r3.png', '1');
INSERT INTO `tab_employees` VALUES ('2002', '10004', '研1', '调BUG中', '1', 'E:\\git_my_demo\\graduation_project\\QT_QQ\\QtQQ\\QtQQ\\Resources\\qtqq_images\\y1.png', '1');
INSERT INTO `tab_employees` VALUES ('2002', '10005', '研2', '改BUG中', '1', 'E:\\git_my_demo\\graduation_project\\QT_QQ\\QtQQ\\QtQQ\\Resources\\qtqq_images\\y2.png', '1');
INSERT INTO `tab_employees` VALUES ('2002', '10006', '研3', '写BUG中', '1', 'E:\\git_my_demo\\graduation_project\\QT_QQ\\QtQQ\\QtQQ\\Resources\\qtqq_images\\y3.png', '1');
INSERT INTO `tab_employees` VALUES ('2003', '10007', '市1', '梦游中', '1', 'E:\\git_my_demo\\graduation_project\\QT_QQ\\QtQQ\\QtQQ\\Resources\\qtqq_images\\s1.png', '1');
INSERT INTO `tab_employees` VALUES ('2003', '10008', '市2', '目标明确，坚定不够', '1', 'E:\\git_my_demo\\graduation_project\\QT_QQ\\QtQQ\\QtQQ\\Resources\\qtqq_images\\s2.png', '1');
INSERT INTO `tab_employees` VALUES ('2003', '10009', '市2', '周末去爬山', '1', 'E:\\git_my_demo\\graduation_project\\QT_QQ\\QtQQ\\QtQQ\\Resources\\qtqq_images\\s3.png', '1');
INSERT INTO `tab_employees` VALUES ('2002', '10010', '小黑', null, '1', 'E:\\QTproject\\Qt_QQ\\QtQQ_Server\\qiniu.png', '1');
INSERT INTO `tab_employees` VALUES ('2003', '10011', '小美', null, '1', 'E:\\QTproject\\Qt_QQ\\QtQQ_Server\\qiniu.png', '1');
INSERT INTO `tab_employees` VALUES ('2001', '10012', '12345他', null, '1', 'E:\\QTproject\\Qt_QQ\\QtQQ_Server\\qiniu.png', '1');
