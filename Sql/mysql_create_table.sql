/*
---------------------mysql-----------------------
Date:2019-5-5 16:14:25
*/

/*-- ----------------------------
-- Table structure for dy_player
-- ----------------------------*/
CREATE TABLE IF NOT EXISTS `dy_player` (
`userid` int not null AUTO_INCREMENT,
 PRIMARY KEY(`userid`)

) ENGINE=InnoDB DEFAULT CHARSET=utf8;
ALTER TABLE `dy_player` ADD COLUMN cid varchar(45) CHARACTER SET latin1 DEFAULT '';
ALTER TABLE `dy_player` ADD COLUMN account varchar(45) CHARACTER SET latin1 DEFAULT '';
ALTER TABLE `dy_player` ADD COLUMN password varchar(45) CHARACTER SET latin1 DEFAULT '';
ALTER TABLE `dy_player` ADD COLUMN face_1 varchar(45) CHARACTER SET latin1 DEFAULT '';
ALTER TABLE `dy_player` ADD COLUMN phonenum varchar(45) CHARACTER SET latin1 DEFAULT '';
ALTER TABLE `dy_player` ADD COLUMN nickname varchar(45) CHARACTER SET latin1 DEFAULT '';
ALTER TABLE `dy_player` ADD COLUMN face_2 varchar(45) CHARACTER SET latin1 DEFAULT '';
ALTER TABLE `dy_player` ADD COLUMN jetton int default 0;
ALTER TABLE `dy_player` ADD COLUMN bindnick varchar(45) CHARACTER SET latin1 DEFAULT '';
ALTER TABLE `dy_player` ADD COLUMN description varchar(45) CHARACTER SET latin1 DEFAULT '';
ALTER TABLE `dy_player` ADD COLUMN regdate varchar(45) CHARACTER SET latin1 DEFAULT '';
ALTER TABLE `dy_player` ADD COLUMN popularity int default 0;
ALTER TABLE `dy_player` ADD COLUMN face_3 varchar(45) CHARACTER SET latin1 DEFAULT '';
ALTER TABLE `dy_player` ADD COLUMN face_4 varchar(45) CHARACTER SET latin1 DEFAULT '';
ALTER TABLE `dy_player` ADD COLUMN sex int default 0;
ALTER TABLE `dy_player` ADD COLUMN email varchar(45) CHARACTER SET latin1 DEFAULT '';
ALTER TABLE `dy_player` ADD COLUMN silent int default 0;
ALTER TABLE `dy_player` ADD COLUMN channel varchar(45) CHARACTER SET latin1 DEFAULT '';
ALTER TABLE `dy_player` ADD COLUMN province varchar(45) CHARACTER SET latin1 DEFAULT '';
ALTER TABLE `dy_player` ADD COLUMN city varchar(45) CHARACTER SET latin1 DEFAULT '';
ALTER TABLE `dy_player` ADD COLUMN imei varchar(45) CHARACTER SET latin1 DEFAULT '';
ALTER TABLE `dy_player` ADD COLUMN tengames int default 0;
ALTER TABLE `dy_player` ADD COLUMN devname varchar(45) CHARACTER SET latin1 DEFAULT '';
ALTER TABLE `dy_player` ADD COLUMN macname varchar(45) CHARACTER SET latin1 DEFAULT '';
ALTER TABLE `dy_player` ADD COLUMN platformid int default 0;
ALTER TABLE `dy_player` ADD COLUMN age int default 0;
ALTER TABLE `dy_player` ADD COLUMN signinmonth int default 0;
ALTER TABLE `dy_player` ADD COLUMN mobiletype int default 0;
ALTER TABLE `dy_player` ADD COLUMN isrobot int default 0;
ALTER TABLE `dy_player` ADD COLUMN exp int default 0;
ALTER TABLE `dy_player` ADD COLUMN level int default 0;
ALTER TABLE `dy_player` ADD COLUMN penulttime int default 0;
ALTER TABLE `dy_player` ADD COLUMN viplevel int default 0;
ALTER TABLE `dy_player` ADD COLUMN bank_jetton int default 0;
ALTER TABLE `dy_player` ADD COLUMN gold int default 0;
ALTER TABLE `dy_player` ADD COLUMN money int default 0;
ALTER TABLE `dy_player` ADD COLUMN caninvite int default 0;
ALTER TABLE `dy_player` ADD COLUMN lasttime int default 0;
ALTER TABLE `dy_player` ADD COLUMN idenstatus int default 0;
ALTER TABLE `dy_player` ADD COLUMN bindtype int default 0;
ALTER TABLE `dy_player` ADD COLUMN isban int default 0;
ALTER TABLE `dy_player` ADD COLUMN bsignin int default 0;
ALTER TABLE `dy_player` ADD COLUMN playtype int default 0;
ALTER TABLE `dy_player` ADD COLUMN agencystatus int default 0;
ALTER TABLE `dy_player` ADD COLUMN blacklist int default 0;
ALTER TABLE `dy_player` ADD COLUMN propertyid int default 0;
ALTER TABLE `dy_player` ADD COLUMN registration1 int default 0;
ALTER TABLE `dy_player` ADD COLUMN registration2 int default 0;
ALTER TABLE `dy_player` ADD COLUMN registration3 int default 0;
ALTER TABLE `dy_player` ADD COLUMN turntablenum int default 0;
ALTER TABLE `dy_player` ADD COLUMN todayshare int default 0;
