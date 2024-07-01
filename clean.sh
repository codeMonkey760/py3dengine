MATH_LIB_CMAKE='py3dengine/math/lib/cmake-build-debug'
MATH_LIB_BUILD='py3dengine/math/extension/build'
PY3DENGINE_EGG_INFO='py3dengine.egg-info'
PY3DENGINE_BUILD='build'

if [ -d $MATH_LIB_CMAKE ]; then
  echo "Deleting $MATH_LIB_CMAKE";
  rm -rf $MATH_LIB_CMAKE
else
  echo "Skipping $MATH_LIB_CMAKE. Not found."
fi

if [ -d $MATH_LIB_BUILD ]; then
  echo "Deleting $MATH_LIB_BUILD";
  rm -rf $MATH_LIB_BUILD
else
  echo "Skipping $MATH_LIB_BUILD. Not found."
fi

if [ -d $PY3DENGINE_EGG_INFO ]; then
  echo "Deleting $PY3DENGINE_EGG_INFO";
  rm -rf $PY3DENGINE_EGG_INFO
else
  echo "Skipping $PY3DENGINE_EGG_INFO. Not found."
fi

if [ -d $PY3DENGINE_BUILD ]; then
  echo "Deleting $PY3DENGINE_BUILD";
  rm -rf $PY3DENGINE_BUILD
else
  echo "Skipping $PY3DENGINE_BUILD. Not found."
fi