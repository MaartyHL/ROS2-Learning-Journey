#include "rclcpp/rclcpp.hpp"
#include "turtlesim/msg/pose.hpp"

class PoseSubscriberNode : public rclcpp::Node
{
public:
    PoseSubscriberNode() : Node("pose_subscriber")
    {
        pose_subscriber_ = this->create_subscription<turtlesim::msg::Pose>("/turtle1/pose", 10, std::bind(&PoseSubscriberNode::poseCallBack, this, std::placeholders::_1));

        RCLCPP_INFO(this->get_logger(), "L'observateur de Position est allume !");
    }

private:
    void poseCallBack (const turtlesim::msg::Pose::SharedPtr pose)
    {
        RCLCPP_INFO(this->get_logger(), "Position Turtle : x = %.2f, y = %.2f", pose->x, pose->y);
    }
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_subscriber_;
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<PoseSubscriberNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}