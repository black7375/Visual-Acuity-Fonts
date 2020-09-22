#! /bin/bash

## == File List ================================================================
## Source
# SOURCE=(
#   # FILE1
#   FILE1 ORGIN FILE PATH
#   FILE1 SYMBOLIC TARGET PATH
#
#   # FILE2
#   FILE2 ORGIN FILE PATH
#   FILE2 SYMBOLIC TARGET PATH
#   ...
# )

## Eye Chart Fonts
EYE_SOURCE=(
  "./eye-chart-fonts/Pelli-EyeChart_11hf.otf"
  "./dist/Pelli.otf"

  "./eye-chart-fonts/Sloan.otf"
  "./dist/Sloan-Letter.otf"
)

## Optician Sans
OPT_SOURCE=(
  "./optician-sans/OpenType-PS/Optician-Sans.otf"
  "./dist/Optician-Sans.otf"

  "./optician-sans/OpenType-TT/Optician-Sans.ttf"
  "./dist/Optician-Sans.ttf"

  "./optician-sans/Web-PS/Optician-Sans.woff2"
  "./dist/Optician-Sans.woff2"

  "./optician-sans/Web-TT/Optician-Sans.woff"
  "./dist/Optician-Sans.woff"
)

## Snellen
SNL_SOURCE=(
  "./snellen/fonts/Sloan.ttf"
  "./dist/Sloan.ttf"

  "./snellen/fonts/Snellen.ttf"
  "./dist/Snellen.ttf"
)

## Snellen MK
SMK_SOURCE=(
  "./snellen-mk/snellen-mk/Snellen.otf"
  "./dist/SnellenMK.otf"

  "./snellen-mk/snellen-mk/Snellen.ttf"
  "./dist/SnellenMK.ttf"
)

## Sources
SOURCES=(
  EYE_SOURCE
  OPT_SOURCE
  SNL_SOURCE
  SMK_SOURCE
)

## == Symbolic Link Generator ==================================================
DIR=$( cd "$(dirname "$0")" ; pwd )

# ./file       => true
# file         => false
# ./depth/file => true
# depth/file   => false
function current-check() {
  local path=$1

  if [ ${path:0:2} == "./" ]; then
    echo true
  else
    echo false
  fi
}

# ./depth1/file        => 1
# ./depth1/depth2/file => 2
# depth1/file          => 1
# depth1/depth2/file   => 2
function dir-depth() {
  local target=$1

  local re=${target//[!\/]}
  local re_len=${#re}
  local depth=$($(current-check ${target})\
                  && echo $((re_len - 1))\
                  || echo ${re_len}
        )
  echo $depth
}

# 1 => ../
# 2 => ../../
function parrent-dir() {
  local depth=$1

  local dir=""
  for (( i = 0; i<${depth}; i++ )); do
    local dir="../${dir}"
  done

  echo $dir
}

# ./dir1/file ./depth1/file        => ../dir1/file
# ./dir1/file ./depth1/depth2/file => ../../dir1/file
function relative-origin-path() {
  local origin=$1
  local target=$2

  local target_depth=$(dir-depth $target)
  local target_parent=$(parrent-dir ${target_depth})

  local origin_path=$($(current-check ${origin})\
                        && echo "${target_parent}${origin:2}"\
                        || echo "${target_parent}${origin}"
        )
  echo "${origin_path}"
}

function symbolic-link() {
  local origin=$1
  local target=$2

  if [ -e $origin ]; then
    local target_dir=${target%/*}
    local origin=$(relative-origin-path ${origin} ${target})
    local target="./${target##*/}"

    cd $target_dir
    ln -sv $origin $target
    cd $DIR
  else
    echo "File ${origin} is not exist"
  fi
}

function symbolic-generator() {
  local -n source=$1

  for (( i = 0; i<${#source[@]}; i++ )); do
    symbolic-link ${source[i]} ${source[i+1]}
    ((i+=1))
  done
}

## == Symbolic Generating ======================================================
for source in ${SOURCES[@]}; do
    symbolic-generator $source
done
