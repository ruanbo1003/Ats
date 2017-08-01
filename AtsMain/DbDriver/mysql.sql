Create Database If Not Exists ctp;

Use ctp;

Create Table If Not Exists test(
	uid INT(11) Primary Key Auto_increment,
	name Varchar(64) Not Null Unique,
	passwd Varchar(256) Not Null,
	time INT(11) Not Null
)Engine=MyIsam Auto_increment=1 Default Charset=utf8;

Create Table If Not Exists Settlement(
	day Varchar(10) Primary Key,
	time INT(11) Not Null
)Engine=MyIsam Default Charset=utf8;
