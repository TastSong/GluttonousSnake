# GluttonousSnake

## 功能

  本项目的主要功能为实现贪吃蛇小游戏，首先进行利用光标的定位来进行一系列的数据初始化，
然后利用键盘上的按键来控制小蛇的移动，在此过程中，小蛇吃到食物后身体会变长，
当它在移动的过程中撞到游戏边界或者咬到自己就结束游戏，并且得到一个食物就会积累相应得分。

## 模块

* Function List: void Pos();//设置光标位置
* void CreatInterface();//创建游戏界面
* void InitializeSnake();//初始化蛇的身体
* int BiteItself(); //判断是否咬到自己
* void CreateFood();//创建食物
* void CantcrossWall();//撞墙
* void SnakeMove();//蛇移动
* void Pause();//暂停游戏
* void GameCircle();//游戏运行
* void WelcomeToGame();//欢迎来到游戏的显示界面
* void EndGame();//游戏结束
* void GameStart();//游戏开始
