#include "gtest/gtest.h"

#include "bvh-parser.h"
#include "config.h"
#include "easylogging++.h"

#include <boost/filesystem.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace bf = boost::filesystem;

INITIALIZE_EASYLOGGINGPP

int main(int argc, char **argv) {
  // Load configuration from file
  el::Configurations conf(EASYLOGGING_CONFIG_FILE_PATH);
  // Reconfigure single logger
  el::Loggers::reconfigureLogger("default", conf);
  el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(ExampleFileTest, ParseTest) {
  bvh::Bvh_parser parser;
  bvh::Bvh data;
  bf::path sample_path = bf::path(TEST_BVH_FILES_PATH) / "example.bvh";
  ASSERT_EQ(0, parser.parse(sample_path, &data));
  ASSERT_NE(nullptr, data.root_joint());
  ASSERT_STREQ("Hips", data.root_joint()->name().c_str());

  std::vector<std::vector<float>> expected_root_joint_data =
      {{8.03, 35.01,88.36, -3.41, 14.78, -164.35},
      {7.81, 35.10, 86.47, -3.78, 12.94, -166.97}};
  ASSERT_EQ(expected_root_joint_data, data.root_joint()->channel_data());
}

TEST(ExampleFileTest, MotionCalculationTest) {
  bvh::Bvh_parser parser;
  bvh::Bvh data;
  bf::path sample_path = bf::path(TEST_BVH_FILES_PATH) / "example.bvh";
  ASSERT_EQ(0, parser.parse(sample_path, &data));
  data.recalculate_joints_ltm();

  glm::vec3 expected_position_01(-17.786824, -46.734459, 81.223086);
  for(int i = 0; i < data.joints().size(); i++)
    std::cout << "NAME: " << data.joints()[i]->name() << ", x = " << data.joints()[i]->joint_position(0).x << ",y = " <<   data.joints()[i]->joint_position(0).y << ", z = " << data.joints()[i]->joint_position(0).z << "\n";
//   ASSERT_EQ(data.root_joint()->joint_position(0), expected_position_01);
}

TEST(GlmTest, RotationTest) {
  glm::mat4 matrix(1.0);
}
