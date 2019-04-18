/*
Navicat MySQL Data Transfer

Source Server         : 牛宝宝
Source Server Version : 50725
Source Host           : 47.106.182.137:6006
Source Database       : db_hzgame

Target Server Type    : MYSQL
Target Server Version : 50725
File Encoding         : 65001

Date: 2019-04-18 22:32:41
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for dy_player
-- ----------------------------
DROP TABLE IF EXISTS `dy_player`;
CREATE TABLE `dy_player` (
  `userid` int(11) NOT NULL AUTO_INCREMENT,
  `cid` varchar(45) NOT NULL COMMENT '设备唯一的标识',
  `account` varchar(45) NOT NULL COMMENT '账号，不对唯一性做检查',
  `password` varchar(45) NOT NULL,
  `nickname` varchar(45) DEFAULT NULL,
  `face_1` varchar(45) NOT NULL,
  `face_2` varchar(45) DEFAULT '',
  `face_3` varchar(45) DEFAULT '',
  `face_4` varchar(45) DEFAULT '',
  `regdate` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `sex` int(11) DEFAULT '0',
  `age` int(11) DEFAULT '0',
  `email` varchar(45) DEFAULT '',
  `phonenum` varchar(45) DEFAULT '',
  `exp` int(11) DEFAULT '0',
  `level` int(11) DEFAULT '0',
  `viplevel` int(11) DEFAULT '0',
  `jetton` bigint(45) DEFAULT '0',
  `gold` int(11) DEFAULT '0',
  `money` int(11) DEFAULT '0',
  `channel` varchar(45) DEFAULT '',
  `province` varchar(45) DEFAULT '',
  `city` varchar(45) DEFAULT '',
  `popularity` int(11) DEFAULT '0',
  `idenstatus` int(11) DEFAULT '0',
  `bindtype` int(11) DEFAULT '0',
  `bindnick` varchar(45) DEFAULT '',
  `platformid` varchar(45) DEFAULT '' COMMENT 'android,ios',
  `imei` varchar(45) DEFAULT '',
  `devname` varchar(45) DEFAULT '',
  `macname` varchar(45) DEFAULT '',
  `mobiletype` int(11) DEFAULT '0',
  `lasttime` int(11) DEFAULT '0',
  `penulttime` int(11) DEFAULT '0',
  `silent` int(11) DEFAULT '0',
  `isban` int(11) DEFAULT '0',
  `isrobot` int(11) DEFAULT '0',
  `playtype` smallint(4) DEFAULT '1' COMMENT '1:超级保守型，需要很好的牌才跟或者all in；--2：保守型，会点牌技，一般好的牌就会跟了。--3：一般型，没有特别的，下牌全凭喜好；4:激进型，会点牌技，有点激进。5:超级激进型，只要拿到稍微好的牌，就会疯狂加注',
  `description` varchar(129) DEFAULT '',
  `blacklist` int(11) DEFAULT '0' COMMENT '列入黑名单',
  `propertyid` int(11) DEFAULT '0' COMMENT '佩戴物品的ID',
  `agencystatus` int(11) DEFAULT '0',
  `bank_password` varchar(45) DEFAULT NULL,
  `bank_jetton` bigint(40) DEFAULT '0',
  `registration1` varchar(45) DEFAULT '',
  `registration2` varchar(45) DEFAULT '',
  `registration3` varchar(45) DEFAULT '',
  `signinmonth` int(11) DEFAULT '0',
  `bsignin` int(11) DEFAULT '0',
  `turntablenum` int(11) DEFAULT '0',
  `tengames` int(11) DEFAULT '0',
  `todayshare` int(11) DEFAULT '0',
  `caninvite` int(11) DEFAULT '0',
  PRIMARY KEY (`userid`)
) ENGINE=InnoDB AUTO_INCREMENT=102353 DEFAULT CHARSET=utf8;
