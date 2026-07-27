#include <chrono>
#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;

class MonNoeudBavard : public rclcpp::Node
{
public:
    MonNoeudBavard() : Node("noeud_bavard")
    {
        timer_ = this->create_wall_timer(
            1s, std::bind(&MonNoeudBavard::direBonjour, this));

    RCLCPP_INFO(this->get_logger(), "Le Neoud Bavard est allume !");
    }
private:
    void direBonjour()
    {
        RCLCPP_INFO(this->get_logger(), "Bonjour Martin ! Je suis un Noeud ROS 2 vivant !");
    }

    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<MonNoeudBavard>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}