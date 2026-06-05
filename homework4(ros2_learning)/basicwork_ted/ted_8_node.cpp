#include <chrono>   //时间   精度高比<ctime>
#include <cmath>    //sin  M_PI
#include "rclcpp/rclcpp.hpp"    //service     request      wall_timer   make_shared
#include "geometry_msgs/msg/twist.hpp"     //  速度信息包
#include "turtlesim/srv/teleport_absolute.hpp"  //turtlesim传送  请求响应

class Ted8Node : public rclcpp::Node
{
public:
    Ted8Node():Node("ted_8_node")    //节点名
    {
        this->declare_parameter("line_v", 2.0);  //线速度  设置默认值
        this->get_parameter("line_v", line_v);   //从yaml中读参数
        this->declare_parameter("angel_v", 2.0);  //线速度
        this->get_parameter("angel_v", angel_v);

        radius = line_v / angel_v;    //半径
        circle_time = 2 * M_PI / angel_v;    //画一个圆的时间  周期

        //发布方：向乌龟传递速度话题  还未发布
        publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);

        //客户端 接收响应   等待发布
        teleport_client_ = this->create_client<turtlesim::srv::TeleportAbsolute>("/turtle1/teleport_absolute");
        teleport_client_->wait_for_service();    //服务准备

        // 服务  请求
        auto req = std::make_shared<turtlesim::srv::TeleportAbsolute::Request>();
        req->x = 5.5 ;
        req->y = 5.5;
        req->theta = M_PI / 2;   //起始朝向   这三行都定义在request上
        teleport_client_->async_send_request(req);   //发送请求

        start_time = this->now(); //当前时间

        //100ms启动一次回调函数
        timer = this->create_wall_timer(std::chrono::milliseconds(100),std::bind(&Ted8Node::timer_callback, this));
    }

private:
    void timer_callback()
    {
        double t = (this->now() - start_time).seconds();   //经过的时间
        int half = static_cast<int>(floor(t / circle_time)); //周期    取整

        auto msg = geometry_msgs::msg::Twist();   //速度消息对象
        msg.linear.x = line_v;
        msg.angular.z = (half % 2 == 0) ? angel_v : -angel_v;  //判断方向  用周期判断方向
        publisher_->publish(msg);
    }

    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;   //发布方
    rclcpp::Client<turtlesim::srv::TeleportAbsolute>::SharedPtr teleport_client_;  //传送客户端
    rclcpp::TimerBase::SharedPtr timer;   //定时器
    rclcpp::Time start_time;    //起始时间
    double line_v, angel_v, radius, circle_time;    //速度  半径   周期 
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);  //初始化
    auto node = std::make_shared<Ted8Node>();  //创建节点对象
    rclcpp::spin(node);   //运行
    rclcpp::shutdown();   //关闭
    return 0;
}