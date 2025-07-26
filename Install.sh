 #!/bin/bash
cmake -B bin/Debug -D GLFW_BUILD_WAYLAND=0
read -p "Press any key to resume ..."
cmake --build bin/Debug --target Editor
read -p "Press any key to resume ..."
