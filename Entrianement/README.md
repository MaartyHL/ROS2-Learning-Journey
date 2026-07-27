# 🎓 ROS 2 C++ Practice & Fundamentals Workshop

![ROS 2](https://img.shields.io/badge/ROS%202-Humble-blue?style=for-the-badge&logo=ros)
![C++](https://img.shields.io/badge/C++-17-00599C?style=for-the-badge&logo=c%2B%2B)
![CMake](https://img.shields.io/badge/CMake-3.8+-064F8C?style=for-the-badge&logo=cmake)

A structured collection of hands-on ROS 2 C++ training projects covering core middleware architecture, node lifecycles, real-time timers, dynamic parameters, publisher/subscriber patterns, telemetry monitoring, and Turtlesim integration.

---

## 📂 Curriculum & Project Breakdown

### 0️⃣ Project 0: Le Nœud Bavard (`mon_premier_noeud`)
- **Location**: `00_le_noeud_bavard/`
- **Core Concepts**: Minimal `rclcpp::Node` initialization, `create_wall_timer` (1s), `RCLCPP_INFO` logging, `rclcpp::spin()` event loop.

### 1️⃣ Project 1: Le Compteur & Paramètres (`mon_compteur`)
- **Location**: `01_noeud_compteur/`
- **Core Concepts**: State variable management (`compteur_`), dynamic ROS 2 parameter declaration (`declare_parameter`) and retrieval (`get_parameter`), live CLI parameter updates (`ros2 param set`).

### 2️⃣ Project 2: Le Chauffeur de Tortue (`turtle_driver`)
- **Location**: `02_chauffeur_de_tortue/`
- **Core Concepts**: ROS 2 Publisher creation (`create_publisher`), velocity commands using `geometry_msgs::msg::Twist` (`linear.x`, `angular.z`), live parameter control for linear/angular speeds.

### 3️⃣ Project 3: L'Observateur de Position (`turtle_subscriber`)
- **Location**: `03_observateur_pose/`
- **Core Concepts**: ROS 2 Subscriber creation (`create_subscription`), topic telemetry subscription (`/turtle1/pose`), message callbacks with `std::bind` and `std::placeholders::_1`, handling `turtlesim::msg::Pose`.

### 4️⃣ Project 4: Le Moniteur de Batterie (`mon_robot_batterie`)
- **Location**: `04_moniteur_batterie/`
- **Core Concepts**: Autonomous battery state management node, dynamic alert thresholds (`seuil_alerte`), conditional warning logs (`RCLCPP_WARN`), real-time status resetting.

---

## 🛠️ How to Build & Run Any Project

```bash
# Navigate to desired project folder (e.g., 04_moniteur_batterie)
cd 04_moniteur_batterie

# Build with colcon
colcon build

# Source environment
source install/setup.bash

# Run executable
ros2 run mon_robot_batterie battery_monitor
```

---

## 👤 Author

**Martin** — Autonomous Robotics Engineering Student at Polytech Nice Sophia.
- GitHub: [@MaartyHL](https://github.com/MaartyHL)
