#include "gtest/gtest.h"
#include <api.h>

TEST(api, string) {
  wchar_t wstr[100];

  chat_to_wchar("1234567890", wstr);
  ASSERT_EQ(wstr, L"1234567890");
}


int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);	//让测试案例接受一系列参数
  RUN_ALL_TESTS();
  system("PAUSE");
  return 0;
}