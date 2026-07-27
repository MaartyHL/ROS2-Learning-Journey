# 🎓 ROS 2 C++ Master Learning Journey & Robotics Projects

![ROS 2](https://img.shields.io/badge/ROS%202-Humble-blue?style=for-the-badge&logo=ros)
![C++](https://img.shields.io/badge/C++-17-00599C?style=for-the-badge&logo=c%2B%2B)
![CMake](https://img.shields.io/badge/CMake-3.8+-064F8C?style=for-the-badge&logo=cmake)
![Linux](https://img.shields.io/badge/Ubuntu-22.04-E95420?style=for-the-badge&logo=ubuntu)

A complete, structured curriculum and repository documenting my journey mastering **ROS 2 in C++ (`rclcpp`)**. The repository transitions from absolute foundational concepts to advanced autonomous robotics control systems.

---

## 📂 Repository Structure & Project Roadmap

```
ROS2-Learning-Journey/
│
├── 📜 README.md                               <── Master Summary & Roadmap
├── 📜 .gitignore                              <── Build artifacts filter
│
└── 🎓 Entrainement/                           <── Core Middleware Fundamentals & Advanced Systems
    ├── 00_le_noeud_bavard/                    <── Minimal Node & Timers
    ├── 01_noeud_compteur/                     <── State & Dynamic Parameters
    ├── 02_chauffeur_de_tortue/                <── Publishers & Twist Commands
    ├── 03_observateur_pose/                   <── Subscribers & Telemetry
    ├── 04_moniteur_batterie/                  <── Real-Time Battery Monitor
    └── 🐢 05_catch_them_all/                  <── Advanced Multi-Node System (Capstone)
        ├── src/my_robot_interfaces/           <── Custom .msg and .srv
        └── src/turtlesim_catch_them_all/      <── Autonomous P-Controller
```

---

## 🛠️ Project Summaries

- **00 Nœud Bavard**: Node lifecycle, 1s `create_wall_timer`, `RCLCPP_INFO` logging.
- **01 Nœud Compteur**: Internal state mutation, dynamic ROS 2 parameters (`declare_parameter`), runtime modification (`ros2 param set`).
- **02 Chauffeur de Tortue**: `cmd_vel` velocity publisher (`geometry_msgs/msg/Twist`), dual parameter linear/angular speed tuning.
- **03 Observateur de Pose**: Topic subscription (`/turtle1/pose`), telemetry callbacks, accessing `turtlesim::msg::Pose` pointer fields.
- **04 Moniteur de Batterie**: Real-time state thresholding node with dynamic alert levels (`seuil_alerte`) and `RCLCPP_WARN` logging.
- **05 Catch Them All**: Closed-loop P-controller autonomous hunter, random target spawner, and custom ROS 2 message/service interfaces (`Turtle.msg`, `TurtleArray.msg`, `CatchTurtle.srv`).

---

## 👤 Author

**Martin** — Autonomous Robotics Engineering Student at Polytech Nice Sophia.
- GitHub: [@MaartyHL](https://github.com/MaartyHL)
