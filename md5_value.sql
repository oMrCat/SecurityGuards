/*
 Navicat Premium Data Transfer

 Source Server         : 15pb
 Source Server Type    : MySQL
 Source Server Version : 50732
 Source Host           : localhost:3306
 Source Schema         : md5_value

 Target Server Type    : MySQL
 Target Server Version : 50732
 File Encoding         : 65001

 Date: 10/10/2022 19:49:36
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for black_table
-- ----------------------------
DROP TABLE IF EXISTS `black_table`;
CREATE TABLE `black_table`  (
  `cpath` varchar(40) CHARACTER SET gb2312 COLLATE gb2312_chinese_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = gb2312 COLLATE = gb2312_chinese_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of black_table
-- ----------------------------
INSERT INTO `black_table` VALUES ('C:\\test\\virus\\病毒查杀测试.rar');
INSERT INTO `black_table` VALUES ('C:\\test\\virus\\新建文本文档.txt');
INSERT INTO `black_table` VALUES ('C:\\test\\virus\\背景色.txt');
INSERT INTO `black_table` VALUES ('C:\\test\\virus\\Button.bmp');

-- ----------------------------
-- Table structure for md5_v
-- ----------------------------
DROP TABLE IF EXISTS `md5_v`;
CREATE TABLE `md5_v`  (
  `num_md5` int(11) NULL DEFAULT NULL,
  `value_md5` varchar(40) CHARACTER SET gb2312 COLLATE gb2312_chinese_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = gb2312 COLLATE = gb2312_chinese_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of md5_v
-- ----------------------------
INSERT INTO `md5_v` VALUES (0, '6cf7a10834a486a4d23a81f2526c3646');
INSERT INTO `md5_v` VALUES (1, 'baaca87fe5ac99e0f1442b54e03056f4');
INSERT INTO `md5_v` VALUES (2, 'd41d8cd98f00b204e9800998ecf8427e');

SET FOREIGN_KEY_CHECKS = 1;
