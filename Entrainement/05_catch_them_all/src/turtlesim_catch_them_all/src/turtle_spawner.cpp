#include <chrono>
#include <cmath>
#include <random>
#include <string>
#include <vector>

#include "rclcpp/rclcpp.hpp"
#include "turtlesim/srv/spawn.hpp"
#include "turtlesim/srv/kill.hpp"
#include "my_robot_interfaces/msg/turtle.hpp"
#include "my_robot_interfaces/msg/turtle_array.hpp"
#include "my_robot_interfaces/srv/catch_turtle.hpp"

using namespace std::chrono_literals;

class TurtleSpawnerNode : public rclcpp::Node {
public:
    TurtleSpawnerNode() : Node("turtle_spawner"), turtle_counter_(0) {
        // 1. Paramètres ROS 2
        this->declare_parameter("spawn_frequency",1.0);
        this->declare_parameter("turtle_name_prefix", "turtle");

        spawn_frequency_ = this->get_parameter("spawn_frequency").as_double();
        turtle_name_prefix_ = this->get_parameter("turtle_name_prefix").as_string();

        // 2. Publisher pour la liste des tortues
        alive_turtles_publisher_ = this->create_publisher<my_robot_interfaces::msg::TurtleArray>("alive_turtles", 10);

        // 3. Clients pour /spawn et /kill
        spawn_client_ = this->create_client<turtlesim::srv::Spawn>("spawn");
        kill_client_ = this->create_client<turtlesim::srv::Kill>("kill");
        catch_turtle_service_ = this->create_service<my_robot_interfaces::srv::CatchTurtle>("catch_turtle", std::bind(&TurtleSpawnerNode::callbackCatchTurtle, this, std::placeholders::_1, std::placeholders::_2));

        // 4. Timerpour gérer les tortues
        double timer_period = 1.0 / spawn_frequency_;
        spawn_timer_ = this->create_wall_timer(
            std::chrono::duration<double>(timer_period),
            std::bind(&TurtleSpawnerNode::spawnNewTurtle, this));
        
        RCLCPP_INFO(this->get_logger(), "Noeud Turtle Spawner démarré");
    }

private:
  void publishAliveTurtles()
  {
    my_robot_interfaces::msg::TurtleArray msg;
    msg.turtles = alive_turtles_;
    alive_turtles_publisher_->publish(msg);
  }

  void spawnNewTurtle() {
    turtle_counter_++;
    std::string turtle_name = turtle_name_prefix_ + std::to_string(turtle_counter_);

    // Générateur aléatoire C++
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist_x_y(1.0, 10.0);
    std::uniform_real_distribution<double> dist_theta(0.0, 2.0 * M_PI);

    double x = dist_x_y(gen);
    double y = dist_x_y(gen);
    double theta = dist_theta(gen);

    if (!spawn_client_->wait_for_service(1s)) {
      RCLCPP_WARN(this->get_logger(), "Le service /spawn n'est pas encore disponible...");
      return;
    }

    auto request = std::make_shared<turtlesim::srv::Spawn::Request>();
    request->x = x;
    request->y = y;
    request->theta = theta;
    request->name = turtle_name;

    // Appel asynchrone du service avec une fonction Lambda C++
    spawn_client_->async_send_request(
      request,
      [this, turtle_name, x, y, theta](rclcpp::Client<turtlesim::srv::Spawn>::SharedFuture future) {
        auto response = future.get();
        if (!response->name.empty()) {
          RCLCPP_INFO(this->get_logger(), "Nouvelle tortue générée : %s à (%.2f, %.2f)", turtle_name.c_str(), x, y);
          
          my_robot_interfaces::msg::Turtle new_turtle;
          new_turtle.name = turtle_name;
          new_turtle.x = x;
          new_turtle.y = y;
          new_turtle.theta = theta;

          alive_turtles_.push_back(new_turtle);
          publishAliveTurtles();
        }
      });    
  }
  void callbackCatchTurtle(
    const std::shared_ptr<my_robot_interfaces::srv::CatchTurtle::Request> request, std::shared_ptr<my_robot_interfaces::srv::CatchTurtle::Response> response)
    {
      std::string turtle_to_catch = request->name;

      if (!kill_client_->wait_for_service(1s)) {
        RCLCPP_WARN(this->get_logger(), "Le service /kill n'est pas disponible...");
        response->success = false;
        return;
      }

      auto kill_request = std::make_shared<turtlesim::srv::Kill::Request>();
      kill_request->name = turtle_to_catch;

      kill_client_->async_send_request(
        kill_request,
        [this, turtle_to_catch](rclcpp::Client<turtlesim::srv::Kill>::SharedFuture future) {
        (void)future;
        RCLCPP_INFO(this->get_logger(), "Tortue %s attrapée et éliminée !", turtle_to_catch.c_str());
        
        // Retirer la tortue de notre vecteur C++
        for (auto it = alive_turtles_.begin(); it != alive_turtles_.end(); ++it) {
          if (it->name == turtle_to_catch) {
            alive_turtles_.erase(it);
            break;
          }
        }
        publishAliveTurtles();
      });
    response->success = true;
  }

// --- Variables et paramètres ---
    double spawn_frequency_;
    std::string turtle_name_prefix_;
    int turtle_counter_;

    // --- Vecteur C++ dynamique pour stocker les tortues actives ---
    std::vector<my_robot_interfaces::msg::Turtle> alive_turtles_;

    // --- Composants ROS 2 (Pointeurs intelligents) ---
    rclcpp::Publisher<my_robot_interfaces::msg::TurtleArray>::SharedPtr alive_turtles_publisher_;
    rclcpp::Service<my_robot_interfaces::srv::CatchTurtle>::SharedPtr catch_turtle_service_;
    rclcpp::Client<turtlesim::srv::Spawn>::SharedPtr spawn_client_;
    rclcpp::Client<turtlesim::srv::Kill>::SharedPtr kill_client_;
    rclcpp::TimerBase::SharedPtr spawn_timer_;
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<TurtleSpawnerNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}