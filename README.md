# GestureTyping
Tsinghua University CS-HCI course team project : using leapmotion to implement a Gesture Typing Project.

### 组员注意

+ 我们不需要**严格遵循**软工课程的开发模式，也不需要繁琐的issue，但是请各位组员做好分支管理和merge request。
+ 每个文件请标明作者，如果没有特殊情况，请不要修改其他队员的文件以免冲突
+ 由于我们采用了C++ Lagacy的API而同时需要在 macOS(make) 和 windows(Visual Studio)上同时进行开发，我们的仓库需要同时维护Makefile和.sln文件，所以对于新的文件的提交，请修改**Master分支的README.md**以通知使用另外一种操作系统的开发者。
+ **现阶段(12月3日)之前没有也不会有严格的任务要求，请组员们在做软工造机网原的空隙量力而为，保障身体健康~~**

### C++ 开发注意事项

由于我们需要在Windows和Mac上同时进行开发和测试，所以需要遵守以下注意事项

+ 不要使用Visual Studio特有的函数和库，比如**scanf_s**，也不要使用Mac下Unix系统特有的函数库比如**asctime**等。
+ 为避免麻烦我们只开启C++11支持。请不要使用更新的特性。
+ 尽可能使用C++的函数而不是C的函数，比如输入输出使用**cin/cout** 而不是**scanf/printf**, 字符串使用**string**而不是**char \***
+ 尽可能采用类成员函数而不是单一的函数
+ 禁止在头文件中定义全局变量，尽可能不使用静态变量
+ 尽可能不使用指针，所有的类请做好析构

### 文件管理

请在这里列出提交的文件，作者和文件的用处，比如

|文件名|作者|文件描述|
|:----:|:-----:|:-----:|
|Sample.cpp|LeapMotion|一个LeapMotion的描述样例|

### 任务安排和组员分工
在11.12日进行讨论。
