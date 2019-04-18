/*
Navicat MySQL Data Transfer

Source Server         : 牛宝宝
Source Server Version : 50725
Source Host           : 47.106.182.137:6006
Source Database       : db_hzgame

Target Server Type    : MYSQL
Target Server Version : 50725
File Encoding         : 65001

Date: 2019-04-18 22:30:10
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for ag_apply
-- ----------------------------
DROP TABLE IF EXISTS `ag_apply`;
CREATE TABLE `ag_apply` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `userid` int(11) DEFAULT '0',
  `nickname` varchar(45) DEFAULT '',
  `rsdate` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `applycode` varchar(45) DEFAULT '',
  `state` int(11) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for ag_author
-- ----------------------------
DROP TABLE IF EXISTS `ag_author`;
CREATE TABLE `ag_author` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `binduserid` int(11) DEFAULT '0',
  `openid` varchar(45) CHARACTER SET latin1 DEFAULT '',
  `nickname` varchar(45) CHARACTER SET latin1 DEFAULT '',
  `unionid` varchar(45) CHARACTER SET latin1 DEFAULT '',
  `datemark` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `bindjulebuid` int(11) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=294 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for ag_drawmoney
-- ----------------------------
DROP TABLE IF EXISTS `ag_drawmoney`;
CREATE TABLE `ag_drawmoney` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `userid` int(11) DEFAULT '0',
  `nickname` varchar(45) DEFAULT '',
  `phone` varchar(45) DEFAULT '',
  `wechat` varchar(45) DEFAULT '',
  `drawnum` float DEFAULT '0',
  `state` int(11) DEFAULT '0' COMMENT '1：发起提款，2：审核中，3：成功',
  `rsdate` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `drawjetton` int(11) DEFAULT '0' COMMENT '金币数量',
  `julebuid` int(11) DEFAULT '0',
  `julebuname` varchar(45) DEFAULT '',
  `realname` varchar(128) DEFAULT '',
  `alipay` varchar(128) DEFAULT '',
  `bankacount` varchar(128) DEFAULT '',
  `bankname` varchar(128) DEFAULT '',
  `bankaddress` varchar(128) DEFAULT '',
  `orderid` int(11) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for ag_paylist
-- ----------------------------
DROP TABLE IF EXISTS `ag_paylist`;
CREATE TABLE `ag_paylist` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `userid` int(11) DEFAULT '0' COMMENT '代理的ID',
  `payuserid` int(11) DEFAULT '0',
  `paynick` varchar(45) DEFAULT '0',
  `aglevel` int(11) DEFAULT '0' COMMENT '代理的等级：1:1级，2:2级，99：该用户是代理',
  `paymoney` int(11) DEFAULT '0' COMMENT '支付的数量',
  `getmoney` float DEFAULT '0' COMMENT '抽水的数量',
  `paydate` timestamp NULL DEFAULT CURRENT_TIMESTAMP COMMENT '支付日期',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for ag_paysoleagent
-- ----------------------------
DROP TABLE IF EXISTS `ag_paysoleagent`;
CREATE TABLE `ag_paysoleagent` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `userid` int(11) DEFAULT NULL COMMENT '代理的ID',
  `payuserid` int(11) DEFAULT NULL,
  `paynick` varchar(45) DEFAULT NULL,
  `aglevel` int(11) DEFAULT NULL COMMENT '代理的等级：1:1级，2:2级，99：该用户是代理',
  `paymoney` int(11) DEFAULT NULL COMMENT '支付的数量',
  `getmoney` float DEFAULT NULL COMMENT '抽水的数量',
  `paydate` timestamp NULL DEFAULT CURRENT_TIMESTAMP COMMENT '支付日期',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for ag_player
-- ----------------------------
DROP TABLE IF EXISTS `ag_player`;
CREATE TABLE `ag_player` (
  `userid` int(11) NOT NULL,
  `nickname` varchar(45) DEFAULT '',
  `account` varchar(45) DEFAULT '' COMMENT '推广码',
  `password` varchar(45) DEFAULT '',
  `email` varchar(45) DEFAULT '',
  `phone` varchar(45) DEFAULT '',
  `wechat` varchar(45) DEFAULT '',
  `qq` varchar(45) DEFAULT '',
  `expandcode` varchar(45) DEFAULT '',
  `loginip` varchar(45) DEFAULT '',
  `rsdate` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `lgdate` datetime DEFAULT '2000-01-01 01:00:01',
  `state` int(11) DEFAULT '0',
  `level` int(11) DEFAULT '0' COMMENT '等级是0的是玩家，99：总代理，1:1级代理，2：2级代理',
  `allmoney` float DEFAULT '0' COMMENT '收益总额',
  `takemoney` float DEFAULT '0' COMMENT '可以提现的总额',
  `logincount` int(11) DEFAULT '0',
  `weekmoney` int(11) DEFAULT '0',
  `soleagent` int(11) DEFAULT '0' COMMENT '所属的总代',
  `agent1` int(11) DEFAULT '0' COMMENT '直接绑定的代理',
  `agent2` int(11) DEFAULT '0' COMMENT '绑定代理的上级代理',
  `agent3` int(11) DEFAULT '0' COMMENT '绑定代理的上上级代理',
  `agent4` int(11) DEFAULT '0' COMMENT '绑定代理的上上上级代理',
  `qrcodeurl` varchar(128) DEFAULT '',
  `timemark` datetime DEFAULT '2000-01-01 01:00:01',
  `moneymark` int(11) DEFAULT '0',
  `bankacount` varchar(45) DEFAULT '',
  `bankname` varchar(45) DEFAULT '',
  `bankaddress` varchar(128) DEFAULT '',
  `alipay` varchar(45) DEFAULT '',
  `realname` varchar(45) DEFAULT '',
  PRIMARY KEY (`userid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for ag_special
-- ----------------------------
DROP TABLE IF EXISTS `ag_special`;
CREATE TABLE `ag_special` (
  `userid` int(11) NOT NULL,
  `password` varchar(45) DEFAULT '',
  `nickname` varchar(45) DEFAULT '',
  `starttime` varchar(45) DEFAULT '',
  `endtime` varchar(45) DEFAULT '',
  `rate` int(11) DEFAULT '0',
  `state` int(11) DEFAULT '0' COMMENT '0是不起效果，1是开始，2是关闭',
  `level` int(11) DEFAULT '0' COMMENT '等级',
  `channel` varchar(45) DEFAULT '',
  `gamelist` varchar(245) DEFAULT '',
  PRIMARY KEY (`userid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for du_channel
-- ----------------------------
DROP TABLE IF EXISTS `du_channel`;
CREATE TABLE `du_channel` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(45) DEFAULT NULL,
  `channel` varchar(45) DEFAULT NULL,
  `description` varchar(45) DEFAULT NULL,
  `state` tinyint(3) DEFAULT NULL,
  `createdate` varchar(45) DEFAULT NULL,
  `lastdate` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `remark` varchar(125) DEFAULT NULL,
  `paytype` varchar(128) DEFAULT '' COMMENT '付费类型列表，1：微信支付，2：支付支付，3：ios支付，4：第三方SDK支付（调起第三方计费）',
  `modules` varchar(128) DEFAULT '' COMMENT '渠道对应开放模块列表',
  `introduce` varchar(256) DEFAULT '' COMMENT '对于房卡模式，这个是显示在登录界面的说明。',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for du_channelversion
-- ----------------------------
DROP TABLE IF EXISTS `du_channelversion`;
CREATE TABLE `du_channelversion` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `channel` varchar(45) DEFAULT 'all',
  `version` int(11) DEFAULT NULL,
  `description` varchar(256) DEFAULT NULL,
  `isused` tinyint(3) DEFAULT '0',
  `createdate` varchar(45) DEFAULT NULL,
  `lastdate` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `downurl` varchar(256) DEFAULT NULL,
  `state` int(11) DEFAULT '0',
  `remark` varchar(256) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for du_server
-- ----------------------------
DROP TABLE IF EXISTS `du_server`;
CREATE TABLE `du_server` (
  `serverid` int(11) NOT NULL AUTO_INCREMENT,
  `servername` varchar(45) DEFAULT NULL,
  `serverip` varchar(45) DEFAULT NULL,
  `gamename` varchar(45) DEFAULT NULL,
  `tcpport` int(11) DEFAULT '0',
  `httpport` int(11) DEFAULT '0',
  `udpport` int(11) DEFAULT '0',
  `nettype` tinyint(3) DEFAULT '0',
  `state` tinyint(3) DEFAULT '0',
  `createdate` datetime DEFAULT '2000-01-01 00:00:00',
  `lastdate` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `remark` varchar(127) DEFAULT NULL,
  PRIMARY KEY (`serverid`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for du_version
-- ----------------------------
DROP TABLE IF EXISTS `du_version`;
CREATE TABLE `du_version` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `clientversion` int(11) DEFAULT NULL,
  `serverversion` int(11) DEFAULT NULL,
  `clientupdate` tinyint(3) DEFAULT '0',
  `serverupdate` tinyint(3) DEFAULT '0',
  `isused` tinyint(3) DEFAULT '0',
  `createdate` varchar(45) DEFAULT NULL,
  `lastdate` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `remark` varchar(256) DEFAULT NULL,
  `downurl` varchar(256) DEFAULT NULL,
  `allupdate` int(11) DEFAULT '0',
  `pkgsize` int(11) DEFAULT '0' COMMENT '包体大小',
  `filename` varchar(45) DEFAULT NULL,
  `channel` varchar(45) DEFAULT 'all',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

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
-- Table structure for dy_broadcast
-- ----------------------------
DROP TABLE IF EXISTS `dy_broadcast`;
CREATE TABLE `dy_broadcast` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `senderid` int(11) DEFAULT NULL,
  `sender` varchar(45) DEFAULT NULL,
  `broadtype` int(11) DEFAULT NULL,
  `msg` varchar(256) DEFAULT NULL,
  `createdate` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `broadtimes` int(11) DEFAULT '0',
  `validday` int(11) DEFAULT '0',
  `cycletype` tinyint(3) DEFAULT '0',
  `isexist` int(11) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for dy_friends
-- ----------------------------
DROP TABLE IF EXISTS `dy_friends`;
CREATE TABLE `dy_friends` (
  `userid` int(11) NOT NULL,
  `cratedate` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `amount` int(11) DEFAULT '0',
  `friends` varchar(6000) DEFAULT '',
  PRIMARY KEY (`userid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for dy_fruitplayer
-- ----------------------------
DROP TABLE IF EXISTS `dy_fruitplayer`;
CREATE TABLE `dy_fruitplayer` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `userid` int(11) DEFAULT NULL,
  `fruitdate` date DEFAULT '2000-01-01',
  `fruittype` tinyint(3) DEFAULT '0' COMMENT '水果机类型',
  `betjetton` int(11) DEFAULT '0' COMMENT '总共投入的钱',
  `recvjetton` int(11) DEFAULT '0' COMMENT '赢到的钱',
  `breakret` tinyint(3) DEFAULT '0' COMMENT '破产保护次数',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for dy_fruituser
-- ----------------------------
DROP TABLE IF EXISTS `dy_fruituser`;
CREATE TABLE `dy_fruituser` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `userid` int(11) DEFAULT '0',
  `fruittype` int(11) DEFAULT '0',
  `maxaction` int(11) DEFAULT '0' COMMENT '总得下注次数',
  `pourjetton` int(11) DEFAULT '0',
  `winjetton` int(11) DEFAULT '0',
  `wintype1` int(11) DEFAULT '0',
  `wintype2` int(11) DEFAULT '0',
  `wintype3` int(11) DEFAULT '0',
  `wintype4` int(11) DEFAULT '0',
  `wintype5` int(11) DEFAULT '0',
  `wintype6` int(11) DEFAULT '0',
  `wintype7` int(11) DEFAULT '0',
  `wintype8` int(11) DEFAULT '0',
  `wintype9` int(11) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for dy_goods
-- ----------------------------
DROP TABLE IF EXISTS `dy_goods`;
CREATE TABLE `dy_goods` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `userid` int(11) DEFAULT '0',
  `goodsid` int(11) DEFAULT '0',
  `amount` int(11) DEFAULT '0',
  `validity` int(11) DEFAULT '0' COMMENT '有效期：0是已经过期 9999是永久拥有',
  `createdate` datetime DEFAULT NULL,
  `iswear` tinyint(3) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for dy_invite
-- ----------------------------
DROP TABLE IF EXISTS `dy_invite`;
CREATE TABLE `dy_invite` (
  `userid` int(11) NOT NULL DEFAULT '0' COMMENT '被邀请者的ID',
  `touserid` int(11) DEFAULT '0',
  `bindstate` int(11) DEFAULT '0',
  `binddate` varchar(32) CHARACTER SET latin1 DEFAULT '',
  PRIMARY KEY (`userid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='邀请码';

-- ----------------------------
-- Table structure for dy_julebu
-- ----------------------------
DROP TABLE IF EXISTS `dy_julebu`;
CREATE TABLE `dy_julebu` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(45) DEFAULT NULL,
  `ownerid` int(11) DEFAULT NULL,
  `rsdate` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `maxmembernum` int(11) DEFAULT '0',
  `membernum` int(11) DEFAULT '0',
  `notice` varchar(128) DEFAULT '',
  `gamemodules` varchar(256) DEFAULT '' COMMENT '包含的游戏的模块',
  `adminlist` varchar(256) DEFAULT '',
  `status` int(11) DEFAULT '99' COMMENT '状态,99是正常状态',
  `chatid` int(11) DEFAULT '0' COMMENT '保存聊天增长的chatid',
  `counttime` varchar(45) DEFAULT '',
  `channel` varchar(45) DEFAULT '' COMMENT '所属的渠道列表',
  `coffers` int(11) DEFAULT '0',
  `partnerlist` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=10577 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for dy_julebu_member
-- ----------------------------
DROP TABLE IF EXISTS `dy_julebu_member`;
CREATE TABLE `dy_julebu_member` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `userid` int(11) DEFAULT '0',
  `julebuid` int(11) DEFAULT '0',
  `rsdate` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `status` int(11) DEFAULT '0' COMMENT '1是正常的，-1是被踢出了',
  `jifen` int(11) DEFAULT '0' COMMENT '每天的积分，会同步到这里，如果说缓存中不存在的时候，需要从这里恢复',
  `limitjifen` int(11) DEFAULT '0',
  `superiorid` int(11) NOT NULL DEFAULT '0',
  `higherid` int(11) DEFAULT '0',
  `getjifen` int(11) DEFAULT '0',
  `qrcodeurl` varchar(45) DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1499 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for dy_loginaward
-- ----------------------------
DROP TABLE IF EXISTS `dy_loginaward`;
CREATE TABLE `dy_loginaward` (
  `userid` int(11) NOT NULL,
  `onlineaward` varchar(129) DEFAULT '|' COMMENT '在线时长奖励',
  `continueaward` varchar(129) DEFAULT '|' COMMENT '连续登陆奖励',
  `signaward` varchar(512) DEFAULT '|' COMMENT '每天签到奖励',
  PRIMARY KEY (`userid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for dy_mail
-- ----------------------------
DROP TABLE IF EXISTS `dy_mail`;
CREATE TABLE `dy_mail` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `senderid` int(11) DEFAULT '0',
  `sender` varchar(45) DEFAULT '',
  `receiverid` int(11) DEFAULT '0',
  `receiver` varchar(45) DEFAULT '',
  `mailtype` tinyint(3) DEFAULT '0' COMMENT '邮件类型',
  `title` varchar(45) DEFAULT '',
  `content` varchar(512) DEFAULT '',
  `senddate` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `markdate` timestamp NULL DEFAULT NULL,
  `mailstate` tinyint(2) DEFAULT '1' COMMENT '邮件状态:0：已经删除；1：未读；2：有附件未读；3：有附件已读；4：已读已领取。',
  `externdata` varchar(256) DEFAULT '',
  `remark` varchar(45) DEFAULT '',
  `validity` tinyint(3) DEFAULT '0' COMMENT '邮件的有效期，-1表示无期限。',
  PRIMARY KEY (`id`),
  KEY `senderid` (`senderid`),
  KEY `receiverid` (`receiverid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for dy_mission
-- ----------------------------
DROP TABLE IF EXISTS `dy_mission`;
CREATE TABLE `dy_mission` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `userid` int(11) DEFAULT '0',
  `missionid` int(11) DEFAULT '0',
  `createdate` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `finish` int(11) DEFAULT '0',
  `state` int(11) DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `userid` (`userid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for dy_order
-- ----------------------------
DROP TABLE IF EXISTS `dy_order`;
CREATE TABLE `dy_order` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `userid` int(11) DEFAULT '0',
  `buyid` int(11) DEFAULT '0',
  `timeid` int(11) DEFAULT '0',
  `status` tinyint(3) DEFAULT '0',
  `channel` varchar(45) DEFAULT NULL,
  `createtime` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `sign` varchar(45) DEFAULT '' COMMENT '腾讯需要的字符字符串，在这里是MD5生成的字符串',
  `shoptype` int(11) DEFAULT '0' COMMENT '购买的入口类型：默认情况下是0和1是钻石，2是商店，为了区分不同的渠道而定制的',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=443 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for dy_orderinfo
-- ----------------------------
DROP TABLE IF EXISTS `dy_orderinfo`;
CREATE TABLE `dy_orderinfo` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `userid` int(11) DEFAULT '0',
  `payid` int(11) DEFAULT '0',
  `orderid` int(11) DEFAULT '0',
  `channel` varchar(45) DEFAULT '0',
  `nonce_str` varchar(45) DEFAULT NULL,
  `time_start` varchar(45) DEFAULT NULL,
  `time_expire` varchar(45) DEFAULT NULL,
  `total_fee` int(11) DEFAULT '0',
  `state` int(11) DEFAULT '0' COMMENT '0:发起支付请求;1:收到支付回调，支付成功，-1：返回失败',
  `ctreatedate` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `out_trade_no` varchar(45) DEFAULT NULL,
  `ch_ordderid` varchar(45) DEFAULT NULL COMMENT '渠道获得的id',
  `remark` varchar(45) DEFAULT NULL,
  `paytype` varchar(45) DEFAULT NULL COMMENT '支付宝，微信',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for dy_payconf
-- ----------------------------
DROP TABLE IF EXISTS `dy_payconf`;
CREATE TABLE `dy_payconf` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `type` int(11) DEFAULT '0',
  `wechat` int(11) DEFAULT '0',
  `alipay` int(11) DEFAULT '0',
  `des` varchar(128) DEFAULT '',
  `remark` varchar(45) DEFAULT '',
  `state` int(11) DEFAULT '0',
  `url` varchar(256) DEFAULT '',
  `method` varchar(45) DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for dy_payrate
-- ----------------------------
DROP TABLE IF EXISTS `dy_payrate`;
CREATE TABLE `dy_payrate` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `payid` int(11) DEFAULT '0',
  `addtype` int(11) DEFAULT '0' COMMENT '加成的类型，：1：按照固定数量加成；2：按照固定比例加成',
  `addamount` int(11) DEFAULT '0' COMMENT '加成的数量',
  `adddes` int(11) DEFAULT '0' COMMENT '加成的描述',
  `addtime` datetime DEFAULT NULL COMMENT '加成的截止日期',
  `createtime` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `isexist` int(11) DEFAULT '1',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

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

-- ----------------------------
-- Table structure for dy_playermsg
-- ----------------------------
DROP TABLE IF EXISTS `dy_playermsg`;
CREATE TABLE `dy_playermsg` (
  `userid` int(11) NOT NULL,
  `nickname` varchar(45) DEFAULT '',
  `mdynickamount` smallint(5) DEFAULT '0' COMMENT '修改昵称的次数',
  `loginday` int(11) DEFAULT '1',
  `loginconday` int(11) DEFAULT '1',
  `maxloginconday` int(11) DEFAULT '1',
  `loginamount` int(11) DEFAULT '1',
  `onlinetime` int(11) DEFAULT '0',
  `maxjetton` int(11) DEFAULT '0',
  `payrmb` int(11) DEFAULT '0',
  `callpayrmb` int(11) DEFAULT '0',
  `paytime` int(11) DEFAULT '0',
  `callpaytime` int(11) DEFAULT '0',
  `maxmoney` int(11) DEFAULT '0',
  `dayrmb` int(11) DEFAULT '0',
  `daypaytime` int(11) DEFAULT '0',
  `lastlogin` timestamp NULL DEFAULT '1999-12-31 08:00:00',
  `lastpay` timestamp NULL DEFAULT '1999-12-31 08:00:00',
  PRIMARY KEY (`userid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for dy_roulette
-- ----------------------------
DROP TABLE IF EXISTS `dy_roulette`;
CREATE TABLE `dy_roulette` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `roulettename` varchar(45) DEFAULT NULL,
  `createdate` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `amount` smallint(5) DEFAULT '0',
  `maxnum` smallint(5) DEFAULT '500',
  `state` tinyint(3) DEFAULT '0' COMMENT '活动的状态：0：没结束：1：已经结束',
  `winid` int(11) DEFAULT '0',
  `awardtime` datetime DEFAULT '2000-01-01 00:00:01',
  `winkey` varchar(45) DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for dy_rouletteplayer
-- ----------------------------
DROP TABLE IF EXISTS `dy_rouletteplayer`;
CREATE TABLE `dy_rouletteplayer` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `userid` int(11) DEFAULT '0',
  `roulettename` varchar(45) DEFAULT NULL,
  `createdate` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `cardid` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for dy_serverinfo
-- ----------------------------
DROP TABLE IF EXISTS `dy_serverinfo`;
CREATE TABLE `dy_serverinfo` (
  `serverid` int(11) NOT NULL AUTO_INCREMENT,
  `servername` varchar(45) DEFAULT NULL,
  `takemin` int(11) DEFAULT '0' COMMENT '最小提现',
  `takerate` int(11) DEFAULT '0' COMMENT '提现手续费',
  `takedaynum` int(11) DEFAULT '0' COMMENT '每日提现次数',
  `wechatpay` int(11) DEFAULT '0' COMMENT '0：不开通微信支付，1：开通微信支付',
  `alipay` int(11) DEFAULT '0' COMMENT '0:不开通阿里支付，1：开通阿里支付',
  `giftrate` int(11) DEFAULT '0' COMMENT '赠送手续费',
  `gamerate` int(11) DEFAULT '0' COMMENT '返奖率',
  `wechatinfo` varchar(128) DEFAULT '' COMMENT '客户微信',
  PRIMARY KEY (`serverid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for dy_sharecode
-- ----------------------------
DROP TABLE IF EXISTS `dy_sharecode`;
CREATE TABLE `dy_sharecode` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `userid` int(11) DEFAULT '0',
  `touserid` int(11) DEFAULT '0',
  `strcode` varchar(45) DEFAULT NULL,
  `state` int(11) DEFAULT '0' COMMENT '0:',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for dy_shoprebate
-- ----------------------------
DROP TABLE IF EXISTS `dy_shoprebate`;
CREATE TABLE `dy_shoprebate` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `shopid` int(11) DEFAULT NULL,
  `shopname` varchar(45) DEFAULT NULL,
  `description` varchar(128) DEFAULT NULL,
  `startdate` timestamp NULL DEFAULT NULL COMMENT '起效日期',
  `enddate` timestamp NULL DEFAULT NULL COMMENT '结束日期',
  `validity` int(11) DEFAULT '0' COMMENT '有效期',
  `distype` int(11) DEFAULT NULL COMMENT '折扣类型：11：得到具体数量增加；12：得到按照百分比增加；21：价格降低具体数量；22：价格降低百分比',
  `discount` int(11) DEFAULT NULL,
  `isexist` tinyint(2) DEFAULT '1' COMMENT '1是存在，0是已经过期',
  `remark` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for dy_syscount
-- ----------------------------
DROP TABLE IF EXISTS `dy_syscount`;
CREATE TABLE `dy_syscount` (
  `id` int(11) NOT NULL,
  `description` varchar(45) DEFAULT NULL COMMENT '描述',
  `type` smallint(3) DEFAULT '0' COMMENT '货币的类型',
  `issue` bigint(20) DEFAULT '0' COMMENT '发行的数量',
  `recycle` bigint(20) DEFAULT '0' COMMENT '回收的货币',
  `count` bigint(20) DEFAULT '0' COMMENT '正在流通的货币',
  `curdate` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for dy_user
-- ----------------------------
DROP TABLE IF EXISTS `dy_user`;
CREATE TABLE `dy_user` (
  `userid` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(45) DEFAULT NULL,
  `password` varchar(45) DEFAULT NULL,
  `phonenum` varchar(45) DEFAULT NULL,
  `channel` varchar(45) DEFAULT NULL,
  `tokenid` varchar(129) DEFAULT NULL,
  `createdate` datetime DEFAULT NULL,
  `email` varchar(129) DEFAULT NULL,
  `nickname` varchar(45) DEFAULT NULL,
  `sex` int(11) DEFAULT '0',
  `age` int(11) DEFAULT '0',
  `viplevel` int(11) DEFAULT '0',
  `coin` bigint(45) DEFAULT '0',
  `gold` bigint(45) DEFAULT '0',
  `money` int(11) DEFAULT '0',
  `face_1` varchar(129) DEFAULT NULL,
  `face_2` varchar(129) DEFAULT NULL,
  `face_3` varchar(129) DEFAULT NULL,
  `face_4` varchar(129) DEFAULT NULL,
  `lastip` varchar(45) DEFAULT NULL,
  `lasttime_1` int(11) DEFAULT '0',
  `lasttime_2` int(11) DEFAULT '0',
  `lasttime_3` int(11) DEFAULT '0',
  `loginnum` int(11) DEFAULT '0',
  `silent` int(11) DEFAULT '0',
  `isban` int(11) DEFAULT '0',
  `exp` int(11) DEFAULT '0',
  PRIMARY KEY (`userid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for dy_wxfriend
-- ----------------------------
DROP TABLE IF EXISTS `dy_wxfriend`;
CREATE TABLE `dy_wxfriend` (
  `userid` int(11) NOT NULL DEFAULT '0',
  `mysign` varchar(45) DEFAULT '' COMMENT '我自己的字符串',
  `touserid` int(11) DEFAULT '0',
  `tonickname` varchar(45) DEFAULT '',
  `tosign` int(11) DEFAULT '0' COMMENT '已经绑定的字符串',
  `bindstate` int(11) DEFAULT '0' COMMENT '已经绑定',
  `gotnum` int(11) DEFAULT '0' COMMENT '已经领取的房卡数',
  `canget` int(11) DEFAULT '0' COMMENT '可以领取的房卡数',
  `binddate` int(11) DEFAULT '1',
  PRIMARY KEY (`userid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for log_behavior
-- ----------------------------
DROP TABLE IF EXISTS `log_behavior`;
CREATE TABLE `log_behavior` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `userid` int(11) DEFAULT '0',
  `nickname` varchar(45) DEFAULT '',
  `level` varchar(45) DEFAULT 'info' COMMENT 'log的等级：debug，info，warning，error',
  `createdate` timestamp NULL DEFAULT NULL,
  `category` varchar(45) DEFAULT '',
  `tag` varchar(45) DEFAULT '',
  `gettype` varchar(45) DEFAULT '' COMMENT '获取到的物品的类型',
  `getamount` float DEFAULT '0' COMMENT '获取到的数量',
  `costtype` varchar(45) DEFAULT '',
  `costamount` float DEFAULT '0',
  `retcode` int(11) DEFAULT '0' COMMENT '错误码',
  `msg` varchar(256) DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=12595 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for log_currencychangedrecord
-- ----------------------------
DROP TABLE IF EXISTS `log_currencychangedrecord`;
CREATE TABLE `log_currencychangedrecord` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `userid` int(11) NOT NULL,
  `currency_type` int(11) NOT NULL COMMENT '货币类型,0为money',
  `initial` int(11) NOT NULL,
  `amount` int(11) NOT NULL,
  `date` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `opt_type` int(11) NOT NULL COMMENT '操作类型,0为代理赠送钻石,1为玩家收到代理赠送的钻石',
  `remark` varchar(256) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=273 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for log_donaterecord
-- ----------------------------
DROP TABLE IF EXISTS `log_donaterecord`;
CREATE TABLE `log_donaterecord` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `userid` int(11) NOT NULL,
  `julebuid` int(11) NOT NULL,
  `donate_state` int(11) NOT NULL COMMENT '货币类型,0为money',
  `initial` int(11) NOT NULL,
  `amount` int(11) NOT NULL,
  `donate_time` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `remark` varchar(256) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for log_economy
-- ----------------------------
DROP TABLE IF EXISTS `log_economy`;
CREATE TABLE `log_economy` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `description` varchar(45) DEFAULT NULL,
  `type` tinyint(3) DEFAULT '0' COMMENT '货币类型',
  `category` smallint(5) DEFAULT '0' COMMENT '操作模块',
  `count` int(11) DEFAULT '0',
  `occurdate` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `issue` bigint(20) DEFAULT '0' COMMENT '发放的总量',
  `recycle` bigint(20) DEFAULT '0' COMMENT '回收的总量',
  `currency` bigint(20) DEFAULT NULL COMMENT '剩余的总量',
  `remark` varchar(256) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for log_exception
-- ----------------------------
DROP TABLE IF EXISTS `log_exception`;
CREATE TABLE `log_exception` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `loglevel` varchar(45) DEFAULT NULL,
  `userid` int(11) DEFAULT NULL,
  `nickname` varchar(45) DEFAULT NULL,
  `tag` varchar(45) DEFAULT NULL,
  `category` varchar(45) DEFAULT NULL,
  `code` int(11) DEFAULT NULL,
  `msg` varchar(512) DEFAULT NULL,
  `createdate` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for log_fanday
-- ----------------------------
DROP TABLE IF EXISTS `log_fanday`;
CREATE TABLE `log_fanday` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `julebuid` int(11) DEFAULT '0',
  `ownerid` int(11) DEFAULT '0',
  `datestr` varchar(45) DEFAULT '',
  `typemsg` varchar(45) DEFAULT '',
  `amount` int(11) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for log_fanjiang
-- ----------------------------
DROP TABLE IF EXISTS `log_fanjiang`;
CREATE TABLE `log_fanjiang` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `julebuid` int(11) DEFAULT '0',
  `ownerid` int(11) DEFAULT '0',
  `createdate` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `typemsg` varchar(45) DEFAULT '',
  `amount` int(11) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for log_giftgold
-- ----------------------------
DROP TABLE IF EXISTS `log_giftgold`;
CREATE TABLE `log_giftgold` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `fromid` int(11) DEFAULT '0',
  `fromnick` varchar(128) DEFAULT '',
  `toid` int(11) DEFAULT '0',
  `tonick` varchar(128) DEFAULT '',
  `fromgoldbegin` int(11) DEFAULT '0',
  `fromgoldafter` int(11) DEFAULT '0',
  `togoldbegin` int(11) DEFAULT '0',
  `togoldafter` int(11) DEFAULT '0',
  `sendnum` int(11) DEFAULT '0',
  `state` int(11) DEFAULT '0',
  `createdate` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `realnum` int(11) DEFAULT '0',
  `julebuid` int(11) DEFAULT '0',
  `msgtype` varchar(45) DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for log_julebuljimitjifenhistory
-- ----------------------------
DROP TABLE IF EXISTS `log_julebuljimitjifenhistory`;
CREATE TABLE `log_julebuljimitjifenhistory` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `julebuid` int(11) DEFAULT '0',
  `executorid` int(11) DEFAULT '0',
  `targetid` int(11) DEFAULT '0',
  `operationtype` int(11) DEFAULT '0',
  `countnum` int(11) DEFAULT '0',
  `timenum` int(11) DEFAULT '0',
  `Ctime` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=9669 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for log_player
-- ----------------------------
DROP TABLE IF EXISTS `log_player`;
CREATE TABLE `log_player` (
  `userid` int(11) NOT NULL,
  `nickname` varchar(45) DEFAULT NULL,
  `regdate` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `lastdate` timestamp NULL DEFAULT '1999-12-31 08:00:00',
  `penultdate` timestamp NULL DEFAULT '1999-12-31 08:00:00',
  `loginday` int(11) DEFAULT '1' COMMENT '登录天数',
  `logincount` int(11) DEFAULT '1' COMMENT '登录次数',
  `onlinetime` int(11) DEFAULT '0' COMMENT '在线时长',
  `fcallpaydate` timestamp NULL DEFAULT '1999-12-31 08:00:00' COMMENT '首次调起付费时间',
  `fpaydate` timestamp NULL DEFAULT '1999-12-31 08:00:00' COMMENT '首次付费时间',
  `lpaydate` timestamp NULL DEFAULT '1999-12-31 08:00:00' COMMENT '最后一次付费时间',
  `callpaycount` int(11) DEFAULT '0' COMMENT '调起付费总额',
  `paycount` int(11) DEFAULT '0' COMMENT '付费总额',
  `callpaytimes` int(11) DEFAULT '0' COMMENT '调起付费次数',
  `paytimes` int(11) DEFAULT '0' COMMENT '付费次数',
  `day1` smallint(5) DEFAULT '1',
  `day2` smallint(5) DEFAULT '0',
  `day3` smallint(5) DEFAULT '0',
  `day4` smallint(5) DEFAULT '0',
  `day5` smallint(5) DEFAULT '0',
  `day6` smallint(5) DEFAULT '0',
  `day7` smallint(5) DEFAULT '0',
  `day8` smallint(5) DEFAULT '0',
  `day9` smallint(5) DEFAULT '0',
  `day10` smallint(5) DEFAULT '0',
  `day11` smallint(5) DEFAULT '0',
  `day12` smallint(5) DEFAULT '0',
  `day13` smallint(5) DEFAULT '0',
  `day14` smallint(5) DEFAULT '0',
  `day15` smallint(5) DEFAULT '0',
  `day16` smallint(5) DEFAULT '0',
  `day17` smallint(5) DEFAULT '0',
  `day18` smallint(5) DEFAULT '0',
  `day19` smallint(5) DEFAULT '0',
  `day20` smallint(5) DEFAULT '0',
  `day21` smallint(5) DEFAULT '0',
  `day22` smallint(5) DEFAULT '0',
  `day23` smallint(5) DEFAULT '0',
  `day24` smallint(5) DEFAULT '0',
  `day25` smallint(5) DEFAULT '0',
  `day26` smallint(5) DEFAULT '0',
  `day27` smallint(5) DEFAULT '0',
  `day28` smallint(5) DEFAULT '0',
  `day29` smallint(5) DEFAULT '0',
  `day30` smallint(5) DEFAULT '0',
  `day60` smallint(5) DEFAULT '0',
  `day90` smallint(5) DEFAULT '0',
  `day120` smallint(5) DEFAULT '0',
  `day150` smallint(5) DEFAULT '0',
  `day180` smallint(5) DEFAULT '0',
  `day210` smallint(5) DEFAULT '0',
  `day240` smallint(5) DEFAULT '0',
  `day270` smallint(5) DEFAULT '0',
  `day300` smallint(5) DEFAULT '0',
  `day330` smallint(5) DEFAULT '0',
  `day360` smallint(5) DEFAULT '0',
  PRIMARY KEY (`userid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for log_race
-- ----------------------------
DROP TABLE IF EXISTS `log_race`;
CREATE TABLE `log_race` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `racetype` int(11) DEFAULT NULL,
  `raceid` int(11) DEFAULT NULL,
  `tableid` int(11) DEFAULT NULL,
  `starttime` datetime DEFAULT NULL,
  `endtime` datetime DEFAULT NULL,
  `suseridlist` varchar(129) DEFAULT NULL,
  `euseridlist` varchar(129) DEFAULT NULL,
  `winid` int(11) DEFAULT NULL,
  `winnickname` varchar(45) DEFAULT NULL,
  `winamount` int(11) DEFAULT NULL,
  `msg` longtext,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for log_special
-- ----------------------------
DROP TABLE IF EXISTS `log_special`;
CREATE TABLE `log_special` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `tableid` int(11) DEFAULT '0',
  `nickname` varchar(45) DEFAULT '',
  `userid` int(11) DEFAULT '0',
  `timemark` varchar(45) DEFAULT '',
  `specialstate` int(11) DEFAULT '0' COMMENT '功能开始的状态',
  `winnum` int(11) DEFAULT '0',
  `des` varchar(4500) DEFAULT '',
  `gametype` varchar(45) DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for log_sysdaily
-- ----------------------------
DROP TABLE IF EXISTS `log_sysdaily`;
CREATE TABLE `log_sysdaily` (
  `dateid` varchar(45) NOT NULL DEFAULT '2000-01-01',
  `activeplayer` int(11) DEFAULT '0',
  `logincount` int(11) DEFAULT '0',
  `validplayer` int(11) DEFAULT '0' COMMENT '有效玩家，每天登录进去，最少呆了15分钟的。',
  `newplayer` int(11) DEFAULT '0',
  `callpayplayer` int(11) DEFAULT '0' COMMENT '调起付费接口人数',
  `payplayer` int(11) DEFAULT '0' COMMENT '付费人数',
  `callpaytimes` int(11) DEFAULT '0' COMMENT '调起付费次数',
  `paytimes` int(11) DEFAULT '0' COMMENT '付费次数',
  `callpaycount` int(11) DEFAULT '0' COMMENT '调起付费接口总额',
  `paycount` int(11) DEFAULT '0' COMMENT '付费总额',
  `newpayplayer` int(11) DEFAULT '0' COMMENT '付费新玩家人数',
  `newpaycount` int(11) DEFAULT '0' COMMENT '付费新玩家总额',
  `remain2` int(11) DEFAULT '0',
  `remain3` int(11) DEFAULT '0',
  `remain7` int(11) DEFAULT '0',
  `remain15` int(11) DEFAULT '0',
  `remain30` int(11) DEFAULT '0',
  `alllist` varchar(100) DEFAULT '|',
  `onlinelist` varchar(10000) DEFAULT '|',
  `minulist` varchar(10000) DEFAULT '|',
  PRIMARY KEY (`dateid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
