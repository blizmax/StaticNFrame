SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for dy_server
-- ----------------------------
DROP TABLE IF EXISTS `dy_server`;
CREATE TABLE `dy_server` (
  `server_id` int(11) NOT NULL,
  `server_type` int(11) DEFAULT '0',
  `server_name` varchar(127) DEFAULT '',
  `server_ip` varchar(127) DEFAULT '',
  `server_port` int(11) DEFAULT '0',
  `server_http_port` int(11) DEFAULT '0',
  `server_max_online` int(11) DEFAULT '0',
  `server_cur_online` int(11) DEFAULT '0',
  `server_state` int(11) DEFAULT '0',
  `proc_cpu` double(10,2) DEFAULT '0',
  `proc_mem` bigint(20) DEFAULT '0',
  `proc_thread` int(11) DEFAULT '0',
  `proc_pid` int(11) DEFAULT '0',
  `system_info` varchar(127) DEFAULT '',
  `total_mem` bigint(20) DEFAULT '0',
  `free_mem` bigint(20) DEFAULT '0',
  `used_mem` bigint(20) DEFAULT '0',
  `proc_name` varchar(127) DEFAULT '',
  `proc_cwd` varchar(127) DEFAULT '',
  `last_date` varchar(127) DEFAULT '',
  PRIMARY KEY (`server_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for du_server
-- ----------------------------
DROP TABLE IF EXISTS `dy_server_detail`;
CREATE TABLE `dy_server_detail` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `server_id` int(11) NOT NULL,
  `server_cur_online` int(11) DEFAULT '0',
  `server_state` int(11) DEFAULT '0',
  `proc_cpu` double(10,2) DEFAULT '0',
  `proc_mem` bigint(20) DEFAULT '0',
  `proc_thread` int(11) DEFAULT '0',
  `total_mem` bigint(20) DEFAULT '0',
  `free_mem` bigint(20) DEFAULT '0',
  `used_mem` bigint(20) DEFAULT '0',
  `last_date` varchar(127) DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;