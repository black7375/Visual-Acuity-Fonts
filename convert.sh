 #! /bin/bash

DIR=$( cd "$(dirname "$0")" ; pwd )

# Clean Files
echo "== Clean ./dist/ ====="
BACKUP="fonts.css"
mv ${DIR}/dist/${BACKUP} ${DIR}
rm -rvf ${DIR}/dist/*
mv ${DIR}/${BACKUP} ${DIR}/dist/
echo "== Clean End ====="
echo ""

# Create Symbolic Link
echo "== Creacte Symlinks ====="
bash ${DIR}/symbolic.sh
echo "== Create Symlinks End ====="
echo ""

# Create Converted Fonts
echo "== Create Converted Fonts ====="
python3 ${DIR}/convert.py
echo "== Create Converted Fonts End ====="
