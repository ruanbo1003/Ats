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

Create Table If Not Exists TickData(
	tradeday Varchar(16),
	updateTime Varchar(16),
	updateMs INT(11),
	InstrumentId Varchar(8),
	lastPrice Double,
	preSettlePrice Double,
	preClosePrice Double,
	preOpenInterest Double,
	openPrice Double,
	highestPrice Double,
	lowestPrice Double,
	volume Int,
	turnover Double,
	openInterest Double,
	closePrice Double,
	settlePrice Double,
	upperLimitPrice Double,
	lowerLimitPrice Double,
	preDelta Double,
	currDelta Double,
	bidP1 Double,
	bitV1 Int,
	askP1 Double,
	askV1 Int,
	bidP2 Double,
	bitV2 Int,
	askP2 Double,
	askV2 Int,
	bidP3 Double,
	bitV3 Int,
	askP3 Double,
	askV3 Int,
	bidP4 Double,
	bitV4 Int,
	askP4 Double,
	askV4 Int,
	bidP5 Double,
	bitV5 Int,
	askP5 Double,
	askV5 Int
)Engine=MyIsam Default Charset=utf8;