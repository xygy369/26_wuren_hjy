#include "rclcpp/rclcpp.hpp"
#include "visualization_msgs/msg/marker.hpp"   //接受marker数据
#include "fsd_common_msgs/msg/map.hpp"   //接受锥桶数据
#include <chrono>
#include <memory>
#include <vector>

using namespace std::chrono_literals;

class ConeVisualizer:public rclcpp::Node
{
public:
    ConeVisualizer():Node("cone_visualizer")
    {
        //发布Marker
        marker_pub_ = this->create_publisher<visualization_msgs::msg::Marker>("visualization_marker", 10);
        //订阅
        map_sub_ = this->create_subscription<fsd_common_msgs::msg::Map>("/estimation/slam/map", 10, std::bind(&ConeVisualizer::map_callback, this, std::placeholders::_1));
        timer_ = this->create_wall_timer(500ms,std::bind(&ConeVisualizer::publishMarker, this));
    }

private:
    void map_callback(const fsd_common_msgs::msg::Map::SharedPtr msg)
    {
        cone_red = msg->cone_red;       
        cone_blue = msg->cone_blue;
        cone_yellow = msg->cone_yellow;
        cone_unknown = msg->cone_unknown;
    }
    void publishMarker()
    {
        int id = 0;  // 用来给每个 Marker 编号
        // 遍历红色锥桶
        for (auto& cone : cone_red)
        {
            visualization_msgs::msg::Marker marker;        
            marker.header.frame_id = "map";        
            marker.header.stamp = this->now();        
            marker.ns = "cones";        
            marker.id = id;          //命名空间和id
            marker.type = visualization_msgs::msg::Marker::SPHERE;         //类型
            marker.action = visualization_msgs::msg::Marker::ADD;        //添加
            marker.pose.position.x = cone.position.x;   // 锥桶的 x        
            marker.pose.position.y = cone.position.y;   // 锥桶的 y        
            marker.pose.position.z = 0.5;        
            marker.scale.x = 0.5;        
            marker.scale.y = 0.5;        
            marker.scale.z = 0.5;        
            marker.color.r = 1.0f;   // 红色        
            marker.color.g = 0.0f;        
            marker.color.b = 0.0f;        
            marker.color.a = 1.0f;           //透明度
            marker.lifetime = rclcpp::Duration::from_seconds(0.0);
            marker_pub_->publish(marker);
            id++;
        }
        // 遍历蓝色锥桶
        for (auto& cone : cone_blue)
        {
            visualization_msgs::msg::Marker marker;        
            marker.header.frame_id = "map";        
            marker.header.stamp = this->now();        
            marker.ns = "cones";        
            marker.id = id;        
            marker.type = visualization_msgs::msg::Marker::SPHERE;        
            marker.action = visualization_msgs::msg::Marker::ADD;        
            marker.pose.position.x = cone.position.x;   // 锥桶的 x        
            marker.pose.position.y = cone.position.y;   // 锥桶的 y        
            marker.pose.position.z = 0.5;        
            marker.scale.x = 0.5;        
            marker.scale.y = 0.5;        
            marker.scale.z = 0.5;        
            marker.color.r = 0.0f;   // 蓝色        
            marker.color.g = 0.0f;        
            marker.color.b = 1.0f;        
            marker.color.a = 1.0f;          //透明度
            marker.lifetime = rclcpp::Duration::from_seconds(0.0);
            marker_pub_->publish(marker);
            id++;
        }
        // 遍历黄色锥桶
        for (auto& cone : cone_yellow)
        {
            visualization_msgs::msg::Marker marker;        
            marker.header.frame_id = "map";        
            marker.header.stamp = this->now();        
            marker.ns = "cones";        
            marker.id = id;        
            marker.type = visualization_msgs::msg::Marker::SPHERE;        
            marker.action = visualization_msgs::msg::Marker::ADD;        
            marker.pose.position.x = cone.position.x;   // 锥桶的 x        
            marker.pose.position.y = cone.position.y;   // 锥桶的 y        
            marker.pose.position.z = 0.5;        
            marker.scale.x = 0.5;        
            marker.scale.y = 0.5;        
            marker.scale.z = 0.5;        
            marker.color.r = 1.0f;           
            marker.color.g = 1.0f;        
            marker.color.b = 0.0f;        
            marker.color.a = 1.0f;          //透明度
            marker.lifetime = rclcpp::Duration::from_seconds(0.0);
            marker_pub_->publish(marker);
            id++;
        }
        // 遍历未知颜色锥桶
        for (auto& cone : cone_unknown)      //未知
        {
            visualization_msgs::msg::Marker marker;        
            marker.header.frame_id = "map";        
            marker.header.stamp = this->now();        
            marker.ns = "cones";        
            marker.id = id;        
            marker.type = visualization_msgs::msg::Marker::SPHERE;        
            marker.action = visualization_msgs::msg::Marker::ADD;        
            marker.pose.position.x = cone.position.x;   // 锥桶的 x        
            marker.pose.position.y = cone.position.y;   // 锥桶的 y        
            marker.pose.position.z = 0.5;        
            marker.scale.x = 0.5;        
            marker.scale.y = 0.5;        
            marker.scale.z = 0.5;        
            marker.color.r = 0.5f;          
            marker.color.g = 0.5f;        
            marker.color.b = 0.5f;        
            marker.color.a = 1.0f;          //透明度
            marker.lifetime = rclcpp::Duration::from_seconds(0.0);
            marker_pub_->publish(marker);
            id++;
        }
    }
    rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr marker_pub_;
    rclcpp::Subscription<fsd_common_msgs::msg::Map>::SharedPtr map_sub_;
    rclcpp::TimerBase::SharedPtr timer_;   
    std::vector<fsd_common_msgs::msg::Cone> cone_red, cone_blue, cone_yellow, cone_unknown;  
};


int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);    
    auto node = std::make_shared<ConeVisualizer>();
    
    rclcpp::spin(node);
    rclcpp::shutdown();    

    return 0;
}
