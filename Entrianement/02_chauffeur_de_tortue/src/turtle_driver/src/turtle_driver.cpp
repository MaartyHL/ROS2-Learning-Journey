#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

using namespace std::chrono_literals;

class TurtleDriverNode : public rclcpp::Node
{
public:
    TurtleDriverNode() : Node("chauffeur_tortue")
    {
        this->declare_parameter("vitesse_lineaire", 2.0);
        this->declare_parameter("vitesse_angulaire", 1.0);

        cmd_vel_publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);

        timer_ = this->create_wall_timer(100ms, std::bind(&TurtleDriverNode::publishVelocity, this));

        RCLCPP_INFO(this->get_logger(), "Le chuaffeur de Tortue est allumé !");
    }
private :
    void publishVelocity()
    {
        double v_lin = this->get_parameter("vitesse_lineaire").as_double();
        double v_ang = this->get_parameter("vitesse_angulaire").as_double();

        geometry_msgs::msg::Twist msg;
        msg.linear.x = v_lin;
        msg.angular.z = v_ang;

        cmd_vel_publisher_->publish(msg);
    }
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<TurtleDriverNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}