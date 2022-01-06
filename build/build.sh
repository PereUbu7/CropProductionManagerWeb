# standard library
echo "Building standard library"
g++ -std=c++20 -fmodules-ts -xc++-system-header iostream
g++ -std=c++20 -fmodules-ts -xc++-system-header string
g++ -std=c++20 -fmodules-ts -xc++-system-header vector
g++ -std=c++20 -fmodules-ts -xc++-system-header algorithm
g++ -std=c++20 -fmodules-ts -xc++-system-header concepts

echo "Building external libraries"
# clang++-12 ../extern/json.hpp -std=c++20 -c -o json.pcm -O3 -Xclang -emit-module-interface
clang++-12 -cc1 ../extern/json.hpp -std=c++20 -emit-pch -o json.pch
# g++ -c -std=c++20 -x c++-header -fmodules-ts ../extern/json.hpp
# g++ -c -std=c++11 -x c++-header -fmodules-ts ../extern/json.hpp

echo "Building Concepts"
g++ -c -std=c++20 -fmodules-ts ../src/Concept/hasId.cpp
echo "Building Infrastructure models"
g++ -c -std=c++20 -fmodules-ts ../src/InternalModel/Infrastructure/crop.cpp
echo "Building Infrastructure repositories"
g++ -c -std=c++20 -fmodules-ts ../src/Infrastructure/iRepository.cpp
g++ -c -std=c++20 -fmodules-ts ../src/Infrastructure/cropRepository.cpp
g++ -c -std=c++20 -fmodules-ts ../src/Infrastructure/repositoryFake.cpp

echo "Building API models"
g++ -c -std=c++20 -fmodules-ts ../src/ModelApi/crop.cpp
echo "Building Core models"
g++ -c -std=c++20 -fmodules-ts ../src/InternalModel/Core/crop.cpp
echo "Building Core interfaces"
g++ -c -std=c++20 -fmodules-ts ../src/Core/iCore.cpp
echo "Building Core implementations"
g++ -c -std=c++20 -fmodules-ts ../src/Core/cropCore.cpp

echo "Building API interfaces"
g++ -c -std=c++20 -fmodules-ts ../src/Api/iApi.cpp
echo "Building API implementations"
g++ -c -std=c++20 -fmodules-ts ../src/Api/cropApi.cpp

echo "Building Serializers"
g++ -c -std=c++20 -fmodules-ts ../src/Serializer/serializer.cpp
g++ -c -std=c++20 -fmodules-ts ../src/Serializer/crop.cpp

echo "Building Application"
g++ -c -std=c++20 -fmodules-ts ../apps/Program.cpp