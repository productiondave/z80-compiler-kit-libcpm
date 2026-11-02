#/usr/bin/env bash

usage() {
  echo "No prefix provided.  Check Make.default or Make.local"
  echo "Usage: make install"
  exit 1
}

if [ $# -lt 1 ]; then
  usage
fi

echo "Installing to $1"

echo "Creating install directory..."
mkdir -pv $1/lib/arch/RETRO
mkdir -pv $1/lib/arch/NOUVEAU
mkdir -pv $1/lib/arch/NABU
mkdir -pv $1/examples/arch/RETRO/skeleton
mkdir -pv $1/examples/arch/NOUVEAU/skeleton
mkdir -pv $1/examples/arch/NABU/skeleton
mkdir -pv $1/include

echo "Copying files..."
cp -fv build/arch/RETRO/libcpm.a $1/lib/arch/RETRO/
cp -fv build/arch/RETRO/crt0.o $1/lib/arch/RETRO/
cp -fv build/arch/NOUVEAU/libcpm.a $1/lib/arch/NOUVEAU/
cp -fv build/arch/NOUVEAU/crt0.o $1/lib/arch/NOUVEAU/
cp -fv build/arch/NABU/libcpm.a $1/lib/arch/NABU/
cp -fv build/arch/NABU/crt0.o $1/lib/arch/NABU/
cp -rv include/* $1/include/

sed "s#INSTALLDIR#$1#" arch/NOUVEAU/skeleton/Makefile.template > arch/NOUVEAU/skeleton/Makefile
sed "s#INSTALLDIR#$1#" arch/NOUVEAU/skeleton/compile_flags.template > arch/NOUVEAU/skeleton/compile_flags.txt
cp -rv arch/NOUVEAU/skeleton/Makefile $1/examples/arch/NOUVEAU/skeleton/
cp -rv arch/NOUVEAU/skeleton/main.c $1/examples/arch/NOUVEAU/skeleton/
cp -rv arch/NOUVEAU/skeleton/compile_flags.txt $1/examples/arch/NOUVEAU/skeleton/

sed "s#INSTALLDIR#$1#" arch/RETRO/skeleton/Makefile.template > arch/RETRO/skeleton/Makefile
sed "s#INSTALLDIR#$1#" arch/RETRO/skeleton/compile_flags.template > arch/RETRO/skeleton/compile_flags.txt
cp -rv arch/RETRO/skeleton/Makefile $1/examples/arch/RETRO/skeleton/
cp -rv arch/RETRO/skeleton/main.c $1/examples/arch/RETRO/skeleton/
cp -rv arch/RETRO/skeleton/compile_flags.txt $1/examples/arch/RETRO/skeleton/

sed "s#INSTALLDIR#$1#" arch/NABU/skeleton/Makefile.template > arch/NABU/skeleton/Makefile
sed "s#INSTALLDIR#$1#" arch/NABU/skeleton/compile_flags.template > arch/NABU/skeleton/compile_flags.txt
cp -rv arch/NABU/skeleton/Makefile $1/examples/arch/NABU/skeleton/
cp -rv arch/NABU/skeleton/main.c $1/examples/arch/NABU/skeleton/
cp -rv arch/NABU/skeleton/compile_flags.txt $1/examples/arch/NABU/skeleton/

