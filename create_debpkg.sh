mkdir -p ./deb_pakage
mkdir -p ./deb_pakage/bin
mkdir -p ./deb_pakage/DEBIAN
touch  ./deb_pakage/DEBIAN/control

cat << EOF > ./deb_pakage/DEBIAN/control
Package: http-server-dcsa
Version: 1.0
Architecture: amd64
Maintainer: coderatny
Description: http server for DCSA labs
EOF

cp ./build/http-server ./deb_pakage/bin/

dpkg-deb --build ./deb_pakage/ http-server-dcsa.deb