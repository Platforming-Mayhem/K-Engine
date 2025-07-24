 #!/bin/bash
cmake -B bin/Debug
read -p "Press any key to resume ..."
cmake --build bin/Debug --target Editor
read -p "Press any key to resume ..."
