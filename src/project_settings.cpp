#include <ThunderAuto/project_settings.h>

void to_json(nlohmann::json& json, const ProjectSettings& settings) {
  json = nlohmann::json {
      {"field", settings.field},
      {"drive_ctrl", static_cast<std::size_t>(settings.drive_controller)},
      {"robot_length", settings.robot_length},
      {"robot_width", settings.robot_width}};
}

void from_json(const nlohmann::json& json, ProjectSettings& settings) {
  settings.field = json.at("field").get<Field>();
  settings.drive_controller =
      static_cast<DriveController>(json.at("drive_ctrl").get<std::size_t>());
  settings.robot_length = json.at("robot_length").get<float>();
  settings.robot_width = json.at("robot_width").get<float>();
}
