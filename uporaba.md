# Navodila za uporabo

(`vscode: ctrl+shift+v`) da ti odpre u markdown obliki in je lazje brati

Ko kopiras ukaze pazi da ne prekopiras zraven \` na zacetku ali koncu

# 1. Install OpenCV

Ne preskoci nobenega ukaza tudi ce se ti zdi preprost usi so pomembni, razen ciscenje ce si dirty boy

## Priprava odvisnosti

`module load "CMake/3.23.1-GCCcore-11.3.0"`

`cp -r /usr/include/openblas $HOME/.local/include`


## Prenos OpenCV

`wget https://github.com/opencv/opencv/archive/refs/tags/4.7.0.zip`

`unzip 4.7.0.zip`

## Priprava build okolja
`srun --reservation=fri  cmake -DCMAKE_INSTALL_PREFIX=$HOME/.local -Sopencv-4.7.0 -Bbuild-opencv`

## Build
`srun --reservation=fri --cpus-per-task 32 make -Cbuild-opencv -j32 install`

Lahko spremenis iz 32 jeder tudi na kaj drugega (na dveh lokacijah popravi ukaz)

zdaj pa si pojdi po kavico ker bo trajalo par minut

## Ciscenje build datotek

! Prvo preveri da se je OpenCV instaliral v ~/.local/include itd

`rm -rf 4.7.0.zip opencv-4.7.0 build-opencv`


# 2. Uporaba projekta projekta
`cd ps-robovi` oziroma kamor koli si ze kloniral repo

## Priprava build okolja (rabis samo enkrat)
`./cmake_install.sh`

## Build
! Ponavadi ni treba ker `./run.sh` avtomatsko builda je pa fajn ce rabis naredit npr clean

`./build.sh ukazi...` ukazi ce jih hoces (`./build.sh help`) ali pa prazno da zbuilda use

## Run

Zbuilda ce je potrebno in pozene na racunskih vozliscih

`./run.sh (mpi/pthread/serial)`

`./run.sh mpi [task count=8] [cpus per task=1]`

`./run.sh pthread [cpus per task=8] [task count=1]`

`./run.sh serial`

Pazi drugacen vrsten red argumentov pri mpi/pthread
