#include <project.h>
#include <pages/path_editor.h>
#include <pages/path_manager.h>
#include <pages/properties.h>

ProjectManager::ProjectManager() { }

ProjectManager::~ProjectManager() { }

void ProjectManager::new_project(ProjectSettings _settings) {
  project.settings = _settings;
  project.points = PathEditorPage::CurvePointTable({
    { 8.124f, 1.78f, 4.73853f, 4.73853f, 1.44372f, 1.70807f, 4.73853, false, true, false },
    { 4.0f,   1.5f,  2.0944f,  2.0944f,  2.0f,     2.0f,     2.0944,  false, false, true },
  });

  working_project = true;
  unsaved = false;

  PathEditorPage::get()->set_project(&project);
  PropertiesPage::get()->set_project(&project);

  save_project();
}

void ProjectManager::open_project(std::string path) {
  project.settings.path = path;

  std::string file_str;
  {
    std::ifstream file(path);
    if (!file) return;
    file_str = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
  }

  std::string::const_iterator file_iter = file_str.cbegin();

  auto count = [&]() -> std::ptrdiff_t {
    std::ptrdiff_t n = 0;
    while (file_iter != file_str.end() && *file_iter != '\n' && *file_iter != ',' && *file_iter != '}') {
      n++;
      file_iter++;
    }
    return n;
  };

  auto get_str = [&]() -> std::string {
    std::string::const_iterator start = file_iter;
    std::ptrdiff_t n = count();
    return std::string(start, start + n);
  };

  project.settings.field.img_type = static_cast<Field::ImageType>(std::stoi(get_str())); ++file_iter;
  if (project.settings.field.img_type == Field::ImageType::CUSTOM) {
    project.settings.field.img = std::filesystem::path(get_str());
  }
  else {
    project.settings.field.img = static_cast<Field::BuiltinImage>(std::stoi(get_str()));
  }
  ++file_iter;
  project.settings.field.min.x = std::stof(get_str()); ++file_iter;
  project.settings.field.min.y = std::stof(get_str()); ++file_iter;
  project.settings.field.max.x = std::stof(get_str()); ++file_iter;
  project.settings.field.max.y = std::stof(get_str()); ++file_iter;
  project.settings.drive_ctrl = static_cast<DriveController>(std::stoi(get_str())); ++file_iter;
  project.settings.max_accel = std::stof(get_str()); ++file_iter;
  project.settings.max_vel = std::stof(get_str()); ++file_iter;
  project.settings.robot_length = std::stof(get_str()); ++file_iter;
  project.settings.robot_width = std::stof(get_str()); ++file_iter;

  project.points.clear();
  while (file_iter != file_str.cend() && *file_iter == '{') {
      ++file_iter;
      PathEditorPage::CurvePoint point;
      point.px = std::stof(get_str()); ++file_iter;
      point.py = std::stof(get_str()); ++file_iter;
      point.h0 = std::stof(get_str()); ++file_iter;
      point.h1 = std::stof(get_str()); ++file_iter;
      point.w0 = std::stof(get_str()); ++file_iter;
      point.w1 = std::stof(get_str()); ++file_iter;
      point.rotation = std::stof(get_str()); ++file_iter;
      point.stop = static_cast<bool>(std::stoi(get_str())); ++file_iter;
      point.begin = static_cast<bool>(std::stoi(get_str())); ++file_iter;
      point.end = static_cast<bool>(std::stoi(get_str())); ++file_iter;

      project.points.push_back(point);
  }

  PathEditorPage::get()->set_project(&project);
  PropertiesPage::get()->set_project(&project);
  working_project = true;
}

void ProjectManager::save_project() {
  const ProjectSettings& settings = project.settings;
  const Field& field = settings.field;

  std::ofstream file(project.settings.path);

  file << static_cast<std::size_t>(settings.field.img_type) << ',';
  if (field.img_type == Field::ImageType::CUSTOM) {
    file << std::get<std::filesystem::path>(field.img).c_str() << ',';
  }
  else {
    file << static_cast<std::size_t>(std::get<Field::BuiltinImage>(field.img)) << ',';
  }
  file << field.min.x << ',' << field.min.y << ',' << field.max.x << ',' << field.max.y << '\n';
  file << static_cast<std::size_t>(settings.drive_ctrl) << ',';
  file << settings.max_accel << ',';
  file << settings.max_vel << '\n';
  file << settings.robot_length << ',';
  file << settings.robot_width << '\n';
  
  for (const PathEditorPage::CurvePoint& pt : project.points) {
    file << '{'
         << pt.px << ','
         << pt.py << ','
         << pt.h0 << ','
         << pt.h1 << ','
         << pt.w0 << ','
         << pt.w1 << ','
         << pt.rotation << ','
         << static_cast<std::size_t>(pt.stop) << ','
         << static_cast<std::size_t>(pt.begin) << ','
         << static_cast<std::size_t>(pt.end) << '}';
  }
  file << '\n';
}

void ProjectManager::save_project_as(std::string path) {
  project.settings.path = path;
  save_project();
}

void ProjectManager::close_project() {
  working_project = false;
}

ProjectManager ProjectManager::instance {};
