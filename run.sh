if [ -d "build" ] 
then
  echo "Removing build directory!"
  rm -r build
else 
  echo "hmm."
fi

mkdir build
cd build 
cmake ..
make
./simulation
