#Maintainer: Fredrik Andersson <fredrik.ks.andersson@gmail.com>
pkgname=cmakgen
pkgver=1.1.1
pkgrel=1
pkgdesc="Tool to automatically generate cmake files for c++ projects"
arch=("x86_64")
makedepends=("tar" "cmake" "make")
source=("$pkgname-$pkgver.tar.gz")
md5sums=("SKIP")

build() {
  cd "$srcdir"

  mkdir _build && cd _build
  cmake ..

  make
}

package() {
  cd "$srcdir"/_build

  make DESTDIR="$pkgdir/" install
}
