#include <chrono>
#include <cmath>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "turtlesim/msg/pose.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "my_robot_interfaces/msg/turtle.hpp"
#include "my_robot_interfaces/msg/turtle_array.hpp"
#include "my_robot_interfaces/srv/catch_turtle.hpp"

using namespace std::chrono_literals;

class TurtleControllerNode : public rclcpp::Node
{
public:
  TurtleControllerNode() : Node("turtle_controller"), pose_received_(false), target_exists_(false)
  {
    // 1. Paramètres des gains du contrôleur P
    this->declare_parameter("Kp_linear", 2.0);
    this->declare_parameter("Kp_angular", 6.0);

    Kp_linear_ = this->get_parameter("Kp_linear").as_double();
    Kp_angular_ = this->get_parameter("Kp_angular").as_double();

    // 2. Subscriber pour la position de notre chasseur
    pose_subscriber_ = this->create_subscription<turtlesim::msg::Pose>(
      "/turtle1/pose", 10,
      std::bind(&TurtleControllerNode::callbackPose, this, std::placeholders::_1));

    // 3. Subscriber pour la liste des tortues cibles vivantes
    alive_turtles_subscriber_ = this->create_subscription<my_robot_interfaces::msg::TurtleArray>(
      "/alive_turtles", 10,
      std::bind(&TurtleControllerNode::callbackAliveTurtles, this, std::placeholders::_1));

    // 4. Publisher de vitesse
    cmd_vel_publisher_ = this->create_publisher<geometry_msgs::msg::Twist>(
      "/turtle1/cmd_vel", 10);

    // 5. Client du service catch_turtle
    catch_turtle_client_ = this->create_client<my_robot_interfaces::srv::CatchTurtle>("catch_turtle");

    // 6. Timer de la boucle de contrôle temps réel (10ms = 100 Hz)
    control_loop_timer_ = this->create_wall_timer(
      10ms, std::bind(&TurtleControllerNode::controlLoop, this));

    RCLCPP_INFO(this->get_logger(), "Le Controleur Chasseur est demarre !");
  }
private:

  void callbackPose(const turtlesim::msg::Pose::SharedPtr pose)
  {
    pose_ = *pose;
    pose_received_ = true;
  }

  void callbackAliveTurtles(const my_robot_interfaces::msg::TurtleArray::SharedPtr msg)
  {
    if (msg->turtles.empty()) {
      target_exists_ = false;
      return;
    }

    // Recherche de la tortue la plus proche
    double min_distance = 1e9;
    my_robot_interfaces::msg::Turtle closest_turtle;

    for (const auto &turtle : msg->turtles) {
      double dx = turtle.x - pose_.x;
      double dy = turtle.y - pose_.y;
      double distance = std::sqrt(dx * dx + dy * dy);

      if (distance < min_distance) {
        min_distance = distance;
        closest_turtle = turtle;
      }
    }

    target_turtle_ = closest_turtle;
    target_exists_ = true;
  }
    void controlLoop()
  {
    if (!pose_received_ || !target_exists_) {
      return;
    }

    double dx = target_turtle_.x - pose_.x;
    double dy = target_turtle_.y - pose_.y;
    double distance = std::sqrt(dx * dx + dy * dy);

    geometry_msgs::msg::Twist msg;

    if (distance < 0.5) {
      // Tortue attrapée ! On s'arrête et on appelle le service
      msg.linear.x = 0.0;
      msg.angular.z = 0.0;
      cmd_vel_publisher_->publish(msg);
      callCatchTurtleService(target_turtle_.name);
    } else {
      // Contrôleur P pour la vitesse linéaire
      msg.linear.x = Kp_linear_ * distance;

      // Calcul du cap désiré avec atan2
      double target_theta = std::atan2(dy, dx);
      double diff_theta = target_theta - pose_.theta;

      // Normalisation de l'angle entre -PI et +PI
      while (diff_theta > M_PI) diff_theta -= 2 * M_PI;
      while (diff_theta < -M_PI) diff_theta += 2 * M_PI;

      // Contrôleur P pour la vitesse angulaire
      msg.angular.z = Kp_angular_ * diff_theta;

      cmd_vel_publisher_->publish(msg);
    }
  }

  void callCatchTurtleService(const std::string &turtle_name)
  {
    if (!catch_turtle_client_->wait_for_service(1s)) {
      return;
    }

    auto request = std::make_shared<my_robot_interfaces::srv::CatchTurtle::Request>();
    request->name = turtle_name;

    catch_turtle_client_->async_send_request(request);
  }

  turtlesim::msg::Pose pose_;
  bool pose_received_;
  my_robot_interfaces::msg::Turtle target_turtle_;
  bool target_exists_;

  double Kp_linear_;
  double Kp_angular_;

  rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_subscriber_;
  rclcpp::Subscription<my_robot_interfaces::msg::TurtleArray>::SharedPtr alive_turtles_subscriber_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_publisher_;
  rclcpp::Client<my_robot_interfaces::srv::CatchTurtle>::SharedPtr catch_turtle_client_;
  rclcpp::TimerBase::SharedPtr control_loop_timer_;

};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<TurtleControllerNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}