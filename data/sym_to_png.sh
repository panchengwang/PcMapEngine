#/sh/bin

CUR_DIR=$(cd $(dirname $0); pwd)
PREVIEW=$CUR_DIR/../build/bin/sym2img
for i in $CUR_DIR/*.json
do
    echo "${PREVIEW} $i"
    FILENAME=`basename $i .json`
    ${PREVIEW} $i > $CUR_DIR/$FILENAME.png
done

