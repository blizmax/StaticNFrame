/*
Navicat MySQL Data Transfer

Source Server         : 45.249.246.175
Source Server Version : 50725
Source Host           : 45.249.246.175:6001
Source Database       : db_yjqp

Target Server Type    : MYSQL
Target Server Version : 50725
File Encoding         : 65001

Date: 2019-05-13 14:25:05
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for dy_admin
-- ----------------------------
DROP TABLE IF EXISTS `dy_admin`;
CREATE TABLE `dy_admin` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `admin_name` varchar(30) DEFAULT NULL COMMENT '后台管理员用户名',
  `admin_pass` char(64) DEFAULT NULL COMMENT '管理员密码',
  `admin_login` int(11) NOT NULL DEFAULT '0' COMMENT '登录次数',
  `admin_myname` varchar(100) DEFAULT NULL COMMENT '管理员名字',
  `admin_email` varchar(100) DEFAULT NULL COMMENT '管理员邮箱',
  `admin_oldip` varchar(20) NOT NULL,
  `admin_ip` varchar(20) DEFAULT NULL COMMENT '登录ip',
  `admin_rsdate` int(11) NOT NULL COMMENT '注册时间',
  `admin_olddate` int(11) NOT NULL,
  `admin_ok` int(11) DEFAULT '0' COMMENT '0不锁，1锁定',
  `admin_date` int(11) DEFAULT NULL COMMENT '登录日期',
  `admin_type` int(11) DEFAULT '1' COMMENT '管理员类型,0为超级，1为普通',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='后台管理员表';

-- ----------------------------
-- Records of dy_admin
-- ----------------------------
INSERT INTO `dy_admin` VALUES ('1', 'admin', '005bdd6701415d7eee101fdf6f2ffc69', '295', null, null, '119.93.131.11', '119.93.131.11', '0', '1553967172', '0', '1553968125', '1');
INSERT INTO `dy_admin` VALUES ('2', 'billow', 'e99a18c428cb38d5f260853678922e03', '39', null, null, '219.133.179.176', '113.89.237.121', '0', '1536381674', '0', '1537428309', '1');
INSERT INTO `dy_admin` VALUES ('3', 'user_hldpk', '37e1c8da07b1a5fecc7344f73c16c636', '77', null, null, '113.77.145.203', '116.18.23.125', '0', '1532616632', '0', '1532674467', '1');
INSERT INTO `dy_admin` VALUES ('4', 'user_jj', '2169eb0c6736edd62abebeb36e458587', '60', null, null, '125.89.89.29', '183.50.98.72', '0', '1540194410', '0', '1540210498', '1');
INSERT INTO `dy_admin` VALUES ('5', 'user_test', 'e99a18c428cb38d5f260853678922e03', '536', null, null, '113.87.182.29', '113.89.238.213', '0', '1557559407', '0', '1557561479', '1');
INSERT INTO `dy_admin` VALUES ('6', 'user_abc', 'e99a18c428cb38d5f260853678922e03', '80', null, null, '113.118.186.200', '108.160.140.38', '0', '1555665330', '0', '1556541478', '0');
