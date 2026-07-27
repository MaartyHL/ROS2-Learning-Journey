#include <chrono>
#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;

class BatteryMonitorNode : public rclcpp::Node
{
public:
    BatteryMonitorNode() : Node("Battery_monitor"), niveau_batterie_(100)
    {
        this->declare_parameter("seuil_alerte", 20);

        timer_ = this->create_wall_timer(1s, std::bind(&BatteryMonitorNode::usage_batterie, this));

        RCLCPP_INFO(this->get_logger(), "Le moniteur de batterie est actif !");
    }

private:
    void usage_batterie()
    {
        niveau_batterie_-=2;
        int seuil = this->get_parameter("seuil_alerte").as_int();
        if(niveau_batterie_>seuil){
            RCLCPP_INFO(this->get_logger(), "La batterie est chargée à %d %% ! ", niveau_batterie_);
        }
        else{
            RCLCPP_WARN(this->get_logger(), "La batterie est chargée à %d %% ! ", niveau_batterie_);
        }
        if (niveau_batterie_<=0){niveau_batterie_=100;}
    }

    rclcpp::TimerBase::SharedPtr timer_;

    int niveau_batterie_;

};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<BatteryMonitorNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}