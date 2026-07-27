#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "std_msgs/msg/float32.hpp"


class SafetyStopNode : public rclcpp::Node
{
public : 
    SafetyStopNode() : Node("safety_stop_node")
    {
        distance_subscriber_ = this->create_subscription<std_msgs::msg::Float32>("/distance_sensor", 10, std::bind(&SafetyStopNode::distanceCallback, this, std::placeholders::_1));
        cmd_vel_publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel_safe", 10);
        this->declare_parameter("distance_securite", 1.0);
    }
private:
    rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr distance_subscriber_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_publisher_;

    void distanceCallback(const std_msgs::msg::Float32::SharedPtr msg)
    {
        float dist = msg->data;
        double secu = this->get_parameter("distance_securite").as_double();

        geometry_msgs::msg::Twist cmd;
        if (dist<secu){
            cmd.linear.x = 0;
            RCLCPP_ERROR(this->get_logger(), "ARRET D'URGENCE ! Distance = %.2f m", dist);
        }
        else if(dist < 2.0 * secu){
            cmd.linear.x = 0.5;
            RCLCPP_WARN(this->get_logger(), "Prudence. Vitesse reduite à 0.5 m/s");
        }
        else {
            cmd.linear.x = 1.5;
            RCLCPP_INFO(this->get_logger(), "Voie libre. Vitesse normale : 1.5 m/s");
        };
        cmd_vel_publisher_->publish(cmd);
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<SafetyStopNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}