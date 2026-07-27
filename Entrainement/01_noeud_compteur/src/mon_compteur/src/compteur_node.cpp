#include <chrono>
#include <string>
#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;

class MonCompteurNode : public rclcpp::Node
{
public:
    MonCompteurNode() : Node("noeud_compteur"), compteur_(0)
    {
        this->declare_parameter("titre", "Compteur de Martin");

        timer_ = this->create_wall_timer(1s, std::bind(&MonCompteurNode::incrementerCompteur, this));

        RCLCPP_INFO(this->get_logger(), "Le Noeud Compteur est demarre !");
    }
private:
    void incrementerCompteur()
    {
        compteur_++;

        std::string titre = this->get_parameter("titre").as_string();

        RCLCPP_INFO(this->get_logger(), "%s : %d", titre.c_str(), compteur_);
    }

    int compteur_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<MonCompteurNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}