#!/bin/sh
clear
COMPILABLE="spaceinvaders"
# Obtener la ruta absoluta del directorio del script (que ahora es el workspace)
WORKSPACE_DIR=$(cd "$(dirname "$0")" && pwd)

# Rutas absolutas actualizadas
SRC_PATH="${WORKSPACE_DIR}/${COMPILABLE}.cc"
OBJ_PATH="${WORKSPACE_DIR}/${COMPILABLE}.o"
OUTPUT_PATH="${WORKSPACE_DIR}/${COMPILABLE}_d"

# Solo compilar si el archivo fuente es más nuevo que el ejecutable o si el ejecutable no existe
if [ "$SRC_PATH" -nt "$OUTPUT_PATH" ] || [ ! -f "$OUTPUT_PATH" ]; then
  clear
  echo "\033[0;34mCompilando...\033[0m"

  # Eliminar archivos temporales antiguos
  rm -f "$OBJ_PATH" "$OUTPUT_PATH"

  # Compilación en modo debug
  clang++ -g -m64 -std=c++11 \
    -I"${WORKSPACE_DIR}/include" \
    -I"${WORKSPACE_DIR}/deps/ESAT/include/" \
    -c "$SRC_PATH" -o "$OBJ_PATH"

  clang++ -g -m64 -std=c++11 \
    -L"${WORKSPACE_DIR}/deps/ESAT/bin/macos64" \
    -lESAT -framework OpenGL -framework Cocoa -framework CoreVideo -framework IOKit \
    "$OBJ_PATH" -o "$OUTPUT_PATH"

  # Verificar si la compilación fue exitosa
  if [ $? -eq 0 ]; then
    echo "\033[0;32mARCHIVO EJECUTABLE COMPILADO\033[0m"
    ./"${COMPILABLE}_d"
  else
    echo "\033[0;31mARCHIVO EJECUTABLE NO COMPILADO\033[0m"
  fi
else
  echo "\033[0;33mEl ejecutable ya está actualizado. Ejecutando sin recompilar...\033[0m"
  ./"${COMPILABLE}_d"
fi
