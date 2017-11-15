#!/bin/sh

if [[ "$(docker images -q device_sim-build:1 2> /dev/null)" == "" ]]; then
  docker build -t device_sim-build:1 -f Dockerfile.build .
fi

BUILD_DIR=_b$RANDOM
mkdir -p $BUILD_DIR
docker run --rm -v $(pwd):/app -w /app_build device_sim-build:1 /bin/sh -c "cmake -Duse_amqp=OFF -Duse_http=OFF -Ddont_use_uploadtoblob=ON -Duse_condition=ON -Duse_wsio=OFF -Duse_cppunittest=OFF /app && cmake --build . --target device_sim && cp device_sim /app/$BUILD_DIR/"
docker build --build-arg APP_DIR=$BUILD_DIR -t $1.azurecr.io/device_sim-run:$2 -f Dockerfile.run .
rm -rf $BUILD_DIR