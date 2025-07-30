 #!/bin/bash
cmake -B bin/Debug -DCMAKE_WARN_DEPRECATED=OFF
read -p "Press any key to resume ..."
cmake --build bin/Debug --target Editor
read -p "Press any key to resume ..."
