#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

class Rectangle {
    int width, height;
  public:
    void set_values (int,int);
} *rect;

void Rectangle::set_values (int x, int y) {
  width = x;
  height = y;
}

int example_constructor_check_wrong_return_code() {
  rect = new Rectangle();
  if (rect == NULL) {
      printf("never printed");
      return 0xbad;
  };
  rect->set_values(42, 23);
}

int example_control_may_reach_end_of_non_void_function() {
  rect = new Rectangle();
  rect->set_values(42, 23);
}

int main( int argc, const char* argv[] )
{
    printf("example_constructor_check_wrong_return_code() = %x\n", example_constructor_check_wrong_return_code());
    printf("example_control_may_reach_end_of_non_void_function() = %x\n", example_control_may_reach_end_of_non_void_function());
}

#ifdef __cplusplus
}
#endif

