一、basicwork\_ted

ted\_8\_node.cpp 主要是这个Ted8Node类，定义了角速度，线速度，半径（计算出），周期（计算出），为了在转动一圈后改变方向，还有起始时间和定时器，采用三元运算符判断方向更改，判断具体转动的周期奇偶性来改变方向

ted\_8.yaml 用来存储线速度和角速度这两个变量，目的是修改后不用重新编译  不过用这个文件必须在cpp源文件中跟线速度和角速度获取上用get\_parameter（）

ted\_8\_launch.py  用来启动乌龟和8字节点，方便run,我是采用python文件编写的 中间的generate\_launch\_description函数首先拼出yaml配置的文件路径，然后找到海龟的包名和可执行文件名（先有的海龟），最后是8字节点的包名加上yaml路径，最后LaunchDescription()启动   运行时编译后刷新环境变量就可以launch了

教程看的是赵虚左的gitbook,中间传递给海龟的消息类型是询问ai（这一部分确实是对文件包的不了解），还有其他编译上的问题也是，就像回调函数的使用上就出现的问题进行询问，yaml配置比较简单，设置了一个config文件夹存放。launch也设置了文件夹存放。

colcon build --packages-select ted\_8

source install/setup.bash

ros2 launch ted\_8 ted\_8\_launch.py

二、advancework

对于这个作业我确实是不太会，询问ai去了解了思路和编写逻辑，就是先让bag播放话题，自己需要去按照模板设置节点订阅话题，收到消息后累加存储，然后间隔时长去发布，调用rviz2去显示锥桶。根据模板来编写，不过需要加上订阅方，后面设置每个锥桶的RGB格式，未知设为0.5，0.5，0.5，

代码启动，先cd \~/advancedwork

colcon build --packages-select fsd\_common\_msgs

colcon build --packages-select cone\_visualizer   //编译包和可视化节点

source install/setup.bash

然后ros2 bag play src/ros2\_homework\_advanced/map\_to\_visualize/map\_to\_visualize\_0.db3 -l  让bag播放

启动ros2 run cone\_visualizer cone\_visualizer  可视化节点（新终端）

最后rviz2(新终端)



