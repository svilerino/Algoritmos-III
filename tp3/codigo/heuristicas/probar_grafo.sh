#!/bin/bash
echo "Salida bqlocal:"
./bqlocal < ${1} 2> /dev/null

echo ""
echo "Salida golosa:"
./golosa < ${1} 2>/dev/null

echo ""
echo "Salida grasp:"
./grasp < ${1} 2>/dev/null
echo ""